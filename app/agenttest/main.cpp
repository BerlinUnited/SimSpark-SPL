// This is a dummy agent created for testing with the SpadesTest. It
// reads length prefixed data (see spades manual 6.3) from fd 3 and
// writes to fd 4. A corresponding agent DB (agentdb.list, entry
// 'default') exists in the spadestest directory.

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

using namespace std;

static char buffer[4096];

void
log(const char* out)
{
  printf(out);
  fflush(NULL);
}

int
getInput()
{
  ssize_t sz = read(3, buffer, sizeof(buffer));

  if ((unsigned int)sz < sizeof(long))
    {
      return 0;
    }

  long len;
  memcpy(&len, buffer, sizeof(long));
  len = ntohl(len);

  // zero terminate received data
  buffer[sizeof(long) + len] = 0;

  return len;
}

void
putOutput(const char* out)
{
  strcpy(&buffer[4], out);
  long l = htonl(strlen(out));
  memcpy(buffer,&l,sizeof(long));
  write(4, buffer, l + sizeof(long));
}

int
main(int /*argc*/, const char *const */*argv*/)
{
  log("AgentTest started\n");

  //
  // perfom any agent init here
  //

  // 'initialization done' message
  log("writing init done message\n");
  putOutput("I");

  // the agent process will block after the init done message is
  // written

  char* data = &buffer[sizeof(long)];
  while (true)
    {
      if (getInput())
        {
          log("received data: ");
          log(data);
          log("\n");
        }
    }
}
