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

#ifndef GLWIDGET_H
#define GLWIDGET_H
/**
 * \file glwidget.h
 * \brief Contains GLWidget class.
 */

#include <QGLWidget>
#include <QTimer>
#include <QMutex>

class QGLContext;

class QPaitEvent;
class QResizeEvent;
class QKeyEvent;

/*!
 \brief Base class for OpenGL-Widgets that can be offered or used in the Gui.

 The GLWidget implements a basic rendering functionality example.
 Derive from GLWidget and reimplement the virtual functions.
 The rendering interval can be controlled using the base class start() function.
 Whenever calling initialization, rendering or update functions (resize or repaint),
 the OpenGL context of the GLWidget is made current in the main thread.

 GLWidgets can be offered to other plugins or threads to use the same OpenGLContext in
 different threads using the OpenGLManager.

 \sa OpenGLManager
*/
class GLWidget : public QGLWidget
{
  Q_OBJECT

public:
    // public functions
    /*!
     \brief Complete constructor.

     \param parent parent widget
     \param timerInterval rendering interval
     \param name name of the GLWidget
     \param shareWidget widget to share QGLContext with
    */
    GLWidget(QWidget* parent = 0, int timerInterval = 20, const QString& name = QString("GLWidget"), GLWidget* shareWidget = 0);
    /*!
     \brief Basic destructor.
    */
    virtual ~GLWidget();

    /*!
     \brief Calls initializeGL()
    */
    void init();
    /*!
     \brief Starts the rendering timer.
    */
    void start();

    /*!
     \brief Returns the rendering timer.
    */
    QTimer& getTimer();
    /*!
     \brief Returns the widgets name.
    */
    const QString& getName() const;

    /*!
     \brief Locks rendering and initialization function access, waiting indefinitely.
    */
    void lock();
    /*!
     \brief Tries to lock rendering and initialization function access, waiting for a specified amount of time.

     If locking succeeds, unlock has to be called by the locking thread.

     \param timeout maximum time to wait in ms
     \return true if locking succeeded
    */
    bool tryLock(int timeout);
    /*!
     \brief Unlocks rendering and initialization access.
    */
    void unlock();

    /*!
     \brief Resizes the widget as soon as it is unlocked.

     \param size target size
    */
    void rememberResize(const QSize& size);
    /*!
     \brief Resize the widget if it was tried to be resized while being locked.
    */
    void applyRememberedResize();

protected:
    // virtual protected functions

    /*!
     \brief Virtual initialization function. Derive in subclass.

     Default implementation initializes simple screen rendering with the widgets size
     as viewport size, and initializing a render list.
    */
    virtual void initializeGL();
    /*!
     \brief Virtual resize function for the OpenGL context. Derive in subclass.

     \param width new width
     \param height new height
    */
    virtual void resizeGL( int width, int height);
    /*!
     \brief Virtual render function. Derive in subclass.

     Default implementation clears the screen and renders the first render list
     (created in the default initializeGL() function).
    */
    virtual void paintGL();
  
    /*!
     \brief Default implementation for key event.
    */
    virtual void keyPressEvent(QKeyEvent* e);
    /*!
     \brief Default implementation for paint event.
    */
    virtual void paintEvent(QPaintEvent* e);
    /*!
     \brief Default implementation for resize event.

     If locked, resize is applied after unlocking the widget.
    */
    virtual void resizeEvent(QResizeEvent* e);
    /*!
     \brief Default implementation for mouse press event.

     Focuses the widget, grabbing the keyboard.
    */
    virtual void mousePressEvent(QMouseEvent* e);
  
    /*!
     \brief Rendering timer call function.

     Locks the widget, renders and unlocks it again.
    */
    virtual void timeOut();

protected slots:
    //protected slots

  /*!
   \brief Slot for rendering timer.
  */
  virtual void timeOutSlot();

private:
    // private members

    QTimer mTimer;       /*!< Rendering timer. */
    QString mName;       /*!< Name for the widget. */
    QMutex mContextLock; /*!< Rendering access lock. */
    bool mLocked;        /*!< True while locked. */

    QSize mRememberedSize;   /*!< Size to resize to after unlocking. */
    bool mHasRememberedSize; /*!< True if widget has to be sized after unlocking. */
};

#endif //GLWIDGET_H
