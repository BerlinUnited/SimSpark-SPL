#ifndef _COMMSERVER_H
#define _COMMSERVER_H

#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>
#include "communit.h"
#include "types.h"
#include <vector>

class CommServer
{
 public:
  typedef std::vector<salt::Vector3f> TPositions;

 public:
  CommServer(std::string host, int port);
  virtual ~CommServer() {};

  bool GetMessage();
  const TPositions& GetPositions();

 protected:
    void ParseTuples(const char* msg, const size_t& len);

 protected:
    // receive buffer
    char mData[MAX_MSG];

    // socket wrapper
    CommUnit mCommUnit;

    // vector of parsed out positions
    TPositions mPositions;
};
#endif
