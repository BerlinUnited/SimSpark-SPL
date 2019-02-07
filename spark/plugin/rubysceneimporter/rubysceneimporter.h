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
#ifndef RUBYSCENEIMPORTER_H
#define RUBYSCENEIMPORTER_H

#include <oxygen/sceneserver/sceneimporter.h>
#include <oxygen/sceneserver/basenode.h>
#include <sfsexp/sexp.h>

class RubySceneImporter : public oxygen::SceneImporter
{
protected:
    //! mapping from parameter name to parameter index
    typedef std::map<std::string, int> TParameterMap;

    //! defines a method invocation
    struct MethodInvocation
    {
        boost::weak_ptr<zeitgeist::Node> node;
        std::string method;
        zeitgeist::ParameterList parameter;
    };

    typedef std::list<MethodInvocation> TMethodInvocationList;

    //! a parameter environment
    struct ParamEnv
    {
        TParameterMap parameterMap;
        boost::shared_ptr<zeitgeist::ParameterList> parameter;
        TMethodInvocationList invocationList;

        ParamEnv() {};
        ParamEnv(boost::shared_ptr<zeitgeist::ParameterList> p)
            : parameter(p) {};
    };

    typedef std::list<ParamEnv> TParameterStack;

    //! mapping from abbreviations to key words
    typedef std::map<std::string, std::string> TTranslationTable;

public:
    RubySceneImporter();
    virtual ~RubySceneImporter();

    virtual bool ImportScene(const std::string& fileName,
                             boost::shared_ptr<oxygen::BaseNode> root,
                             boost::shared_ptr<zeitgeist::ParameterList> parameter);

    virtual bool ParseScene(const std::string& scene,
                             boost::shared_ptr<oxygen::BaseNode> root,
                             boost::shared_ptr<zeitgeist::ParameterList> parameter);

    /** constrols the behavior of the importer for complete scenes
        (instead of delta scenes). if 'unlink' is true, the scene
        below the root node given in ImportScene will automatically be
        unlinked before the scene is imported
     */
    void SetUnlinkOnCompleteScenes(bool unlink);

    /** registers all created nodes in the RubySceneDict */
    void EnableSceneDictionary(bool enable);

protected:
    virtual bool ParseScene(const char* scene, int size,
                             boost::shared_ptr<oxygen::BaseNode> root,
                             boost::shared_ptr<zeitgeist::ParameterList> parameter);


    bool ReadHeader(sexp_t* sexp);
    bool ReadGraph(sexp_t* sexp, boost::shared_ptr<oxygen::BaseNode> root);
    bool ReadDeltaGraph(sexp_t* sexp, boost::shared_ptr<oxygen::BaseNode> root);
    boost::shared_ptr<zeitgeist::Object> CreateInstance(const std::string& className);

    void PushInvocation(const MethodInvocation& invoc);
    bool Invoke(const MethodInvocation& invoc);
    bool InvokeMethods();
    bool ReadMethodCall(sexp_t* _sexp, boost::shared_ptr<oxygen::BaseNode> node);

    bool ReplaceVariable(std::string& param);
    bool EvalParameter(sexp_t* sexp, std::string& value);

    boost::shared_ptr<oxygen::BaseNode> CreateNode(sexp_t* sexp);
    bool ParseTemplate(sexp_t* sexp);
    bool ParseDefine(sexp_t* sexp);
    // bool ParseSwitch(sexp_t* sexp, std::string& value);
    bool ParseSwitch(sexp_t* sexp, boost::shared_ptr<oxygen::BaseNode> root);

    void PushParameter(boost::shared_ptr<zeitgeist::ParameterList> parameter);
    void PopParameter();
    ParamEnv& GetParamEnv();

    void InitTranslationTable();

    std::string Lookup(const std::string& key);


protected:
    /** true if a scene is automatically unlinked before a new scene
        is imported */
    bool mAutoUnlink;

    /** true, if a delta scene is applied */
    bool mDeltaScene;

    /** true, if all created nodes are registered in the RubySceneDict */
    bool mUpdateSceneDict;

    /** the major version of the scen graph file */
    int mVersionMajor;

    /** the minor version of the scen graph file */
    int mVersionMinor;

    /** the last supplied fileName */
    std::string mFileName;

    /** a stack of parameter environments */
    TParameterStack mParameterStack;

    /** the abbreviaton table */
    TTranslationTable mTranslationTable;

    /** the s-expression library memory management object */
    sexp_mem_t *mSexpMemory;
};

DECLARE_CLASS(RubySceneImporter);

#endif // RUBYSCENEIMPORTER_H
