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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
/**
 * \file settingsdialog.h
 * \brief Contains SettingsDialog class, the main window inheriting SettingsWidget windows, and the helper-class NumberedToolButton.
 */

#include "ui_settingsdialog.h"

#include <QToolButton>
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif

/*!
 \brief Helper-Class. A QToolButton, that contains a number and emits it when clicked.
*/
class NumberedToolButton : public QToolButton
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Basic constructor.

     \param number contained number
    */
    NumberedToolButton(int number)
    {
        mNumber = number;
        connect(this, SIGNAL(clicked()), this, SLOT(emitNumber()));
    }

public: signals:

    /*!
     \brief Signal called when clicked, containing the buttons number.

     \param mNumber contained number.
    */
    void clicked(int mNumber);

public slots:

    /*!
     \brief Slot that emits the contained number as clicked(int).
    */
    void emitNumber()
    {
        emit clicked(mNumber);
    }

public:
    //public members

    int mNumber; /*!< Contained number. */
};

class AbstractPlugin;
class QLabel;
class SettingsWidget;

/*!
 \brief Dialog Window that contains SettingsWidget from the Carbon and any source that provides one.

 The dialog sorts registered Widgets using a topic name and a page (subtopic) name. Topics may (and should) contain in icon that
 identifies the topic in a Topic Toolbox on the left Side of the Window.
 Chosing a topic displays its sub-pages as tabs of a QTabWidget on the right side of the window. The Dialog Buttons Apply, Undo, Ok, and Close
 call the virtual Functions of all contained SettingsWidgets. If they are programmed correctly (using this interface) this allows changing
 and checking or reverting all Setting changes before actually applying them.

 /todo At the moment, the Dialog is created once by the Carbon and only hidden instead of closed when needed. This allows adding a
 new widget simply by creating it in a plugins constructor and attaching it directly (easy solution). However this uses a small amount of
 memory to store this window all the time, which might increase significantly if there should be setting-windows which use more memory.
 Using a Factory and a registration function for a setting widgets constructor would be more elegant and would allow creation of the
 settings window whenever needed, so it can be closed and it does not use space in the memory. But most likely the used space will always be so small,
 that it is not worth the amount of work.
*/
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    //public types

    struct TopicDefinition;

    /*!
     \brief Public helper struct containing page definition pointers and values.
    */
    struct PageDefinition
    {
        /*!
         \brief Detailed constructor.
        */
        PageDefinition(QString pageName, SettingsWidget** widget, AbstractPlugin* creator, boost::weak_ptr<TopicDefinition> topic)
            : mTopic(topic)
        {
            mWidget = widget;
            if (creator != 0)
                mReferencers.push_back(creator);
            mPageName = pageName;
            mTabIndex = 0;
            mUpdateActivated = false;
        }
        /*!
         \brief Basic constructor.
        */
        PageDefinition()
        {
            mWidget = 0;
            mPageName = "";
            mTabIndex = 0;

            mReferencers.clear();
        }

        /*!
         \brief Basic destructor.

         Contains only pointers, but does not own objects. They are not deleted.
        */
        virtual ~PageDefinition()
        {
            mWidget = 0;

            mReferencers.clear();
        }

        /*!
         \brief Returns true if the referencer is already in the list.
        */
        bool containsReferencer(AbstractPlugin* referencer)
        {
            for (std::vector<AbstractPlugin*>::iterator it = mReferencers.begin(); it != mReferencers.end(); it++)
                if (*it == referencer) return true;
            return false;
        }

        /*!
         \brief Removes a referencer if it is in the list.
        */
        void removeReferencer(AbstractPlugin* referencer)
        {
            for (std::vector<AbstractPlugin*>::iterator it = mReferencers.begin(); it != mReferencers.end(); )
            {
                if (*it == referencer)
                {
                    *it = 0;
                    it = mReferencers.erase(it);
                }
                else
                    it++;
            }
        }

        /*!
         \brief Adds a new referencer if it is not in the list.
        */
        void addReferencer(AbstractPlugin* referencer)
        {
            if (!containsReferencer(referencer))
                mReferencers.push_back(referencer);
        }

        std::vector<AbstractPlugin*> mReferencers;  /*!< Referencing plugins. */
        SettingsWidget** mWidget; /*!< Pointer to the settings widget contained in a tab of this topics tab widget. */
        QString mPageName;        /*!< Name of the Page. */
        boost::weak_ptr<TopicDefinition> mTopic;  /*!< Pointer to parent topic. */
        int mTabIndex;            /*!< Index in tab widget. */
        bool mUpdateActivated;    /*!< Ensures that update functions are not called before the end of a providing plugin constructor. */
    };

    /*!
     \brief Helper-Type containing a list of PageDefinitions.
    */
    typedef std::vector<boost::shared_ptr<PageDefinition>> TPageList;

    /*!
     \brief Public helper struct containing a page list and topic definition pointers and values.
    */
    struct TopicDefinition
    {
        /*!
         \brief Detailed constructor.
        */
        TopicDefinition(QString topicName, QTabWidget* tabWidget, QWidget* stackedWidgetPage)
        {
            mTabWidget = tabWidget;
            mTopicName = topicName;
            mStackedWidgetPage = stackedWidgetPage;
            mButton = 0;
            mNameLabel = 0;
            mIndex = -1;
        }
        /*!
         \brief Basic constructor.
        */
        TopicDefinition()
        {
            mTabWidget = 0;
            mTopicName = "";
            mStackedWidgetPage = 0;
            mButton = 0;
            mNameLabel = 0;
            mIndex = -1;
        }

        /*!
         \brief Basic destructor.

         Does not own pointers. They are not deleted.
        */
        virtual ~TopicDefinition()
        {
            mTabWidget = 0;
            mStackedWidgetPage = 0;
            mButton = 0;
            mNameLabel = 0;
        }

        QString mTopicName;          /*!< Name of the Topic. */
        TPageList mPageList;         /*!< List of child pages (in a TabWidget). */
        QTabWidget* mTabWidget;      /*!< Pointer to TabWidget, which contains the pages. */
        QWidget* mStackedWidgetPage; /*!< Pointer to the StackedWidget page that contains the TabWidget. */
        QLabel* mNameLabel;          /*!< Pointer to the Title Label in the StackedWidget page. */
        NumberedToolButton* mButton; /*!< Pointer to the topic button in the list. */
        int mIndex;                  /*!< Index of the topic. */
    };

    /*!
     \brief Helper-Type containing a list of TopicDefinitions.
    */
    typedef std::vector<boost::shared_ptr<TopicDefinition>> TTopicList;

public:
    // public functions

    /*!
     \brief Basic Constructor.

     \param parent parent widget
    */
    explicit SettingsDialog(QWidget *parent = 0);

    /*!
     \brief Returns true if a topic with the given name already exists.

     \param topicname name to search for
    */
    bool hasTopic(const QString& topicname);
    /*!
     \brief Returns if a page with the given name exists in the given topic.

     \param topicname name of topic to search for
     \param pagename name of page to search for
    */
    bool hasPage(const QString& topicname, const QString& pagename);

    /*!
     \brief Returns current PageDefinition or null pointer if there is none.
    */
    boost::shared_ptr<PageDefinition> getCurrentPage();
    /*!
     \brief Returns current TopicDefinition or null pointer if there is none.
    */
    boost::shared_ptr<TopicDefinition> getCurrentTopic();

    /*!
     \brief Include a Widget SettingsWidget under the name of a topic and a page.

     The widget is given as a pointer to an external Widget!
     This means it must not be deleted after creation. The SettingsDialog will delete the settingswidget when the creator is destroyed
     or removed from the PluginManager and set *widget to 0. So by checking *widget 's content being 0, it can be checked if the
     SettingsWidget still exists.
     Additionally, the signal settingsWidgetAboutToBeDeleted(SettingsWidget*), settingsWidgetAboutToBeDeleted(QString topic, QString page) and
     settingsWidgetDeleted(QString topic, QString page) are emitted before and after deletion.

     \param topic target topic name
     \param page target page name
     \param widget pointer to widget to attach, preferably a SettingsWidget
     \param creator creator of the widget
     \param iconpath path to an icon for the topic button
    */
    void includeSettingWidget(QString topic, QString page, SettingsWidget** widget, AbstractPlugin* creator, QString iconpath = QString(""));

    /*!
     \brief Notifies the Dialog that another plugin is using this page.

     The page is deleted as soon as there are no registered plugins using this page anymore.
     \param topic target topic name
     \param page target page name
     \param registree plugin that is using the settings widget
     \return true if there are a topic and page with these names
    */
    bool registerToSettingWidget(QString topic, QString page, AbstractPlugin* registree);


protected:
    // protected functions

    /*!
     \brief Return topic definition pointer by name.

     Pointer is only valid as long as the topic list is not changed!
     \param topicname name of the topic
     \return null pointer if topic is not found
    */
    boost::shared_ptr<TopicDefinition> getTopicDefinition(const QString& topicname);
    /*!
     \brief Return page definition pointer by name.

     Pointer is only valid as long as the page list is not changed!
     \param topicname name of the topic
     \param pagename name of the page
     \return null pointer if topic or page is not found
    */
    boost::shared_ptr<PageDefinition> getPageDefinition(const QString& topicname, const QString& pagename);
    /*!
     \brief Return page definition pointer by name.

     Pointer is only valid as long as the page list is not changed!
     \param topic reference to the topic
     \param pagename name of the page
     \return null pointer if page is not found
    */
    boost::shared_ptr<PageDefinition> getPageDefinition(const TopicDefinition& topic, const QString& pagename);

    /*!
     \brief Creates a new topic with the name topicname.

     If the topic already exists, the existing definition is returned.
     \param topicname name for the topic
     \param iconpath path to an icon for the topic button
     \return pointer to the added topic with name topicname
    */
    boost::shared_ptr<TopicDefinition> createTopic(const QString& topicname, const QString& iconpath = QString(""));
    /*!
     \brief Creates a new page with the name pagename in topic topicname.

     If the topic does not exist yet, it is created.
     If the page already exists, the existing definition is returned.
     \param topicname name of the topic
     \param pagename name of the page
     \return pointer to the added topic with name topicname
    */
    boost::shared_ptr<PageDefinition> createPage(const QString& topicname, const QString& pagename);

    /*!
     \brief Adds a new page to the stacked widget, that represents one topic.

     \param topicname name of the topic
     \param topic pointer to TopicDefinition to fill data in
    */
    void addStackedWidgetPage(const QString& topicname, TopicDefinition* topic);
    /*!
     \brief Returns the StackedWidget page for a topic name.

     \param topicname name of topic page
    */
    QWidget* getStackedWidgetPage(const QString& topicname);
    /*!
     \brief Returns the index of the StackedWidget page for a topic name.

     \param topicname name of topic page
    */
    int getStackedWidgetPageIndex(const QString& topicname);

    /*!
     \brief Clears all topics and the StackedWidget.
    */
    void clearTopics();

private:
    //privat functions

    /*!
     \brief Updates the title and tooltip of the page definitions tab widget page.

     \param page page to update
    */
    void updateTabWidgetPage(PageDefinition& page);

    /*!
     \brief Update the view when showed.

     \param event event
    */
    virtual void showEvent ( QShowEvent * event );

public: signals:

    /*!
     \brief Emitted before a settings widget is deleted.

     The pointer is still valid.
    */
    void settingsWidgetAboutToBeDeleted(SettingsWidget* widget);
    /*!
     \brief Emitted before a settings widget is deleted.
    */
    void settingsWidgetAboutToBeDeleted(QString topic, QString page);
    /*!
     \brief Emitted after a settings widget is deleted.
    */
    void settingsWidgetDeleted(QString topic, QString page);

public slots:

    /*!
     \brief Changes the visible topic page.

     Called when a topic button is clicked.
     \param index index of the topic button
    */
    void topicButtonClicked(int index);

    /*!
     \brief Applys changes and closes window.
    */
    void ok();
    /*!
     \brief Discards changes and closes window.
    */
    void abort();
    /*!
     \brief Applys changes.
    */
    void apply();
    /*!
     \brief Discards changes.
    */
    void reset();
    /*!
     \brief Calls the update function of a SettingsWidget.

     \param topic index of the topic
     \param page index of the page
    */
    void updateSettings(int topic, int page);
    /*!
     \brief Calls the update function of all SettingsWidgets.
    */
    void updateSettings();
    /*!
     \brief Calls the updateSettings function of the current SettingsWidget.
     */
    void updateCurrentSettings();

    /*!
     \brief Removes all widgets provided by a specific creator.

     Activated by the pluginmanager whenever a plugin is removed.
     \param creator creator to delete widgets from
    */
    void removeSettingWidgets(AbstractPlugin* creator);

private:
    // private members

    Ui::SettingsDialog ui; /*!< UI definition object. */
    TTopicList mTopicList; /*!< List of topics and pages. */
    std::vector<std::pair<QString, QTabWidget> > mStackedWidgetPageNames;  /*!< List of stacked widget page names. */
};

#endif // SETTINGSDIALOG_H
