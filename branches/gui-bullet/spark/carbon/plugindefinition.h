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

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H
/**
 * \file plugindefinition.h
 * \brief Contains PluginDefinition class, which is a part of a SimulationSetup.
 */

#include "plugintype.h"
#include "attachableframe.h"
#include <QString>
#include <QStringList>

/*!
 \brief Definition for a Plugin and its parameters.

 Used in ProgramSetup definitions.
 
 Plugin Parameters specify call or setup parameters for the plugin. For simple plugins and signal plugins, they are used as run() call arguments. 
 For AttachableFrames they define the attachment points.

 PluginDefinitions can either specify a Plugin precisely or vaguely. A precise definition always contains a name, caption, plugintype (used to control the other fields),
 a reload definition and parameters/attachment points. It specifies which plugin to load and how to load it. A vague definition is used to describe classes of plugins and may
 also contain a frame type. A Plugin matches the definition, when all fields match the definitions data fields that are present. E.g. a definition with Caption "Plugin" and
 frame type "Console" will match with all plugins named "Plugin" and are of the plugin type "Frame", and frame type "Console". When used as vague definition, Parameters and
 References are ignored.
*/
class PluginDefinition : public QObject
{
    Q_OBJECT

public:
    //public types

    /*!
     \brief Definition of the reload behaviour of a plugin.

     Comes to use when a new SimulationSetup is initialized.
    */
    enum EReloadDefinition
    {
        RD_INHERIT,     /*!< Use the specification of the parent SimulationSetup. */
        RD_RELOAD,      /*!< Reload the plugin. Useful for plugins with specific setup dependant information. */
        RD_NO_RELOAD    /*!< Do not reload the plugin. Useful for general plugins and those with general information. */
    };

    /*!
     \brief List of PluginDefinition data fiels.
    */
    enum EPluginData
    {
        PD_NAME = 0,     /*!< Name of the plugin class.*/
        PD_CAPTION,      /*!< Caption of the plugin. */ 
        PD_LIBRARY_NAME, /*!< Name of the library the plugin belongs to. */
        PD_TYPE,         /*!< EPluginType of the plugin. */
        PD_RELOAD,       /*!< Reload definition of the plugin. */
        PD_PARAMETER,    /*!< List of initialization parameters for initialization of the plugin.*/
        PD_ATTACHMENTS,  /*!< List of attachment points for the plugin. Only used if plugin is an attachable frame.*/
        PD_UNDEFINED     /*!< Undefined value. Used to signalize that every entry has changed in definitionChanged() */
    };

public:
    // public functions

    /*! 
     \brief Complete constructor.

     Fill the definition with the specifications in the plugin.
     \param plugin plugin to create definition for
    */
    PluginDefinition(AbstractPlugin* plugin);

    /*!
     \brief Complete constructor.

     \param name name of the plugin
     \param caption caption of the plugin
     \param plugintype type of the plugin
     \param reloaddefinition reload behaviour of the plugin
     \param parameter single execution parameter for the plugin
     \param attachment single attachment point for the plugin. only for attachable frame plugins
    */
    PluginDefinition(const QString& name = QString("pluginclass"), const QString& caption = QString("newplugin"), EPluginType plugintype = PT_NORMAL,
        EReloadDefinition reloaddefinition = RD_INHERIT, const QString& parameter = QString(), const QString& attachment = QString());
    /*!
     \brief Complete constructor.

     \param name name of the plugin
     \param caption caption of the plugin
     \param plugintype type of the plugin
     \param reloaddefinition reload behaviour of the plugin
     \param parameters list of execution parameters for the plugin
     \param attachments list of attachment points for the plugin. only for attachable frame plugins
    */
    PluginDefinition(const QString& name, const QString& caption, EPluginType plugintype,
        EReloadDefinition reloaddefinition, const QStringList& parameters, const QStringList& attachments = QStringList());
    /*!
     \brief Copy constructor. Does not copy connections.
    */
    PluginDefinition(const PluginDefinition& obj);
    /*!
     \brief Copy operator. Does not copy connections.
    */
    PluginDefinition& operator=(const PluginDefinition& obj);

    /*!
     \brief Basic destructor.
    */
    virtual ~PluginDefinition();

    /*!
     \brief Specify an id. 
     
     This is simply a storage to identify the definition with. The usage is handled externally.

     \param id id to store
     */
    void setId(int id);
    /*!
     \brief Specify the definition to be an extension definition. 
     
     This is simply a storage to identify the definition with. The usage is handled externally.

     \param extension true to declare extension, false to declare normal plugin
     */
    void setIsExtension(bool extension);
    /*!
     \brief Set the plugin name.

     Empty name is not allowed.

     \return true if name was changed
    */
    bool setName(const QString& name);
    /*!
     \brief Set the plugin caption.

     Empty caption is not allowed.

     \return true if caption was changed
    */
    bool setCaption(const QString& caption);
    /*!
     \brief Set the library name (without extension).

     \return true if name was changed
    */
    bool setLibraryName(const QString& name);
    /*!
     \brief Set the plugin type.

     PT_NONE is not allowed. Resets all entries of AttachableFrame information when changed to a non-frame.

     \return true if type was changed
    */
    bool setPluginType(EPluginType type);
    /*!
     \brief Set the reload definition.
    */
    void setReloadDefinition(EReloadDefinition definition);

    /*!
     \brief Returns the stored id.

     The id is no guaranteed identification of the setup. It is a storage for an id value that can be used to identify the definition.
     Its usage is handled externally.

     \return stored id
    */
    int getId() const;
    /*!
     \brief Returns the extension flag.

     The flag is no guaranteed indicator that the definition is the definition of a current extension plugin. 
     It is a storage for the flag that is used by the PluginManager externally.
    */
    bool isExtension() const;
    /*!
     \brief Returns the plugin name.
    */
    const QString& getName() const;
    /*!
     \brief Returns the plugin caption.
    */
    const QString& getCaption() const;
    /*!
     \brief Returns the plugin caption.
    */
    const QString& getLibraryName() const;
    /*!
     \brief Returns the plugin type.
    */
    EPluginType getPluginType() const;
    /*!
     \brief Returns the reload definition.
    */
    EReloadDefinition getReloadDefinition() const;
    /*!
     \brief Returns reference to parameter list.
    */
    const QStringList& getParameters() const;
    /*!
     \brief Returns reference to attachment point list.
    */
    const QStringList& getAttachments() const;

    /*!
     \brief Set the parameter list.
    */
    void setParameters(const QStringList& parameters);
    /*!
     \brief Returns true if the parameter is already contained.
    */
    bool containsParameter(const QString& parameter) const;
    /*!
     \brief Adds the parameter if it is not already in the list.
    */
    void addParameter(const QString& parameter);
    /*!
     \brief Romoves all parameter with this name.

     \param parameter name
     \param all if true, all occurences are deleted, if not only 1 is deleted
    */
    void removeParameter(const QString& parameter, bool all = true);
    /*!
     \brief Removes all parameters.
    */
    void clearParameters();

    /*!
     \brief Set the attachment list.
    */
    void setAttachments(const QStringList& attachments);
    /*!
     \brief Returns true if the attachment is already contained.
    */
    bool containsAttachment(const QString& attachment) const;
    /*!
     \brief Adds the attachment if it is not already in the list.
    */
    void addAttachment(const QString& attachment);
    /*!
     \brief Romoves all attachment with this name.

     \param attachment name
     \param all if true, all occurences are deleted, if not only 1 is deleted
    */
    void removeAttachment(const QString& attachment, bool all = true);
    /*!
     \brief Removes all attachments.
    */
    void clearAttachments();
    /*!
     \brief Returns true if the definition describes this plugin.

     If vague is false, class name, plugin type, frame type and plugin caption have to be specified and identical. 
     If vague is true, only set entries are used to compare. For example, if the definition has an empty name or captions, all plugin names/captions will match. 
     If the FrameType is FT_UNDEFINED, all frame types will match.

     \sa PluginDefinition

     \param plugin plugin to match with
     \param vague detail needed to match
    */
    bool pluginMatches(const AbstractPlugin* plugin, bool vague = false) const;

public: signals:

    /*!
     \brief Emitted when updateDataChanged is called or any data field is set using the access functions.
    */
    void dataChanged(EPluginData field);

public slots:
    /*!
     \brief Emits dataChanged. 

     PT_UNDEFINED is used to inform that every field has or might have changed.
    */
    void updateDataChanged(EPluginData field = PD_UNDEFINED);

public:
    // public members

    QString mName;           /*!< Name of the Plugin. Either use this name (unique) or a frametype (more general). */
    QString mCaption;        /*!< Caption for the Plugin. */
    QString mLibraryName;    /*!< Filename of the library the plugin is included in. (Empty string in case of built-in plugin) */
    EPluginType mPluginType; /*!< Type of the plugin.*/
    QStringList mParameterList;    /*!< List of initialization parameters. */
    QStringList mAttachmentList;   /*!< Priorized list of attachment points. Only used for attachable frame plugins.*/
    EReloadDefinition mReloadDefinition;    /*!< Defines reload behaviour of this plugin. */

    int mId; /*!< Storage for plugin id associated with or created from this setup. */
    bool mIsExtension; /*!< Storage for a flag that signals this definition to be associated with or created from a plugin extension. */
};

#endif //PLUGINDEFINITION_H
