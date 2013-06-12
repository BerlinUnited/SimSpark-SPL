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

#include "settingsdialog.h"
#include "carbon.h"
#include "settingswidget.h"
#include "pluginmanager.h"

#include <exception>
#include <QLayout>
#include <QToolButton>
#include <QLabel>

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    clearTopics();

    ui.toolWidget->layout()->setContentsMargins(0,0,0,0);

    //Receive plugin removal signals from the manager
    PluginManager* manager = Carbon::get()->getPluginManager();
    connect(manager, SIGNAL(aboutToRemovePlugin(AbstractPlugin*)), SLOT(removeSettingWidgets(AbstractPlugin*)));
}

bool SettingsDialog::registerToSettingWidget(QString topic, QString page, AbstractPlugin* registree)
{
    LOG_ASSERT(registree);

    shared_ptr<PageDefinition> pagePtr(getPageDefinition(topic, page));

    if (pagePtr == 0)
        return false;

    LOG_DEBUG() << "Registering plugin " << registree->getCaption() << " to topic " << topic << " and page " << page << ".";

    pagePtr->addReferencer(registree);
    updateTabWidgetPage(*pagePtr);

    return true;
}

void SettingsDialog::includeSettingWidget(QString topicname, QString pagename, SettingsWidget** widget, AbstractPlugin* creator, QString topicicon)
{
    LOG_DEBUG() << "Including new settings widget" <<
        " with topic " << topicname << " and page " << pagename << " to settings dialog.";
    if (creator != 0)
        LOG_DEBUG() << "Plugin: " << creator->getCaption();

    shared_ptr<TopicDefinition> topic(createTopic(topicname, topicicon));
    shared_ptr<PageDefinition>  page (createPage (topicname, pagename ));

    try
    {
        if (page->mWidget != 0)
        {
            //Already contains a widget, replace in tab
            LOG_WARNING() << "Replacing SettingsWidget '" << topicname.toStdString().c_str() << '/' << pagename.toStdString().c_str() << "' in SettingsDialog.";
            page->addReferencer(creator);

            //Change tabwidget content
            topic->mTabWidget->removeTab(page->mTabIndex);
            page->mWidget = widget;
            topic->mTabWidget->insertTab(page->mTabIndex, *widget, pagename);
        }
        else
        {
            //New widget, insert as new tab
            page->mWidget = widget;
            page->addReferencer(creator);

            //Add tabwidget page
            page->mTabIndex = topic->mTabWidget->addTab(*widget, pagename);
        }
        updateTabWidgetPage(*page);
        page->mUpdateActivated = true;
    }
    catch (std::exception& e)
    {
        LOG_ERROR() << "Exception while including SettingWidget:\n" << e.what();
    }
}

void SettingsDialog::removeSettingWidgets(AbstractPlugin* creator)
{
    LOG_ASSERT(creator);

    //Search all topics
    for (TTopicList::iterator t_it = mTopicList.begin(); t_it != mTopicList.end();)
    {
        shared_ptr<TopicDefinition> topic(*t_it);
        //Search all pages
        for (TPageList::iterator p_it = topic->mPageList.begin(); p_it != topic->mPageList.end();)
        {
            shared_ptr<PageDefinition> page(*p_it);
            bool erased = false;

            //Compare creator
            if (page->containsReferencer(creator))
            {
                LOG_DEBUG() << "Removing settings widget reference of plugin " << creator->getCaption() << " to topic " << t_it->get()->mTopicName <<
                    " and page " << p_it->get()->mPageName << " from settings dialog.";

                //Remove one reference
                page->removeReferencer(creator);

                //Delete if there are no referencers left
                if (page->mReferencers.empty())
                {
                    LOG_DEBUG() << "Settings page " << p_it->get()->mPageName << " has no referecers. Deleting.";

                    //Delete this page and its widget
                    emit settingsWidgetAboutToBeDeleted(topic->mTopicName, page->mPageName);
                    emit settingsWidgetAboutToBeDeleted(*page->mWidget);

                    //Remove tab (deletes widget)
                    topic->mTabWidget->removeTab(page->mTabIndex);
                    //Set to 0, so external creators know of the deletion
                    *(page->mWidget) = 0;

                    //Delete page definition
                    p_it = topic->mPageList.erase(p_it);

                    emit settingsWidgetDeleted(topic->mTopicName, page->mPageName);
                    erased = true;
                }
                else
                {
                    //Update Tabwidgets text
                    updateTabWidgetPage(*page);
                }
            }
            if (!erased)
            {
                p_it++;
            }
        }

        //Delete topic if empty
        if (topic->mPageList.size() == 0)
        {
            LOG_DEBUG() << "Settings topic " << topic->mTopicName << " is empty. Deleting.";

            //Remove stacked widget page
            topic->mStackedWidgetPage->deleteLater();

            //Remove button
            topic->mButton->deleteLater();

            //Delete topic definition
            t_it = mTopicList.erase(t_it);

            //Update indices of following topics (so a button click will still activate the correct page)
            for (auto i_it = t_it; i_it != mTopicList.end(); i_it++)
            {
                //Lower index
                i_it->get()->mIndex--;

                //Update the numbered button
                i_it->get()->mButton->mNumber = i_it->get()->mIndex;
            }
        }
        else
        {
             t_it++;
        }
    }
}

bool SettingsDialog::hasTopic(const QString& topicname)
{
    return getTopicDefinition(topicname) != 0;
}

bool SettingsDialog::hasPage(const QString& topicname, const QString& pagename)
{
    return getPageDefinition(topicname, pagename) != 0;
}

boost::shared_ptr<SettingsDialog::PageDefinition> SettingsDialog::getCurrentPage()
{
    shared_ptr<TopicDefinition> topic(getCurrentTopic());
    if (topic.get() == 0 || topic->mTabWidget->count() == 0)
        return shared_ptr<PageDefinition>();

    return topic->mPageList.at(topic->mTabWidget->currentIndex());
}

boost::shared_ptr<SettingsDialog::TopicDefinition> SettingsDialog::getCurrentTopic()
{
    if (mTopicList.empty())
        return shared_ptr<TopicDefinition>();

    return mTopicList.at(ui.stackedWidget->currentIndex());
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

boost::shared_ptr<SettingsDialog::TopicDefinition> SettingsDialog::getTopicDefinition(const QString& topicname)
{
    for (TTopicList::iterator t_it = mTopicList.begin(); t_it != mTopicList.end(); t_it++ )
    {
        if (t_it->get()->mTopicName.compare(topicname) == 0)
        {
            return *t_it;
        }
    }
    return shared_ptr<TopicDefinition>();
}

boost::shared_ptr<SettingsDialog::PageDefinition> SettingsDialog::getPageDefinition(const QString& topicname, const QString& pagename)
{
    shared_ptr<TopicDefinition> topic(getTopicDefinition(topicname));
    if (topic == 0)
        return shared_ptr<PageDefinition>();

    for (TPageList::iterator it = topic->mPageList.begin(); it != topic->mPageList.end(); it++ )
    {
        if (it->get()->mPageName.compare(pagename) == 0)
        {
            return *it;
        }
    }
    return shared_ptr<PageDefinition>();
}

boost::shared_ptr<SettingsDialog::PageDefinition> SettingsDialog::getPageDefinition(const TopicDefinition& topic, const QString& pagename)
{
    for (TPageList::const_iterator it = topic.mPageList.begin(); it != topic.mPageList.end(); it++ )
    {
        if (it->get()->mPageName.compare(pagename) == 0)
        {
            return *it;
        }
    }
    return shared_ptr<PageDefinition>();
}

boost::shared_ptr<SettingsDialog::TopicDefinition> SettingsDialog::createTopic(const QString& topicname, const QString& iconpath)
{
    shared_ptr<TopicDefinition> topic(getTopicDefinition(topicname));
    if (topic.get() == 0)
    {
        topic = shared_ptr<TopicDefinition>(new TopicDefinition(topicname, 0, 0));
        mTopicList.push_back(shared_ptr<TopicDefinition>(topic));

        //Add new stacked widget page with a label and an empty tabwidget
        QWidget* newWidget = new QWidget;
        QVBoxLayout* newLayout = new QVBoxLayout;
        QTabWidget* newTabWidget = new QTabWidget;
        QLabel* newLabel = new QLabel(topicname);
        QFont font(newLabel->font());
        font.setPointSize(14);
        newLabel->setFont(font);
        newTabWidget->clear();
        newLayout->setContentsMargins(0,0,0,0);
        newLayout->addWidget(newLabel);
        newLayout->addWidget(newTabWidget);
        newWidget->setLayout(newLayout);
        ui.stackedWidget->addWidget(newWidget);

        //Add new entry in toolbox
        NumberedToolButton* button = new NumberedToolButton(getStackedWidgetPageIndex(topicname));
        if (iconpath.compare("") != 0)
            button->setIcon(QIcon(iconpath));
        else
            button->setIcon(QIcon(":none"));
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setIconSize(QSize(30,30));
        button->setText(topicname);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        ui.toolWidget->layout()->addWidget(button);
        button->setAutoRaise(true);

        topic->mTabWidget = newTabWidget;
        topic->mStackedWidgetPage = newWidget;
        topic->mNameLabel = newLabel;
        topic->mButton = button;
        topic->mIndex = button->mNumber;

        //Clicking button emits index of the button
        connect(button, SIGNAL(clicked(int)), this, SLOT(topicButtonClicked(int)));

        //Changing the page or topic updates settings
        connect(newTabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateCurrentSettings()));
    }
    return topic;
}

boost::shared_ptr<SettingsDialog::PageDefinition> SettingsDialog::createPage(const QString& topicname, const QString& pagename)
{
    shared_ptr<TopicDefinition> topic(getTopicDefinition(topicname));
    if (topic.get() == 0)
        topic = shared_ptr<TopicDefinition>(createTopic(topicname));

    shared_ptr<PageDefinition> page(getPageDefinition(*topic, pagename));
    if (page.get() == 0)
    {
        page = shared_ptr<PageDefinition>(new PageDefinition(pagename, 0, 0, topic)); //stores topic as parent. other parameters are defined later
        topic->mPageList.push_back(shared_ptr<PageDefinition>(page));
        page->mWidget = 0;
        page->mTabIndex = 0;
    }

    return page;
}

QWidget* SettingsDialog::getStackedWidgetPage(const QString& topicname)
{
    int index = getStackedWidgetPageIndex(topicname);
    if (index == -1) return 0; //not found
    if (index < 0 || index >= ui.stackedWidget->count())
    {
        LOG_ERROR() << "Index out of bounds.";
        return 0;
    }
    return (SettingsWidget*)(ui.stackedWidget->widget(index));
}

int SettingsDialog::getStackedWidgetPageIndex(const QString& topicname)
{
    int run = 0;
    for (TTopicList::iterator it = mTopicList.begin(); it != mTopicList.end(); it++)
    {
        if (it->get()->mTopicName.compare(topicname) == 0)
            return run;
        run++;
    }
    return -1;
}

void SettingsDialog::clearTopics()
{
    while (ui.stackedWidget->count() > 0)
    {
        ui.stackedWidget->removeWidget(ui.stackedWidget->currentWidget());
    }

    mTopicList.clear();
}

void SettingsDialog::showEvent ( QShowEvent * event )
{
    //Update all settings
    updateSettings();
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void SettingsDialog::updateTabWidgetPage(PageDefinition& page)
{
    QString name;
    QString toolTip;
    int referencers = page.mReferencers.size();
    if (referencers == 1)
    {
        name = page.mPageName;
        toolTip = QString(tr("Used by 1 plugin."));
    }
    else
    {
        name = QString("%1 (%2)").arg(page.mPageName).arg(page.mReferencers.size());
        toolTip = QString(tr("Used by %1 plugin/s.")).arg(page.mReferencers.size());
    }

    shared_ptr<TopicDefinition> topic = page.mTopic.lock();
    if (topic.get())
    {
        topic->mTabWidget->setTabText(page.mTabIndex, name);
        topic->mTabWidget->setTabToolTip(page.mTabIndex, toolTip);
    }
    else
    {
        LOG_ERROR() << "Topic pointer expired without removing the child page!";
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SettingsDialog::topicButtonClicked(int index)
{
    if ((int)ui.stackedWidget->count() <= index || index < 0)
    {
        LOG_ERROR() << "Index " << index << " out of bounds.";
        return;
    }

    if (ui.stackedWidget->currentIndex() == index)
        return;

    //Change page
    ui.stackedWidget->setCurrentIndex(index);

    //Update Settings for this view
    updateCurrentSettings();
}

void SettingsDialog::ok()
{
    apply();
    emit accept();
}

void SettingsDialog::abort()
{
    reset();
    emit reject();
}

void SettingsDialog::apply()
{
    //Search all topics
    for (TTopicList::iterator t_it = mTopicList.begin(); t_it != mTopicList.end(); t_it++ )
    {
        //Search all pages
        for (TPageList::iterator p_it = t_it->get()->mPageList.begin(); p_it != t_it->get()->mPageList.end(); p_it++ )
        {
            //Apply changes
            if (p_it->get()->mUpdateActivated)
            {
                (*(p_it->get()->mWidget))->applyChanges();
            }
        }
    }
}

void SettingsDialog::reset()
{
    //Search all topics
    for (TTopicList::iterator t_it = mTopicList.begin(); t_it != mTopicList.end(); t_it++ )
    {
        //Search all pages
        for (TPageList::iterator p_it = t_it->get()->mPageList.begin(); p_it != t_it->get()->mPageList.end(); p_it++ )
        {
            //Undo changes
            if (p_it->get()->mUpdateActivated)
                (*(p_it->get()->mWidget))->undoChanges();
        }
    }
}

void SettingsDialog::updateSettings(int topicindex, int pageindex)
{
    if (topicindex < 0 || topicindex >= (int)mTopicList.size())
    {
        LOG_ERROR() << "Topic index " << topicindex << "out of bounds.";
        return;
    }
    if (pageindex < 0 || pageindex >= (int)mTopicList.at(topicindex)->mPageList.size())
    {
        LOG_ERROR() << "Page index " << pageindex << "out of bounds.";
        return;
    }

    shared_ptr<PageDefinition> page(mTopicList.at(topicindex)->mPageList.at(pageindex));
    if (page->mUpdateActivated)
        (*(page->mWidget))->updateSettings();
}

void SettingsDialog::updateCurrentSettings()
{
    shared_ptr<PageDefinition> page(getCurrentPage());

    if (page != 0)
        if (page->mUpdateActivated)
            (*(page->mWidget))->updateSettings();
}

void SettingsDialog::updateSettings()
{
    //Search all topics
    for (TTopicList::iterator t_it = mTopicList.begin(); t_it != mTopicList.end(); t_it++ )
    {
        //Search all pages
        for (TPageList::iterator p_it = t_it->get()->mPageList.begin(); p_it != t_it->get()->mPageList.end(); p_it++ )
        {
            //Update settings
            if (p_it->get()->mUpdateActivated)
                (*(p_it->get()->mWidget))->update();
        }
    }
}
