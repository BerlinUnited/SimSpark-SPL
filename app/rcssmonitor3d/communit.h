#ifndef _COMMUNIT_H
#define _COMMUNIt_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>

#include "types.h"
#include "tcpsocket.h"

class CommUnit
{
public:
  CommUnit () {};
  ~CommUnit ();
  CommUnit (std::string host, int port);

  bool SendMessage(const char* buf);
  bool SendMessage(const std::string& msg);
  int GetMessage (char* buf, int size);
  void CloseConnection ();

protected:
  rcss::net::TCPSocket mSocket;
};

#endif
