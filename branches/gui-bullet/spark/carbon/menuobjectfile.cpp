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

#include "menuobjectfile.h"
#include "abstractplugin.h"
#include "menuobjectconnection.h"
#include "menuobjectcall.h"

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QDir>
#include <QFile>
#include <QCheckBox>
#include <QTextEdit>
#include <QToolBar>

namespace MenuSystem
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

File::File(const QString& name, const QString& file, const AbstractPlugin* creator)
    : MenuObject(name, MenuManager::MOT_FILE, MenuManager::MOL_LOCAL, creator)
{
    mFile = file;
}

File::~File()
{
    cleanEntry();
}

QObject* File::getObject() const
{
    return 0;
}

bool File::attach()
{
    // The object is attached to its target position
    if (isAttached())
    {
        LOG_WARNING() << "Menu Object " << mName << " is already attached to parent " << mCreator->getCaption() << ".";
        return false;
    }

    //Parse XML-File, create and attach objects
    bool success = loadFile();

    if (success)
    {
        mAttached = true;
    }

    return success;
}

bool File::cleanEntry()
{
    if (mDeleted)
        return false;

    for (MenuManager::TMenuObjects::iterator it = mChildren.begin(); it != mChildren.end(); it++)
    {
        (*it)->cleanEntry();
    }
    mDeleted = true;

    return true;
}

bool File::update(QObject* object)
{
    LOG_INFO() << "TODO";
    return true;
}

void File::storeMacroText(QObject* id, const QString& macroName, const QString& macroValue)
{
    //Store macro
    TGroupedMacroMap::iterator it = mMacroMaps.find(id);
    if (it == mMacroMaps.end())
    {
        //No Macros for this id found, add list
        mMacroMaps.insert(std::make_pair(id, TMacroMap()));
        it = mMacroMaps.find(id);
    }

    //Search macro in list
    TMacroMap::iterator it2 = it->second.find(macroName);
    if (it2 == it->second.end())
    {
        //Macro not yet in the list, add
        it->second.insert(std::make_pair(macroName, macroValue));
    }
    else
    {
        //Change macro
        it2->second = macroValue;
    }
}

const QString& File::getMacroText(QObject* id, const QString& macro) const
{
    //Read out macro
    TGroupedMacroMap::const_iterator it = mMacroMaps.find(id);
    if (it == mMacroMaps.end())
    {
        //No Macros for this id found
        LOG_WARNING() << "No macros for id " << id << " found.";
        return macro;
    }

    //Search macro in list
    TMacroMap::const_iterator it2 = it->second.find(macro);
    if (it2 == it->second.end())
    {
        //Requestet macros for not found
        LOG_WARNING() << "Macro " << macro << " for id " << id << " not found.";
        return macro;
    }

    //else
    return it2->second;
}

bool File::applyMacros(QString& string, QObject* id, Call* caller)
{
    //Get Macro map
    TGroupedMacroMap::iterator it = mMacroMaps.find(id);
    if (it == mMacroMaps.end())
    {
        //no macros
        return true;
    }
    TMacroMap& map = it->second;

    int start = 0;
    //Process macros
    if (string.indexOf('$') != -1)
    {
        //First iteration
        while (true)
        {
            start = string.indexOf('$', start);
            if (start == -1)
                break;

            processMacro(string, start, map, caller);
            start++; //dont check the same macro again
        }
    }

    start = 0;
    if (string.indexOf('%') != -1)
    {
        //Second iteration
        while (true)
        {
            start = string.indexOf('%', start);
            if (start == -1)
                break;

            processMacro(string, start, map, caller);
            start++; //dont check the same macro again
        }
    }

    return true;
}

bool File::setVariable(const QString& name, const QString& typeStr, const QString& stringValue)
{
    LOG_DEBUG() << "Storing variable " << name << "(" << typeStr.toStdString().c_str() << ") with value " << stringValue << ".";

    //String variable
    if(typeStr.compare("QString", Qt::CaseInsensitive))
    {
        TStringVariableMap::iterator pos = mStringVariables.find(name);
        if (pos != mStringVariables.end())
        {
            pos->second = stringValue;
        }
        else
        {
            mStringVariables.insert(std::make_pair(name, stringValue));
        }
    }
    //Int variable
    else if (typeStr.compare("int", Qt::CaseInsensitive))
    {
        int value = stringValue.toInt();
        TIntVariableMap::iterator pos = mIntVariables.find(name);
        if (pos != mIntVariables.end())
        {
            pos->second = value;
        }
        else
        {
            mIntVariables.insert(std::make_pair(name, value));
        }
    }
    //Float variable
    else if (typeStr.compare("float", Qt::CaseInsensitive))
    {
        float value = stringValue.toFloat();
        TFloatVariableMap::iterator pos = mFloatVariables.find(name);
        if (pos != mFloatVariables.end())
        {
            pos->second = value;
        }
        else
        {
            mFloatVariables.insert(std::make_pair(name, value));
        }
    }
    //Bool variable
    else if (typeStr.compare("bool", Qt::CaseInsensitive))
    {
        //'', '0' and 'false' is false, everything else is true
        bool value = ((stringValue.compare("0") != 0) && (stringValue.compare("") != 0) &&
                      (stringValue.compare("false", Qt::CaseInsensitive) != 0));
        TBoolVariableMap::iterator pos = mBoolVariables.find(name);
        if (pos != mBoolVariables.end())
        {
            pos->second = value;
        }
        else
        {
            mBoolVariables.insert(std::make_pair(name, value));
        }
    }
    else
    {
        //Unknown type
        LOG_ERROR() << "Cant save value " << stringValue << " to variable " << name << ". Unkown variable type " << typeStr.toStdString().c_str() <<".";
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

bool File::loadFile()
{
    //Open and load document
    QDomDocument document(mName);
    QString absPath = QDir::currentPath().append('/').append(mFile);
    QFile file(absPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        LOG_ERROR() << "Could not open MenuObject definition file at " << absPath << ".";
        return false;
    }
    else
    {
        LOG_DEBUG() << "Opened MenuObject definition file " << mFile << ".";
    }
    QString errorMsg;
    int errorLine;
    int errorColumn;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        LOG_ERROR() << "Failed to load MenuObject definition file " << mFile << ": error at line " << errorLine << ", column " << errorColumn << " : " << errorMsg.toStdString().c_str();
        file.close();
        return false;
    }
    else
    {
        LOG_DEBUG() << "Succeeded to load MenuObject definition file.";
    }
    file.close();

    QDomElement docElem = document.documentElement();
    QString docElName = docElem.nodeName();

    return loadObjects(&docElem);
}

bool File::loadObjects(QDomElement* source)
{
    int oneLoaded = false;

    //Parse document
    QDomNode n = source->firstChild();
    while(!n.isNull())
    {
        //Comments
        if (n.isComment())
        {
            //skipping comment
            n = n.nextSibling();
            continue;
        }

        //What node?
        QString name = n.nodeName();
        if (name.compare("menu") == 0)
        {
            //Create menu object and provide to Manager
            MenuManager::EMenuObjectLocation menuLocation;
            QString menuName;
            QMenu* newMenu = loadMenu(&(n.toElement()), menuName, menuLocation);
            if (newMenu == 0)
            {
                LOG_ERROR() << "Could not create menu object from file.";
            }
            else
            {
                bool success = MenuManager::getManager()->provideMenu(mCreator, menuName, newMenu, menuLocation);
                if (success)
                    oneLoaded = true;
                else
                {
                    LOG_ERROR() << "Failed to provide Menu from file. Deleting it.";
                    delete newMenu;
                }
            }
        }
        else if (name.compare("toolbar") == 0)
        {
            //Create toolbar object and provide to Manager
            MenuManager::EMenuObjectLocation toolBarLocation;
            QString toolBarName("");
            QToolBar* newToolBar = loadToolBar(&(n.toElement()), toolBarName, toolBarLocation);
            if (newToolBar == 0)
            {
                LOG_ERROR() << "Could not create toolbar object from file.";
            }
            else
            {
                bool success = MenuManager::getManager()->provideToolBar(mCreator, toolBarName, newToolBar, toolBarLocation);
                if (success) oneLoaded = true;
            }
        }
        else
        {
            //Unknown node
            LOG_WARNING() << "Ignoring unknown node " << name << ".";
        }

        //Next node
        n = n.nextSibling();
    }

    return oneLoaded;
}

QMenu* File::loadMenu(QDomElement* source, QString& nameRef, MenuManager::EMenuObjectLocation& locationRef)
{
    //Read attributes
    if (!source->hasAttribute("name"))
    {
        LOG_ERROR() << "Menu definition is missing a name attribute ('name').";
        return 0;
    }
    if (!source->hasAttribute("location"))
    {
        LOG_ERROR() << "Menu definition is missing a location attribute ('location').";
        return 0;
    }

    QDomAttr nameAttr = source->attributeNode("name");
    QDomAttr locationAttr = source->attributeNode("location");

    //Set name
    if (nameAttr.value().compare("") == 0)
    {
        LOG_ERROR() << "Menu's name attribute is empty.";
        return 0;
    }
    else
    {
        nameRef = nameAttr.value();
    }

    //Set location
    if (locationAttr.value().compare("main") == 0)
    {
        locationRef = MenuManager::MOL_MAIN;
    }
    else if (locationAttr.value().compare("local") == 0)
    {
        locationRef = MenuManager::MOL_LOCAL;
    }
    else
    {
        LOG_ERROR() << "Menu's location " << locationAttr.value() << " is no defined location. Has to be 'main' or 'local'.";
        return 0;
    }

    //Create Menu
    QMenu* newMenu = new QMenu(nameRef);

    //Parse actions element
    QDomNode n = source->firstChild();
    while(!n.isNull())
    {
        //Comments
        if (n.isComment())
        {
            //Skipping comment;
            n = n.nextSibling();
            continue;
        }

        QString nodeName = n.nodeName();
        if (nodeName.compare("action") == 0)
        {
            //Create action and add to menu
            bool success = loadAction(&(n.toElement()), newMenu);
            if (!success)
            {
                LOG_ERROR() << "Could not create an action object from file.";
            }
        }
        else
        {
            //Unknown node
            LOG_WARNING() << "Ignoring unknown node " << nodeName << ".";
        }

        //Next node
        n = n.nextSibling();
    }

    return newMenu;
}

QToolBar* File::loadToolBar(QDomElement* source, QString& nameRef, MenuManager::EMenuObjectLocation& locationRef)
{
    //Read attributes
    if (!source->hasAttribute("name"))
    {
        LOG_ERROR() << "ToolBar definition is missing a name attribute ('name').";
        return 0;
    }
    if (!source->hasAttribute("location"))
    {
        LOG_ERROR() << "ToolBar definition is missing a location attribute ('location').";
        return 0;
    }

    QDomAttr nameAttr = source->attributeNode("name");
    QDomAttr locationAttr = source->attributeNode("location");

    //Set name
    if (nameAttr.value().compare("") == 0)
    {
        LOG_ERROR() << "ToolBar name attribute is empty.";
        return 0;
    }
    else
    {
        nameRef = nameAttr.value();
    }

    //Set location
    if (locationAttr.value().compare("main") == 0)
    {
        locationRef = MenuManager::MOL_MAIN;
    }
    else if (locationAttr.value().compare("local") == 0)
    {
        locationRef = MenuManager::MOL_LOCAL;
    }
    else
    {
        LOG_ERROR() << "ToolBar location " << locationAttr.value() << " is no defined location. Has to be 'main' or 'local'.";
        return 0;
    }

    //Create ToolBar
    QToolBar* newToolBar = new QToolBar(nameRef);

    //Parse actions element
    QDomNode n = source->firstChild();
    while(!n.isNull())
    {
        //Comments
        if (n.isComment())
        {
            //Skipping comment
            n = n.nextSibling();
            continue;
        }

        QString nodeName = n.nodeName();
        if (nodeName.compare("tool") == 0)
        {
            //Create tool action and add to menu
            bool success = loadTool(&(n.toElement()), newToolBar);
            if (!success)
            {
                LOG_ERROR() << "Could not create an tool action object from file.";
            }
        }
        else
        {
            //Unknown node
            LOG_WARNING() << "Ignoring unknown node " << nodeName << ".";
        }

        //Next node
        n = n.nextSibling();
    }

    return newToolBar;
}

bool File::loadAction(QDomElement* source, QMenu* target)
{
    QString name("");
    QString icon("");
    bool hasIcon = false;

    //Read attributes

    //name
    {
        QDomAttr attr = source->attributeNode("name");
        if (attr.isNull())
        {
            LOG_ERROR() << "Action node is missing a name attribute ('name').";
            return false;
        }
        name = attr.value();
    }

    //icon
    {
        QDomAttr attr = source->attributeNode("icon");
        if (!attr.isNull())
        {
            icon = attr.value();
            hasIcon = true;
        }
    }

    //Create object
    QAction* newAction;
    if (hasIcon)
        newAction = new QAction(QIcon(icon), name, 0);
    else
        newAction = new QAction(name, 0);

    //Load connections for action
    bool success = loadConnections(source, newAction, name, MTT_ACTION);

    if (success)
    {
        //Add action to menu
        target->addAction(newAction);
    }
    else
    {
        delete newAction;
        return false;
    }

    return true;
}

bool File::loadTool(QDomElement* source, QToolBar* target)
{
    QString name("");
    QString icon("");
    QString type("");
    bool hasIcon = false;

    //Read attributes

    //name
    {
        QDomAttr attr = source->attributeNode("name");
        if (attr.isNull())
        {
            LOG_ERROR() << "Tool node is missing a name attribute ('name').";
            return false;
        }
        name = attr.value();
    }

    //type
    {
        QDomAttr attr = source->attributeNode("type");
        if (attr.isNull())
        {
            LOG_WARNING() << "Tool node is missing a type attribute ('type'). 'Action'' assumed.";
            type = "action";
        }
        else
            type = attr.value();
    }

    //icon
    {
        QDomAttr attr = source->attributeNode("icon");
        if (!attr.isNull())
        {
            icon = attr.value();
            hasIcon = true;
        }
    }

    //Create action
    QObject* newTool;
    EMenuToolType toolType;

    if (type.compare("action") == 0)
    {
        toolType = MTT_ACTION;
        if (hasIcon)
            newTool = new QAction(QIcon(icon), name, 0);
        else
            newTool = new QAction(name, 0);

        //read additional parameters

    }
    else if (type.compare("checkbox") == 0)
    {
        toolType = MTT_CHECKBOX;
        if (hasIcon)
            LOG_WARNING() << "Checkbox does not support icons (yet).";

        newTool = new QCheckBox(name);

        //read additional parameters

    }
    else if (type.compare("textedit") == 0)
    {
        toolType = MTT_TEXTEDIT;
        if (hasIcon)
            LOG_WARNING() << "Checkbox does not support icons (yet).";

        newTool = new QTextEdit(name);

        //read additional parameters

    }
    else
    {
        LOG_ERROR() << "Unknown tool " << type << ".";
        return false;
    }

    //Load connections for action
    bool success = loadConnections(source, newTool, name, toolType);

    if (success)
    {
        //Add Tool (Action/Widget) to toolbar
        switch(toolType)
        {
        case MTT_ACTION:
            target->addAction((QAction*) newTool);
            break;
        default:
            target->addWidget((QWidget*) newTool);
        }
    }
    else
    {
        delete newTool;
        return false;
    }

    return true;
}

bool File::loadConnections(QDomElement* source, QObject* object, const QString& name, EMenuToolType type)
{
    int connections = 0;

    //Fill macros
    storeMacroText(object, "an", name);
    storeMacroText(object, "tn", name);

    //Parse connection elements
    QDomNode n = source->firstChild();
    while(!n.isNull())
    {
        //Comments
        if (n.isComment())
        {
            //Skipping comment
            n = n.nextSibling();
            continue;
        }

        QDomElement e = n.toElement();
        QString nodeName = n.nodeName();

        //----------------------
        //VAR Node
        //----------------------
        if (nodeName.compare("var") == 0)
        {
            //Load variable and store it
            QString nameStr;
            QString typeStr;
            QString valueStr;
            bool success = loadVarNode(&e, nameStr, typeStr, valueStr);

            //Store the variable
            if (success)
                setVariable(nameStr, typeStr, valueStr);
            else
            {
                LOG_ERROR() << "Failed to load variable node.";
            }
        }

        //----------------------
        //CONNECTION Node
        //----------------------
        else if (nodeName.compare("connection") == 0)
        {
            //Create connection object. The connection receives signals from created objects, processes macros and sends
            // the call to the specified slots.
            Connection* connection = new Connection(this, object, name, type, connections);
            bool keepConnection = false;

            QDomAttr signalAttr = e.attributeNode("signal");
            if (signalAttr.isNull())
            {
                LOG_ERROR() << "Connection node is missing 'signal' attribute.";
                continue;
            }
            QString signal = signalAttr.value();

            //Signal invoking this connection
            connection->setSignal(signal);

            //Parse all var and call nodes
            QDomNode subnode = e.firstChild();
            while (!subnode.isNull())
            {
                QDomElement subnodeElement = subnode.toElement();
                QString subName = subnode.nodeName();

                //----------------------
                //SUB-VAR Node
                //----------------------
                if (subName.compare("var") == 0)
                {
                    //Load variable store definition
                    QString nameStr;
                    QString typeStr;
                    QString valueStr;

                    bool success = loadVarNode(&subnodeElement, nameStr, typeStr, valueStr);

                    //Add variable definition call
                    if (success)
                    {
                        connection->addVariableCall(nameStr, typeStr, valueStr);
                        keepConnection = true;
                    }
                    else
                    {
                        LOG_ERROR() << "Failed to load variable node.";
                    }
                }
                //----------------------
                //CALL Node
                //----------------------
                else if (subName.compare("call") == 0)
                {
                    QDomAttr slotAttr = subnodeElement.attributeNode("slot");
                    if (slotAttr.isNull())
                        LOG_ERROR() << "Call node is missing slot attribute.";
                    else
                    {
                        //Add function invocation call
                        QString slotStr = slotAttr.value();
                        Call* call = connection->addCall(slotStr);
                        keepConnection = true;

                        //Check macro attribute
                        QDomAttr macroAttr = subnodeElement.attributeNode("macro");
                        if (!macroAttr.isNull())
                        {
                            storeMacroText(object, macroAttr.value(), slotStr);
                        }

                        //Scan argument list
                        std::vector<CallArgument::EArgumentType> typeList;
                        QString type;
                        int startIndex = slotStr.indexOf('(')+1;

                        if (startIndex == -1)
                        {
                            LOG_ERROR() << "'slot' definitions have to be of the form 'foo(type, type)'.";
                            continue;
                        }

                        int endIndex;
                        bool endLoop = false;
                        bool errorHappened = false;
                        while (!endLoop)
                        {
                            endIndex = slotStr.indexOf(',', startIndex);
                            if (endIndex == -1)
                            {
                                endIndex = slotStr.indexOf(')', startIndex);
                                endLoop = true;
                            }
                            if (endIndex == -1)
                            {
                                LOG_ERROR() << "Missing opening/closing bracket in slot definition.";
                                errorHappened = true;
                                break;
                            }
                            type = slotStr.mid(startIndex, endIndex-startIndex);
                            type.remove(' ');

                            //no argument?
                            if (endIndex <= startIndex)
                            {
                                endLoop = true;
                                continue;
                            }

                            if ((type.compare("qstring", Qt::CaseInsensitive) == 0) || (type.compare("string", Qt::CaseInsensitive) == 0))
                            {
                                typeList.push_back(CallArgument::AT_STRING);
                            }
                            else if (type.compare("bool", Qt::CaseInsensitive) == 0)
                            {
                                typeList.push_back(CallArgument::AT_BOOL);
                            }
                            else if (type.compare("int", Qt::CaseInsensitive) == 0)
                            {
                                typeList.push_back(CallArgument::AT_INTEGER);
                            }
                            else if (type.compare("float", Qt::CaseInsensitive) == 0)
                            {
                                typeList.push_back(CallArgument::AT_FLOAT);
                            }
                            else
                            {
                                LOG_ERROR() << "Unknown argument type: " << type.toStdString().c_str() << ". Only string, bool, int and float are supported.";
                                errorHappened = true;
                                endLoop = true;
                            }

                            //new start index
                            startIndex = endIndex + 1;
                        }

                        if (errorHappened)
                        {
                            LOG_ERROR() << "Ignoring this Call definition!";
                            subnode = subnode.nextSibling();
                            continue;
                        }

                        //Add arguments to call
                        QDomNode argnode = subnode.firstChild();
                        int run = 0;
                        while(!argnode.isNull())
                        {
                            if (run >= (int)typeList.size())
                            {
                                LOG_WARNING() << "Specified more arguments than given in Definition of target function " << slotStr << 
                                    ". Ignoring the following arguments.";
                                break;
                            }
                            QString lowestName = argnode.nodeName();
                            if (lowestName.compare("arg") == 0)
                            {
                                //Read argument
                                QString argument = argnode.toElement().text();
                                QDomAttr macro = argnode.toElement().attributeNode("macro");

                                //Add to call
                                if (macro.isNull())
                                    call->addArgument(argument, typeList.at(run));
                                else
                                    call->addArgument(argument, typeList.at(run), macro.value());
                            }
                            else
                            {
                                LOG_WARNING() << "Ignoring unknown node " << lowestName << ". Was expecting 'arg'.";
                            }

                            argnode = argnode.nextSibling();
                            run++;
                        }

                        //Check if argument amount matches the list
                        //:TODO:
                    }
                }
                //----------------------
                //UNKNOWN Node
                //----------------------
                else
                {
                    LOG_WARNING() << "Ignoring unknown node " << subName << ". Only 'call' and 'var' are defined below 'connection'.";
                }

                subnode = subnode.nextSibling();
            }

            if (keepConnection)
            {
                QString indexStr = QString("%1").arg(connections);
                storeMacroText(object, QString("s").append(indexStr), signal);
                mConnections.push_back(connection);
                connections++;
            }
            else
            {
                delete connection;
            }
        }
        //----------------------
        //UNKNOWN Node
        //----------------------
        else
        {
            //Unknown node
            LOG_WARNING() << "Ignoring unknown node " << nodeName << ".";
        }

        //Next node
        n = n.nextSibling();
    }

    return (connections >= 1);
}

bool File::loadVarNode(QDomElement* parent, QString& name, QString& type, QString& value)
{
    QDomAttr nameAttr = parent->attributeNode("name");
    if (nameAttr.isNull())
    {
        LOG_ERROR() << "Cannot store variable value without variable name.";
        return false;
    }
    else
    {
        QString nameStr = nameAttr.value();
        QDomAttr typeAttr = parent->attributeNode("type");
        QString typeStr;
        if (typeAttr.isNull())
        {
            LOG_WARNING() << "No 'type' attribute in var node with name " << nameAttr.value() << " given. Assuming string.";
            typeStr = "QString";
        }
        else
            typeStr = typeAttr.value();

        if ((typeStr.compare("qstring", Qt::CaseInsensitive) != 0) && 
            (typeStr.compare("bool", Qt::CaseInsensitive) != 0) &&
            (typeStr.compare("int", Qt::CaseInsensitive) != 0) &&
            (typeStr.compare("float", Qt::CaseInsensitive) != 0))
        {
            LOG_ERROR() << "Type " << typeStr << " is unnkown. Using string instead.";
            typeStr = "qstring";
        }

        QString valueStr = parent->text();

        //Return
        name = nameStr;
        type = typeStr;
        value = valueStr;
    }

    return true;
}


bool File::processMacro(QString& string, int& start, TMacroMap& map, Call* caller)
{
    if (string.size() <= start)
    {
        LOG_WARNING() << "Macro symbol without definition found: " << string;
        string = string.left(string.size()-1);
        return false;
    }

    int contentstart = start+1;
    int contentend = start+1;
    int end = contentend;

    QString macro;
    QString replaceText("");
    if (string.at(start+1) == '(')
    {
        //Bracket embraced
        contentstart = start+2;
        contentend = string.indexOf(')', contentstart)-1;
        if (contentend == -1)
        {
            LOG_WARNING() << "Macro symbol is missing closing bracket: " << string;
            return false;
        }
        macro = string.mid(contentstart, contentend-contentstart+1);
        end = contentend+1;
    }
    else
    {
        //Single symbol
        macro = string.at(start+1);
    }

    //Process reserved macros
    if (macro.compare("i") == 0)
    {
        //Connection index
        Connection* connection = caller->getParentConnection();
        replaceText = QString("%1").arg(connection->getIndex());
    }
    else if (macro.compare("ic") == 0)
    {
        //Call index
        Connection* connection = caller->getParentConnection();
        int index = connection->getCallIndex(caller);
        if (index == -1)
        {
            LOG_ERROR() << "Call not found.";
            return false;
        }
        replaceText = QString("%1").arg(index);
    }
    else if (macro.compare("arg") == 0 ||
             macro.compare("v") == 0 ||
             macro.compare("vs") == 0 ||
             macro.compare("vb") == 0 ||
             macro.compare("vi") == 0 ||
             macro.compare("vf") == 0)
    {
        //Variable of argument

        if (string.size() < contentend+2 || string.at(contentend+2)!='(')
        {
            LOG_ERROR() << "Variable and argument macros need a second bracket specifying the name: " << string;
            return false;
        }

        QString second;
        int secondstart = contentend+3;
        int secondend = string.indexOf(')', secondstart);
        if (secondend == -1 || secondend == secondstart)
        {
            LOG_ERROR() << "Variable and argument macros need a second bracket specifying the name. Missing second bracket or argument: " << string;
            return false;
        }
        second = string.mid(secondstart, secondend-secondstart);
        end = secondend;

        if (macro.at(0) == 'a')
        {
            //Signal argument (connections arguments)
            Connection* connection = caller->getParentConnection();
            bool successful = connection->readArgument(second.toInt(), replaceText);
            if (!successful)
            {
                LOG_ERROR() << "Could not find argument with index " << second.toInt();
                return false;
            }
        }
        else if (macro.size() == 1 || macro.at(1) == 's')
        {
            //String variable
            TStringVariableMap::iterator pos = mStringVariables.find(second);
            if (pos == mStringVariables.end())
            {
                LOG_WARNING() << "Could not find string variable " << second;
                return false;
            }
            replaceText = pos->second;
        }
        else if (macro.at(1) == 'b')
        {
            //Bool variable
            TBoolVariableMap::iterator pos = mBoolVariables.find(second);
            if (pos == mBoolVariables.end())
            {
                LOG_WARNING() << "Could not find bool variable " << second;
                return false;
            }
            replaceText = QString("%1").arg(pos->second);
        }
        else if (macro.at(1) == 'i')
        {
            //Int variable
            TIntVariableMap::iterator pos = mIntVariables.find(second);
            if (pos == mIntVariables.end())
            {
                LOG_WARNING() << "Could not find int variable " << second;
                return false;
            }
            replaceText = QString("%1").arg(pos->second);
        }
        else if (macro.at(1) == 'f')
        {
            //Float variable
            TFloatVariableMap::iterator pos = mFloatVariables.find(second);
            if (pos == mFloatVariables.end())
            {
                LOG_WARNING() << "Could not find float variable " << second;
                return false;
            }
            replaceText = QString("%1").arg(pos->second);
       }
    }
    else
    {
        //Custom Macro
        TMacroMap::iterator it = map.find(macro);
        if (it == map.end())
        {
            //Cant replace, leave it
            return true;
        }
        else
        {
            replaceText = it->second;
        }
    }

    //Replace the text in the string
    string.replace(start, end-start+1, replaceText);

    return true;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

} //namespace MenuSystem