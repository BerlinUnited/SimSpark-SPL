#include "communit.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <rcssbase/net/exception.hpp>
#include <errno.h>

using namespace std;
using namespace rcss::net;

CommUnit::CommUnit(string host, int port)
{
  cout << "(CommUnit) connecting to ";
  cout << host << ":" << port << endl;

  Addr local(INADDR_ANY,INADDR_ANY);
  mSocket.bind(local);

  try
    {
      Addr server(port,string(host));
      mSocket.connect(server);
    }
  catch (ConnectErr error)
    {
      cerr << "(CommUnit) connection failed with: \n\t"
           << strerror(errno) << endl;
    }

  if (mSocket.isConnected())
    {
      cout << "(CommUnit) connected successfully" << endl;
    }
}

CommUnit::~CommUnit ()
{
  CloseConnection ();
}

void CommUnit::CloseConnection ()
{
  mSocket.close();
}

bool
CommUnit::SendMessage(const std::string& msg)
{
    return SendMessage(msg.c_str());
}

bool
CommUnit::SendMessage(const char* buf)
{
  int l = strlen(buf);
  return (mSocket.send(buf, l) == l);
}

int CommUnit::GetMessage (char* buf, int size)
{
  int l = mSocket.recv(0,buf,size,0);

  if (l < 0)
    {
      // non blocking socket, no data available
      l = 0;
    }

  return l;
}
