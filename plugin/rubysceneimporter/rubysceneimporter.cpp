/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: rubysceneimporter.cpp,v 1.1 2004/04/08 14:56:02 rollmark Exp $

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
#include <boost/scoped_array.hpp>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace std;

/**

    (RubySceneGraph <version major> <version minor>)
    (
    (NodeName
        .MethodCall param param ...
        .MethodCall param ...
        (NodeName
            .MethodCall param param
            .MethodCall param param
        )
        (NodeName
            .MethodCall param param
            .MethodCall param param
        )
        .MethodCall param param ...
        .MethodCall param ...
    )
*/

RubySceneImporter::RubySceneImporter() : SceneImporter()
{
    mVersionMajor = 0;
    mVersionMinor = 0;
}

RubySceneImporter::~RubySceneImporter()
{
}

bool RubySceneImporter::ImportScene(const shared_ptr<salt::RFile> file,
                                    shared_ptr<BaseNode> root)
{
    if (file.get() == 0)
        {
            return false;
        }

    // read entire file into a temporary buffer
    scoped_array<char> buffer(new char[file->Size() + 1]);
    file->Read(buffer.get(), file->Size());
    buffer[file->Size()] = 0;

    // parse s-expressions
    pcont_t* pcont = init_continuation(buffer.get());
    sexp_t* sexp = iparse_sexp(buffer.get(),file->Size(),pcont);

    // read file magic and version
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
    destroy_sexp(sexp);
    sexp = iparse_sexp(buffer.get(),file->Size(),pcont);

    bool ok = ReadGraph(sexp,root);

    destroy_sexp(sexp);
    destroy_continuation(pcont);

    return ok;
}

bool RubySceneImporter::ReadHeader(sexp_t* sexp)
{
    // (RubySceneGraph <float version>)
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

    if (val != "RubySceneGraph")
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


    GetLog()->Normal()
        << "(RubySceneImporter) reading ruby scene graph version "
        << major << "." << minor << "\n";

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
                << "(RubySceneImporter) ERROR: unknown class '"
                << className << "'\n";
            return shared_ptr<BaseNode>();
        }

    shared_ptr<BaseNode> node = shared_dynamic_cast<BaseNode>(obj);

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(RubySceneImporter) ERROR: class '"
                << className << " is not derived from BaseNode'\n";
            return shared_ptr<BaseNode>();
        }

    return node;
}

bool RubySceneImporter::ReadMethodCall(sexp_t** sexp, shared_ptr<BaseNode> node)
{
    if (
        (sexp == 0) ||
        ((*sexp) == 0) ||
        ((*sexp)->ty != SEXP_VALUE)
        )
        {
            return false;
        }


    // read the method name
    string method = ((*sexp)->val);

    if (
        (method.size() < 2) ||
        (method[0] != '.')
        )
        {
            GetLog()->Error()
                << "(RubySceneImporter) ERROR: invalid method name '"
                << method << "'\n";
            return false;
        }

    method.erase(method.begin(),method.begin()+1);
    (*sexp) = (*sexp)->next;

    // collect the parameters
    ParameterList parameter;

    while (
           ((*sexp) != 0) &&
           ((*sexp)->ty == SEXP_VALUE)
           )
        {
            string param = (*sexp)->val;

            if (param.size() == 0)
                {
                    continue;
                }

            if (param[0] == '.')
                {
                    break;
                }

            parameter.AddValue(param);
            (*sexp) = (*sexp)->next;
        }

    // invoke the method on the object
    // TODO: check if methodname is valid
    node->Invoke(method, parameter);

    return true;
}

bool RubySceneImporter::InvokeMethods(sexp_t** sexp, shared_ptr<BaseNode> node)
{
    if (
        (node.get() == 0) ||
        (sexp == 0) ||
        ((*sexp) == 0)
        )
        {
            return false;
        }

    while ((*sexp) != 0)
        {
            if ((*sexp)->ty == SEXP_LIST)
                {
                    return true;
                }

            if (! ReadMethodCall(sexp,node))
                {
                    return false;
                }
        }

    return true;
}

bool RubySceneImporter::ReadGraph(sexp_t* sexp, shared_ptr<BaseNode> root)
{
    if (sexp == 0)
        {
            return false;
        }

    bool firstValue = true;
    shared_ptr<BaseNode> node;

    while (sexp != 0)
        {
            switch (sexp->ty)
                {
                case SEXP_VALUE:
                    if (firstValue)
                        {
                            // a node declaration
                            node = CreateNode(sexp);

                            if (node.get() == 0)
                                {
                                    return false;
                                }

                            // add node to the scene graph
                            root->AddChildReference(node);
                            root = node;
                        } else
                            {
                                // list of method invokations
                                if (! InvokeMethods(&sexp,node))
                                    {
                                        return false;
                                    }

                                continue;
                            }

                    firstValue = false;
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




