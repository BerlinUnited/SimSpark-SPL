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

#ifndef TYPEDLINEEDIT_H
#define TYPEDLINEEDIT_H
/**
 * \file typedlineedit.h
 * \brief Contains TypedLineEdit class.
 */

#include <QLineEdit>
#include <QValidator>

class QWheelEvent;

/*!
 \brief A line edit which can be used to enter values of different data types.

 When changing/specifying the type of the edit a validator for the data can be specified.
 Number type text edit can be manipulated with the mouse wheel.
*/
class TypedLineEdit : public QLineEdit
{
    Q_OBJECT

private:
    //private members

    /*!
     \brief Data types for the line edit.
    */
    enum EType
    {
        T_INT,    /*!< Integer type. */
        T_DOUBLE, /*!< Double floating point type. */
        T_STRING  /*!< String type. */
    };

public:
    // public functions

    /*!
     \brief  Basic constructor.

     \param parent parent widget
    */
    TypedLineEdit(QWidget* parent);
    /*!
     \brief Destructor.
    */
    virtual ~TypedLineEdit();

    /*!
     \brief Change to int type and specify a validator and scroll width.

     \param validator integer validator
     \param scrollWidth amount of change for one wheel event
    */
    void setIntType(const QIntValidator* validator = 0, int scrollWidth = 1);
    /*!
     \brief Change to double type and specify a validator and scroll width.

     \param validator double validator
     \param scrollWidth amount of change for one wheel event
    */
    void setDoubleType(const QDoubleValidator* validator = 0, double scrollWidth = 0.05);
    /*!
     \brief Change to string type and specify a validator.

     \param validator regular expression validator
    */
    void setStringType(const QRegExpValidator* validator = 0);
    /*!
     \brief Set the text of the edit.

     The value will be ignored if it does not match the specified validator.
     \param text new text
     \param skipfocus if true, the text wont be change when in focus
    */
    void setText(const QString& text, bool skipfocus = true);

    /*!
     \brief Wheel event. changes int and float text edit values.

     \param e event
    */
    virtual void wheelEvent(QWheelEvent* e);

public: signals:
    //public signals

    /*!
     \brief Emitted when the value of the text edit was changed.
    */
    void typedChange();

private:
    // private members

    EType mType; /*!< Current type of the edit. */
    double mScroll; /*!< Scroll amount for wheel event in case of int or double type. */
};

#endif // TYPEDLINEEDIT_H
