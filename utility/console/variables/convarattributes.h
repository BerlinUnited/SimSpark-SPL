/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _CONVARATTRIBUTES_H_
#define _CONVARATTRIBUTES_H_

#include <string>
// #include <macros.h>  // contains evil stuff, try to avoid

class ConVarCallBack;

/*! \class ConVarAttributes
  $Id: convarattributes.h,v 1.1 2002/08/12 17:13:09 fruit Exp $

    ConVarAttributes

    Contains the attributes of a console variable (ConVar + ConVarState).

        defaults (as returned by getDefault):
	name            : ""
	removable       : true;
	constant        : false;
	group           : eCVG_USER;
	mCallBackObject : NULL;

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/

class ConVarAttributes
{
public:
    //! the possible groups: a ConVar belongs to arbitrary many groups
    enum EConVarGroup
    {
        //! user defined: typically created by the user with the console
        eCVG_USER               = 0x00000000,        
        /*! a ConVar which may be bound by ConBind to a key and 
            represents a command
	*/
        eCVG_COMMAND            = 0x00000001,
        //! a console configuration variable from ConsoleSettings
        eCVG_CON_SET            = 0x00000002,
        /*! a console configuration variable from ConsoleSettings: 
            specialized for color
	*/
        eCVG_CON_SET_COLOR      = 0x00000004,
        /*! a constant defined and used by ConsoleSettings: 
	    specialized for color 
	*/
        eCVG_CONST_COLOR        = 0x00000008,
        //! this makes the enum to be 32-bits
        eCVG_FORCE32BIT         = 0xffffffff    
    };

    ConVarAttributes();
    ConVarAttributes (const ConVarAttributes& attributes);
    
    //! returns a ConVarAttribute object with default values
    static ConVarAttributes getDefault();

    //! sets the name of the variable
    void setName(std::string name);
    //! returns the name of the variable
    std::string getName() const;
    /*! Sets if the variable is removable.
        If not a user may not delete it with a console command. 
    */
    void setRemovable(bool removable = true);
    //! returns if the variable should be removable with the console
    bool isRemovable() const;
    //! sets if the variable is constant: it is unchangable after construction
    void setConstant(bool constant = true);
    //! returns if the variable is constant
    bool isConstant() const;
    
    //! sets the group
    void setGroup(EConVarGroup group);
    //! returns the set of groups of which the variable is a member
    EConVarGroup getGroup() const;
    //! adds the variable to a group
    void addGroup(EConVarGroup group);
    //! removes the variable from a group
    void removeGroup(EConVarGroup group);
    //! tests the group membership
    bool isInGroup(EConVarGroup group) const;
    /*! Sets a callback method and index (ConVarCallBack). This method
        will be executed by CallBack(). It enables to signal
        immediately the change of a variable to another object
    */
    void setCallBack(ConVarCallBack* callBackObject, int callBackIndex);
    /*! Calls the callback method. This method should be called by
        the variable on every change of its contained value.
    */
    void callBack() const;
    //! sets a help string
    void setHelp(std::string help);
    //! returns the help string
    std::string getHelp() const;
    //! returns a string description of the groups
    std::string dumpGroup() const;
    //! returns a string description of the remove state
    std::string dumpRemovable() const;
    //! returns a string description of the constante state
    std::string dumpConstant() const;

protected:
    std::string mName;                    
    std::string mHelp;
    bool mRemovable;                    
    bool mConstant;                        
    EConVarGroup mGroup;
    ConVarCallBack* mCallBackObject;
    int mCallBackIndex;

    static ConVarAttributes sDefault;
};

#endif // _CONVARATTRIBUTES_H_

