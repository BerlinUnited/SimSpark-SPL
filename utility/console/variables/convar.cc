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
    M_state = state;
}

ConVar::ConVar(const ConVar& con_var)
{
    *M_state = *con_var.M_state;
}

void
ConVar::operator =(const ConVar& con_var) 
{
    *M_state = *con_var.M_state;
}

ConVar::~ConVar()
{
    delete M_state;
}

ConVar*
ConVar::clone() const
{
    return new ConVar(M_state->clone());
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
ConVar::createFloat(TFloat value, const ConVarAttributes& attributes)
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
ConVar::createVector(TFloat x, TFloat y, TFloat z,
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
ConVar::createRefInt(int *value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarRefInt(attributes, value));
}

ConVar*
ConVar::createRefFloat(TFloat* value, const ConVarAttributes& attributes)
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
ConVar::createSeparator(char value, const ConVarAttributes& attributes)
{
    return new ConVar(new ConVarSeparator(attributes, string(1, value)));
}

ConVarAttributes ConVar::getAttributes() const
{
    return M_state->getAttributes();
}

void
ConVar::setAttributes(const ConVarAttributes& attributes)
{
    M_state->setAttributes(attributes);
}

ConVar::ConVarType 
ConVar::getType() const
{
    return M_state->getType();
}

bool 
ConVar::isOfType(ConVarType type) const
{
    return M_state->isOfType(type);
}

bool 
ConVar::isConvertibleTo(ConVarType type) const
{
    // test if this variable can return a variable of the asked type
    switch (type)
    {
        case S_CVT_UNDEFINED:
            if (isUndefined())
            {
                return true;
            }

            if (isVariable())
            {
                ConVar* dummy;
                if (getVariable(&dummy))
                {
                    return dummy->isConvertibleTo(type);
                }
                else
                {
                    return false;
                }
            }
            return false;
        case S_CVT_BOOL:
        case S_CVT_REF_BOOL:
        {
            bool dummy;
            return getBool(dummy);
        }
        case S_CVT_INT:
        case S_CVT_REF_INT:
        {
            int dummy;
            return getInt(dummy);
        }
        case S_CVT_FLOAT:
        case S_CVT_REF_FLOAT:
        {
            TFloat dummy;
            return getFloat(dummy);
        }
        case S_CVT_STRING:
        case S_CVT_REF_STRING:
        {
            string dummy;
            return getString(dummy);
        }
        case S_CVT_CHARSTRING:
        case S_CVT_REF_CHARSTRING:
        {
            char* dummy;
            bool success;

            success = getCharString(&dummy);
            if (success)
            {
                delete dummy;
                return true;
            }
            else
            {
                return false;
            }
        }
        case S_CVT_VECTOR:
        case S_CVT_REF_VECTOR:
        {
            Vector3 dummy;
            return getVector(dummy);
        }
        case S_CVT_COMMAND:
        {
            const ConCommand* dummy;
            return getCommand(&dummy);
        }
        case S_CVT_VARIABLE:
            return isVariable();
        case S_CVT_LABEL:
            return isLabel();
        case S_CVT_SEPARATOR:
            return isSeparator();
        default:
            return false;
    }
}

bool 
ConVar::isBool() const
{
    return isOfType(S_CVT_BOOL);
}

bool 
ConVar::isInt() const
{
    return isOfType(S_CVT_INT);
}

bool 
ConVar::isFloat() const
{
    return isOfType(S_CVT_FLOAT);
}

bool 
ConVar::isString() const
{
    return isOfType(S_CVT_STRING);
}

bool 
ConVar::isCharString() const
{
    return isOfType(S_CVT_CHARSTRING);
}

bool 
ConVar::isVector() const
{
    return isOfType(S_CVT_VECTOR);
}

bool 
ConVar::isRefBool() const
{
    return isOfType(S_CVT_REF_BOOL);
}

bool 
ConVar::isRefInt() const
{
    return isOfType(S_CVT_REF_INT);
}

bool 
ConVar::isRefFloat() const
{
    return isOfType(S_CVT_REF_FLOAT);
}

bool 
ConVar::isRefString() const
{
    return isOfType(S_CVT_REF_CHARSTRING);
}

bool 
ConVar::isRefCharString() const
{
    return isOfType(S_CVT_REF_CHARSTRING);
}

bool 
ConVar::isRefVector() const
{
    return isOfType(S_CVT_REF_VECTOR);
}

bool 
ConVar::isVariable() const
{
    return isOfType(S_CVT_VARIABLE);
}

bool 
ConVar::isCommand() const
{
    return isOfType(S_CVT_COMMAND);
}

bool 
ConVar::isUndefined() const
{
    return isOfType(S_CVT_UNDEFINED);
}

bool 
ConVar::isLabel() const
{
    return isOfType(S_CVT_LABEL);
}

bool 
ConVar::isSeparator() const
{
    return isOfType(S_CVT_SEPARATOR);
}

// value access + type definition
bool 
ConVar::setBool(bool value)
{
    if (isUndefined())
    {
        M_state = new ConVarBool(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setBool(value);
    }
}

bool 
ConVar::setInt(int value)
{
    if (isUndefined())
    {
        M_state = new ConVarInt(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setInt(value);
    }
}

bool 
ConVar::setFloat(TFloat value)
{
    if (isUndefined())
    {
        M_state = new ConVarFloat(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setFloat(value);
    }
}

bool 
ConVar::setString(const string& value)
{
    if (isUndefined())
    {
        M_state = new ConVarString(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setString(value);
    }
}

bool 
ConVar::setCharString(const char* value)
{
    if (isUndefined())
    {
        M_state = new ConVarCharString(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setCharString(value);
    }
}

bool 
ConVar::setVector(const Vector3& value)
{
    if (isUndefined())
    {
        M_state = new ConVarVector(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setVector(value);
    }
}

bool 
ConVar::setVector(TFloat x, TFloat y, TFloat z)
{
    return setVector(Vector3(x, y, z));
}

bool 
ConVar::setRefBool(bool* value)
{
    if (isUndefined())
    {
        M_state = new ConVarRefBool(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setRefInt(int* value)
{
    if (isUndefined())
    {
        M_state = new ConVarRefInt(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setRefFloat(TFloat* value)
{
    if (isUndefined())
    {
        M_state = new ConVarRefFloat(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setRefString(string* value)
{
    if (isUndefined())
    {
        M_state = new ConVarRefString(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setRefCharString(char** value)
{
    if (isUndefined())
    {
        M_state = new ConVarRefCharString(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setRefVector(Vector3* value)
{
    if (isUndefined())
    {
        M_state = new ConVarRefVector(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setCommand(const ConCommand* value)
{
    if (isUndefined())
    {
        M_state = new ConVarCommand(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setCommand(value);
    }
}

bool 
ConVar::setVariable(ConVar* value)
{
    if (isUndefined())
    {
        M_state = new ConVarVariable(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setVariable(value);
    }
}

bool 
ConVar::setLabel(const string& value)
{
    if (isUndefined())
    {
        M_state = new ConVarLabel(M_state->getAttributes(), value);
        return true;
    }
    else
    {
        return M_state->setLabel(value);
    }
}

bool 
ConVar::setSeparator(const string& value)
{
    if (isUndefined())
    {
        M_state = new ConVarSeparator(M_state->getAttributes(), value);
        return true;
    }
    else if (isSeparator())
    {
        return M_state->setString(value);
    }
    else
    {
        return false;
    }
}

bool 
ConVar::setSeparator(char value)
{
    return setSeparator(string(1, value));
}

// value access
bool 
ConVar::getBool(bool& value) const
{
    return M_state->getBool(value);
}

bool 
ConVar::getInt(int &value) const
{
    return M_state->getInt(value);
}

bool 
ConVar::getFloat(TFloat& value) const
{
    return M_state->getFloat(value);
}

bool 
ConVar::getString(string& value) const
{
    return M_state->getString(value);
}

bool 
ConVar::getCharString(char** value) const
{
    return M_state->getCharString(value);
}

bool 
ConVar::getVariable(ConVar** value) const
{
    return M_state->getVariable(value);
}

bool 
ConVar::getCommand(const ConCommand ** value) const
{
    return M_state->getCommand(value);
}

bool 
ConVar::getVector(Vector3& value) const
{
    return M_state->getVector(value);
}

bool 
ConVar::getLabel(string& value) const
{
    return M_state->getLabel(value);
}

bool 
ConVar::getSeparator(string& value) const
{
    if (isSeparator())
    {
        return M_state->getString(value);
    }
    else
    {
        return false;
    }
}

string 
ConVar::dump() const
{
    return M_state->dump();
}

string 
ConVar::dumpWithSignature() const
{
    return M_state->dumpWithSignature();
}

string 
ConVar::dumpValue() const
{
    return M_state->dumpValue();
}
