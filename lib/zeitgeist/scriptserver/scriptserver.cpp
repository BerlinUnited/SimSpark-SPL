/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: scriptserver.cpp,v 1.19 2004/04/23 15:30:42 fruit Exp $

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

void
getParameterList(VALUE args, ParameterList& params)
{
    int argc = RARRAY(args)->len;

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
        ss << "ZeitgeistObject.new (" << (unsigned long) leaf.get() <<")";
        v = RbEvalStringWrap(ss.str());
    }

    return v;
}


VALUE
selectObject(VALUE /*self*/, VALUE path)
{
    shared_ptr<Leaf> leaf = gMyPrivateContext->Select(STR2CSTR(path));
    return ScriptServer::GetZeitgeistObject(leaf).Get();
}

VALUE
selectCall(VALUE /*self*/, VALUE functionName, VALUE args)
{
    ParameterList in;
    getParameterList(args, in);

    Class::TCmdProc cmd =
        gMyPrivateContext->GetObject()->GetClass()->GetCmdProc
        (STR2CSTR(functionName));

    GCValue out;

    if (cmd != 0)
    {
        out = cmd(static_cast<Object*>(gMyPrivateContext->GetObject().get()), in);
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

    Object *obj = (Object*)NUM2INT(objPointer);
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
newObject(VALUE /*self*/, VALUE className, VALUE pathStr)
{
    shared_ptr<Leaf> leaf =
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
    shared_ptr<Leaf> leaf = gMyPrivateContext->Get(STR2CSTR(path));
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
    rb_define_global_function("new",          RUBY_METHOD_FUNC(newObject), 2);
    rb_define_global_function("delete",       RUBY_METHOD_FUNC(deleteObject), 1);
    rb_define_global_function("get",          RUBY_METHOD_FUNC(getObject), 1);
    rb_define_global_function("ls",           RUBY_METHOD_FUNC(listObjects), 0);
    rb_define_global_function("pushd",        RUBY_METHOD_FUNC(pushd), 0);
    rb_define_global_function("popd",         RUBY_METHOD_FUNC(popd), 0);
    rb_define_global_function("dirs",         RUBY_METHOD_FUNC(dirs), 0);

    mRelPathPrefix = "../../";
}

ScriptServer::~ScriptServer()
{
}

void
ScriptServer::UpdateCachedAllNodes()
{
    // build list of all nodes, searching recursively
    TLeafList leaves;
#if 1
    if (shared_ptr<Leaf> root = GetParent().lock())
    {
        if (root)
        {
            std::cerr << "server root " << root->GetName() << "\n";
            std::cerr << "UpdateCached all ChildNodes\n";
            shared_ptr<Leaf> blah = make_shared(GetParent());
            blah->GetChildrenSupportingClass("Leaf",leaves,true);
        }
    }
#else
//     shared_ptr<Leaf> root = GetCore()->GetRoot();
//     std::cerr << "my name " << this->GetName() << "\n";
//     std::cerr << "this " << this << "\n";
//     root->GetChildrenSupportingClass("Leaf",leaves,true);
//    shared_ptr<Class> markus = shared_static_cast<Class>(GetCore()->Get("/classes/zeitgeist/ClassClass"));
//    std::cerr << "classclass name " << markus->GetCore().get() << "\n";

#endif

    // update all Leaves found
    for (TLeafList::iterator iter = leaves.begin();
         iter != leaves.end();
         ++iter
        )
    {
        shared_ptr<Leaf> leaf = shared_static_cast<Leaf>(*iter);
        std::cerr << "UpdateCached: " << leaf->GetName() << "\n";
        leaf->UpdateCached();
    }
}

bool
ScriptServer::Run(shared_ptr<salt::RFile> file)
{
    if (file.get() == 0)
    {
        return false;
    }

    boost::scoped_array<char> buffer(new char[file->Size() + 1]);
    file->Read(buffer.get(), file->Size());
    buffer[file->Size()] = 0;

    bool ok = Eval (buffer.get());
    UpdateCachedAllNodes();
    return ok;
}

bool
ScriptServer::Run(const string &fileName)
{
    shared_ptr<salt::RFile> file = GetFile()->Open(fileName.c_str());
    if (file.get() == 0)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Cannot locate file '"
                          << fileName << "'\n";
        return false;
    }

    GetLog()->Normal() << "(ScriptServer) Running " << fileName << endl;

    return Run(file);
}

bool
ScriptServer::Eval(const string &command)
{
    int error;
    RbEvalStringWrap(command,error);
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
                GetLog()->Debug() << "(ScriptServer) Ruby ERROR: "
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

bool
ScriptServer::RunInitScriptInternal(const string &sourceDir, const string &name,
                                    bool copy, const string& destDir)
{
    // run the init script in the sourceDir
    string sourcePath = sourceDir + "/" + name;
    GetLog()->Normal() << "(ScriptServer) Running " << sourcePath << "... ";

    shared_ptr<salt::StdFile> file(new(salt::StdFile));
    if (
        (! file->Open(sourcePath.c_str())) ||
        (! Run(file))
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

    GetLog()->Normal() << "Copying " << sourcePath
                       << " to " << destPath << endl;

    stringstream s;
    s << "cp " << sourcePath << " " << destPath;
    system(s.str().c_str());

    return true;
}

bool
ScriptServer::GetDotDirName(string& dotDir)
{
    if (mDotName == "")
    {
        GetLog()->Warning() << "(ScriptServer) WARNING: Dot directory name unset.\n";
        return false;
    }

    char* home = getenv("HOME");
    if (!home)
    {
        GetLog()->Warning() << "(ScriptServer) WARNING: $HOME is unset.\n";
        return false;
    }

    dotDir = string(home) + "/" + mDotName;

    return true;
}

bool
ScriptServer::CreateDotDir(const string& dotDir)
{
    char cwd[PATH_MAX+1];
    if (getcwd(cwd,sizeof(cwd)) == NULL)
    {
        GetLog()->Error()
            << "(ScriptServer) ERROR: Cannot get current directory\n";
        return false;
    }

    if (chdir(dotDir.c_str()) == 0)
    {
        // dot dir exists; change back to original directory
        chdir(cwd);
        return true;
    }

    // dot dir is not existent, try to create it
    if (mkdir(dotDir.c_str(),0777) != 0)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Cannot create directory '"
                          << dotDir << "'\n";
        return false;
    }

    GetLog()->Normal() << "(ScriptServer) Created Directory '"
                       << dotDir << "'\n";

    return true;
}

bool
ScriptServer::RunInitScript(const string &fileName, const string &relPath,
                            EInitScriptType type)
{
    string dotDir;
    bool validDotDir =
        (type == IS_USERLOCAL) &&
        GetDotDirName(dotDir) &&
        CreateDotDir(dotDir);

    // some macro magic (not at all)
    string pkgdatadir = PREFIX "/share/" PACKAGE_NAME;

    bool ok =
        (
            (
                (validDotDir) && (RunInitScriptInternal(dotDir, fileName, false))
                )
            || (RunInitScriptInternal(pkgdatadir,  fileName, validDotDir, dotDir))
            || (RunInitScriptInternal(mRelPathPrefix+relPath, fileName, validDotDir, dotDir))
            );

    if (! ok)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Cannot locate init script '"
                          << fileName << "'\n";
    }

    return ok;
}

