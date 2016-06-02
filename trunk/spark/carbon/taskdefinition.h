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

#ifndef TASKDEFINITION_H
#define TASKDEFINITION_H
/**
 * \file taskdefinition.h
 * \brief Contains TaskDefinition class, which is a part of a SimulationSetup.
 */

#include <QThread>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QVariant>

#include <vector>

/*!
 \brief Definition for a simulation task (a thread or process).

 It is used in SimulationSetup definitions to specify new Simulations, and in the SimulationTask classes to describe the parameters of the task.
*/
class TaskDefinition : public QObject
{
    Q_OBJECT

public:
    // public types and static members/functions

    /*!
     \brief List of TaskDefinition data fiels.

     TD_NAME always identifies the name of the task.
     TD_PRIORITY always identifies the task priority (thread or process priority).
     The useage of the other data fields depends on the task type.
    */
    enum ETaskData
    {
        TD_NAME = 0, /*!< Name of the Task. Used for references between tasks. Should be unique. */
        TD_PRIORITY, /*!< Thread or process priority. */ 
        TD_FIRST,    /*!< Name or path of an Executable or classname of a plugin to execute. */
        TD_SECOND,   /*!< Main script for spark thread or process initialization or caption of a plugin to execute. */
        TD_THIRD,    /*!< Secondary script for spark thread or process initialization or type id or name of a plugin to execute. */
        TD_LIST_1,   /*!< List of additional scripts to execute upon spark thread or process initialization. */
        TD_LIST_2,   /*!< List of parameters for the thread or plugin, used as command line parameters for processes or run() parameters for thread-plugins. */
        TD_UNDEFINED /*!< Undefined value. Used to signalize that every entry has changed in definitionChanged() */
    };

    /*!
     \brief Type of the task.
    */
    enum ETaskType
    {
        TT_SERVERTHREAD = 0, /*!< Interface to a spark server thread. */
        TT_PLUGINTHREAD,     /*!< Interface to a thread executing a Plugin or SignalPlugin. */
        TT_PROCESS,          /*!< Interface to an executable started as external process. */
        TT_SPARKPROCESS      /*!< Interface to an external spark process.*/
    };

    static const ETaskType mTypes[4];        /*!< Ordered list of all task types. */
    static const QString mTypeNames[4];      /*!< Ordered list of all task type names. */
    static const QString mTypeNamesShort[4]; /*!< Ordered list of all task type names in lowercase and without blanks. Used in save files.*/
    static const int mMaxTypes;              /*!< Amount of task types. */

    /*!
     \brief Returns the name for a given task type.

     getTypeString(ETaskType type) is more efficient.

     \param type type to return string for
     \param uppercase if true, string is in upper case
    */
    static QString getTypeString(ETaskType type, bool uppercase);
    /*!
     \brief Returns the name for a given task type.

     \param type type to return string for
    */
    static const QString& getTypeString(ETaskType type);
    /*!
     \brief Returns short string for a given task type

     \param type type to return string for
    */
    static const QString& getTypeStringShort(ETaskType type);
    
    static bool isThread(ETaskType type);  /*!< Returns true if the given task type is a thread type.*/
    static bool isProcess(ETaskType type); /*!< Returns true if the given task type is a process type.*/
    static bool isPlugin(ETaskType type);  /*!< Returns true if the given task type is a plugin type.*/
    static bool isSpark(ETaskType type);   /*!< Returns true if the given task type is a spark type.*/

    static const QThread::Priority mPriorityTypes[8]; /*!< List of all task priority types. */
    static const QString mPriorityTypeNames[8];       /*!< List of names for all task priority types. */
    static const int mMaxPriorityTypes;               /*!< Amount of task priority types. */
    /*!
     \brief Returns the name for a given task priority type.

     getPriorityTypeString(QThread::Priority type) is more efficient.

     \param type type to return string for
     \param uppercase if true, string is in upper case
    */
    static QString getPriorityTypeString(QThread::Priority type, bool uppercase);
    /*!
     \brief Returns the name for a given task priority type.

     \param type type to return string for
    */
    static const QString& getPriorityTypeString(QThread::Priority type);

public:
    // public functions
    /*!
     \brief Complete constructor.

     \param type task type
     \param name task name
     \param priority task priority
     \param first first data field, executable name or plugin class name
     \param second second data field, simulation script or plugin caption
     \param third third data field, spark script or plugin type
     \param firstList list of additional scripts or plugin thread references
     \param secondList command line parameters for spark or run() parameters for plugin
    */
    TaskDefinition(ETaskType type, const QString& name, QThread::Priority priority = QThread::InheritPriority, 
                     const QString& first = QString(""), const QString& second = QString(""), const QString& third = QString(""),
                     const std::vector<QString>& firstList = std::vector<QString>(), const std::vector<QString>& secondList = std::vector<QString>());
    /*!
     \brief Complete constructor.

     \param type task type
     \param name task name
     \param priority task priority
     \param first first data field, executable name or plugin class name
     \param second second data field, simulation script or plugin caption
     \param third third data field, spark script or plugin type
     \param firstList list of additional scripts or plugin thread references
     \param secondList command line parameters for spark or run() parameters for plugin
    */
    TaskDefinition(ETaskType type, const QString& name, QThread::Priority priority = QThread::InheritPriority, 
                     const QString& first = QString(""), const QString& second = QString(""), const QString& third = QString(""),
                     const QStringList& firstList = QStringList(), const QStringList& secondList = QStringList());
    /*!
     \brief Copy constructor.
    */
    TaskDefinition(const TaskDefinition& obj);
    /*!
     \brief Basic constructor.
    */
    TaskDefinition();
    /*!
     \brief Destructor.
    */
    virtual ~TaskDefinition();
    /*!
     \brief Copy operator.
    */
    const TaskDefinition& operator=(const TaskDefinition& obj);

    /*!
     \brief Returns the task type.
    */
    ETaskType getType() const;
    /*!
     \brief Returns the name.
    */
    const QString& getName() const;
    /*!
     \brief Returns any data field as QVariant copy.
    */    
    QVariant getData(ETaskData field) const;
    /*!
     \brief Returns the task priority.
    */    
    QThread::Priority getPriority() const;
    /*!
     \brief Returns the first data field.
    */
    const QString& getFirst() const;
    /*!
     \brief Returns the first data field.
    */
    const QString& getSecond() const;
    /*!
     \brief Returns the first data field.
    */
    const QString& getThird() const;
    /*!
     \brief Returns const list reference.
    */
    const QStringList& getFirstList() const;
    /*!
     \brief Returns list reference.

     Be sure to call updateDefinitionChanged() after changing any value.
    */
    QStringList& getFirstListNc();
    /*!
     \brief Returns const list reference.
    */
    const QStringList& getSecondList() const;
    /*!
     \brief Returns list reference.

     Be sure to call updateDefinitionChanged() after changing any script value.
    */
    QStringList& getSecondListNc();

    /*!
     \brief Emits definitionChanged(). Is called automatically after all set functions but has to be called manually when using non-const access to lists.
    */
    void updateDefinitionChanged(ETaskData field = TD_UNDEFINED);

    /*!
     \brief Sets the task type.
    */
    void setType(ETaskType type);
    /*!
     \brief Sets the name.
    */
    void setName(const QString& name);
    /*!
     \brief Sets the first value.
    */
    void setFirst(const QString& value);
    /*!
     \brief Sets the second value.
    */
    void setSecond(const QString& value);
    /*!
     \brief Sets the third value.
    */
    void setThird(const QString& value);
    /*!
     \brief Add a list of values at the list end.
    */
    void addToFirstList(const std::vector<QString>& values);
    /*!
     \brief Add a new value at the list end.
    */
    void addToFirstList(const QString& value);
    /*!
     \brief Deletes all parameters.
    */
    void clearFirstList();
    /*!
     \brief Add a list of values at the list end.
    */
    void addToSecondList(const std::vector<QString>& values);
    /*!
     \brief Add a new value at the list end.
    */
    void addToSecondList(const QString& value);
    /*!
     \brief Deletes all parameters.
    */
    void clearSecondList();
    /*!
     \brief Sets the task priority.
    */
    void setPriority(QThread::Priority priority);
    /*!
     \brief Removes a single entry of the list at a given index.
    */
    void removeFromFirstList(int index);
    /*!
     \brief Removes a single entry of the list at a given index.
    */
    void removeFromSecondList(int index);
    /*!
     \brief Swaps two entries at indices index1 and index2.
    */
    void swapInFirstList(int index1, int index2);
    /*!
     \brief Swaps two entries at indices index1 and index2.
    */
    void swapInSecondList(int index1, int index2);
    
    /*!
     \brief Returns the name for the task definitions task type.

     getTypeString() is more efficient.

     \param uppercase if true, string is in upper case
    */
    QString getTypeString(bool uppercase) const;
    /*!
     \brief Returns the name for the task definitions task type.
    */
    const QString& getTypeString() const;
    /*!
     \brief Returns short string for the task definitions task type.
    */
    const QString& getTypeStringShort() const;
    
    bool isThread()  const; /*!< Returns true if the given task type is a thread type.*/
    bool isProcess() const; /*!< Returns true if the given task type is a process type.*/
    bool isPlugin()  const; /*!< Returns true if the given task type is a plugin type.*/
    bool isSpark()   const; /*!< Returns true if the given task type is a spark type.*/

public: signals:
    //public signals

    /*!
     /brief Emitted when an entry of the task definition was changed. Not emitted in initialization function or constructor.
    */
    void definitionChanged(TaskDefinition::ETaskData);

private:
    // private members

    ETaskType mType; /*!< Type of the task. */
    QString mName;   /*!< Name of the task. Used to reference it by plugins, but does not neccesarily need to be unique. */
    QString mFirst;  /*!< Absolute or relative path to executable or plugin class name. */
    QString mSecond; /*!< Absolute or relative path to main script or plugin caption. */
    QString mThird;  /*!< Absolute or relative path to secondary script or plugin type. */
    QStringList mFirstList;      /*!< List of absolute or relative path to additional scripts or initial plugin references. */
    QStringList mSecondList;     /*!< List of parameters for the task and/or its executable or plugin execution parameters. */
    QThread::Priority mPriority; /*!< Thread or process scheduling priority. */
};

#endif //TASKDEFINITION_H
