/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef Q_MOC_RUN
#include <boost/any.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/scoped_array.hpp>
#endif
#include <sstream>
#include <salt/fileclasses.h>
#include <zeitgeist/core.h>
#include <zeitgeist/corecontext.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <sys/stat.h>
// rubywrapper.h should be before scriptserver.h (gcvalue.h). also, they were
// moved down to prevent a compilation error with mingw32
#include "rubywrapper.h"
#include "scriptserver.h"

#ifdef HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

using namespace boost;
using namespace std;
using namespace zeitgeist;

boost::shared_ptr<CoreContext> gMyPrivateContext;

void
getParameterList(VALUE args, ParameterList& params)
{
    int argc = RARRAY_LEN(args);

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

        params.AddValue(var);
    }
}

GCValue
ScriptServer::GetZeitgeistObject(boost::shared_ptr<Leaf> leaf)
{
    GCValue v;

    if (leaf.get() != 0)
    {
        stringstream ss;
        ss << "ZeitgeistObject.new(" << (unsigned long) leaf.get() <<")";
        v = RbEvalStringWrap(ss.str());
    }

    return v;
}


VALUE
selectObject(VALUE /*self*/, VALUE path)
{
    boost::shared_ptr<Leaf> leaf = gMyPrivateContext->Select(STR2CSTR(path));
    return ScriptServer::GetZeitgeistObject(leaf).Get();
}

VALUE
selectCall(VALUE /*self*/, VALUE functionName, VALUE args)
{
    ParameterList in;
    getParameterList(args, in);

    Class::TCmdProc cmd =
        gMyPrivateContext->GetSelection()->GetClass()->GetCmdProc
        (STR2CSTR(functionName));

    GCValue out;

    if (cmd != 0)
    {
        out = cmd(static_cast<Object*>(gMyPrivateContext->GetSelection().get()), in);
    } else
    {
        gMyPrivateContext->GetCore()->GetLogServer()->Error()
            << "(ScriptServer) ERROR: Unknown function '"
            << STR2CSTR(functionName) << "'" << endl;
    }

    return out.Get();
}

VALUE
thisCall(VALUE /*self*/, VALUE objPointer, VALUE functionName, VALUE args)
{
    ParameterList in;
    getParameterList(args, in);

    Object *obj = (Object*)NUM2ULONG(objPointer);
    Class::TCmdProc cmd =
        obj->GetClass()->GetCmdProc(STR2CSTR(functionName));

    GCValue out;

    if (cmd != 0)
    {
        out = cmd(obj, in);
    } else
    {
        gMyPrivateContext->GetCore()->GetLogServer()->Error()
            << "(ScriptServer) ERROR: Unknown function '"
            << STR2CSTR(functionName) << "'" << endl;
    }

    return out.Get();
}

VALUE
importBundle(VALUE /*self*/, VALUE path)
{
    gMyPrivateContext->GetCore()->ImportBundle(STR2CSTR(path));
    return Qnil;
}

VALUE
run (VALUE /*self*/, VALUE file)
{
    gMyPrivateContext->GetCore()->GetScriptServer()->Run(STR2CSTR(file));
    return Qnil;
}

VALUE
newObject(VALUE /*self*/, VALUE className, VALUE pathStr)
{
    boost::shared_ptr<Leaf> leaf =
        gMyPrivateContext->New(STR2CSTR(className), STR2CSTR(pathStr));
    return ScriptServer::GetZeitgeistObject(leaf).Get();
}

VALUE
deleteObject(VALUE /*self*/, VALUE name)
{
    gMyPrivateContext->Delete(STR2CSTR(name));
    return Qnil;
}

VALUE
getObject(VALUE /*self*/, VALUE path)
{
    boost::shared_ptr<Leaf> leaf = gMyPrivateContext->Get(STR2CSTR(path));
    return ScriptServer::GetZeitgeistObject(leaf).Get();
}

VALUE
listObjects(VALUE /*self*/)
{
    gMyPrivateContext->ListObjects();
    return Qnil;
}

VALUE
pushd(VALUE /*self*/)
{
    gMyPrivateContext->Push();
    return Qnil;
}

VALUE
popd(VALUE /*self*/)
{
    gMyPrivateContext->Pop();
    return Qnil;
}

VALUE
dirs(VALUE /*self*/)
{
    gMyPrivateContext->Dir();
    return Qnil;
}

ScriptServer::ScriptServer()
{
    ruby_init();

    // register built-in commands
    rb_define_global_function("selectObject", RUBY_METHOD_FUNC(selectObject), 1);
    rb_define_global_function("selectCall",   RUBY_METHOD_FUNC(selectCall), 2);
    rb_define_global_function("thisCall",     RUBY_METHOD_FUNC(thisCall), 3);
    rb_define_global_function("importBundle", RUBY_METHOD_FUNC(importBundle), 1);
    rb_define_global_function("run",          RUBY_METHOD_FUNC(run), 1);
    rb_define_global_function("new",          RUBY_METHOD_FUNC(newObject), 2);
    rb_define_global_function("delete",       RUBY_METHOD_FUNC(deleteObject), 1);
    rb_define_global_function("get",          RUBY_METHOD_FUNC(getObject), 1);
    rb_define_global_function("ls",           RUBY_METHOD_FUNC(listObjects), 0);
    rb_define_global_function("pushd",        RUBY_METHOD_FUNC(pushd), 0);
    rb_define_global_function("popd",         RUBY_METHOD_FUNC(popd), 0);
    rb_define_global_function("dirs",         RUBY_METHOD_FUNC(dirs), 0);

    mRelPathPrefix = string("..") + salt::RFile::Sep() + ".." + salt::RFile::Sep();
}

ScriptServer::~ScriptServer()
{
}

void
ScriptServer::UpdateCachedAllNodes()
{
    GetLog()->Debug() << "(ScriptServer) updating cached script variables\n";
    GetCore()->GetRoot()->UpdateCached();
}

bool
ScriptServer::Run(boost::shared_ptr<salt::RFile> file)
{
    if (file.get() == 0)
    {
        return false;
    }

    boost::scoped_array<char> buffer(new char[file->Size() + 1]);
    file->Read(buffer.get(), file->Size());
    buffer[file->Size()] = 0;

    bool ok = Eval(buffer.get());
    UpdateCachedAllNodes();

    return ok;
}

bool
ScriptServer::Run(const string &fileName)
{
    return Run(GetFile()->OpenResource(fileName));
}

bool
ScriptServer::Eval(const string &command)
{
    int error;
    RbEvalStringWrap(command,error);
    return (error == 0);
}

bool
ScriptServer::Eval(const std::string &command, GCValue& value)
{
    int error;
    value = RbEvalStringWrap(command,error);
    return (error == 0);
}

void
ScriptServer::CreateVariable(const string &varName, int value)
{
    // create a string with: "createVariable 'varName', value"
    stringstream s;
    s << "createVariable('" << varName << "', " << value << ")";
    Eval(s.str());
}

void
ScriptServer::CreateVariable(const string &varName, float value)
{
    // create a string with: "createVariable 'ns', 'varName', value"
    stringstream s;
    s << "createVariable('" << varName << "', " << value << ")";
    Eval(s.str());
}

void
ScriptServer::CreateVariable(const string &varName, const string &value)
{
    // create a string with: "createVariable 'ns', 'varName', 'value'"
    stringstream s;
    s << "createVariable('" << varName << "', '" << value << "')";
    Eval(s.str());
}

bool
ScriptServer::ParseVarName(const string& varName, string& nameSpace, string& name)
{
    stringstream  ss(varName);
    string current;
    vector<string> tokens;

    // segment varName
    while(! ss.eof())
    {
        getline(ss, current,'.');
        if (current.size())
        {
            tokens.push_back(current);
        }
    }

    if (tokens.size() != 2)
    {
        return false;
    }

    nameSpace = tokens[0];
    name = tokens[1];

    return (
        (nameSpace.size() >= 1) &&
        (nameSpace[0] >= 'A') &&
        (nameSpace[0] <= 'Z') &&
        (name.size() >= 1) &&
        (name[0] >= 'A') &&
        (name[0] <= 'Z')
        );
}

bool
ScriptServer::ExistsVariable(const string &varName)
{
    return (! GetVariable(varName).IsNil());
}

GCValue
ScriptServer::GetVariable(const string &varName)
{
    string nameSpace;
    string name;

    if (! ParseVarName(varName,nameSpace,name))
    {
        return GCValue();
    }

    GCValue v;
    if (nameSpace != "")
    {
        // get namespace class
        GCValue ns = rb_const_get(rb_cObject, rb_intern(nameSpace.c_str()));

        if (! ns.IsNil())
        {
            // get member variable of namespace object
            ID var = rb_intern(name.c_str());

            int error;
            RbArguments arg(ns.Get(), var, 0, 0);
            v = rb_protect(
                RbFuncallWrap,
                reinterpret_cast<VALUE>(&arg), &error
                );

            if (error)
            {
                GetLog()->Error() << "(ScriptServer) Ruby ERROR: "
                                  << RbGetError() << "\n";
                v = Qnil;
            }
        }
    } else
    {
        v = rb_const_get(rb_cObject, rb_intern(name.c_str()));
    }

    return v;
}

bool
ScriptServer::GetVariable(const string &varName, int &value)
{
    return GetVariable(varName).GetInt(value);
}

bool
ScriptServer::GetVariable(const std::string &varName, float &value)
{
    return GetVariable(varName).GetFloat(value);
}

bool
ScriptServer::GetVariable(const string &varName, bool &value)
{
    return GetVariable(varName).GetBool(value);
}

bool
ScriptServer::GetVariable(const string &varName, string &value)
{
    return GetVariable(varName).GetString(value);
}

boost::shared_ptr<CoreContext>
ScriptServer::GetContext() const
{
    return gMyPrivateContext;
}

bool
ScriptServer::SetupDotDir()
{
    string dotDir;
    if (GetDotDirName(dotDir) && CreateDotDir(dotDir))
    {
        GetFile()->AddResourceLocation(dotDir);
        return true;
    }
    return false;
}

bool
ScriptServer::ConstructInternal()
{
    if (! Leaf::ConstructInternal())
    {
        return false;
    }

    gMyPrivateContext = GetCore()->CreateContext();
    return true;
}

void
ScriptServer::SetInitRelPathPrefix(const std::string &relPathPrefix)
{
    mRelPathPrefix = relPathPrefix;
}

ScriptServer::ERunScriptErrorType
ScriptServer::RunInitScriptInternal(const string &sourceDir, const string &name,
                                    bool copy, const string& destDir)
{
    // run the init script in the sourceDir
    string sourcePath = sourceDir + salt::RFile::Sep() + name;
    GetLog()->Debug() << "(ScriptServer) Running " << sourcePath << "... " << endl;

    boost::shared_ptr<salt::StdFile> file(new(salt::StdFile));
    if (! file->Open(sourcePath.c_str()))
    {
        GetLog()->Error() << "(ScriptServer) Script not found " << sourcePath << endl;
        return eNotFound;
    } else if (! Run(file))
    {
        GetLog()->Error() << "(ScriptServer) Error in script " << sourcePath << endl;
        return eError;
    } else
    {
        GetLog()->Debug() << "(ScriptServer) Script ended OK " << sourcePath << endl;
    }

    // copy it to the destDir
    if (! copy)
    {
        return eOK;
    }

    string destPath = destDir + salt::RFile::Sep() + name;

    GetLog()->Debug() << "Copying " << sourcePath
                       << " to " << destPath << endl;

    stringstream s;
#ifdef WIN32
    s << "copy \"" << sourcePath << "\" \"" << destPath << '"';
#else
    s << "cp \"" << sourcePath << "\" \"" << destPath << '"';
#endif
    system(s.str().c_str());

    return eOK;
}

bool
ScriptServer::GetDotDirName(string& dotDir)
{
    if (mDotName == "")
    {
        GetLog()->Warning() << "(ScriptServer) WARNING: Dot directory name unset.\n";
        return false;
    }

    const char* envName =
#ifdef WIN32
        "USERPROFILE";
#else
        "HOME";
#endif

    char* home = getenv(envName);
    if (!home)
    {
        GetLog()->Warning() << "(ScriptServer) WARNING: $HOME is unset.\n";
        return false;
    }

    dotDir = string(home) + salt::RFile::Sep() + mDotName;
    return true;
}

bool
ScriptServer::CreateDotDir(const string& dotDir)
{
    char cwd[PATH_MAX+1];
#if WIN32
    if (GetCurrentDirectory(PATH_MAX, cwd) == 0)
#else
    if (getcwd(cwd,sizeof(cwd)) == NULL)
#endif
    {
        GetLog()->Error()
            << "(ScriptServer) ERROR: Cannot get current directory\n";
        return false;
    }

#if WIN32
    if (SetCurrentDirectory(dotDir.c_str()))
#else
    if (chdir(dotDir.c_str()) == 0)
#endif
    {
        // dot dir exists; change back to original directory
        chdir(cwd);
        return true;
    }

    // dot dir is not existent, try to create it
#if WIN32
    if (! CreateDirectory(dotDir.c_str(), 0))
#else
    if (mkdir(dotDir.c_str(),0777) != 0)
#endif
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Cannot create directory '"
                          << dotDir << "'\n";
        return false;
    }

    GetLog()->Debug() << "(ScriptServer) Created Directory '"
                       << dotDir << "'\n";

    return true;
}

bool
ScriptServer::RunInitScript(const string &fileName, const string &relPath,
                            EInitScriptType type)
{
    GetLog()->Debug() << "(ScriptServer) Attempting to run init script '"
                       << fileName << "'\n";

    string dotDir;
    bool validDotDir =
        (type == IS_USERLOCAL) &&
        GetDotDirName(dotDir) &&
        CreateDotDir(dotDir);

    // get the (OS specific) path to the script directory
    string pkgdatadir = salt::RFile::BundlePath();
#if __APPLE__
#if USE_COREFOUNDATION
    pkgdatadir += "Contents/Resources/";
#endif
#endif

    ERunScriptErrorType result = eNotFound;

    // Trying directory given in mRelPathPrefix
    if (!mRelPathPrefix.empty())
    {
        result = RunInitScriptInternal(mRelPathPrefix, fileName, validDotDir, dotDir);
        if (result == eOK)
        {
            GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                              << mRelPathPrefix << salt::RFile::Sep() << fileName << "'\n";
            return true;
        }
        else if (result == eError)
        {
            GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                              << mRelPathPrefix << salt::RFile::Sep() << fileName << "'\n";
            return false;
        }
    }


    if (validDotDir)
    {
        // Trying dot-dir in home directory
        result = RunInitScriptInternal(dotDir, fileName, false);
        if (result == eOK)
        {
            GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                              << dotDir << salt::RFile::Sep() << fileName << "'\n";
            return true;
        }
        else if (result == eError)
        {
            GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                              << dotDir << salt::RFile::Sep() << fileName << "'\n";
            return false;
        }

        GetLog()->Debug() << "(ScriptServer) : Did not find init script '"
                           << dotDir << salt::RFile::Sep() << fileName << "'\n";
    }



    // Trying package data directory
    result = RunInitScriptInternal(pkgdatadir,  fileName, validDotDir, dotDir);
    if (result == eOK)
    {
        GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                          << pkgdatadir << salt::RFile::Sep() << fileName << "'\n";
        return true;
    }
    else if (result == eError)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                          << pkgdatadir << salt::RFile::Sep() << fileName << "'\n";
        return false;
    }
    GetLog()->Debug() << "(ScriptServer) : Did not find init script '"
                      << pkgdatadir << salt::RFile::Sep() << fileName << "'\n";


    // Trying relative path to cwd
    result = RunInitScriptInternal(mRelPathPrefix+relPath, fileName, validDotDir, dotDir);
    if (result == eOK)
    {
        GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                          <<  mRelPathPrefix+relPath << salt::RFile::Sep() << fileName << "'\n";
        return true;
    }
    else if (result == eError)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                          << mRelPathPrefix+relPath << salt::RFile::Sep() << fileName << "'\n";
    }

    GetLog()->Error() << "(ScriptServer) ERROR: Cannot locate init script '"
                          << fileName << "'\n";
    return (result == eOK);
}
