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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
/**
 * \file windowmanager.h
 * \brief Contains WindowManager class, which handles instantiation and connection of Gui Elements.
 */

#include "windowdefinition.h"

#include <QString>
#include <map>

/*!
  \brief Export macro to export a main window layout from a shared library.

  <b>Parameter:</b>\n
  CLASS: Class name of the QMainWindow derived class.

  <b>Usage:</b>\n
  Define a class derived from QMainWindow.
  Use the export macro in a separate export-header file.
  Using the export macro in a file with a Qt derived file (a file that will be parsed by the qt meta object compiler)
  won't work.

  The main application will call the export function defined by this macro.
  This will register the static logger singleton, the static plugin factory and the static carbon pointer in the shared library and then instantiate
  the main window class and return it to the main application.
*/
#define EXPORT_LAYOUT(CLASS) \
    extern "C"{ \
        LIB_EXPORT void* ExportLayout(void* loggerRegistration, void* carbon, void* pluginFactory) \
        { \
            if (loggerRegistration != 0) Logger::registerStaticLogger((Logger::LoggerRegistration)loggerRegistration); \
            if (carbon != 0) Carbon::registerStaticCarbonPointer((Carbon*)carbon); \
            if (pluginFactory != 0) PluginFactory::registerStaticFactory((PluginFactory*)pluginFactory); \
            return (void*) new CLASS(); \
        }}

class Carbon;
class QTabWidget;
class QDockWidget;
class QMainWindow;
class QLayout;

/*!
 \brief Manages FrameAttachmentPoints. Connects AttachableFrames to FrameAttachmentPoints.

 The GuiLayout may register FrameAttachmentPoints to the WindowManager so AttachableFrames can be connected to them.

 The Manager receives frame attachment information and instantiated frames from the plugin manager and attaches
 these frames to their target AttachmentPoint.
*/
class WindowManager
{

public:
    // public types

    /*!
     \brief Helper-Type
    */
    typedef std::map<int, WindowDefinition*> TWindowDefinitionMap;
    /*!
     \brief Helper-Type
    */
    typedef std::map<QString, FrameAttachmentPoint*> TFrameAttachmentMap;

public:
    // public functions

    /*!
     \brief Basic constructor.

     \param parent carbon instance that created the manager
    */
    WindowManager(Carbon* parent);
    /*!
     \brief Basic destructor.
    */
    virtual ~WindowManager();

    /*!
     \brief Returns true if window with given id exists.

     The window id is identical to the contained plugins id.
    */
    bool hasWindow(int id);
    /*!
     \brief Returns pointer to window with given id and 0 if no window was found.

     The window id is identical to the contained plugins id.
    */
    WindowDefinition* getWindow(int id);

    /*!
     \brief Specifies a new main window.

     Should only be used once by the MainWindow and before AttachableFrame initialization.
     \param window new main window
    */
    void setMainWindow(QMainWindow* window);

    /*!
     \brief Returns pointer to the MainWindow.
    */
    QMainWindow* getMainWindow();

    /*!
     \brief Returns true if a FrameAttachmentPoint with the given name exists.

     \param name name of the point to search for
    */
    bool hasFrameAttachmentPoint(QString name);
    /*!
     \brief Returns pointer to a "default" flagged FrameAttachmentPoint, or 0 if nothing was found.

     Pointer may change when Attachments are changed. Dont store it, only use it for temporary access.

     If free is true, only attachment point which still can accept connections are considered.

     \param free search for free default attachment point
    */
    FrameAttachmentPoint* findDefaultFrameAttachmentPoint(bool free);
    /*!
     \brief Returns pointer to a FrameAttachmentPoint with the given name, or 0 if nothing was found.

     Pointer may change when Attachments are changed. Dont store it, only use it for temporary access.
     \param name name to search for
    */
    FrameAttachmentPoint* findFrameAttachmentPoint(QString name);
    /*!
     \brief Returns pointer to a FrameAttachmentPoint with the given attached frame, or 0 if nothing was found.

     Pointer may change when Attachments are changed. Dont store it, only use it for temporary access.
     \param frame frame to search for
    */
    FrameAttachmentPoint* findFrameAttachmentPoint(AttachableFrame* frame);
    /*!
     \brief Returns reference to FrameAttachmentPoint map.
    */
    TFrameAttachmentMap& getFrameAttachmentPoints();

    /*!
     \brief Returns true if a frame can be attached with a given attachment point name.

     Checks if the attachment point exists and if it still is able to attach a new frame.

     \param attachmentName name of the attachment
    */
    bool canAttach(QString attachmentName);
    /*!
     \brief Creates an attachment of an AttachableFrame and the first free "default" flagged attachment point.

     Used as fallback when an attachment point is not found.

     \param frame frame to attach
     \param caption caption of the frame
     \return true if attachment was a success
    */
    bool createDefaultAttachment(AttachableFrame* frame, QString caption);
    /*!
     \brief Creates an attachment of an AttachableFrame and an attachment point.

     \param frame frame to attach
     \param attachmentName name of the attachment
     \param caption caption of the frame
     \return true if attachment was a success
    */
    bool createAttachment(AttachableFrame* frame, QString attachmentName, QString caption);
    /*!
     \brief If possible, unattaches a frame of his position and reattaches it elsewhere.

     \param frame frame to reattach
     \param attachmentName name of the attachment
     \return true if reattachment was a success
    */
    bool changeAttachment(AttachableFrame* frame, QString attachmentName);
    /*!
     \brief Notification of a frames destruction. Removes attachments of this frame.

     \param frame frame pointer
    */
    void notifyDestruction(AttachableFrame* frame);

    /*!
     \brief Registers a new FrameAttachmentPoint.

     Can be called from a MainWindow to specify a new location to attach AttachableFrames to.
     \param attachmentName unique name of the attachment point
     \param attachmentPoint pointer to the attachment point
     \param defaultPoint if true, this attachment is flagged as default point to connect plugins without attachment target to
    */
    void registerFrameAttachmentPoint(QString attachmentName, QWidget* attachmentPoint, bool defaultPoint = false);
    /*!
     \brief Registers a new FrameAttachmentPoint.

     Can be called from a MainWindow to specify a new location to attach AttachableFrames to.
     \param attachmentName unique name of the attachment point
     \param attachmentPoint pointer to the attachment point
     \param defaultPoint if true, this attachment is flagged as default point to connect plugins without attachment target to
    */
    void registerFrameAttachmentPoint(QString attachmentName, QFrame* attachmentPoint, bool defaultPoint = false);
    /*!
     \brief Registers a new FrameAttachmentPoint.

     Can be called from a MainWindow to specify a new location to attach AttachableFrames to.
     \param attachmentName unique name of the attachment point
     \param attachmentPoint pointer to the attachment point
     \param defaultPoint if true, this attachment is flagged as default point to connect plugins without attachment target to
    */
    void registerFrameAttachmentPoint(QString attachmentName, QTabWidget* attachmentPoint, bool defaultPoint = false);
    /*!
     \brief Registers a new FrameAttachmentPoint.

     Can be called from a MainWindow to specify a new location to attach AttachableFrames to.
     \param attachmentName unique name of the attachment point
     \param attachmentPoint pointer to the attachment point
     \param defaultPoint if true, this attachment is flagged as default point to connect plugins without attachment target to
    */
    void registerFrameAttachmentPoint(QString attachmentName, QDockWidget* attachmentPoint, bool defaultPoint = false);
    /*!
     \brief Registers a new FrameAttachmentPoint.

     Can be called from a MainWindow to specify a new location to attach AttachableFrames to.
     \param attachmentName unique name of the attachment point
     \param attachmentPoint pointer to the attachment point
     \param defaultPoint if true, this attachment is flagged as default point to connect plugins without attachment target to
    */
    void registerFrameAttachmentPoint(QString attachmentName, QMainWindow* attachmentPoint, bool defaultPoint = false);
    /*!
     \brief Registers a new FrameAttachmentPoint.

     Can be called from a MainWindow to specify a new location to attach AttachableFrames to.
     \warning The attachment type has to match the provided widgets type. Creator-Attachmentpoints take any QWidget, while the other attachment points
     need a widget that matches the specified EFrameAttachmentType.

     \param attachmentName unique name of the attachment point
     \param attachmentPoint pointer to the attachment point
     \param type type of the attachment point
     \param defaultPoint if true, this attachment is flagged as default point to connect plugins without attachment target to
    */
    void registerFrameAttachmentPoint(QString attachmentName, QWidget* attachmentPoint, FrameAttachmentPoint::EFrameAttachmentType type, bool defaultPoint = false);

public:
    //public static functions

    /*!
     \brief Changes a layout to a fixed layout style.

     Should be called by every AttachableFrame to unify the main layout of all created frames.
     \param layout layout to change.
    */
    static void changeToNeutralLayout(QLayout* layout);
    /*!
     \brief Creates a layout object with a fixed layout style.
    */
    static QLayout* createNeutralLayout();

private:
    //private members

    Carbon* mParent;   /*!< Carbon that created the WindowManager. */
    QMainWindow* mMainWindow; /*!< Pointer to the MainWindow. HAS to be set by the MainWindow. */

    TWindowDefinitionMap mWindowDefinitions;    /*!< Map of all WindowDefinition objects (connections between frames and attachment points). */
    TFrameAttachmentMap mFrameAttachmentPoints; /*!< Map of all attachment points. */
};

#endif // WINDOWMANAGER_H
