#include <boost/any.hpp>
#include <boost/scoped_array.hpp>
#include <sstream>
#include "scriptserver.h"
#include <zeitgeist/corecontext.h>
#include <zeitgeist/core.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <sys/stat.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;

boost::shared_ptr<CoreContext> gMyPrivateContext;

void getParameterList(VALUE args, Class::TParameterList &params)
{
  int argc = RARRAY(args)->len;

  params.resize(argc);

  for (int i = 0; i<argc; ++i)
    {
      VALUE argument = rb_ary_entry(args, i);

      boost::any var;

      // do type conversion
      switch (TYPE(argument))
        {
        case T_STRING:
          {
            char *c = STR2CSTR(argument);
            var = c;
            //printf("string: '%s'\n",boost::any_cast<char*>(var));
          }
          break;
        case T_FIXNUM:
          {
            int i = FIX2INT(argument);
            var = i;
            //printf("int: '%d'\n", boost::any_cast<int>(var));
          }
          break;
        case T_FLOAT:
          {
            float f = (float)NUM2DBL(argument);
            var = f;
            //printf("float: '%f'\n", boost::any_cast<float>(var));
          }
          break;
        case T_TRUE:
          {
            var = true;
            //printf("bool: 'true'\n");
          }
          break;
        case T_FALSE:
          {
            var = false;
            //printf("bool: 'false'\n");
          }
          break;
        }
      params[i] = var;
    }
}

VALUE selectObject(VALUE /*self*/, VALUE path)
{
  shared_ptr<Leaf> leaf = gMyPrivateContext->Select(STR2CSTR(path));

  if (leaf.get() != NULL)
    {
      stringstream s;
      s << "ZeitgeistObject.new(" << (unsigned long) leaf.get() <<")";
      return rb_eval_string(s.str().c_str());
    }
  else
    return Qnil;
}

VALUE selectCall(VALUE /*self*/, VALUE functionName, VALUE args)
{
  Class::TParameterList in;

  getParameterList(args, in);

  Class::TCmdProc cmd =
    gMyPrivateContext->GetObject()->GetClass()->GetCmdProc(STR2CSTR(functionName));

  if (cmd != NULL)
    {
      cmd(static_cast<Object*>(gMyPrivateContext->GetObject().get()), in);
    }
  else
    {
      gMyPrivateContext->GetCore()->GetLogServer()->Error()
        << "ERROR: Unknown function '" << STR2CSTR(functionName) << "'" << endl;
    }

  return Qnil;
}

VALUE thisCall(VALUE /*self*/, VALUE objPointer, VALUE functionName, VALUE args)
{
  Class::TParameterList in;

  getParameterList(args, in);

  Object *obj = (Object*)NUM2INT(objPointer);
  Class::TCmdProc cmd =
    obj->GetClass()->GetCmdProc(STR2CSTR(functionName));

  if (cmd != NULL)
    {
      cmd(obj, in);
    }
  else
    {
      gMyPrivateContext->GetCore()->GetLogServer()->Error()
        << "ERROR: Unknown function '" << STR2CSTR(functionName) << "'" << endl;
    }

  return Qnil;
}

VALUE importBundle(VALUE /*self*/, VALUE path)
{
  gMyPrivateContext->GetCore()->ImportBundle(STR2CSTR(path));
  return Qnil;
}

VALUE newObject(VALUE /*self*/, VALUE className, VALUE pathStr)
{
  shared_ptr<Leaf> leaf =
    gMyPrivateContext->New(STR2CSTR(className), STR2CSTR(pathStr));

  if (leaf.get() != NULL)
    {
      stringstream s;
      s << "ZeitgeistObject.new(" << (unsigned long) leaf.get() <<")";
      return rb_eval_string(s.str().c_str());
    }
  else
    return Qnil;
}

VALUE deleteObject(VALUE /*self*/, VALUE name)
{
  gMyPrivateContext->Delete(STR2CSTR(name));

  return Qnil;
}

VALUE getObject(VALUE /*self*/, VALUE path)
{
  shared_ptr<Leaf> leaf = gMyPrivateContext->Get(STR2CSTR(path));

  if (leaf.get() != NULL)
    {
      stringstream s;
      s << "ZeitgeistObject.new(" << (unsigned long) leaf.get() <<")";
      return rb_eval_string(s.str().c_str());
    }
  else
    return Qnil;
}

VALUE listObjects(VALUE /*self*/)
{
  gMyPrivateContext->ListObjects();

  return Qnil;
}

VALUE pushd(VALUE /*self*/)
{
  gMyPrivateContext->Push();

  return Qnil;
}

VALUE popd(VALUE /*self*/)
{
  gMyPrivateContext->Pop();

  return Qnil;
}

VALUE dirs(VALUE /*self*/)
{
  gMyPrivateContext->Dir();

  return Qnil;
}

ScriptServer::ScriptServer()
{
  ruby_init();

  // register built-in commands
  rb_define_global_function("selectObject", RUBY_METHOD_FUNC(selectObject), 1);
  rb_define_global_function("selectCall", RUBY_METHOD_FUNC(selectCall), 2);
  rb_define_global_function("thisCall", RUBY_METHOD_FUNC(thisCall), 3);
  rb_define_global_function("importBundle",       RUBY_METHOD_FUNC(importBundle), 1);
  rb_define_global_function("new",        RUBY_METHOD_FUNC(newObject), 2);
  rb_define_global_function("delete",     RUBY_METHOD_FUNC(deleteObject), 1);
  rb_define_global_function("get", RUBY_METHOD_FUNC(getObject), 1);
  rb_define_global_function("ls", RUBY_METHOD_FUNC(listObjects), 0);
  rb_define_global_function("pushd",      RUBY_METHOD_FUNC(pushd), 0);
  rb_define_global_function("popd",       RUBY_METHOD_FUNC(popd), 0);
  rb_define_global_function("dirs",       RUBY_METHOD_FUNC(dirs), 0);
}

ScriptServer::~ScriptServer()
{
}

bool ScriptServer::Run(salt::RFile* file)
{
  if (file == NULL)
    {
      return false;
    }

  boost::scoped_array<char> buffer(new char[file->Size() + 1]);
  file->Read(buffer.get(), file->Size());
  buffer[file->Size()] = 0;

  return Eval (buffer.get());
}

bool ScriptServer::Run(const string &fileName)
{
  /*int error;
    rb_load_protect(rb_str_new2(fileName.c_str()), 0, &error);

    if (error)
    {
    VALUE mes = rb_inspect(rb_gv_get("$!"));
    GetLog()->Error().Printf("ERROR: %s\n", RSTRING(mes)->ptr);
    return false;
    }
    return true;*/
  salt::RFile* file = GetFile()->Open(fileName.c_str());

  if (file == NULL)
    {
      GetLog()->Error().Printf("ERROR: ScriptServer::Run() - Can't locate file '%s'\n",
                               fileName.c_str());
      return false;
    }

  bool result = Run(file);
  delete file;

  return result;
}

bool ScriptServer::Eval(const string &command)
{
  int error;
  rb_eval_string_protect(command.c_str(), &error);
  if (error)
    {
      VALUE mes = rb_inspect(rb_gv_get("$!"));
      GetLog()->Error().Printf("ERROR: %s\n", RSTRING(mes)->ptr);
      return false;
    }
  return true;
}

void ScriptServer::CreateVariable(const string &varName, int value)
{
  stringstream s;
  // create a string with: "createVariable 'varName', value"
  s << "createVariable '" << varName << "', " << value;
  Eval(s.str());
}

void ScriptServer::CreateVariable(const string &varName, float value)
{
  stringstream s;
  // create a string with: "createVariable 'ns', 'varName', value"
  s << "createVariable '" << varName << "', " << value;
  Eval(s.str());
}

void ScriptServer::CreateVariable(const string &varName, const string &value)
{
  stringstream s;
  // create a string with: "createVariable 'ns', 'varName', 'value'"
  s << "createVariable '" << varName << "', '" << value << "'";
  Eval(s.str());
}

VALUE ScriptServer::GetVariable(const string &varName)
{
  stringstream    s(varName);
  string                  current;
  vector<string>  tokens;

  // parse varName
  while(!s.eof())
    {
      getline(s, current,'.');
      if (current.size())
        tokens.push_back(current);
    }

  if (tokens.size() != 2)
    {
      GetLog()->Error() << "ERROR: Invalid variable '" << varName << "'" << endl;
      return Qnil;
    }
  else
    {
      // get class
      VALUE ns =      rb_const_get(rb_cObject, rb_intern(tokens[0].c_str()));
      if (NIL_P(ns))
        {
          GetLog()->Error() << "ERROR: Invalid namespace '"
                            << tokens[0] << "'" << endl;
          return false;
        }
      ID var= rb_intern(tokens[1].c_str());
      return rb_funcall(ns, var, 0);
    }
}

bool ScriptServer::GetVariable(const string &varName, int &value)
{
  VALUE val = GetVariable(varName);

  if (NIL_P(val))
    {
      return false;
    }
  else
    {
      value = NUM2INT(val);
      return true;
    }
}

bool ScriptServer::GetVariable(const string &varName, bool &value)
{
  VALUE val = GetVariable(varName);

  if (NIL_P(val))
    {
      return false;
    }
  else
    {
      if (TYPE(val) == T_TRUE)
        {
          value = true;
          return true;
        }
      if (TYPE(val) == T_FALSE)
        {
          value = false;
          return true;
        }
      return false;
    }
}

bool ScriptServer::GetVariable(const string &varName, string &value)
{
  VALUE val = GetVariable(varName);

  if (NIL_P(val))
    {
      return false;
    }
  else
    {
      value = STR2CSTR(val);
      return true;
    }
}

boost::shared_ptr<CoreContext> ScriptServer::GetContext() const
{
  return gMyPrivateContext;
}

bool ScriptServer::ConstructInternal()
{
  if (Leaf::ConstructInternal() == false) return false;

  gMyPrivateContext = GetCore()->CreateContext();

  return true;
}

bool ScriptServer::RunInitScript(const string &sourceDir, const string &name,
                                 bool copy, const string& destDir)
{
  // run the init script in the sourceDir
  string sourcePath = sourceDir + "/" + name;
  GetLog()->Normal() << "Running " << sourcePath << "... ";

  salt::StdFile file;
  if (
      (! file.Open(sourcePath.c_str())) ||
      (! Run(&file))
       )
  {
    GetLog()->Normal() << "failed" << endl;
    return false;
  } else
    {
      GetLog()->Normal() << "ok" << endl;
    }

  // copy it to the destDir
  if (! copy)
    {
      return true;
    }

  string destPath = destDir + "/" + name;

  GetLog()->Normal() << "Copying " << sourcePath << " to " << destPath << endl;

  stringstream s;
  s << "cp " << sourcePath << " " << destPath;
  system(s.str().c_str());

  return true;
}

bool ScriptServer::RunInitScript(const string &fileName, const string &relPath)
{
  // create and change to the user's dot directory
  bool validDotDir = true;

  if (mDotName == "")
    {
      GetLog()->Warning() << "WARNING: Dot directory name unset." << endl;
      validDotDir = false;
    }

  char* home = getenv("HOME");
  string dotDir;
  if (home)
    {
      dotDir = string(home) + "/" + mDotName;
    } else
      {
        validDotDir = false;
      }

  char* cwd = getcwd(NULL,0);
  if (cwd == NULL)
    {
      GetLog()->Error() << "ERROR: Cannot get current directory" << endl;
      validDotDir = false;
    }

  if (
      (validDotDir) &&
      (chdir(dotDir.c_str()) != 0)
      )
    {
      if (mkdir(dotDir.c_str(),0777) != 0)
        {
          GetLog()->Error() << "ERROR: Cannot create directory '" << dotDir << "'" << endl;
          validDotDir = false;
        } else
          {
            GetLog()->Normal() << "Created Directory " << dotDir << endl;
          }
    } else
      {
        chdir(cwd);
      }

  // destroy the buffer allocated by getcwd()
  delete cwd;

  // convert the Macro PREFIX to a string constant
  #define XSTRINGIFY(s) STRINGIFY(s)
  #define STRINGIFY(s) #s
  string prefix = XSTRINGIFY(PREFIX);

  bool ok =
    (
     (
       (validDotDir) && (RunInitScript(dotDir, fileName, false))
       )
      || (RunInitScript(prefix,  fileName, validDotDir, dotDir))
      || (RunInitScript(relPath, fileName, validDotDir, dotDir))
      );

  if (! ok)
    {
      GetLog()->Error() << "ERROR: Cannot locate init script " << fileName << endl;
    }

  return ok;
}
