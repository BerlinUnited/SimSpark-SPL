/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: rubysceneimporter.cpp,v 1.9 2004/05/07 12:09:47 rollmark Exp $

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
#include "rubysceneimporter.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <boost/scoped_array.hpp>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace std;

/**
    (RubySceneGraph <version major> <version minor>)
    (
    (template varName1 varName2 ...)
    (define varName value ...)
    (node <NodeName>
        method param param ...
        method param ...

        (node <NodeName>
            method param param ...
            method param ...
            )

            (node <NodeName>
            method param param ...
            method param ...
            )

        method param param ...
        method param ...
        )
*/

RubySceneImporter::RubySceneImporter() : SceneImporter()
{
    mVersionMajor = 0;
    mVersionMinor = 0;
    mDeltaScene = false;
    mAutoUnlink = false;
}

RubySceneImporter::~RubySceneImporter()
{
}

void RubySceneImporter::SetUnlinkOnCompleteScenes(bool unlink)
{
    mAutoUnlink = unlink;
}

bool RubySceneImporter::ImportScene(const std::string& fileName,
                                    shared_ptr<BaseNode> root,
                                    shared_ptr<ParameterList> parameter)
{
    // try to open the file
    shared_ptr<salt::RFile> file = GetFile()->Open(fileName);

    if (file.get() == 0)
        {
            GetLog()->Error() << "(RubySceneImporter) ERROR: cannot open file '"
                              << fileName << "'\n";
            return false;
        }

    mFileName = fileName;

    // read entire file into a temporary buffer
    scoped_array<char> buffer(new char[file->Size() + 1]);
    file->Read(buffer.get(), file->Size());
    buffer[file->Size()] = 0;

    return ParseScene(buffer.get(), file->Size(), root, parameter);
}

bool RubySceneImporter::ParseScene(const std::string& scene,
                                   shared_ptr<BaseNode> root,
                                   shared_ptr<ParameterList> parameter)
{
    mFileName = "<from string>";
    return ParseScene(scene.c_str(),scene.size(),root,parameter);
}

bool RubySceneImporter::ParseScene(const char* scene, int size,
                                   boost::shared_ptr<oxygen::BaseNode> root,
                                   boost::shared_ptr<zeitgeist::ParameterList> parameter)
{
    // parse s-expressions
    pcont_t* pcont = init_continuation(const_cast<char*>(scene));
    sexp_t* sexp = iparse_sexp(const_cast<char*>(scene),size,pcont);

    // read scene magic and version
    if (
        (sexp == 0) ||
        (! ReadHeader(sexp)) ||
        (mVersionMajor != 0) ||
        (mVersionMinor != 1)
        )
        {
            destroy_sexp(sexp);
            destroy_continuation(pcont);
            return false;
        }

    // advance to next sexpression- the scene graph
    PushParameter(parameter);

    destroy_sexp(sexp);
    sexp = iparse_sexp(const_cast<char*>(scene),size,pcont);

    if (
        (! mDeltaScene) &&
        (mAutoUnlink)
        )
        {
            root->UnlinkChildren();
        }

    bool ok = mDeltaScene ?
        ReadDeltaGraph(sexp,root) :
        ReadGraph(sexp,root);

    destroy_sexp(sexp);
    destroy_continuation(pcont);

    PopParameter();
    return ok;
}

void RubySceneImporter::PushParameter(shared_ptr<ParameterList> parameter)
{
    mParameterStack.push_back(ParamEnv(parameter));
}

void RubySceneImporter::PopParameter()
{
    if (mParameterStack.empty())
        {
            GetLog()->Debug()
                << "(RubySceneImporter) ERROR: PopParameter "
                << "called on empty stack\n";
            return;
        }

    mParameterStack.pop_back();
}

RubySceneImporter::ParamEnv& RubySceneImporter::GetParamEnv()
{
    if (mParameterStack.empty())
        {
            GetLog()->Debug()
                << "(RubySceneImporter) ERROR: GetParamEnv "
                << "called on empty stack\n";

            static ParamEnv nullEnv;
            return nullEnv;
        }

    return mParameterStack.back();
}

bool RubySceneImporter::ReadHeader(sexp_t* sexp)
{
    // (RubySceneGraph <int majorVersion> <int minorVersion>)
    if (
        (sexp == 0) ||
        (sexp->ty != SEXP_LIST) ||
        (sexp->list == 0)
        )
        {
            return false;
        }

    sexp = sexp->list;
    if (sexp->ty != SEXP_VALUE)
        {
            return false;
        }

    string val(sexp->val);

    mDeltaScene = false;
    if (val == "RubyDeltaScene")
        {
            mDeltaScene = true;
        }  else if (val != "RubySceneGraph")
            {
                return false;
            }

    // try to advance to version number
    sexp = sexp->next;
    if (
        (sexp == 0) ||
        (sexp->ty != SEXP_VALUE)
        )
        {
            return false;
        }

    string mastr(sexp->val);
    int major = atoi(mastr.c_str());

    if (major < 0)
        {
            return false;
        }

    sexp = sexp->next;

    if (
        (sexp == 0) ||
        (sexp->ty != SEXP_VALUE)
        )
        {
            return false;
        }

    string mistr(sexp->val);
    int minor = atoi(mistr.c_str());

    if (minor < 0)
        {
            return false;
        }

    mVersionMajor = major;
    mVersionMinor = minor;

    return true;
}

shared_ptr<Object> RubySceneImporter::CreateInstance(const string& className)
{
    static const string prefixes[] =
        {
            "",
            "oxygen/",
            "kerosin/"
        };

    const int n = sizeof(prefixes)/sizeof(string);
    for (int i=0;i<n;++i)
        {
            string name = prefixes[i] + className;
            if (GetCore()->ExistsClass(name))
                {
                    return GetCore()->New(name);
                }
        }

    return shared_ptr<Object>();
}

shared_ptr<BaseNode> RubySceneImporter::CreateNode(sexp_t* sexp)
{
    if (sexp == 0)
        {
            return shared_ptr<BaseNode>();
        }

    string className(sexp->val);

    // create a class instance
    shared_ptr<Object> obj = CreateInstance(className);

    if (obj.get() == 0)
        {
            GetLog()->Error()
                << "(RubySceneImporter) ERROR: in file '" << mFileName
                << "': unknown class '"
                << className << "'\n";
            return shared_ptr<BaseNode>();
        }

    shared_ptr<BaseNode> node = shared_dynamic_cast<BaseNode>(obj);

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(RubySceneImporter) ERROR: in file '" << mFileName
                << className << "': is not derived from BaseNode'\n";
            return shared_ptr<BaseNode>();
        }

    return node;
}

bool RubySceneImporter::ReadMethodCall(sexp_t* sexp, shared_ptr<BaseNode> node)
{
    if (sexp == 0)
        {
            return false;
        }

    // read the method name
    string method = sexp->val;
    sexp = sexp->next;

    // collect the parameters
    const ParamEnv& env = GetParamEnv();
    ParameterList parameter;

    while (sexp != 0)
        {
            if (sexp->ty != SEXP_VALUE)
                {
                    GetLog()->Error()
                        << "(RubySceneImporter) ERROR: in file '"
                        << mFileName
                        << "': parameterlist contains another list\n";
                    return false;
                }

            string param = sexp->val;

            if (param.size() == 0)
                {
                    continue;
                }

            if (param[0] == '$')
                {
                    // replace a template parameter
                    param.erase(param.begin(),param.begin()+1);
                    TParameterMap::const_iterator iter = env.parameterMap.find(param);

                    if (iter == env.parameterMap.end())
                        {
                            GetLog()->Error()
                                << "(RubySceneImporter) ERROR: in file '"
                                << mFileName
                                << "': unknown parameter '"
                                << param << "'\n";
                            return false;
                        }

                    int idx = (*iter).second;

                    if (idx >= env.parameter->GetSize())
                        {
                            GetLog()->Error()
                                << "(RubySceneImporter) ERROR: in file '"
                                << mFileName
                                << "': parameter value '"
                                << param << "' not supplied\n";
                            return false;
                        }

                    string value;
                    ParameterList::TVector::const_iterator pIter = (*env.parameter)[idx];
                    if (! env.parameter->AdvanceValue(pIter,value))
                        {
                            GetLog()->Error()
                                << "(RubySceneImporter) ERROR: in file '"
                                << mFileName
                                << "': failed to read parameter value '"
                                << param << "'\n";
                            return false;
                        }

                    param = value;

                }

            parameter.AddValue(param);
            sexp = sexp->next;
        }

    // invoke the method on the object
    shared_ptr<Class> theClass = node->GetClass();

    if (theClass.get() == 0)
        {
            GetLog()->Error()
                << "(RubySceneImporter) ERROR: cannot get class object for node "
                << node->GetFullPath() << "\n";
            return false;
        }

    if (! theClass->SupportsCommand(method))
        {
            GetLog()->Error()
                << "(RubySceneImporter) ERROR: in file '" << mFileName
                << "': unknown method name '"
                << method << "' for node '" << node->GetFullPath()
                << "' (a " << theClass->GetName() << ")\n";
            return false;
        }

    node->Invoke(method, parameter);
    return true;
}

bool RubySceneImporter::ParseTemplate(sexp_t* sexp)
{
    if (sexp == 0)
        {
            return false;
        }

    ParamEnv& env = GetParamEnv();

    while (
           (sexp != 0) &&
           (sexp->ty == SEXP_VALUE)
           )
        {
            string param = sexp->val;

            if (param.size() == 0)
                {
                    sexp = sexp->next;
                    continue;
                }

            if (
                (param[0] != '$') ||
                (param.size() < 2)
                )
                {
                    GetLog()->Error()
                        << "(RubySceneImporter) ERROR: in file '" << mFileName
                        << "': template parameter name expected\n";
                    return false;
                }

            param.erase(param.begin(),param.begin()+1);
            TParameterMap::const_iterator iter = env.parameterMap.find(param);

            if (iter != env.parameterMap.end())
                {
                    GetLog()->Error()
                        << "(RubySceneImporter) ERROR: in file '" << mFileName
                        << "': duplicate template parameter name '" << param << "'\n";
                    return false;
                }

            int idx = env.parameterMap.size();
            env.parameterMap[param] = idx;
            sexp = sexp->next;
        }

    return true;
}

bool RubySceneImporter::ReadDeltaGraph(sexp_t* sexp, shared_ptr<BaseNode> root)
{
    TLeafList::const_iterator iter = root->begin();

    while (sexp != 0)
        {
            switch (sexp->ty)
                {
                case SEXP_VALUE:
                    {
                        string name(sexp->val);

                        if (name == "node")
                            {
                                while (
                                       (sexp != 0) &&
                                       (sexp->ty != SEXP_LIST)
                                       )
                                    {
                                        sexp = sexp->next;
                                    }
                                continue;
                            } else
                                {
                                    return ReadMethodCall(sexp, root);
                                }
                    }
                    break;

                case SEXP_LIST:
                    {
                        sexp_t* sub = sexp->list;
                        if (sub != 0)
                            {
                                shared_ptr<BaseNode> node;

                                if (
                                    (sub->ty == SEXP_VALUE) &&
                                    (string(sub->val) == "node")
                                    )
                                    {
                                        node = shared_dynamic_cast<BaseNode>(*iter);
                                        if (iter != node->end())
                                            {
                                                ++iter;
                                            }
                                    } else
                                        {
                                            node = root;
                                        }

                                if (! ReadDeltaGraph(sub, node))
                                    {
                                        return false;
                                    }
                            }
                    }
                    break;

                default:
                    return false;

                }

            sexp = sexp->next;
        }

    return true;
}

bool RubySceneImporter::ReadGraph(sexp_t* sexp, shared_ptr<BaseNode> root)
{
    while (sexp != 0)
        {
            switch (sexp->ty)
                {
                case SEXP_VALUE:
                    {
                        string name(sexp->val);

                        if (name == "node")
                            {
                                sexp = sexp->next;
                                shared_ptr<BaseNode> node
                                    = CreateNode(sexp);

                                if (node.get() == 0)
                                    {
                                        return false;
                                    }

                                root->AddChildReference(node);
                                root = node;
                            } else if (name == "template")
                                {
                                    sexp = sexp->next;
                                    return ParseTemplate(sexp);
                                } else
                                    {
                                        return ReadMethodCall(sexp, root);
                                    }
                    }
                    break;

                case SEXP_LIST:
                    if (! ReadGraph(sexp->list,root))
                        {
                            return false;
                        }
                    break;

                default:
                    return false;
                }

            sexp = sexp->next;
        }

    return true;
}




