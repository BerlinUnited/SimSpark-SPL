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

#include "taskdefinition.h"
#include "carbon.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

const TaskDefinition::ETaskType TaskDefinition::mTypes[4] = {TT_SERVERTHREAD, TT_PLUGINTHREAD, TT_PROCESS, TT_SPARKPROCESS};
const QString TaskDefinition::mTypeNamesShort[4] = 
    {"serverthread", "pluginthread", "process", "sparkprocess"};
const QString TaskDefinition::mTypeNames[4] = 
    {"Server Thread", "Plugin Thread", "Process", "Spark Process"};
const int TaskDefinition::mMaxTypes = 4;

const QString& TaskDefinition::getTypeString(ETaskType type)
{
    return mTypeNames[type];
}

QString TaskDefinition::getTypeString(ETaskType type, bool uppercase)
{
    if (uppercase)
        return mTypeNames[type].toUpper();

    return mTypeNames[type];
}

const QString& TaskDefinition::getTypeStringShort(ETaskType type)
{
    return mTypeNamesShort[type];
}

bool TaskDefinition::isThread(ETaskType type)    {return type == TT_SERVERTHREAD || type == TT_PLUGINTHREAD;}
bool TaskDefinition::isProcess(ETaskType type)   {return type == TT_PROCESS      || type == TT_SPARKPROCESS;}
bool TaskDefinition::isPlugin(ETaskType type)    {return type == TT_PLUGINTHREAD;}
bool TaskDefinition::isSpark(ETaskType type)     {return type == TT_SERVERTHREAD || type == TT_SPARKPROCESS;}

const QThread::Priority TaskDefinition::mPriorityTypes[8] = {QThread::IdlePriority, QThread::LowestPriority, QThread::LowPriority,
                                                               QThread::NormalPriority, QThread::HighPriority, QThread::HighestPriority,
                                                               QThread::TimeCriticalPriority, QThread::InheritPriority};
const QString TaskDefinition::mPriorityTypeNames[8] = {"Idle", "Lowest", "Low", "Normal", "High", "Highest", "TimeCritical", "Inherit"};
const int TaskDefinition::mMaxPriorityTypes = 8;


const QString& TaskDefinition::getPriorityTypeString(QThread::Priority type)
{
    return mPriorityTypeNames[type];
}

QString TaskDefinition::getPriorityTypeString(QThread::Priority type, bool uppercase)
{
    if (uppercase)
        return mPriorityTypeNames[type].toUpper();

    return mPriorityTypeNames[type];
}

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

TaskDefinition::TaskDefinition(ETaskType type, const QString& name, QThread::Priority priority, 
    const QString& first, const QString& second, const QString& third,
    const std::vector<QString>& firstList, const std::vector<QString>& secondList)
{
    mType = type;
    mName = name;
    mFirst = first;
    mPriority = priority;
    mSecond = second;
    mThird = third;

    for (auto it = secondList.begin(); it != secondList.end(); it++) mSecondList.push_back(*it);
    for (auto it = firstList.begin();  it != firstList.end();  it++) mFirstList.push_back(*it);
}

TaskDefinition::TaskDefinition(TaskDefinition::ETaskType type, const QString& name, QThread::Priority priority, const QString& first, 
        const QString& second, const QString& third, const QStringList& firstList, const QStringList& secondList)
{
    mType = type;
    mName = name;
    mFirst = first;
    mPriority = priority;
    mSecond = second;
    mThird = third;

    mSecondList = secondList;
    mFirstList = firstList;
}

TaskDefinition::TaskDefinition(const TaskDefinition& obj)
{
    *this = obj;
}

TaskDefinition::TaskDefinition()
{
    mType = TT_PLUGINTHREAD;
    mName = "Undefined";
    mFirst = "";
    mPriority = QThread::InheritPriority;
    mSecond = "";
    mThird = "";

    mSecondList.clear();
    mFirstList.clear();
}

TaskDefinition::~TaskDefinition()
{

}

const TaskDefinition& TaskDefinition::operator=(const TaskDefinition& rhs)
{
    mType = rhs.mType;
    mName = rhs.mName;
    mFirst = rhs.mFirst;
    mPriority = rhs.mPriority;
    mSecond = rhs.mSecond;
    mThird = rhs.mThird;

    mSecondList = rhs.mSecondList;
    mFirstList = rhs.mFirstList;

    return *this;
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

TaskDefinition::ETaskType TaskDefinition::getType() const
{
    return mType;
}

const QString& TaskDefinition::getName() const                               
{
    return mName;
}

QVariant TaskDefinition::getData(ETaskData field) const
{
    switch (field)
    {
    case TD_NAME:     return mName;          break;
    case TD_PRIORITY: return (int)mPriority; break;
    case TD_FIRST:    return mFirst;         break;
    case TD_SECOND:   return mSecond;        break;
    case TD_THIRD:    return mThird;         break;
    case TD_LIST_1:   return mFirstList;     break;
    case TD_LIST_2:   return mSecondList;    break;
    case TD_UNDEFINED: //fallthrough
    default:
        LOG_ERROR() << "Unknown data field " << field;
    }

    return QVariant();
}

const QString& TaskDefinition::getFirst() const                         
{
    return mFirst;
}

const QString& TaskDefinition::getSecond() const                         
{
    return mSecond;
}

const QString& TaskDefinition::getThird() const                    
{
    return mThird;
}

const QStringList& TaskDefinition::getFirstList() const            
{
    return mFirstList;
}

QStringList& TaskDefinition::getFirstListNc()             
{
    return mFirstList;
}

const QStringList& TaskDefinition::getSecondList() const     
{
    return mSecondList;
}

QStringList& TaskDefinition::getSecondListNc()
{
    return mSecondList;
}

QThread::Priority TaskDefinition::getPriority() const                        
{
    return mPriority;
}

void TaskDefinition::updateDefinitionChanged(ETaskData field)
{
    emit definitionChanged(field);
}

void TaskDefinition::setType(TaskDefinition::ETaskType type)    
{
    mType = type;
}

void TaskDefinition::setName(const QString& name)                                
{
    if (name.compare(mName) == 0)
        return;

    mName = name;
    emit definitionChanged(TD_NAME);
}

void TaskDefinition::setFirst(const QString& first)                    
{
    if (first.compare(mFirst) == 0)
        return;

    mFirst = first;
    emit definitionChanged(TD_FIRST);
}

void TaskDefinition::setSecond(const QString& second)                    
{
    if (second.compare(mSecond) == 0)
        return;

    mSecond = second;
    emit definitionChanged(TD_SECOND);
}

void TaskDefinition::setThird(const QString& third)                    
{
    if (third.compare(mThird) == 0)
        return;

    mThird = third;
    emit definitionChanged(TD_THIRD);
}

void TaskDefinition::addToFirstList(const std::vector<QString>& values)       
{
    for (auto it = values.begin(); it != values.end(); it++)
    {
        mFirstList.push_back(*it);
    }

    if (values.size() > 0)
        emit definitionChanged(TD_LIST_1);
}

void TaskDefinition::addToFirstList(const QString& value)                     
{
    mFirstList.push_back(value);

    emit definitionChanged(TD_LIST_1);
}

void TaskDefinition::clearFirstList()                                           
{
    if (mFirstList.size() == 0)
        return;

    mFirstList.clear();
    emit definitionChanged(TD_LIST_1);
}

void TaskDefinition::addToSecondList(const std::vector<QString>& values)       
{
    for (auto it = values.begin(); it != values.end(); it++)
    {
        mSecondList.push_back(*it);
    }

    if (values.size() > 0)
        emit definitionChanged(TD_LIST_2);
}

void TaskDefinition::addToSecondList(const QString& value)                     
{
    mSecondList.push_back(value);

    emit definitionChanged(TD_LIST_2);
}

void TaskDefinition::clearSecondList()                                           
{
    if (mSecondList.size() == 0)
        return;

    mSecondList.clear();
    emit definitionChanged(TD_LIST_2);
}

void TaskDefinition::setPriority(QThread::Priority priority)                     
{
    if (mPriority == priority)
        return;

    mPriority = priority;
    emit definitionChanged(TD_PRIORITY);
}

void TaskDefinition::removeFromFirstList(int index)
{
    if (index < 0 || index >= mFirstList.size())
    {
        LOG_ERROR() << "Index " << index << " out of bounds [0," <<  mFirstList.size() << "].";
        return;
    }

    mFirstList.erase(mFirstList.begin() + index);
    emit definitionChanged(TD_LIST_1);
}

void TaskDefinition::removeFromSecondList(int index)
{
    if (index < 0 || index >= mSecondList.size())
    {
        LOG_ERROR() << "Index " << index << " out of bounds [0," <<  mSecondList.size() << "].";
        return;
    }

    mSecondList.erase(mSecondList.begin() + index);
    emit definitionChanged(TD_LIST_2);
}

void TaskDefinition::swapInFirstList(int index1, int index2)
{
    if (index1 < 0 || index1 >= mFirstList.size())
    {
        LOG_ERROR() << "Index1 " << index2 << " out of bounds [0," <<  mFirstList.size() << "].";
        return;
    }    
    if (index2 < 0 || index2 >= mFirstList.size())
    {
        LOG_ERROR() << "Index2 " << index2 << " out of bounds [0," <<  mFirstList.size() << "].";
        return;
    }

    if (index1 == index2)
        return;

    //Swap
    QString swap = mFirstList.at(index1);
    mFirstList.replace(index1, mFirstList.at(index2));
    mFirstList.replace(index2, swap);

    emit definitionChanged(TD_LIST_1);
}

void TaskDefinition::swapInSecondList(int index1, int index2)
{
    if (index1 < 0 || index1 >= mSecondList.size())
    {
        LOG_ERROR() << "Index1 " << index2 << " out of bounds [0," <<  mSecondList.size() << "].";
        return;
    }    
    if (index2 < 0 || index2 >= mSecondList.size())
    {
        LOG_ERROR() << "Index2 " << index2 << " out of bounds [0," <<  mSecondList.size() << "].";
        return;
    }

    if (index1 == index2)
        return;

    //Swap
    QString swap = mSecondList.at(index1);
    mSecondList.replace(index1, mSecondList.at(index2));
    mSecondList.replace(index2, swap);

    emit definitionChanged(TD_LIST_2);
}


QString TaskDefinition::getTypeString(bool uppercase) const {return TaskDefinition::getTypeString(mType, uppercase);}
const QString& TaskDefinition::getTypeString()        const {return TaskDefinition::getTypeString(mType);}
const QString& TaskDefinition::getTypeStringShort()   const {return TaskDefinition::getTypeStringShort(mType);}

bool TaskDefinition::isThread()   const {return TaskDefinition::isThread(mType);}
bool TaskDefinition::isProcess()  const {return TaskDefinition::isProcess(mType);}
bool TaskDefinition::isPlugin()   const {return TaskDefinition::isPlugin(mType);}
bool TaskDefinition::isSpark()    const {return TaskDefinition::isSpark(mType);}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

