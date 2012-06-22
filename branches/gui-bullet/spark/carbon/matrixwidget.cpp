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

#include "matrixwidget.h"
#include "carbon.h"

#include <QValidator>
#include <QMenu>


QMatrix4x4 MatrixWidget::mSavedMatrix = QMatrix4x4();
bool MatrixWidget::hasSavedMatrix = false;

QMatrix4x4 MatrixWidget::getSavedMatrix()
{
    return mSavedMatrix;
}

void MatrixWidget::saveMatrix(const QMatrix4x4& mat)
{
    mSavedMatrix = mat;
    hasSavedMatrix = true;
}

QMatrix4x4 MatrixWidget::mZeroMatrix     = QMatrix4x4(0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
QMatrix4x4 MatrixWidget::mOneMatrix      = QMatrix4x4(1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0);;
QMatrix4x4 MatrixWidget::mIdentityMatrix = QMatrix4x4(1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0);;

MatrixWidget::MatrixWidget(QWidget* parent) :
    QWidget(parent)
{
    mInit = false;

    ui.setupUi(this);

    ui.lineEdit11->setDoubleType(new QDoubleValidator());
    ui.lineEdit12->setDoubleType(new QDoubleValidator());
    ui.lineEdit13->setDoubleType(new QDoubleValidator());
    ui.lineEdit14->setDoubleType(new QDoubleValidator());
    ui.lineEdit21->setDoubleType(new QDoubleValidator());
    ui.lineEdit22->setDoubleType(new QDoubleValidator());
    ui.lineEdit23->setDoubleType(new QDoubleValidator());
    ui.lineEdit24->setDoubleType(new QDoubleValidator());
    ui.lineEdit31->setDoubleType(new QDoubleValidator());
    ui.lineEdit32->setDoubleType(new QDoubleValidator());
    ui.lineEdit33->setDoubleType(new QDoubleValidator());
    ui.lineEdit34->setDoubleType(new QDoubleValidator());
    ui.lineEdit41->setDoubleType(new QDoubleValidator());
    ui.lineEdit42->setDoubleType(new QDoubleValidator());
    ui.lineEdit43->setDoubleType(new QDoubleValidator());
    ui.lineEdit44->setDoubleType(new QDoubleValidator());

    identityMatrix();

    connect(ui.lineEdit11, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit11, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit12, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit12, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit13, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit13, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit14, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit14, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit21, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit21, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit22, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit22, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit23, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit23, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit24, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit24, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit31, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit31, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit32, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit32, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit33, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit33, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit34, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit34, SIGNAL(typedChange()), this, SLOT(createMatrix()));
    connect(ui.lineEdit41, SIGNAL(editingFinished()), this, SLOT(createMatrix())); connect(ui.lineEdit41, SIGNAL(typedChange()), this, SLOT(createMatrix()));

    updateFields();
    mInit = true;
}

MatrixWidget::~MatrixWidget()
{
}

QMatrix4x4& MatrixWidget::getMatrix()
{
    return mMatrix;
}

void MatrixWidget::setMatrix(double m11, double m12, double m13, double m14, 
    double m21, double m22, double m23, double m24, 
    double m31, double m32, double m33, double m34, 
    double m41, double m42, double m43, double m44)
{
    QMatrix4x4 mat(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    if (mMatrix != mat)
    {
        mMatrix = mat;
        updateMatrixChanged();
    }
}

void MatrixWidget::invert()
{
    bool invertible = false;
    QMatrix4x4 inverted = mMatrix.inverted(&invertible);
    if (!invertible)
        LOG_INFO() << "Matrix not invertible.";
    else
    {
        if (mMatrix != inverted)
        {
            mMatrix = inverted;
            updateMatrixChanged();
        }
    }
}

void MatrixWidget::transpose()
{
    if (mMatrix != mMatrix.transposed())
    {
        mMatrix = mMatrix.transposed();
        updateMatrixChanged();
    }
}

void MatrixWidget::inverseTranspose()
{
    bool invertible = false;
    QMatrix4x4 inverted = mMatrix.inverted(&invertible);
    if (!invertible)
        LOG_INFO() << "Matrix not invertible.";
    else
    {
        QMatrix4x4 transposed = inverted.transposed();
        if (mMatrix != transposed)
        {
            mMatrix = transposed;
            updateMatrixChanged();
        }
    }
}

void MatrixWidget::identityMatrix()
{
    if (!mMatrix.isIdentity())
    {
        mMatrix.setToIdentity();
        updateMatrixChanged();
    }
}

void MatrixWidget::zeroMatrix()
{
    if (mMatrix != mZeroMatrix)
    {
        mMatrix.fill(0.0);
        updateMatrixChanged();
    }
}

void MatrixWidget::oneMatrix()
{
    if (mMatrix != mOneMatrix)
    {
        mMatrix.fill(1.0);
        updateMatrixChanged();
    }
}

void MatrixWidget::createMatrix()
{
    QMatrix4x4 mat(
        ui.lineEdit11->text().toDouble(), ui.lineEdit12->text().toDouble(), ui.lineEdit13->text().toDouble(), ui.lineEdit14->text().toDouble(),
        ui.lineEdit21->text().toDouble(), ui.lineEdit22->text().toDouble(), ui.lineEdit23->text().toDouble(), ui.lineEdit24->text().toDouble(),
        ui.lineEdit31->text().toDouble(), ui.lineEdit32->text().toDouble(), ui.lineEdit33->text().toDouble(), ui.lineEdit34->text().toDouble(),
        ui.lineEdit41->text().toDouble(), ui.lineEdit42->text().toDouble(), ui.lineEdit43->text().toDouble(), ui.lineEdit44->text().toDouble());
    if (mat != mMatrix)
    {
        mMatrix = mat;
        emit matrixChanged();
    }
}

void MatrixWidget::copy() const
{
    saveMatrix(mMatrix);
}

void MatrixWidget::paste()
{
    mMatrix = getSavedMatrix();
    updateMatrixChanged();
}

void MatrixWidget::updateMatrixChanged()
{
    if (mInit)
    {
        updateFields();
        emit matrixChanged();
    }
}


void MatrixWidget::updateFields(bool skipfocus)
{
    if (!skipfocus || !ui.lineEdit11->hasFocus()) ui.lineEdit11->setText(QString::number(mMatrix.row(0).x()));
    if (!skipfocus || !ui.lineEdit12->hasFocus()) ui.lineEdit12->setText(QString::number(mMatrix.row(0).y()));
    if (!skipfocus || !ui.lineEdit13->hasFocus()) ui.lineEdit13->setText(QString::number(mMatrix.row(0).z()));
    if (!skipfocus || !ui.lineEdit14->hasFocus()) ui.lineEdit14->setText(QString::number(mMatrix.row(0).w()));
    if (!skipfocus || !ui.lineEdit21->hasFocus()) ui.lineEdit21->setText(QString::number(mMatrix.row(1).x()));
    if (!skipfocus || !ui.lineEdit22->hasFocus()) ui.lineEdit22->setText(QString::number(mMatrix.row(1).y()));
    if (!skipfocus || !ui.lineEdit23->hasFocus()) ui.lineEdit23->setText(QString::number(mMatrix.row(1).z()));
    if (!skipfocus || !ui.lineEdit24->hasFocus()) ui.lineEdit24->setText(QString::number(mMatrix.row(1).w()));
    if (!skipfocus || !ui.lineEdit31->hasFocus()) ui.lineEdit31->setText(QString::number(mMatrix.row(2).x()));
    if (!skipfocus || !ui.lineEdit32->hasFocus()) ui.lineEdit32->setText(QString::number(mMatrix.row(2).y()));
    if (!skipfocus || !ui.lineEdit33->hasFocus()) ui.lineEdit33->setText(QString::number(mMatrix.row(2).z()));
    if (!skipfocus || !ui.lineEdit34->hasFocus()) ui.lineEdit34->setText(QString::number(mMatrix.row(2).w()));
    if (!skipfocus || !ui.lineEdit41->hasFocus()) ui.lineEdit41->setText(QString::number(mMatrix.row(3).x()));
    if (!skipfocus || !ui.lineEdit42->hasFocus()) ui.lineEdit42->setText(QString::number(mMatrix.row(3).y()));
    if (!skipfocus || !ui.lineEdit43->hasFocus()) ui.lineEdit43->setText(QString::number(mMatrix.row(3).z()));
    if (!skipfocus || !ui.lineEdit44->hasFocus()) ui.lineEdit44->setText(QString::number(mMatrix.row(3).w()));
}

void MatrixWidget::showCustomContextMenu(QPoint pos)
{
   //Show menu with available commands
   QMenu menu(this);
   QAction* action;
   
   //Add context commands
   menu.addAction(tr("Invert"), this, SLOT(invert()));
   menu.addAction(tr("Transpose"), this, SLOT(invert()));
   menu.addAction(tr("Inverse-Transpose"), this, SLOT(inverseTranspose()));
   menu.addSeparator();
   
   menu.addAction(tr("Identiy Matrix"), this, SLOT(identityMatrix()));
   menu.addAction(tr("Zero Matrix"), this, SLOT(zeroMatrix()));
   menu.addAction(tr("One Matrix"), this, SLOT(oneMatrix()));

   menu.addAction(tr("Copy"), this, SLOT(copy()));
   action = menu.addAction(tr("Paste"), this, SLOT(copy()));
   action->setEnabled(hasSavedMatrix);

   //Open menu
   menu.exec(mapToGlobal(pos));
}