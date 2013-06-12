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

#ifndef MENUOBJECT_CALL_H
#define MENUOBJECT_CALL_H
/**
 * \file menuobjectcall.h
 * \brief Contains MenuSystem::Call, MenuSystem::CallArgument and MenuSystem::VariableCall classes.
 */

#include <vector>
#include <QString>

namespace MenuSystem
{

class Connection;

/*!
 \brief Helper class to store signal and slot parameters as strings.

 Created by reading out an
 \verbatim <arg macro="macroname">argumentvalue</arg> \endverbatim
 node from an Tool-Definition-xml-file.

 This class is used to store arguments of incoming signals to a Connection::trigger() function, which are possibly read out
 in a Call, as well as to store the arguments of the outgoing calls as strings.

 It consists of a type and a value as well as a macro flag and name.
 When the macro flag mConnectedToMacro is true, the value of this argument will be saved to the owners Macro list using the name
 mMacroName, every time the argument is read out, processed and sent in a Call.
*/
class CallArgument
{
public:
    /*!
     \brief Supported parameter types.

     To extend this list, new types have to be added in several locations. The File loads the variables, Connection
     receive them, and Call as well as VariableCall send/process them.
    */
    enum EArgumentType
    {
        AT_STRING, /*!< QString argument. ("qstring" or "QString")*/
        AT_INTEGER, /*!< Integer argument.  ("int")*/
        AT_BOOL, /*!< Boolean argument. ("bool") */
        AT_FLOAT /*!< Single precision float argument. ("float") */
    };

public:
    /*!
     \brief Basic constructor.

     \param value value of the argument
     \param type type of the argument
    */
    CallArgument(const QString& value, EArgumentType type);
    /*!
     \brief Basic constructor when there exists a macro connection.

     \param value value of the argument
     \param type type of the argument
     \param macroName macro to store the argument value to upon activation
    */
    CallArgument(const QString& value, EArgumentType type, const QString& macroName);
    /*!
     \brief Basic destructor.
    */
    virtual ~CallArgument();

public:
    bool mConnectedToMacro; /*!< If true, the value will be stored to mMacroName when the argument is used. */
    QString mMacroName; /*!< Macro to store value to. */
    QString mValue; /*!< Value of the argument as string. */
    EArgumentType mType; /*!< Type of the argument. */
};


/*!
 \brief Defines one function to call whenever a triggering signal arrives at a Connection.

 Created by reading out a
 \verbatim <call macro ="macroname" slot="slotname(parameternames)"> ... <\call> \endverbatim
 node from an Tool-Definition-xml-file.

 A Call contains the arguments for the function call, which contain macros. When the call is activated, the macros
 are processed and the registered function is called using the processed arguments.

 The function to call is stored in mSlot and may only be a function of the Plugin that provided the File
 this Call was created from.
*/
class Call
{
    /*!
     \brief Helpter-Type for an argument list.
    */
    typedef std::vector<CallArgument*> TArguments;

public:
    /*!
     \brief Basic constructor.

     \param parent creating connection
     \param slot name of the function to call
    */
    Call(Connection* parent, const QString& slot);
    /*!
     \brief Basic destructor.
    */
    virtual ~Call();

    /*!
     \brief Calls the specified function after processing the contained arguments macros.
    */
    virtual void call();

    /*!
     \brief Add a new argument to the list.

     \param value value of the argument as string (may contain macros)
     \param type type of the argument
     \param macro optional macro-target to store the processed arguments value to when used
    */
    void addArgument(const QString& value, CallArgument::EArgumentType type, const QString& macro);
    /*!
     \brief Add a new argument to the list.

     \param value value of the argument as string (may contain macros)
     \param type type of the argument
    */
    void addArgument(const QString& value, CallArgument::EArgumentType type);

    /*!
     \brief Returns pointer to the connection that created the Call.
    */
    Connection* getParentConnection() const;
    /*!
     \brief Returns the name and argument list of the target function.
    */
    const QString& getSlot() const;
    /*!
     \brief Returns the name of the target function.
    */
    const QString& getSlotName() const;
    /*!
     \brief Returns the argument list of the target function.
    */
    const QString& getSlotArguments() const;

    /*!
     \brief Stores the value of an argument in argumentText.

     \param index index of the argument
     \param argumentText target to store value to
     \return true if the argument was found
    */
    bool readArgument(int index, QString& argumentText) const;
protected:
    /*!
     \brief Constructor for derived Calls.

     \param parent creating connection
    */
    Call(Connection* parent);

private:
    /*!
     \brief Deletes all arguments.

     Called at time of destruction.
    */
    void deleteArguments();

protected:
    QString mSlot; /*!< Target function name and argument list. */
    QString mSlotName; /*!< Target function name. */
    QString mSlotArguments; /*!< Target function argument list. */
    TArguments mArguments; /*!< List of arguments for the call. */
    Connection* mParentConnection; /*!< Creator connection. */
};

/*!
 \brief Defines a function call that simply calls a variable store function.

 Created by reading out a
 \verbatim <var name="variablename" type="variabletype">variablevalue</var> \endverbatim
 node from an Tool-Definition-xml-file.

 'var' nodes in a 'connection' node are read only once and initialize the variable. In a 'call' node they are executed every time the call
 is triggered. The VariableCall stores the name, type and value to store as strings.

 Before saving, the value has its macros processed, so it can read out arguments of the incoming signal, variables or other macros.
 Variables are stored in the File object and are accessible by all Tools created from the Tooldefinition-File.
*/
class VariableCall : Call
{
public:
    /*!
     \brief Basic constructor.

     \param parent creating connection
     \param name name of the variable
     \param type type of the variable
     \param value value of the variable (containing macros)
    */
    VariableCall(Connection* parent, const QString& name, const QString& type, const QString& value);
    /*!
     \brief Basic destructor.
    */
    virtual ~VariableCall();

    /*!
     \brief Stores the specified variable after processing the contained arguments macros.
    */
    virtual void call();

private:
    QString mName; /*!< Name of the variable to store in. */
    QString mType; /*!< Type of the variable to store in. */
    QString mValue; /*!< Value to store. */
};

} //namespace MenuSystem

#endif // MENUOBJECT_CALL_H
