/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: scriptserver.h,v 1.5 2003/09/26 12:22:10 fruit Exp $

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

#ifndef SCRIPTSERVER_H__
#define SCRIPTSERVER_H__

#ifndef __GNUC__
#define EXTERN extern __declspec(dllimport)
#endif
#include <ruby.h>
#undef EXTERN

#include <zeitgeist/leaf.h>
#include <zeitgeist/class.h>

namespace zeitgeist
{

class CoreContext;

/** The ScriptServer provides scripting facilities to the zeitgeist
    framework. Up to now is realized using Ruby, though the
    ScritpServer could easily be extended to support other script
    languages.
 */
class ScriptServer : public Leaf
{
        //
        // types
        //
public:
protected:
private:

        //
        // functions
        //
public:
  /** constructs the ScriptServer */
        ScriptServer();
        virtual ~ScriptServer();

        /** loads an runs a ruby script, returns true on success */
        bool Run(const std::string &fileName);

        /** evaluates a ruby statement, returns true on success */
        bool Eval(const std::string &command);

        // script variable functions

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

        /** returns the context, the ScriptServer operates in */
        boost::shared_ptr<CoreContext> GetContext() const;

protected:
        /** initializes the ScriptServer and runs the default startup
            script 'sys/script/zeitgeist.rb', returning true on
            success.
        */
        virtual bool ConstructInternal();

private:
        ScriptServer(const ScriptServer& obj);
        ScriptServer& operator=(const ScriptServer& obj);

        VALUE GetVariable(const std::string &varName);

        //
        // members
        //
public:
protected:
private:
};

DECLARE_CLASS(ScriptServer);

} //namespace zeitgeist

#endif //SCRIPTSERVER_H__
