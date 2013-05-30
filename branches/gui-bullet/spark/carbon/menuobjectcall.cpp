/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "menuobjectcall.h"
#include "carbon.h"
#include "menuobjectconnection.h"
#include "abstractplugin.h"

namespace MenuSystem
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Call
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Call::Call(Connection* parent, const QString& slot)
{
    mParentConnection = parent;
    mSlot = slot;
    int start = mSlot.indexOf('(');
    int end = mSlot.lastIndexOf(')');
    mSlotName = mSlot.left(start);
    mSlotArguments = mSlot.mid(start, end-start+1);
}

Call::Call(Connection* parent)
{
    mParentConnection = parent;
    mSlot = "";
    mSlotName = "";
    mSlotArguments = "";
}

Call::~Call()
{
    deleteArguments();
}

void Call::addArgument(const QString& value, CallArgument::EArgumentType type, const QString& macro)
{
    mArguments.push_back(new CallArgument(value, type, macro));
}

void Call::addArgument(const QString& value, CallArgument::EArgumentType type)
{
    mArguments.push_back(new CallArgument(value, type));
}

void Call::call()
{
    AbstractPlugin* plugin = mParentConnection->getParentFile()->getCreator(); 
    if (!plugin->isQObject())
    {
        LOG_ERROR() << "Cannot invoke menu object call on non-qobject-derived plugins. Use SignalPlugin or AttachableFrame instead.";
        return;
    }

    //Process arguments with macros and invoke method
    int argCount = mArguments.size();
    QGenericArgument* arguments = new QGenericArgument[argCount]; //Generic arguments for invocation
    std::vector<QString> stringParams; //Storage for the converted parameters (QGenericArguments only take references to data)
    std::vector<bool> boolParams;
    std::vector<float> floatParams;
    std::vector<int> intParams;
    File* file = mParentConnection->getParentFile();
    QString parameterText("");

    if (argCount > 10)
    {
        LOG_WARNING("Qt only supports 10 arguments for function calls. Ignoring the rest.");
    }

    //Fill qt argument list
    for (int i = 0; i < argCount && i < 10; i++)
    {
        if (i != 0) parameterText.append(",");

        //Apply macros
        QString processedString = mArguments.at(i)->mValue;
        file->applyMacros(processedString, mParentConnection->getObject(), this);

        //Update custom macro for this argument
        if (mArguments.at(i)->mConnectedToMacro)
        {
            file->storeMacroText(mParentConnection->getObject(), mArguments.at(i)->mMacroName, processedString);
        }

        switch (mArguments.at(i)->mType)
        {
        case CallArgument::AT_STRING:
            stringParams.push_back(processedString);
            arguments[i] = Q_ARG(QString, stringParams.back());
            parameterText.append("QString");
            break;
        case CallArgument::AT_INTEGER:
            intParams.push_back(processedString.toInt());
            arguments[i] = Q_ARG(int, intParams.back());
            parameterText.append("int");
            break;
        case CallArgument::AT_BOOL:
            boolParams.push_back(((processedString.compare("false", Qt::CaseInsensitive) != 0) && (processedString.compare("0") != 0)));
            arguments[i] = Q_ARG(bool, boolParams.back());
            parameterText.append("bool");
            break;
        case CallArgument::AT_FLOAT:
            floatParams.push_back(processedString.toFloat());
            arguments[i] = Q_ARG(float, floatParams.back());
            parameterText.append("float");
            break;
        default:
            LOG_ERROR() << "Unknown argument type.";
            break;
        }
    }

    //parameterText.append(")");

    QObject* object = plugin->getQObject();

    //Check if function parameter match
    //QString callString = mSlotName;
    //callString.append(parameterText);
    if (!QMetaObject::checkConnectArgs(mSlot.toStdString().c_str(), parameterText.toStdString().c_str()))
    {
        //Call function
        switch (argCount)
        {
        case 0: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str());
            break;
        case 1: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0]);
            break;
        case 2: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1]);
            break;
        case 3: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2]);
            break;
        case 4: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3]);
            break;
        case 5: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
            break;
        case 6: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4],
                                          arguments[5]);
            break;
        case 7: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4],
                                          arguments[5], arguments[6]);
            break;
        case 8: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4],
                                          arguments[5], arguments[6], arguments[7]);
            break;
        case 9: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4],
                                          arguments[5], arguments[6], arguments[7], arguments[8]);
            break;
        case 10: QMetaObject::invokeMethod(object, mSlotName.toStdString().c_str(), arguments[0], arguments[1], arguments[2], arguments[3], arguments[4],
                                           arguments[5], arguments[6], arguments[7], arguments[8], arguments[9]);
            break;
        }
    }
    else
    {
        //Cant call!
        LOG_WARNING() << "Cant call function " << mSlot << " using argument signature '" << parameterText << "'.";
    }
    delete[] arguments;
}

Connection* Call::getParentConnection() const
{
    return mParentConnection;
}

const QString& Call::getSlot() const
{
    return mSlot;
}

const QString& Call::getSlotName() const
{
    return mSlotName;
}

const QString& Call::getSlotArguments() const
{
    return mSlotArguments;
}

bool Call::readArgument(int index, QString& argumentText) const
{
    if (index >= 0 && index < (int)mArguments.size())
    {
        argumentText = mArguments.at(index)->mValue;
    }
    else
    {
        LOG_ERROR() << "Index " << index << " out of bounds.";
        return false;
    }

    return true;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void Call::deleteArguments()
{
    while (mArguments.size() > 0)
    {
        delete mArguments.back();
        mArguments.pop_back();
    }
}

//--------------------------------------------------------------
// VariableCall
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

//Uses base protected constructor
VariableCall::VariableCall(Connection* parent, const QString& name, const QString& type, const QString& value)
    : Call(parent)
{
    mName = name;
    mType = type;
    mValue = value;
}

VariableCall::~VariableCall()
{
}

void VariableCall::call()
{
    //Process arguments with macros and save variable
    File* file = mParentConnection->getParentFile();

    QString processedString = mValue;
    file->applyMacros(processedString, mParentConnection->getObject(), this);

    //Changes the string automatically using qt conversion
    file->setVariable(mName, mType, processedString);
}

//--------------------------------------------------------------
// CallArgument
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

CallArgument::CallArgument(const QString& value, EArgumentType type)
{
    mMacroName = "";
    mValue = value;
    mType = type;
    mConnectedToMacro = false;
}

CallArgument::CallArgument(const QString& value, EArgumentType type, const QString& macroName)
{
    mMacroName = macroName;
    mValue = value;
    mType = type;
    mConnectedToMacro = true;
}

CallArgument::~CallArgument()
{
}

} //namespace MenuSystem