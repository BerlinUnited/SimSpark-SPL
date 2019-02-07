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

#ifndef MENUOBJECT_CONNECTION_H
#define MENUOBJECT_CONNECTION_H
/**
 * \file menuobjectconnection.h
 * \brief Contains MenuSystem::Connection class, which specifies a connection between a menu object signal and several Call or VariableCall objects.
 */

#include "menuobjectfile.h"

#include <QObject>
#include <map>
#include <vector>
#include <QString>

namespace MenuSystem
{

class Call;
class VariableCall;
class CallArgument;

/*!
 \brief Class specifying a connection between a ProvidedMenuObject from an xml-file and several target functions to call.

 Created by reading a
 \verbatim <connection signal="signalname(signalarguments)"> ... <\connection> \endverbatim
 node from the Tool-Definition-xml-file.

 A Connection posesses a signal definition and is called whenever a specified MenuObject's function is called, e.g. QAction::trigger().
 When the connection is triggered, the arguments of the signal are stored as CallArgument and all contained Calls are
 invoked in order of their specification in the Tooldefinition-File of the parent File.

 \note
 For every signal-signature the Connection should be able to receive there has to be a trigger() function with that signature.
 To extend the receiving capabilities, simply add a new trigger() function.

 The Connection stores macros that can be written and read out by child MenuSystem::Call or MenuSystem::VariableCall objects.
 When a macro value is stored by one of those children using the xml-attribute 'macro="name"', the value of the call is stored as a
 string macro in the connection using the identifier "name", whenever the Call is invoked.
 The macro can be read out using the '\%' or '\$' synmbol in any Call or VariableCall value.
 See menuobjectfile.h for more information on macros.

 \sa MenuSystem::File
*/
class Connection : QObject
{
    Q_OBJECT

public:
    // public types

    /*!
     \brief Helper-Type defining a list of Calls.
    */
    typedef std::vector<Call*> TCalls;
    /*!
     \brief Helper-Type defining a list of CallArguments.
    */
    typedef std::vector<CallArgument*> TArguments;

public:
    // public functions
    /*!
     \brief Basic constructor.

     \param parent file object that owns this connection
     \param object menu object that sends the signal which triggers this connection
     \param name name of the menu object
     \param type type of the menu object
     \param index index of this connection in the parents list
    */
    Connection(File* parent, QObject* object, const QString& name, File::EMenuToolType type, int index);
    /*!
     \brief Basic destructor.
    */
    virtual ~Connection();

    /*!
     \brief Specify the signal that triggers the connection.

     Tries to connect the menu objects signal with this connections trigger() function.
     Only works if there is a Connection::trigger() function with the same signature than the specified signal.

     \todo
     This method uses a rather ugly hack to fake the SIGNAL and SLOT macro of Qt. If you can come up with a better solution,
     go ahead and change it. (See In-Code-Comments)

     \param signal signal to connect to
    */
    void setSignal(const QString& signal);

    /*!
     \brief Adds a new variable store call that should be invoked when a signal is received.

     \param name name of the variable
     \param type type of the variable
     \param value value of the variable (containing macros)
    */
    VariableCall* addVariableCall(const QString& name, const QString& type, const QString& value);
    /*!
     \brief Adds a new call to a function of the creating plugin that shall be invoked when a signal is received.

     \param slot name and signature of the function to call
    */
    Call* addCall(const QString& slot);

    /*!
     \brief Returns pointer to the File that owns this connection.
    */
    File* getParentFile() const;
    /*!
     \brief Returns pointer to the MenuObject whose signals are received.
    */
    QObject* getObject() const;

    /*!
     \brief Returns the index of a child Call.

     \param call child call to search for
     \return -1 of child was not found
    */
    int getCallIndex(const Call* call) const;
    /*!
     \brief Returns the index of this connection.
    */
    int getIndex() const;
    /*!
     \brief Stores the value of an argument in argumentText.

     The arguments are cleared and updated every time the specified signal is received.
     \param index index of the argument
     \param argumentText target to store value to
     \return true if the argument was found
    */
    bool readArgument(int index, QString& argumentText) const;

private:
    // private functions

    /*!
     \brief Deletes all Calls.

     Used by destructor.
    */
    void deleteCalls();
    /*!
     \brief Deletes all arguments.

     Used by destructor.
    */
    void deleteSignalArguments();

    /*!
     \brief Calls all contained Calls.
    */
    void callAll();

public: signals:

public slots:
    /*!
     \brief Trigger function that may be connected to a signal of mObject.
    */
    void trigger();
    /*!
     \brief Trigger function that may be connected to a signal of mObject.

     \param arg1 int argument
    */
    void trigger(int arg1);
    /*!
     \brief Trigger function that may be connected to a signal of mObject.

     \param arg1 bool argument
    */
    void trigger(bool arg1);
    /*!
     \brief Trigger function that may be connected to a signal of mObject.

     \param arg1 string argument
    */
    void trigger(QString arg1);

private:
    // private members

    TCalls mCalls; /*!< List of all calls to invoke. */
    QString mSignal; /*!< Signal name and argument list to connect to. */
    QString mSlot; /*!< Name and argument list of the trigger() function to connect to. */
    QString mArgumentString; /*!< Argument list. */
    int mIndex; /*!< Index of this connection, as specified in the Tooldefinition file. */

    TArguments mSignalArguments; /*!< Arguments of the last received signal. */

    QObject* mObject;  /*!< Object to receive signals from. */
    QString mName; /*!< Name of the menu object. */
    File::EMenuToolType mType; /*!< Type of the menu object. */

    File* mParentObjectFile; /*!< Pointer to parent File. */
};

} //namespace MenuSystem

#endif //MENUOBJECT_CONNECTION_H
