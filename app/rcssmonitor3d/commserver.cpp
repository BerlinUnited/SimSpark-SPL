#include "commserver.h"

using namespace std;

CommServer::CommServer(string host, int port) : mCommUnit(host,port)
{
}

bool CommServer::GetMessage()
{
  char rawmsg[MAX_MSG];

  int l =  mCommUnit.GetMessage(rawmsg, MAX_MSG);
  if (l == 0)
    {
      return (false);
    }

  // zero terminate and parse received data
  rawmsg[l] = 0;
  ParseTuples(rawmsg, strlen(rawmsg));

  return (true);
}


void CommServer::ParseTuples(const char* msg, const size_t& len)
{
  // clear all previous vectors
  mPositions.clear();

  //extract all breakets
  static char* tmp[MAX_MSG];

  sscanf(msg, "( %[^)] )", mData);

  for(uint i = 0; i < len; i++)
    {
      switch(mData[i])
        {
        case 'p' :
          // player pos
          salt::Vector3f vec;
          sscanf(mData, "%[a-z] %f %f %f",tmp, &vec[0], &vec[1], &vec[2]);

          cout << " pos x: "<< vec[0]
               << " pos y:" << vec[1]
               << " pos z:" << vec[2]
               << endl;

          mPositions.push_back(vec);
          break;
        }
    }
}

const CommServer::TPositions& CommServer::GetPositions()
{
  return mPositions;
}
