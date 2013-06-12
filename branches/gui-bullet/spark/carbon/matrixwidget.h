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

#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H
/**
 * \file matrixwidget.h
 * \brief Contains MatrixWidget class.
 */

#include "ui_matrixwidget.h"

#include <QWidget>
#include <QMatrix4x4>

/*!
 \brief Widget containing, displaying and being able to modify a 4x4 matrix.
*/
class MatrixWidget : public QWidget
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief  Basic constructor.

     \param parent parent widget
    */
    MatrixWidget(QWidget* parent);
    /*!
     \brief Destructor.
    */
    virtual ~MatrixWidget();

    /*!
     \brief Returns the contained matrix.

     If you change the matrix manually, call updateMatrixChanged() to emit the changed signal and update the
     display fields.
    */
    QMatrix4x4& getMatrix();
    /*!
     \brief Specify the values of the matrix row-wise.
    */
    void setMatrix(
        double m11, double m12, double m13, double m14,
        double m21, double m22, double m23, double m24, 
        double m31, double m32, double m33, double m34, 
        double m41, double m42, double m43, double m44);

    /*!
     \brief Notify the widget that the matrix was changed.

     Updates the dispay fields and emits matrixChanged().
    */
    void updateMatrixChanged();

public:
    //public static functions

    /*!
     \brief Returns a copy of the static saved 4x4 matrix.

     Initially a zero-matrix.
    */
    static QMatrix4x4 getSavedMatrix();
    /*!
     \brief Saves a 4x4 matrix as static value to be used later.

     \param mat matrix to save
    */
    static void saveMatrix(const QMatrix4x4& mat);

public:
    //public static members

    static QMatrix4x4 mSavedMatrix; /*!< Saved 4x4 matrix. */
    static bool hasSavedMatrix; /*!< True if a matrix was saved. */

    static QMatrix4x4 mZeroMatrix; /*!< Static zero matrix. */
    static QMatrix4x4 mOneMatrix; /*!< Static one matrix. */
    static QMatrix4x4 mIdentityMatrix; /*!< Static identity matrix. */

public: signals:

    /*!
     \brief Emitted when the contained matrix changed.
    */
    void matrixChanged();

public slots:

    /*!
     \brief Update the contained matrix from the values contained in the text edit fields.
    */
    void createMatrix();
    /*!
     \brief Update the contained text edit fields from the contained matrix.

     \param skipfocus if true, a currently focused text edit will not be updated
    */
    void updateFields(bool skipfocus = true);

    /*!
     \brief Invert the contained matrix and update the display fields.
    */
    void invert();
    /*!
     \brief Transpose the contained matrix and update the display fields.
    */
    void transpose();
    /*!
     \brief Invert and transpose the contained matrix and update the display fields.
    */
    void inverseTranspose();

    /*!
     \brief Replace the contained matrix with the identity matrix and update the display fields.
    */
    void identityMatrix();
    /*!
     \brief Replace the contained matrix with the zero matrix and update the display fields.
    */
    void zeroMatrix();
    /*!
     \brief Replace the contained matrix with the one matrix and update the display fields.
    */
    void oneMatrix();

    /*!
     \brief Saves the current matrix to be reused later.
    */
    void copy() const;
    /*!
     \brief Paste a previously copied matrix and updates the display fields.
    */
    void paste();

    /*!
     \brief Shows a context menu for matrix commands.

     \param pos local position to popup at
    */
    void showCustomContextMenu(QPoint pos);

private:
    // private members

    Ui::MatrixWidget ui; /*!< UI definition object. */
    QMatrix4x4 mMatrix; /*!< Contained 4x4 matrix. */

    bool mInit; /*!< True after initialization. */
};

#endif // MATRIXWIDGET_H
