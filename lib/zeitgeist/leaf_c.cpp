#include "leaf.h"
#include "logserver/logserver.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

OUT_FUNCTION(getName)
{
  if( in.empty() )
    {
      Leaf *leaf = static_cast<Leaf*>(obj);
      out = rb_str_new2(leaf->GetName().c_str());
    }
}

OUT_FUNCTION(getFullPath)
{
  if (in.empty())
    {
      Leaf *leaf = static_cast<Leaf*>(obj);
      out = rb_str_new2(leaf->GetFullPath().c_str());
    }
}

OUT_FUNCTION(isLeaf)
{
  if( in.empty() )
    {
      Leaf *leaf = static_cast<Leaf*>(obj);
      out = leaf->IsLeaf() ? Qtrue:Qfalse;
    }
}

void CLASS(Leaf)::DefineClass()
{
  DEFINE_FUNCTION(getFullPath);
  DEFINE_FUNCTION(getName);
  DEFINE_FUNCTION(isLeaf);
}
