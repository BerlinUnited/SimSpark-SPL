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

#ifndef MENUOBJECT_FILE_H
#define MENUOBJECT_FILE_H
/**
 * \file menuobjectfile.h
 * \brief Contains MenuSystem::File class, which specifies a tool-definition-xml file to load MenuObjects from.
 */

#include "menuobject.h"

#include <map>

class QDomElement;

namespace MenuSystem
{

class Connection;
class Call;

/*!
 \brief Class defining an xml-file to dynamically load and create MenuObjects.

 Files define specific Widgets and Actions for the calling Plugin to use. So when extending for example the external
 communication protocol, the tools can be extended and additional tools can be added without changing the GUI.

 The XML-File defining objects features various menu objects and functions calls. See ProvidedExample.xml for an example.
 Additionally, Files use a macro and variable system. All text entries will be scanned upon activation of the function and
 macros will be applied. Syntax: %X or %(XYZ) and '%' instead of '%'. '$' will be replaced in a first pass, '%' in a second pass.
 Own macros can be registered with 'macro="X"' in any argument (e.g. < arg macro="name" >value< /arg >) or connection node.
 They will be updated to the contained text whenever the call is invoked.
 Macros are stored locally for each 'connection' node. So using the same macro in two different connections wont collide with each other.
 When you want to share data between multiple connections, use global variables.

 Automatic macros usable in connections:\n
  %(an) or %(tn) action/tool name\n
  %(sX) signal name at index X\n

 Reserved (irreplaceable) macros: (1-letter calls dont need brackets)\n
  %(i) connection index\n
  %(ic) calls index\n
  %(v)(XYZ) string variable with name XYZ\n
  %(vs)(XYZ) string variable with name XYZ\n
  %(vi)(XYZ) integer variable with name XYZ\n
  %(vf)(XYZ) float variable with name XYZ\n
  %(vb)(XYZ) bool variable with name XYZ\n
  %(arg)(X) SIGNAL (not slot!) argument with index X\n\n

 Variables are stored in the File and globally accesible for all function calls and arguments of this file.
 They can be stored using an < var > xml node:\n
 < var name="name" type="string">value</ var > \n
 This entry used in an action or tool is called once upon creation of the File (initialization).
 The same entry in a connection registers a VariableCall function. This variable will be set every time the
 connections signal is sent.
 Variables are typed. The can be either one of 'string', 'int', 'bool', or 'float' and are stored in separate Lists.
 So storing < var name="X" type="string" >value1< /var > and < var name="X" type="int" >100< /var > won't overwrite each other.

 Example for Two-Pass macros:
 %(arg)(mString$i) will return the variable 'mStringX' where X is the argument index.
 %(s$i) will return signal name at index X. Watch out: '%s$i' will NOT, since the replaced $i will not be used in the second pass when not using brackets.
*/
class File : public MenuObject
{
public:
    //public types

    /*!
     \brief Types for Tools and Menu Entries that can be provided by a file.

     Actions are used both in Menus and ToolBoxes.
     In Menus they can be assigned sub-menus with additional actions.
     Checkboxes and Textedits may currently only be added to ToolBars.
    */
    enum EMenuToolType
    {
        MTT_ACTION, /*!< Action Object for menus and ToolBars. */
        MTT_CHECKBOX, /*!< Checkbox for ToolBars. */
        MTT_TEXTEDIT /*!< TextEdit for ToolBars. */
    };

protected:
    //protected types

    /*!
     \brief Helper-Type that contains QString variables and their names.
    */
    typedef std::map<QString, QString> TStringVariableMap;
    /*!
     \brief Helper-Type that contains Integer variables and their names.
    */
    typedef std::map<QString, int> TIntVariableMap;
    /*!
     \brief Helper-Type that contains Float variables and their names.
    */
    typedef std::map<QString, float> TFloatVariableMap;
    /*!
     \brief Helper-Type that contains Boolean variables and their names.
    */
    typedef std::map<QString, bool> TBoolVariableMap;

    /*!
     \brief Helper-Type that contains a list of macro names and definitions.
    */
    typedef std::map<QString, QString> TMacroMap;
    /*!
     \brief Helper-Type that contains a list of MacroMaps for different QObjects.
    */
    typedef std::map<QObject*, TMacroMap> TGroupedMacroMap;

    /*!
     \brief Helper-Type that contains Connections for QObjects.
    */
    typedef std::vector<Connection*> TConnectionList;

public:
    // public functions

    /*!
     \brief Basic constructor.

     Location of a provided file is always local.
     \param name name of the MenuObject
     \param file file to load from
     \param creator creator of the MenuObject, pointer to Plugin or 0 for MenuManager
    */
    File(const QString& name, const QString& file, const AbstractPlugin* creator = 0);

    virtual ~File();

    /*!
     \brief Deletes this container and all its created MenuObject from the file.
    */
    virtual bool cleanEntry();
    /*!
     \brief Updates the file Information, replacing the old one.

     The Object type has to be the same than the one already contained (QString).
     \param object Should be a QString.
    */
    virtual bool update(QObject* object);
    /*!
     \brief Attaches the objects specified in the file to their target locations.
    */
    virtual bool attach();

    /*!
     \brief Registers or changes an existing macro for one provided menu object.

     Macros really really should not contain '%' or '$' symbols, since they will be recognized as macros in the next invocation,
     but for the sake of hacking its not forbidden.
     \param id object to change or store macro for
     \param macroName macro to change or store
     \param macroText text for the macro
    */
    void storeMacroText(QObject* id, const QString& macroName, const QString& macroText);
    /*!
     \brief Returns the text contained in a macro definition of an object.

     \param id object to return macro definition from
     \param macro name of the macro to return
    */
    const QString& getMacroText(QObject* id, const QString& macro) const;

    /*!
     \brief Applys all macros of an object to a text in multiple passes.

     First, all text entries with leading $ are replaced, then those with leading %.
     If a registered macro is found, the function to replace the text is called, for example loading a variable.

     If a pointer to the Call (caller) is given, this enables specific macros accessing the arguments of the call
     and its parent connection.
     \param string string to replace in
     \param id macros of this object will be used to replace
     \param caller pointer to the caller
    */
    bool applyMacros(QString& string, QObject* id, Call* caller);

    /*!
     \brief Saves a global variable for all connections and objects from this File.

     \param name name of the variable
     \param type type of the variable (QString, int, bool, float)
     \param value value of the variable as string
    */
    bool setVariable(const QString& name, const QString& type, const QString& value);

protected:
    // protected functions
    
    /*!
     \brief Returns the filename.
    */
    virtual QObject* getObject() const;

private:

    /*!
     \brief Opens the contained file and begins loading its provided objects.

     Calls loadObjects()
     \return true if loading was (partially) successful.
    */
    bool loadFile();
    /*!
     \brief Loads menus, toolbars, etc. from the file.

     Called by loadFile()
     \param source parent element
     \return true if loading was (partially) successful.
    */
    bool loadObjects(QDomElement* source);
    /*!
     \brief Loads a menu when loadObjects() found a menu node.

     \param source parent element
     \param nameRef write-back for the menu name
     \param locationRef write-back for the menu location
     \return the created menu
    */
    QMenu* loadMenu(QDomElement* source, QString& nameRef, MenuManager::EMenuObjectLocation& locationRef);
    /*!
     \brief Loads a toolbar when loadObjects() found a toolbar node.

     \param source parent element
     \param nameRef write-back for the toolbar name
     \param locationRef write-back for the toolbar location
     \return the created toolbar
    */
    QToolBar* loadToolBar(QDomElement* source, QString& nameRef, MenuManager::EMenuObjectLocation& locationRef);
    /*!
     \brief Loads actions for a created QMenu.

     \param source parent element
     \param target menu to store actions in
     \return true if loading was (partially) successful.
    */
    bool loadAction (QDomElement* source, QMenu* target);
    /*!
     \brief Loads submenus for a created QAction.

     \param source parent element
     \param target action to attach menu to
     \return true if loading was (partially) successful.
    */
    bool loadSubMenu (QDomElement* source, QAction* target);
    /*!
     \brief Loads an action or widget as a tool for a QToolBar.

     \param source parent element
     \param target toolbar to store object to
     \return true if loading was successful
    */
    bool loadTool(QDomElement* source, QToolBar* target);

    /*!
     \brief Loads connections for a menu object like menu or toolbar.

     Loading connections for a toolbar, menu, etc. also initialize variables and macros.
     Connections with at least one variable or function call are stored as a Connection object in mConnections.
     A signal coming from the referenced object will be received by the connection, the message processed using macros, and then
     sent to the slots of the connection. (Saving variables or calling slots of the AbstractPlugin mCreator.

     \param source parent element
     \param object object to receive signals from
     \param name name of the object
     \param type type of the object
     \return true if loading was successful.
    */
    bool loadConnections(QDomElement* source, QObject* object, const QString& name, EMenuToolType type);

    /*!
     \brief Loads a variable node for a connection or call.

     \param source parent element
     \param name the name will be stored here
     \param the type will be stored here
     \param value the value will be stored here
     \return true if loading was successful
    */
    bool loadVarNode(QDomElement* source, QString& name, QString& type, QString& value);

    /*!
     \brief Processes the next macro of the given string.

      Called by applyMacros().

      First, reserved macros are tested (e.g. %i, %(arg) etc. Afterwards, custom macros are checked.
      This means, saving a macro with the name of a reserved macro will make it inaccessible since it cant be found.

     \param string reference to the string to replace in
     \param start index to start searching from
     \param map macro map to use
     \param caller caller to include information from when macros are used
    */
    bool processMacro(QString& string, int& start, TMacroMap& map, Call* caller);

private:
    // private members

    QString mFile; /*!< File to load from. */
    MenuManager::TMenuObjects mChildren; /*!< Children created using the files information. */

    TConnectionList mConnections; /*!< List of all connections in this file, stored by QObject id (pointer). */
    TStringVariableMap mStringVariables; /*!< List of all string variables. */
    TIntVariableMap mIntVariables; /*!< List of all int variables. */
    TFloatVariableMap mFloatVariables; /*!< List of all float variables. */
    TBoolVariableMap mBoolVariables; /*!< List of all bool variables. */

    TGroupedMacroMap mMacroMaps; /*!< List of all Macro-Collections for every object. */

};

} //namespace MenuSystem

#endif // MENUOBJECT_FILE_H
