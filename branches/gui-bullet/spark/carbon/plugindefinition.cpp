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

#include "plugindefinition.h"
#include "abstractplugin.h"
#include "plugin.h"
#include "signalplugin.h"
#include "attachableframe.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PluginDefinition::PluginDefinition(AbstractPlugin* plugin)
{
    LOG_ASSERT(plugin);

    mName = plugin->getClassName();
    mCaption = plugin->getCaption();
    mPluginType = plugin->getPluginType();
    mReloadDefinition = PluginDefinition::RD_INHERIT;
    
    mIsExtension = plugin->isExtension();
    mId = plugin->getPluginId();

    mParameterList.clear();
    mAttachmentList.clear();
}

PluginDefinition::PluginDefinition(const QString& name, const QString& caption, EPluginType plugintype, 
    EReloadDefinition reloaddefinition, const QString& parameter, const QString& attachment)
{
    mName = name;
    mCaption = caption;
    mPluginType = plugintype;
    mReloadDefinition = reloaddefinition;
    mIsExtension = false;
    mId = -1;

    mParameterList.clear();
    mAttachmentList.clear();

    if (parameter.compare("") != 0)
        addParameter(parameter); 
    if (attachment.compare("") != 0)
        addAttachment(attachment); 
}

PluginDefinition::PluginDefinition(const QString& name, const QString& caption, EPluginType plugintype, 
    EReloadDefinition reloaddefinition, const QStringList& parameters, const QStringList& attachments)
    : 
    mName(name),
    mCaption(caption),
    mPluginType(plugintype),
    mReloadDefinition(reloaddefinition),
    mParameterList(parameters),
    mAttachmentList(attachments)
{
    mIsExtension = false;
    mId = -1;

    if (plugintype != PT_FRAME && attachments.size() > 0)
    {
        LOG_WARNING() << "Attachment points only available for attachable frame plugins.";
        mAttachmentList.clear();
    }
}

PluginDefinition::PluginDefinition(const PluginDefinition& obj) :
    mName            (obj.mName),
    mCaption         (obj.mCaption),
    mPluginType      (obj.mPluginType),
    mParameterList   (obj.mParameterList),
    mAttachmentList  (obj.mAttachmentList),
    mReloadDefinition(obj.mReloadDefinition),
    mIsExtension     (obj.mIsExtension),
    mId              (obj.mId)
{
}

PluginDefinition& PluginDefinition::operator=(const PluginDefinition& obj)
{
    mName             = obj.mName;
    mCaption          = obj.mCaption;
    mPluginType       = obj.mPluginType;
    mParameterList    = obj.mParameterList;
    mAttachmentList   = obj.mAttachmentList;
    mReloadDefinition = obj.mReloadDefinition;
    mIsExtension      = obj.mIsExtension;
    mId               = obj.mId;

    updateDataChanged();

    return *this;
}

PluginDefinition::~PluginDefinition()
{
    mParameterList.clear();
}

void PluginDefinition::setId(int id)
{
    mId = id;
}

void PluginDefinition::setIsExtension(bool extension)
{
    mIsExtension = extension;
}

bool PluginDefinition::setName(const QString& name)
{
    QString newName = mName;
    if (name.compare("") != 0)
        newName = name;
    else
        newName = "undefined";

    if (newName.compare(mName) == 0)
        return false;

    mName = newName;
    updateDataChanged(PD_NAME);
    return true;
}

bool PluginDefinition::setCaption(const QString& caption)
{
    QString newCaption = mCaption;
    if (caption.compare("") != 0)
        newCaption = caption;
    else
        newCaption = "undefined";

    if (newCaption.compare(mCaption) == 0)
        return false;

    mCaption = newCaption;
    updateDataChanged(PD_CAPTION);
    return true;
}

bool PluginDefinition::setLibraryName(const QString& name)
{
    QString newName = name;

    if (newName.compare(mLibraryName) == 0)
        return false;

    mLibraryName = newName;
    updateDataChanged(PD_LIBRARY_NAME);
    return true;
}


bool PluginDefinition::setPluginType(EPluginType type)
{
    if (type == mPluginType)
        return false;

    mPluginType = type;

    updateDataChanged(PD_TYPE);
    return true;
}

void PluginDefinition::setReloadDefinition(EReloadDefinition definition)
{
    if (mReloadDefinition == definition)
        return;

    mReloadDefinition = definition;
    updateDataChanged(PD_RELOAD);
}

int PluginDefinition::getId() const
{
    return mId;
}

bool PluginDefinition::isExtension() const
{
    return mIsExtension;
}

const QString& PluginDefinition::getName() const
{
    return mName;
}

const QString& PluginDefinition::getCaption() const                                   
{
    return mCaption;
}

const QString& PluginDefinition::getLibraryName() const                                   
{
    return mLibraryName;
}

EPluginType PluginDefinition::getPluginType() const                            
{
    return mPluginType;
}

PluginDefinition::EReloadDefinition PluginDefinition::getReloadDefinition() const                            
{
    return mReloadDefinition;
}

const QStringList& PluginDefinition::getParameters() const           
{
    return mParameterList;
}

void PluginDefinition::setParameters(const QStringList& parameters)
{
    mParameterList = parameters;
}

bool PluginDefinition::containsParameter(const QString& parameter) const       
{
    for (auto it = mParameterList.begin(); it != mParameterList.end(); it++)
    {
        if (it->compare(parameter) == 0)
        {
            return true;
        }
    }

    return false;
}

void PluginDefinition::addParameter(const QString& parameter)               
{
    if (!containsParameter(parameter))
    {
        mParameterList.push_back(parameter);
        updateDataChanged(PD_PARAMETER);
    }
}

void PluginDefinition::removeParameter(const QString& parameter, bool all)            
{
    bool changed = false;
    for (auto it = mParameterList.begin(); it != mParameterList.end();)
    {
        if (it->compare(parameter) == 0)
        {
            it = mParameterList.erase(it);
            changed = true;
            if (!all)
                break;
        }
        else
        {
            it++;
        }
    }

    if (changed)
        updateDataChanged(PD_PARAMETER);
}

void PluginDefinition::clearParameters()                                     
{
    if (mParameterList.empty())
        return;

    mParameterList.clear();
    updateDataChanged(PD_PARAMETER);
}

void PluginDefinition::setAttachments(const QStringList& attachments)
{
    mAttachmentList = attachments;
}

const QStringList& PluginDefinition::getAttachments() const           
{
    return mAttachmentList;
}

bool PluginDefinition::containsAttachment(const QString& parameter) const       
{
    for (auto it = mAttachmentList.begin(); it != mAttachmentList.end(); it++)
    {
        if (it->compare(parameter) == 0)
        {
            return true;
        }
    }

    return false;
}

void PluginDefinition::addAttachment(const QString& attachment)               
{
    if (mPluginType == PT_FRAME && !containsAttachment(attachment))
    {
        mAttachmentList.push_back(attachment);
        updateDataChanged(PD_ATTACHMENTS);
    }
}

void PluginDefinition::removeAttachment(const QString& parameter, bool all)            
{
    bool changed = false;
    for (auto it = mAttachmentList.begin(); it != mAttachmentList.end();)
    {
        if (it->compare(parameter) == 0)
        {
            it = mAttachmentList.erase(it);
            changed = true;
            if (!all)
                break;
        }
        else
        {
            it++;
        }
    }

    if (changed)
        updateDataChanged(PD_ATTACHMENTS);
}

void PluginDefinition::clearAttachments()                                     
{
    if (mAttachmentList.empty())
        return;

    mAttachmentList.clear();
    updateDataChanged(PD_ATTACHMENTS);
}

bool PluginDefinition::pluginMatches(const AbstractPlugin* plugin, bool vague) const
{
    if (vague)
    {
        if (plugin->getPluginType() == mPluginType)
        if (mName.compare("") == 0 || plugin->getClassName().compare(mName) == 0)
        if (mCaption.compare("") == 0|| plugin->getCaption().compare(mCaption) == 0)
        {
            return true;
        }
        return false;
    }
    else
    {
        if (plugin->getClassName().compare(mName) == 0)
        if (plugin->getCaption().compare(mCaption) == 0)
        if (plugin->getPluginType() == mPluginType)
        if (plugin->isExtension() == mIsExtension)
        {
            return true;
        }
        return false;
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PluginDefinition::updateDataChanged(EPluginData field)
{
    emit dataChanged(field);
}