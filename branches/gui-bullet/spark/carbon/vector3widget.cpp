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

#include "vector3widget.h"
#include "carbon.h"

#include <QValidator>
#include <QMenu>

QVector3D Vector3Widget::mSavedVector = QVector3D();
bool Vector3Widget::hasSavedVector = false;

QVector3D Vector3Widget::getSavedVector()
{
    return mSavedVector;
}

void Vector3Widget::saveVector(const QVector3D& vec)
{
    mSavedVector = vec;
    hasSavedVector = true;
}

Vector3Widget::Vector3Widget(QWidget* parent) :
    QWidget(parent)
{
    mInit = false;

    ui.setupUi(this);

    ui.lineEdit11->setDoubleType(new QDoubleValidator());
    ui.lineEdit12->setDoubleType(new QDoubleValidator());
    ui.lineEdit13->setDoubleType(new QDoubleValidator());

    connect(ui.lineEdit11, SIGNAL(editingFinished()), this, SLOT(createVector())); connect(ui.lineEdit11, SIGNAL(typedChange()), this, SLOT(createVector()));
    connect(ui.lineEdit12, SIGNAL(editingFinished()), this, SLOT(createVector())); connect(ui.lineEdit12, SIGNAL(typedChange()), this, SLOT(createVector()));
    connect(ui.lineEdit13, SIGNAL(editingFinished()), this, SLOT(createVector())); connect(ui.lineEdit13, SIGNAL(typedChange()), this, SLOT(createVector()));

    mVector = QVector3D(0.0, 0.0, 0.0);

    updateFields();
    mInit = true;
}

Vector3Widget::~Vector3Widget()
{
}

QVector3D& Vector3Widget::getVector()
{
    return mVector;
}

void Vector3Widget::setVector(double x, double y, double z, bool skipfocus)
{
    QVector3D vec(x, y, z);
    if (mVector != vec)
    {
        mVector = vec;
        updateVectorChanged(skipfocus);
    }
}

void Vector3Widget::negative()
{
    if (!mVector.isNull())
    {
        mVector = mVector * (-1.0);
        updateVectorChanged();
    }
}

void Vector3Widget::null()
{
    if (!mVector.isNull())
    {
        mVector = mVector * (0.0);
        updateVectorChanged();
    }
}

void Vector3Widget::createVector()
{
    QVector3D vec(ui.lineEdit11->text().toDouble(), ui.lineEdit12->text().toDouble(), ui.lineEdit13->text().toDouble());
    if (vec != mVector)
    {
        mVector = vec;
        emit vectorChanged();
    }
}

void Vector3Widget::copy() const
{
    saveVector(mVector);
}

void Vector3Widget::paste()
{
    mVector = getSavedVector();
    updateVectorChanged();
}

void Vector3Widget::updateVectorChanged(bool skipfocus)
{
    if (mInit)
    {
        updateFields(skipfocus);
        emit vectorChanged();
    }
}


void Vector3Widget::updateFields(bool skipfocus)
{
    if(!ui.lineEdit11->hasFocus() || ! skipfocus) ui.lineEdit11->setText(QString::number(mVector.x()));
    if(!ui.lineEdit12->hasFocus() || ! skipfocus) ui.lineEdit12->setText(QString::number(mVector.y()));
    if(!ui.lineEdit13->hasFocus() || ! skipfocus) ui.lineEdit13->setText(QString::number(mVector.z()));
}

void Vector3Widget::showCustomContextMenu(QPoint pos)
{
   //Show menu with available commands
   QMenu menu(this);
   QAction* action;
   
   //Add context commands
   menu.addAction(tr("Negative"), this, SLOT(invert()));
   menu.addAction(tr("Null"), this, SLOT(null()));
   menu.addSeparator();

   menu.addAction(tr("Copy"), this, SLOT(copy()));
   action = menu.addAction(tr("Paste"), this, SLOT(copy()));
   action->setEnabled(hasSavedVector);

   //Open menu
   menu.exec(mapToGlobal(pos));
}