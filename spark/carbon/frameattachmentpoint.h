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

#ifndef FRAMEATTACHMENTPOINT_H
#define FRAMEATTACHMENTPOINT_H
/**
 * \file frameattachmentpoint.h
 * \brief Contains FrameAttachmentPoint and ExtendingFrameAttachmentPoint classes, defining different widgets where
 * an AttachableFrame can be attached to.
 */

#include <QObject>
#include <QString>
#include <QPoint>
#include <QMap>
#include <vector>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class QWidget;
class QFrame;
class QTabWidget;
class QDockWidget;
class QMainWindow;
class AttachableFrame;
class NumberedActionReceiver;

/*!
 \brief Defines where and how frames derived from AttachableFrame can be attached.

 FrameAttachmentPoints are defined and created in the main gui layout (e.g. GuiMainWindow or DockableMainWindow) and are identified
 with a unique name.
 They are basically widgets of different types that are able to inherit child frames.
 Some are able to connect many frames (like TabWidget attachment points), and some may only accept one.
 See GuiMainWindow.cpp constructor to find an example of how to add a FrameAttachmentPoint to a QMainWindow or other QWidgets.
*/
class FrameAttachmentPoint : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FrameAttachmentPoint)

public:
    // protected types

    /*!
     \brief The type of the Widget to attach to.

     WARNING: This type has to match the given Widgets type or the application may crash.
    */
    enum EFrameAttachmentType
    {
        ATTACHMENT_NONE,                /*!< Illegal Attachment. */
        ATTACHMENT_WIDGET,              /*!< Attachmentpoint on a QWidget (single). */
        ATTACHMENT_FRAME,               /*!< Attachmentpoint on a QFrame (single). */
        ATTACHMENT_TABWIDGET,           /*!< Attachmentpoint on a QTabWidget (multiple). */
        ATTACHMENT_DOCKWIDGET,          /*!< Attachmentpoint on a QDockWidget (single). */
        ATTACHMENT_MAINWINDOW,          /*!< Attachmentpoint on a QMainWindow (single). */
        ATTACHMENT_DOCKWIDGET_CREATOR   /*!< Attachmentpoint on one of an extending list of QDockWidget (multiple). */
    };

    /*!
     \brief Context menu commands for a selected plugin.
    */
    enum EFrameCommand
    {
        COMMAND_COPY,   /*!< Copies the plugin. */
        COMMAND_DELETE, /*!< Deletes the plugin. */
        
        COMMAND_MAX_ID  /*!< Not a command, used to get the highest command id. */
    };

public:
    //public static creation functions

    /*!
     \brief Create a FrameAttachmentPoint of a specific type.

     \param name name of the attachment point (used to identify the point)
     \param attachmentPoint widget to attach AttachableFrames to
     \param type type of the AttachmentPoint
     \param defaultPoint if true, this point is used for frames that do not have an AttachmentPoint specified or cant be connected to it
    */
    static FrameAttachmentPoint* createFrameAttachmentPoint(QString name, QWidget* attachmentPoint,
        EFrameAttachmentType type = ATTACHMENT_WIDGET, bool defaultPoint = false);

protected:
    // protected constructor

    /*!
     \brief Protected constructor.
    */
    FrameAttachmentPoint();

public:
    //public functions

    /*!
     \brief Destructor.
    */
    virtual ~FrameAttachmentPoint();

    /*!
     \brief Returns the name.
    */
    QString getName();
    /*!
     \brief Returns the attachment point.
    */
    QWidget* getAttachmentPoint();
    /*!
     \brief Returns the attachment type.
    */
    FrameAttachmentPoint::EFrameAttachmentType getEFrameAttachmentType();

    /*!
     \brief Returns the amount of frames attached to this attachment point.
    */
    int getAttachmentCount();
    /*!
     \brief Returns the maximum possible amount of frames that can be attached to this attachment point.
    */
    int getMaxAttachmentCount();
    /*!
     \brief Checks if frames can be attached to this attachment point currently.
    */
    bool isAttachable();
    /*!
     \brief Checks if the attachment point is flagged to be used for plugins without attachment target.
    */
    bool isDefaultAttachment();

    /*!
     \brief Attaches an AttachableFrame to this attachment point.

     The QWidget of this attachment point will become the parent of the attached frame.

     \param frame frame to attach
    */
    virtual bool attach(AttachableFrame* frame);
    /*!
     \brief Unattaches an AttachableFrame from this attachment point.

     If the frame is not a child the call is ignored.

     \param frame frame to unattach
    */
    virtual void unattach(AttachableFrame* frame);
    /*!
     \brief Deletes all action receivers.
    */
    void clearActionReceivers();

public slots:
    /*!
     \brief Show context menu at given point. Called by gui when right clicked on an attachment point.

     The context menu contains the list of all connected frames and for each of them a submenu with different commands and all target attachment points.
     Clicking one of them calls onContextMenuClick().
    */
    void showContextMenu(QPoint position);
    /*!
     \brief Executes command for an attached frame with index 'child'.
     
     Executes command 'command'.
     May delete the plugin, copy or move it to the attachment point at index 'command - MAX_COMMAND_ID' if possible.
    */
    void onContextMenuClick(int child, int command);

protected:
    //protected functions

    /*!
     \brief Constructor for derived attachment points.

     \param type type of the attachment
     \param name unique name for the attachment point
     \param parentWidget attachment target
     \param defaultPoint if true, the attachment point is flagged to be used as default attachment for plugins without target attachment
    */
    FrameAttachmentPoint(EFrameAttachmentType type, QString name, QWidget* parentWidget, bool defaultPoint = false);

    /*!
     \brief Initialization function called by the constructor.

     \param name name for the attachment point
     \param attachmentPoint pointer to the QWidget
     \param type type of the QWidget
     \param defaultPoint if true, the attachment point is flagged to be used as default attachment for plugins without target attachment
    */
    virtual void initialize(QString name, QWidget* attachmentPoint, EFrameAttachmentType type, bool defaultPoint);

protected:
    // protected members

    bool mDefaultPoint;        /*!< If true, the attachment point is flagged to be used as default attachment for plugins without target attachment */
    int mMaxAttachmentCount;   /*!< Maximum number of frames that can be attached. */
    QString mName;             /*!< Unique name of the attachment point. Naming is controlled by WindowManager. */
    QWidget* mAttachmentPoint; /*!< Attachment point target. */

    QMap<int, AttachableFrame* > mAttachedFrames;          /*!< AttachableFrames attached to this attachment point. */
    FrameAttachmentPoint::EFrameAttachmentType mType;      /*!< Type of the attachment point. */
    std::vector<NumberedActionReceiver*> mActionReceivers; /*!< Action receivers for context menu */
};

/*!
 \brief FrameAttachmentPoint that creates additional independant widgets to attach an AttachableFrame to.

 Works just like a normal FrameAttachmentPoint except that it does not contain attached AttachableFrames directly,
 but creates a new widget for every frame to attach. So the attachment point is not necessarily the parent widget of
 the AttachableFrame.
*/
class ExtendingFrameAttachmentPoint : public FrameAttachmentPoint
{
public:
    //public types

    /*!
     \brief Type of the extending attachment point.
    */
    enum EExtendingType
    {
        ET_DOCKWIDGET  /*!< Creates QDockWidgets */
    };

    /*!
     \brief Default constructor.

     \param type type of the attachment point
    */
    ExtendingFrameAttachmentPoint(EExtendingType type);
    /*!
     \brief Destructor.
    */
    virtual ~ExtendingFrameAttachmentPoint();

    /*!
     \brief Attaches an AttachableFrame to this attachment point.

     A new widget depending on the attachment point type will be created and will become the parent of the attached frame.
     \param frame frame to attach
    */
    virtual bool attach(AttachableFrame* frame);
    /*!
     \brief Unattaches an AttachableFrame from this attachment point.

     The parent widget of the frame (created by this attachment point) will be removed.
     If the frame is not a child the call is ignored.
     \param frame frame to unattach
    */
    virtual void unattach(AttachableFrame* frame);

private:
    //private functions

    /*!
     \brief Initialization function called by the constructor.

     \param name name for the attachment point
     \param attachmentPoint pointer to the QWidget
     \param type type of the QWidget
     \param defaultPoint if true, the attachment point is flagged to be used as default attachment for plugins without target attachment
    */
    virtual void initialize(QString name, QWidget* parent, EFrameAttachmentType type, bool defaultPoint);

protected:
    //protected members

    EExtendingType mExtendingType; /*!< Type of the extending attachment point. */
    QMap<AttachableFrame*, QWidget*> mWidgetMap; /*!< Map containing the created widget for every connected AttachableFrame. */
};

#endif // FRAMEATTACHMENTPOINT_H
