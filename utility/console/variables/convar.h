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
#ifndef _CONVAR_H_
#define _CONVAR_H_

// #include <macros.h> // contains evil stuff
#include <list>

#include "convarattributes.h"

class ConVarState;
class ConCommand;
class Vector3;
class ConVarCallBack;

/*! \class ConVar
  $Id: convar.h,v 1.1 2002/08/12 17:13:09 fruit Exp $

    ConVar - Console Variables

    A wrapper around a variable which provides runtime information
    like the type, the name, a group membership, if it is constant,
    ...

    Variables of the basic and for the console important types may be
    stored: int, float, string, ConCommand, Vector3 and of course
    another ConVar.

    The contained value may be stored by copy or reference depending
    on the used variable type, i.e. the memory space of the wrapped
    variable my be a part of the ConVar or it may be just a reference
    to a variable defined somewhere else in the program.

    ConVars must be created with the named constructors ConVar*
    createXXX(...). createUndefined() creates a generic variable
    whose type will be replaced with the first value
    assignment. I.e. an undefined variable will be changed to an int
    variable with setInt(...).
  
    On definition of a new variable type
    - ConVar::EConVarType has to be extended
    - an appropriate subclass of ConVarState has to be defined
    - an appropriate named constructor (createXXX) has to be added to
      ConVar and ConVarServer
    - appropriate set/get methods might have to be added to ConVar and
      ConVarState
    - ConVar::IsConvertableTo has to be extended

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/

class ConVar
{
public:
    //! the possible types: a ConVar has exactly one type
    enum EConVarType
    {
        //! The type has still to be defined -> no value has been be stored
        eCVT_UNDEFINED            = 0x00000000,        
        //! an integer
        eCVT_INT                    = 0x00000001,        
        //! a floating point number
        eCVT_FLOAT                = 0x00000002,        
        //! a string
        eCVT_STRING                = 0x00000003,        
        //! a char[] string
        eCVT_CHARSTRING            = 0x00000004,        
        //! a vector from the support library
        eCVT_VECTOR                = 0x00000005,        
        //! a reference to an integer
        eCVT_REF_INT                = 0x00000006,        
        //! a reference to a floating point number
        eCVT_REF_FLOAT            = 0x00000007,        
        //! a reference to a string
        eCVT_REF_STRING            = 0x00000008,        
        //! a reference to a char[] string
        eCVT_REF_CHARSTRING        = 0x00000009,        
        //! a reference to a vector from the support library
        eCVT_REF_VECTOR            = 0x0000000A,        
        //! a ConVar
        eCVT_VARIABLE            = 0x0000000B,        
        /*! A command: the value must be >0, 
	    see ConsoleBindings for more detailed information.
	*/
        eCVT_COMMAND                = 0x0000000C,        
        //! a bool
        eCVT_BOOL                = 0x0000000D,
        //! a reference to a bool
        eCVT_REF_BOOL            = 0x0000000E,
        //! a label; used by the ConsoleParser, not in the ConVarServer
        eCVT_LABEL                = 0x0000000F,
        //! a label; used by the ConsoleParser, not in the ConVarServer
        eCVT_SEPARATOR            = 0x00000010,
        
        //! this makes the enum to be 32-bits
        eCVT_FORCE32BIT            = 0xffffffff         
    };



    //! a container for the types of a ConVar
    typedef std::list<EConVarType>    tEConVarTypes;
    //! a container of ConVars
    typedef std::vector<ConVar*>        tConVars;

    ~ConVar();
    
    ConVar* Clone() const;
    
    //! named constructors
    //! if no attributes are given the default attributes are taken (ConVarAttributes::getDefault)
    //! the ConVar* stores a copy of the provided value.
    //! the reference types (CreateRefxxx) store only a reference to the supplied value,
    //! therefore its deletion remains the responsibility of the caller
    static ConVar* createUndefined(const ConVarAttributes& attributes = 
				   ConVarAttributes::getDefault());
    static ConVar* createBool(bool value, const ConVarAttributes& attributes =
			      ConVarAttributes::getDefault());
    static ConVar* createInt(int value, const ConVarAttributes& attributes = 
			     ConVarAttributes::getDefault());
    static ConVar* createFloat(float value, const ConVarAttributes& attributes =
			       ConVarAttributes::getDefault());
    static ConVar* createString(const std::string& value, 
				const ConVarAttributes& attributes = 
				ConVarAttributes::getDefault());
    static ConVar* createCharString(const char* value, 
				    const ConVarAttributes& attributes = 
				    ConVarAttributes::getDefault());
    static ConVar* createVector(float x, float y, float z, 
				const ConVarAttributes& attributes = 
				ConVarAttributes::getDefault());
    static ConVar* createVector(const Vector3& value, const ConVarAttributes& 
				attributes = ConVarAttributes::getDefault());
    static ConVar* createCommand(const ConCommand* value, 
				 const ConVarAttributes& attributes = 
				 ConVarAttributes::getDefault());
    static ConVar* createVariable(ConVar* value, 
				  const ConVarAttributes& attributes = 
				  ConVarAttributes::getDefault());
    static ConVar* createRefBool(bool* value, 
				 const ConVarAttributes& attributes = 
				 ConVarAttributes::getDefault());
    static ConVar* createRefInt(int* value, 
				const ConVarAttributes& attributes = 
				ConVarAttributes::getDefault());
    static ConVar* createRefFloat(float* value, 
				  const ConVarAttributes& attributes = 
				  ConVarAttributes::getDefault());
    static ConVar* createRefString(std::string* value, 
				   const ConVarAttributes& attributes = 
				   ConVarAttributes::getDefault());
    /*! The referenced char string must already be defined as it will
        be deleted on its change.
    */
    static ConVar* createRefCharString(char** value, 
				       const ConVarAttributes& attributes = 
				       ConVarAttributes::getDefault());
    static ConVar* createRefVector(Vector3* value, 
				   const ConVarAttributes& attributes = 
				   ConVarAttributes::getDefault());
    static ConVar* createLabel(const std::string& value, 
			       const ConVarAttributes& attributes = 
			       ConVarAttributes::getDefault());
    static ConVar* createSeparator(const std::string& value, 
				   const ConVarAttributes& attributes = 
				   ConVarAttributes::getDefault());
    static ConVar* createSeparator(const char value, 
				   const ConVarAttributes& attributes = 
				   conVarAttributes::getDefault());
    
    //! returns the attributes of the variable
    ConVarAttributes getAttributes() const;
    //! sets the attributes of the variable
    void setAttributes(const ConVarAttributes& attributes);
    
    //! which type has this variable?
    EConVarType getType() const;
    //! is this variable of the given type?
    bool isOfType (EConVarType type) const;
    //! may this ConVar be converted to a ConVar of the given type? 
    //! e.g. a eCVT_INT may be converted to a eCVT_FLOAT
    bool isConvertibleTo(EConVarType type) const;

    //! is this ConVar of the asked type?
    bool isBool() const;
    bool isInt() const;
    bool isFloat() const;
    bool isString() const;
    bool isCharString() const;
    bool isVector() const;
    bool isRefBool() const;
    bool isRefInt() const;
    bool isRefFloat() const;
    bool isRefString() const;
    bool isRefCharString() const;
    bool isRefVector() const;
    bool isVariable() const;
    bool isCommand() const;
    bool isUndefined() const;
    bool isLabel() const;
    bool isSeparator() const;

    /*! These methods set the value of the variabel, if
        - the value is convertable to the variables type
        - or the variables is currently of type undefined.
          In this case it will change its type to the type given in
          the set method, e.g. setInt will replace an undefined
          variable with a variable of type int.
    
	The setRefXXX methods are only applicable on an undefined
	variable.  the value will be copied, its garbage collection is
	the responsibility of the caller
    */
    bool setBool(const bool value);
    bool setInt(const int value);
    bool setFloat(const float value);
    bool setString(const std::string& value);
    bool setCharString(const char* value);
    bool setVector(const Vector3& value);
    bool setVector(const float x, const float y, const float z);
    bool setRefBool(bool* value);
    bool setRefInt(int* value);
    bool setRefFloat(float* value);
    bool setRefString (std::string* value);
    bool setRefCharString (char** value);
    bool setRefVector(Vector3* value);
    bool setCommand(const ConCommand* value);
    bool setVariable(ConVar* value);
    bool setLabel(const std::string& value);
    bool setSeparator(const std::string& value);
    bool setSeparator(const char value);

    /*! These methods copy the ConVar value into the parameters 
        if a conversion is possible (isConvertible)
    */
    bool getBool(bool& value) const;
    bool getInt(int& value) const;
    bool getFloat(float& value) const;
    bool getString(std::string& value) const;
    bool getVector(Vector3& value) const;
    bool getCharString(char** value) const;
    bool getVariable(ConVar** value) const;
    bool getCommand(const ConCommand** value) const;
    bool getLabel(std::string& value) const;
    bool getSeparator(std::string& value) const;

    //! returns a short variable description: name and value
    std::string dump() const;
    //! returns a long variable description: every attribute
    std::string dumpWithSignature() const;
    //! returns the value
    std::string dumpValue() const;

protected:
    //! the contained variable state, i.e. the real variable
    ConVarState* mState;

    //! a ConVar should only be created with a named constructor (createXXX)
    ConVar(ConVarState* state);
    ConVar(const ConVar& conVar);
    void operator =(const ConVar& conVar);
};

#endif // _CONVAR_H_
