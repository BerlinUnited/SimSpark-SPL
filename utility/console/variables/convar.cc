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
#include "convar.h"
#include "convarstate.h"
#include "convarundefined.h"
#include "convarbool.h"
#include "convarint.h"
#include "convarfloat.h"
#include "convarstring.h"
#include "convarcharstring.h"
#include "convarvector.h"
#include "convarrefbool.h"
#include "convarrefint.h"
#include "convarreffloat.h"
#include "convarrefstring.h"
#include "convarrefcharstring.h"
#include "convarrefvector.h"
#include "convarcommand.h"
#include "convarvariable.h"
#include "convarlabel.h"
#include "convarseparator.h"

using namespace std;

ConVar::ConVar(ConVarState* state)
{
    mState = state;
}

ConVar::ConVar(const ConVar& conVar)
{
    *mState = *conVar.mState;
}

void 
ConVar::operator =(const ConVar& conVar)
{
    *mState = *conVar.mState;
}

ConVar::~ConVar() 
{
    delete mState;
}

ConVar* 
ConVar::clone() const
{
    return new ConVar(mState->clone());
}

ConVar* 
ConVar::createUndefined(const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarUndefined(attributes));
}

ConVar* 
ConVar::createBool(bool value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarBool(attributes, value));
}

ConVar* 
ConVar::createInt(int value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarInt(attributes, value));
}

ConVar* 
ConVar::createFloat(float value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarFloat(attributes, value));
}

ConVar* 
ConVar::createString(const std::string& value, 
           const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarString(attributes, value));
}

ConVar* 
ConVar::createCharString(const char* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarCharString(attributes, value));
}

ConVar* 
ConVar::createVector(const Vector3& value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarVector(attributes, value));
}

ConVar* 
ConVar::createVector(float x, float y, float z, 
           const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarVector(attributes, Vector3(x, y, z)));
}

ConVar* 
ConVar::createCommand(const ConCommand* value, 
            const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarCommand(attributes, value));
}

ConVar* 
ConVar::createVariable(ConVar* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarVariable(attributes, value));
}

ConVar* 
ConVar::createRefBool(bool* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefBool(attributes, value));
}

ConVar* 
ConVar::createRefInt(int* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefInt(attributes, value));
}

ConVar* 
ConVar::createRefFloat(float* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefFloat(attributes, value));
}

ConVar* 
ConVar::createRefString(string* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefString(attributes, value));
}

ConVar* 
ConVar::createRefCharString(char** value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefCharString(attributes, value));
}

ConVar* 
ConVar::createRefVector(Vector3* value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefVector(attributes, value));
}

ConVar* 
ConVar::createLabel(const std::string& value, 
          const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarLabel(attributes, value));
}

ConVar* 
ConVar::createSeparator(const std::string& value, 
         const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarSeparator(attributes, value));
}

ConVar* 
ConVar::createSeparator(const char value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarSeparator(attributes, string(1, value)));
}


ConVarAttributes 
ConVar::getAttributes() const
{
    return mState->getAttributes();
}

void 
ConVar::setAttributes(const ConVarAttributes& attributes)
{
    mState->setAttributes(attributes);
}

ConVar::EConVarType 
ConVar::getType()  const
{
    return mState->getType();
}

bool 
ConVar::isOfType(EConVarType type) const
{
    return mState->isOfType(type);
}

bool 
ConVar::isConvertibleTo(EConVarType type) const
{
    // test if this variable can return a variable of the asked type
    switch(type)
    {
    case eCVT_UNDEFINED:
   if(isUndefined())
   {
       return true;
   }

   if (isVariable())
   {
       ConVar* dummy;
       if (getVariable(&dummy))
       {
      return dummy->isConvertibleTo(type);
       } else {
      return false;
       }
   }
   return false;
    case eCVT_BOOL: 
    case eCVT_REF_BOOL: 
   bool dummy;
   return getBool(dummy);
    case eCVT_INT: 
    case eCVT_REF_INT: 
   int dummy;
   return getInt(dummy);
    case eCVT_FLOAT: 
    case eCVT_REF_FLOAT: 
   float dummy;
   return getFloat(dummy);
    case eCVT_STRING: 
    case eCVT_REF_STRING: 
   string dummy;
   return getString(dummy);
    case eCVT_CHARSTRING: 
    case eCVT_REF_CHARSTRING: 
   char* dummy;
   bool success;

   success = getCharString(&dummy);
   if (success)
   {
       delete dummy;
       return true;
   } else {
       return false;
   }
    case eCVT_VECTOR: 
    case eCVT_REF_VECTOR: 
   Vector3 dummy;
   return getVector(dummy);
    case eCVT_COMMAND: 
   const ConCommand* dummy;
   return getCommand(&dummy);
    case eCVT_VARIABLE: 
   return isVariable();
    case eCVT_LABEL: 
   return isLabel();
    case eCVT_SEPARATOR: 
   return isSeparator();
    default:
   return false;
    }
}

bool 
ConVar::isBool() const
{
    return isOfType(eCVT_BOOL);
}

bool 
ConVar::isInt() const
{
    return isOfType(eCVT_INT);
}

bool 
ConVar::isFloat() const
{
    return isOfType(eCVT_FLOAT);
}

bool 
ConVar::isString() const
{
    return isOfType(eCVT_STRING);
}

bool 
ConVar::isCharString() const
{
    return isOfType(eCVT_CHARSTRING);
}

bool 
ConVar::isVector() const
{
    return isOfType(eCVT_VECTOR);
}

bool 
ConVar::isRefBool() const
{
    return isOfType(eCVT_REF_BOOL);
}

bool 
ConVar::isRefInt() const
{
    return isOfType(eCVT_REF_INT);
}

bool 
ConVar::isRefFloat() const
{
    return isOfType(eCVT_REF_FLOAT);
}

bool 
ConVar::isRefString() const
{
    return isOfType(eCVT_REF_CHARSTRING);
}

bool 
ConVar::isRefCharString() const
{
    return isOfType(eCVT_REF_CHARSTRING);
}

bool 
ConVar::isRefVector() const
{
    return isOfType(eCVT_REF_VECTOR);
}

bool 
ConVar::isVariable() const
{
    return isOfType(eCVT_VARIABLE);
}

bool 
ConVar::isCommand() const
{
    return isOfType(eCVT_COMMAND);
}

bool 
ConVar::isUndefined() const
{
    return isOfType(eCVT_UNDEFINED);
}

bool 
ConVar::isLabel() const
{
    return isOfType(eCVT_LABEL);
}

bool 
ConVar::isSeparator() const
{
    return isOfType(eCVT_SEPARATOR);
}

// value access + type definition

bool 
ConVar::setBool(const bool value)
{
    if (isUndefined()) 
    {
        mState = new ConVarBool(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setBool(value);
    }
}

bool 
ConVar::setInt(int value)
{
    if (isUndefined()) 
    {
        mState = new ConVarInt(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setInt(value);
    }
}

bool 
ConVar::setFloat(float value)
{
    if (isUndefined()) 
    {
        mState = new ConVarFloat(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setFloat(value);
    }
}

bool 
ConVar::setString(const string& value)
{
    if (isUndefined()) 
    {
        mState = new ConVarString(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setString(value);
    }
}

bool 
ConVar::setCharString(const char* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarCharString(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setCharString(value);
    }
}

bool 
ConVar::setVector(const Vector3& value)
{
    if (isUndefined()) 
    {
        mState = new ConVarVector(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setVector(value);
    }
}

bool 
ConVar::setVector(float x, float y, float z)
{
    return setVector(Vector3(x, y, z));
}

bool 
ConVar::setRefBool(bool* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarRefBool(mState->getAttributes(), value);
        return true;
    } else {
        return false;
    }
}

bool 
ConVar::setRefInt(int* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarRefInt(mState->getAttributes(), value);
        return true;
    } else {
        return false;
    }
}

bool 
ConVar::setRefFloat(float* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarRefFloat(mState->getAttributes(), value);
        return true;
    } else {
        return false;
    }
}

bool 
ConVar::setRefString(string* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarRefString(mState->getAttributes(), value);
        return true;
    } else {
        return false;
    }
}

bool 
ConVar::setRefCharString(char** value)
{
    if (isUndefined()) 
    {
        mState = new ConVarRefCharString(mState->getAttributes(), value);
        return true;
    } else {
        return false;
    }
}

bool 
ConVar::setRefVector(Vector3* value)
{
    if (isUndefined())
    {
        mState = new ConVarRefVector(mState->getAttributes(), value);
        return true;
    } else {
        return false;
    }
}

bool 
ConVar::setCommand(const ConCommand* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarCommand(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setCommand(value);
    }
}

bool 
ConVar::setVariable(ConVar* value)
{
    if (isUndefined()) 
    {
        mState = new ConVarVariable(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setVariable(value);
    }        
}

bool 
ConVar::setLabel(const string& value)
{
    if (isUndefined()) 
    {
        mState = new ConVarLabel(mState->getAttributes(), value);
        return true;
    } else {
        return mState->setLabel(value);
    }
}

bool 
ConVar::setSeparator(const string& value)
{
    if (isUndefined()) 
    {
        mState = new ConVarSeparator(mState->getAttributes(), value);
        return true;
    } else if (isSeparator()) {
        return mState->setString(value);
    } else {
        return false;
    }
}

bool 
ConVar::setSeparator(const char value)
{
    return setSeparator(string(1, value));
}

// value access

bool 
ConVar::getBool(bool& value) const
{
    return mState->getBool(value);
}

bool 
ConVar::getInt(int& value) const
{
    return mState->getInt(value);
}

bool 
ConVar::getFloat(float& value) const
{
    return mState->getFloat(value);
}

bool 
ConVar::getString(string& value) const
{
    return mState->getString(value);
}
        
bool 
ConVar::getCharString(char** value) const
{
    return mState->getCharString(value);
}

bool 
ConVar::getVariable(ConVar** value) const
{
    return mState->getVariable(value);
}

bool 
ConVar::getCommand(const ConCommand** value) const
{
    return mState->getCommand(value);
}

bool 
ConVar::getVector(Vector3& value) const
{
    return mState->getVector(value);
}

bool 
ConVar::getLabel(string& value) const
{
    return mState->getLabel(value);
}

bool 
ConVar::getSeparator(string& value) const
{
    if (isSeparator()) 
    {
        return mState->getString(value);
    } else {
        return false;
    }
}

string 
ConVar::dump() const
{
    return mState->dump();
}

string 
ConVar::dumpWithSignature() const
{
    return mState->dumpWithSignature();
}

string 
ConVar::dumpValue() const
{
    return mState->dumpValue();
}
