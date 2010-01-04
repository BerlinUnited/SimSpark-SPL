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

        ScriptServer
        :TODO: Class description for ScriptServer

        HISTORY:
                09.07.2002 MK
                        - initial version

*/
#ifndef ZEITGEIST_SCRIPTSERVER_H
#define ZEITGEIST_SCRIPTSERVER_H

#include <zeitgeist/leaf.h>
#include <zeitgeist/class.h>
#include <salt/fileclasses.h>
#include "gcvalue.h"

namespace zeitgeist
{

class CoreContext;

/** The ScriptServer provides scripting facilities to the zeitgeist
    framework. Up to now is realized using Ruby, though the
    ScritpServer could easily be extended to support other script
    languages.
*/
class ZEITGEIST_API ScriptServer : public Leaf
{
    //
    // types
    //
public:
    enum EInitScriptType
        {
            IS_COMMON,  // the init script is common for all users,
                        // i.e. it won't be copied to the local dot
                        // directory
            IS_USERLOCAL // the init script is local to the user, i.e
                         // it will be copied to the user's dot
                         // direcotry
        };

    enum ERunScriptErrorType
    {
        eUninit,   // not initialized
        eOK,       // no errors
        eNotFound, // could not find script
        eError    // some error occured while executing the script
    };


protected:
private:

    //
    // functions
    //
public:
    /** constructs the ScriptServer */
    ScriptServer();
    virtual ~ScriptServer();

    /** loads and runs a ruby script, returns true on success */
    bool Run(const std::string &fileName);

    /** loads and runs a ruby script from an RFile */
    bool Run(boost::shared_ptr<salt::RFile> file);

    /** sets the relative path prefix RunInitScript() searches for an
        InitScript. Default is '../../', suitable for all applications
        started from '/app'
     */
    void SetInitRelPathPrefix(const std::string &relPathPrefix);

    /** searches in ~/<mDotName>/ if dotScript is true,
     * PREFIX/share/PACKAGE_NAME/ and <mRelPathPrefix><relPath>/ for
     * the script <fileName>.  If found the script is run and if
     * dotScript is true copied to ~/<dotName>/. If this directory is
     * missing it is automatically created
     */
    bool RunInitScript(const std::string &fileName, const std::string &relPath,
                       EInitScriptType type = IS_USERLOCAL);

    /** sets name of the dot directory */
    void SetDotName(const std::string &dotName) { mDotName = dotName; }

    /** evaluates a ruby statement, returns true on success */
    bool Eval(const std::string &command);

    /** evaluates a ruby statement, returns true on success; \param
        value receives the result value if any */
    bool Eval(const std::string &command, GCValue& value);

    /** notify all nodes to update their cached references */
    void UpdateCachedAllNodes();

    // script variable functions

    /** returns true if the variable varName exists */
    bool ExistsVariable(const std::string &varName);

    /** creates a ruby integer variable */
    void CreateVariable(const std::string &varName, int value);

    /** creates a ruby float variable */
    void CreateVariable(const std::string &varName, float value);

    /** creates a ruby string variable */
    void CreateVariable(const std::string &varName, const std::string &value);

    /** reads the value of a ruby integer, returns true on success */
    bool GetVariable(const std::string &varName, int &value);

    /** reads the value of a ruby boolean, returns true on success */
    bool GetVariable(const std::string &varName, bool &value);

    /** reads the value of a ruby string, returns true on success */
    bool GetVariable(const std::string &varName, std::string &value);

    /** reads the value of a ruby float, returns true on success */
    bool GetVariable(const std::string &varName, float &value);

    /** returns the context, the ScriptServer operates in */
    boost::shared_ptr<CoreContext> GetContext() const;

    /** creates dot directory and adds it to resource search paths */
    bool SetupDotDir();

    /** constructs the ZeitgeistObject corresponding to a given
        leaf */
    static GCValue GetZeitgeistObject(boost::shared_ptr<Leaf> leaf);

protected:
    /** initializes the ScriptServer and runs the default startup
        script 'sys/script/zeitgeist.rb', returning true on
        success.
    */
    virtual bool ConstructInternal();

    /** parses a ruby variable into a namespace and variable name
        part. Valid variable names are 'Namespace.Name'.

        \returns true if a valid variable name was parsed

        \param varName is the variable name to parse
        \param nameSpace receives the namespace
        \param name receives the variable name
     */
    bool ParseVarName(const std::string& varName,
                      std::string& nameSpace, std::string& name);

    /** returns the content of a variable*/
    GCValue GetVariable(const std::string &varName);

    /** private helper function */
    ERunScriptErrorType RunInitScriptInternal(const std::string &dir, const std::string &name,
                                              bool copy,  const std::string& destDir = "");

    /** construct the path of the local dot directory that contains
        the users init scripts
     */
    bool GetDotDirName(std::string& dotDir);

    /** checks if the directory <dotDir> exists and if not creates it
     */
    bool CreateDotDir(const std::string& dotDir);

private:
    ScriptServer(const ScriptServer& obj);
    ScriptServer& operator=(const ScriptServer& obj);
    //
    // members
    //
public:
protected:
    /** the name of the users dot directory, used by ::RunInitScript to search
     * for script files: ~/<mDotName>
     */
    std::string mDotName;

    /** the prefix RunInitScript() prepends all relativ paths
     */
    std::string mRelPathPrefix;

private:
  };

DECLARE_CLASS(ScriptServer);

} //namespace zeitgeist

#endif //ZEITGEIST_SCRIPTSERVER_H
