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

#ifndef VECTOR4WIDGET_H
#define VECTOR4WIDGET_H
/**
 * \file vector4widget.h
 * \brief Contains Vector4Widget class.
 */

#include "ui_vector4widget.h"

#include <QWidget>
#include <QVector4D>

/*!
 \brief Widget containing four double type TypedLineEdit to display and change a QVector3D.
*/
class Vector4Widget : public QWidget
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief  Basic constructor.

     \param parent parent widget
    */
    Vector4Widget(QWidget* parent);
    /*!
     \brief Destructor.
    */
    virtual ~Vector4Widget();

    /*!
     \brief Returns the vector.
    */
    QVector4D& getVector();
    /*!
     \brief Changes values of the vector and updates the display.

     \param x x value
     \param y y value
     \param z z value
     \param w w value
     \param skipfocus if true, focused fields wont be updated
    */
    void setVector(double x, double y, double z, double w, bool skipfocus = true);

    /*!
     \brief Updates display and emits vectorChanged()

     \param skipfocus if true, focused fields wont be updated
    */
    void updateVectorChanged(bool skipfocus = true);

public:
    //public static functions

    /*!
     \brief Returns the saved static vector.

    */
    static QVector4D getSavedVector();
    /*!
     \brief Stores the specified vector as static value.

     \param vec vector to store
    */
    static void saveVector(const QVector4D& vec);
    /*!
     \brief Returns true if there is a saved static vector.
    */
    static bool hasCopyValue();

public: signals:
    //public signals

    /*!
     \brief Emitted when the contained vector changed.
    */
    void vectorChanged();

public slots:
    //public slots

    /*!
     \brief Update contained vector from the TypedLineEdits.
    */
    void createVector();
    /*!
     \brief Update the TypedLineEdits from the contained vector.

     \param skipfocus if true, focused text fields will be skipped
    */
    void updateFields(bool skipfocus = true);

    /*!
     \brief Negate the contained vector and update display.
    */
    void negative();
    /*!
     \brief Set the contained vector to null and update display.
    */
    void null();

    /*!
     \brief Copy the contained vector (save it as static value).
    */
    void copy() const;
    /*!
     \brief Paste a reviously copied vector.
    */
    void paste();

    /*!
     \brief Show context menu for the vector at the given position.

     \param pos local popup position
    */
    void showCustomContextMenu(QPoint pos);

public:
    //public static members

    static QVector4D mSavedVector; /*!< Saved static vector. */
    static bool hasSavedVector;     /*!< True if there was a vector copied previously. */

private:
    // private members

    Ui::Vector4Widget ui; /*!< Ui definition object. */
    QVector4D mVector; /*!< Contained vector. */

    bool mInit; /*!< True after initialization. */
};

#endif // VECTOR4WIDGET_H
