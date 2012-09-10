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

#include "menuobjectconnection.h"
#include "menuobjectcall.h"
#include "menuobjectfile.h"

#include <map>
#include <vector>
#include <QString>

namespace MenuSystem
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Connection::Connection(File* parent, QObject* object, const QString& name, File::EMenuToolType type, int index)
{
    mObject = object;
    mName = name;
    mType = type;
    mParentObjectFile = parent;
    mIndex = index;
    mSignal = "";
}

Connection::~Connection()
{
    deleteCalls();
}

VariableCall* Connection::addVariableCall(const QString& name, const QString& type, const QString& value)
{
    VariableCall* newCall = new VariableCall(this, name, type, value);
    mCalls.push_back((Call*)newCall);

    return newCall;
}

Call* Connection::addCall(const QString& slot)
{
    mCalls.push_back(new Call(this, slot));

    return mCalls.back();
}

void Connection::setSignal(const QString& signal)
{
    mSignal = signal;

    //Connect the senders signal to this connections trigger
    int indexOfOpeningBracket = signal.indexOf('(');
    int indexOfClosingBracket = signal.lastIndexOf(')');
    mArgumentString = signal.mid(indexOfOpeningBracket+1, indexOfClosingBracket-indexOfOpeningBracket-1);
    mSlot = "trigger(";
    mSlot.append(mArgumentString);
    mSlot.append(")");

    //:HACK: Fakes Qts SIGNAL and SLOT Macros. Optionally it would be possible to use an Interface lass for all
    //Signal functions Qt offers, but this would be huge.
    //The most elegant version would be using a scripting language to create a new MetaObject, but this is troublesome
    QString modifiedSignal = mSignal.prepend("2");
    QString modifiedSlot = mSlot.prepend("1");

    //Faking SIGNAL and SLOT to connect to Connection function
    bool success = connect(mObject, qFlagLocation(modifiedSignal.toStdString().c_str()), this, qFlagLocation(modifiedSlot.toStdString().c_str()));

    if (!success)
    {
        LOG_ERROR() << "Failed to connect Signal " << mSignal << " with Slot " << mSlot << ".";
    }
}

File* Connection::getParentFile() const
{
    return mParentObjectFile;
}

QObject* Connection::getObject() const
{
    return mObject;
}

int Connection::getCallIndex(const Call* call) const
{
    int run = 0;
    for (TCalls::const_iterator it = mCalls.begin(); it != mCalls.end(); it++)
    {
        if ((*it) == call)
        {
            return run;
        }
        run++;
    }
    return -1;
}

int Connection::getIndex() const
{
    return mIndex;
}

bool Connection::readArgument(int index, QString& argumentText) const
{
    if (index >= 0 && index < (int)mSignalArguments.size())
    {
        argumentText = mSignalArguments.at(index)->mValue;
    }
    else
    {
        LOG_ERROR() << "Index " << index << " out of bounds.";
        return false;
    }

    return true;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void Connection::deleteCalls()
{
    while (mCalls.size() > 0)
    {
        delete mCalls.back();
        mCalls.pop_back();
    }
}

void Connection::deleteSignalArguments()
{
    while (mSignalArguments.size() > 0)
    {
        delete mSignalArguments.back();
        mSignalArguments.pop_back();
    }
}

void Connection::callAll()
{
    for (TCalls::iterator it = mCalls.begin(); it != mCalls.end(); it++)
    {
        (*it)->call();
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------


void Connection::trigger()
{    
    deleteSignalArguments();
    callAll();
}

void Connection::trigger(int arg1)
{
    deleteSignalArguments();
    mSignalArguments.push_back(new CallArgument(QString("%1").arg(arg1), CallArgument::AT_INTEGER));
    callAll();
}

void Connection::trigger(bool arg1)
{
    deleteSignalArguments();
    mSignalArguments.push_back(new CallArgument(QString("%1").arg(arg1), CallArgument::AT_BOOL));
    callAll();
}

void Connection::trigger(QString arg1)
{
    deleteSignalArguments();
    mSignalArguments.push_back(new CallArgument(QString("%1").arg(arg1), CallArgument::AT_STRING));
    callAll();
}

} //namespace MenuSystem