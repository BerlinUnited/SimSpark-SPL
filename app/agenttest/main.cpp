// This is a dummy agent created for testing with the SpadesTest. It
// reads length prefixed data (see spades manual 6.3) from fd 3 and
// writes to fd 4. A corresponding agent DB (agentdb.list, entry
// 'default') exists in the spadestest directory.

#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>
#include <sstream>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;

// the receive buffer of the agent
static char buffer[4096];

// pointer to the payload received in the buffer
static char* msg_data = buffer + sizeof(long);

// parser instance
static shared_ptr<BaseParser> parser;

// input file descriptor
const int readFd = 3;

// outp file descriptor
const int writeFd = 4;

// the default team name
std::string teamName = "Robolog";

// set to 1 to write debug information to stdout
#define ENABLE_LOGGING 0

#ifdef ENABLE_LOGGING
void Log(const char* out)
{
   printf(out);
   fflush(0);
}
#else
 #define Log(x)
#endif

bool SelectInput()
{
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(readFd,&readfds);

  return select(readFd+1,&readfds, 0, 0, 0) > 0;
}

bool GetInput()
{
  // read the first message segment
  if (! SelectInput())
    {
      return false;
    }

  unsigned int bytesRead = read(readFd, buffer, sizeof(buffer));
  if (bytesRead < sizeof(unsigned int))
    {
      return false;
    }

  // msg is prefixed with it's total length
  unsigned int msgLen = ntohl(*(unsigned int*)buffer);

  // read remaining message segments
  unsigned int msgRead = bytesRead - sizeof(unsigned int);
  char *offset = buffer + bytesRead;

  while (msgRead < msgLen)
    {
      if (! SelectInput())
        {
          return false;
        }

      msgRead += read(3, offset, sizeof(buffer) - msgRead);
      offset += msgRead;
    }

  // zero terminate received data
  (*offset) = 0;

  return true;
}

void PutOutput(const char* out)
{
  strcpy(msg_data, out);
  unsigned int len = strlen(out);
  unsigned int netlen = htonl(len);
  memcpy(buffer,&netlen,sizeof(netlen));
  write(writeFd, buffer, len + sizeof(netlen));
}

void PutOutput(const std::string& str)
{
  PutOutput(str.c_str());
}

// --------------------

void CreateAgent()
{
  // use the create effector to setup the Sensors and Effectors
  Log(msg_data);
  Log("\n");
  Log("creating agent\n");
  PutOutput("A(create)");
}

void InitAgent()
{
  Log(msg_data);
  Log("\n");
  Log("sending init command\n");

  std::stringstream ss;
  ss << "A(init (unum " << getpid() << ") (teamname " << teamName << "))";
  PutOutput(ss.str().c_str());
}

void RandomBehave(int numSensation)
{
  static bool back = false;

  Log("received another sensation ");
  Log(msg_data);

  if (numSensation % 100 == 0) back = !back;

  Log("using dash effector\n ");
  if (back)
    {
      if (numSensation % 10 == 0)
        PutOutput("A(dash 0 300 0)");
      else
        PutOutput("A(dash -50 -10 -50)");
    } else {
      if (numSensation % 10 == 0)
        PutOutput("A(dash 0 300 0)");
      else
        PutOutput("A(dash 50 -10 50)");
    }
}

bool GetObjectPos(const Predicate& predicate, const string& name, Vector3f& pos)
{
  // find the PerfectVision data about the object
  Predicate::Iterator objIter(predicate);

  // advance to the section about object 'name'
  if (! predicate.FindParameter(objIter,name))
    {
      return false;
    }

  // advance to the 'pos' entry in the object's section
  if (! predicate.FindParameter(objIter,"pos"))
    {
      return false;
    }

  // read the position vector
  if (! predicate.GetValue(objIter,pos))
    {
      return false;
    }

  return true;
}

void Behave(int /*numSensation*/)
{
  shared_ptr<Predicate::TList> predicates =
    parser->Parse(msg_data);

  Log("received sensation ");
  Log(msg_data);
  Log("\n");

  for (
       Predicate::TList::const_iterator iter = predicates->begin();
       iter != predicates->end();
       ++iter
       )
    {
      const Predicate& predicate = (*iter);

      // check for the PerfectVision perceptor
      if (predicate.name != "PerfectVision")
        {
          Log ("skipped ");
          Log (predicate.name.c_str());
          Log("\n");
          continue;
        }

      Log("PerfectVision\n");

      Vector3f ballVec;
      if (! GetObjectPos(predicate, "Ball", ballVec))
        {
          return;
        }

      const float dist = ballVec.Length();

      static char buffer[512];
      sprintf(buffer,"******** ballVec %.2f %.2f %.2f, l= %.2f\n",ballVec[0],ballVec[1],ballVec[2],dist);
      Log(buffer);

      if (dist < 0.3 + 0.111 + 0.04)
        {
          // kick the ball
          Log("Kicking \n");
          PutOutput("A(kick up 200000)");
        } else
          {
            // seek the ball
            if (dist > 1)
              {
                ballVec *= 20;
              } else
                {
                  ballVec *= 5;
                }

            stringstream ss;
            ss << "A(dash"
               << " " << ballVec[0]
               << " " << ballVec[1]
               << "  " << ballVec[2]
               << ")";

            PutOutput(ss.str().c_str());
          }
    }
}

// --------------------


void ProcessSensation()
{
  // Stime time data

  // This is a sensation to be given to the agent. It begins a
  // thinking cycle. The first time is the simulation time the
  // sensation was generated (also known as the send time) and the
  // second is the time that the sensation is delivered to the agent
  // (also known as the arrive time). If the parameter send agent send
  // time is off, -1 is always sent as the send time, and if the
  // parameter send agent arrive time is off, -1 is always sent as the
  // arrive time. The data is an arbitrary data string generated by
  // the world model. The agent can reply with act messages, and must
  // finish with a done thinking message.

  static int numSensation = 0;

  switch (numSensation)
    {
    case 0:
      Log ("received first sensation ");
      CreateAgent();
      break;

    case 1:
      Log("received second sensation ");
      InitAgent();
      break;

    default:
      Behave(numSensation);
    }

  Log("writing done thinking message\n");
  PutOutput("D");

   ++numSensation;
}

void ProcessInitMessage()
{
  // Ddata

  // After startup, this initialization data message is sent to the
  // agent. On a normal startup, data will be empty. On an agent
  // migration (see Section 5.3), the data returned by the old agent
  // processes will be given to the new agent. An initialization done
  // message should be sent once the initialization data has been
  // processed and all other startup is complete.

  Log("received init message 'D'\n");

  // init here

  Log("writing init done message\n");
  PutOutput("I");
}

void ProcessThinkTimeMessage()
{
  // Ktime

  // This is a think time message that notifies the agent how much
  // thinking time was used for the last thinking cycle. These are
  // only sent if send agent think times is on.

  Log ("received a think time message ");
  Log (msg_data);
  Log ("\n");

  Log("writing done thinking message\n");
  PutOutput("D");
}


//-----------------------------------------------------------------------
void printHelp()
{
  cout << "\nusage: agenttest [options]" << endl;
  cout << "\noptions:" << endl;
  cout << " --help      print this message." << endl;
  cout << " --teamname  sets the team name. " << endl;
  cout << "\n";
}

void ReadOptions(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "--teamname" ) == 0 )
        {
          if( i+1  < argc)
            {
              teamName = argv[i+1];
              ++i;
              Log("setting teamname to ");
              Log(teamName.c_str());
              Log("\n");
            }
        }
      else if( strcmp( argv[i], "--help" ) == 0 )
        {
          printHelp();
          exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
  //init zeitgeist and oxygen
  Zeitgeist zg("." PACKAGE_NAME);
  Oxygen oygen(zg);

  // read command line options
  ReadOptions(argc,argv);

  // init the s-expression parser
  if (! zg.GetCore()->ImportBundle("sexpparser"))
    {
      return 1;
    }

  parser = shared_dynamic_cast<BaseParser>(zg.GetCore()->New("SexpParser"));
  if (parser.get() == 0)
    {
      Log("cannot create SexpParser");
    }

  Log("AgentTest started\n");

  timeval tv;
  gettimeofday(&tv, 0);
  srand(tv.tv_usec);

  //
  // perfom any agent init here
  //

  // 'initialization done' message
  while (true)
    {
      if (! GetInput())
        {
          break;
        }

      switch(msg_data[0])
        {
        case 'D' :
          ProcessInitMessage();
          break;

        case 'S' :
          ProcessSensation();
          break;

        case 'K' :
          ProcessThinkTimeMessage();
          break;

        default:
          Log("received unknown data: ");
          Log(msg_data);
          Log("\n");
          break;
        }
    }
}
