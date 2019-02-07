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

#include "sparkpropertydata.h"
#include "cutelogger/logger.h"
#include "simspark.h"

#include <QStringList>

using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

namespace SparkProperty
{

//--------------------------------------------------------------
// Data-To-String formatting functions
//--------------------------------------------------------------

float Data::F_NAN  = std::numeric_limits<float>::quiet_NaN(); 
float Data::F_INF  = std::numeric_limits<float>::infinity(); 
float Data::F_NINF = -std::numeric_limits<float>::infinity(); 

double Data::D_NAN  = std::numeric_limits<double>::quiet_NaN(); 
double Data::D_INF  = std::numeric_limits<double>::infinity(); 
double Data::D_NINF = -std::numeric_limits<double>::infinity(); 

inline QString formatVector3f(const Vector3f& vec)
{
    return QString("(%1, %2, %3)").arg(
        QString::number(vec[0]), 
        QString::number(vec[1]), 
        QString::number(vec[2]));
}

inline QString formatAABB3(const AABB3& bound)
{
    return formatVector3f(bound.minVec) + " " + formatVector3f(bound.maxVec);
}

inline QString formatMat4x4(const Matrix& m)
{
    QString str = "(";
    for (int i = 0; i < 16 ; ++i)
    {
        if (i != 0) str += ",";
        str += QString::number(m.m[i]);
    }
    str += ")";
    
    return str;
}

inline QString formatFloat(const float f)
{
    return QString::number(f);
}

inline QString formatDouble(const double d)
{
    return QString::number(d);
}

inline QString formatInt(const int i)
{
    return QString::number(i);
}

inline QString formatLong(const long l)
{
    return QString::number(l);
}

inline QString formatBool(const bool b)
{
    return b ? "true" : "false";
}

inline QString formatRGBA(const RGBA& col)
{
    return QString("(%1, %2, %3, %4)").arg(
        QString::number(col.r()),
        QString::number(col.g()), 
        QString::number(col.b()),
        QString::number(col.a()));
}

//--------------------------------------------------------------
// String-to-Data creation functions
//--------------------------------------------------------------

Vector3f Data::createVector3f(const QString& str)
{
    QString s = str;
    s.remove(QRegExp("\\s|\\(|\\)")); //remove brackets and whitespaces
    QStringList parts = s.split(QRegExp(",|;")); //split into single float numbers
    if (parts.size() != 3)
    {
        LOG_ERROR() << "Illegal parameters for Vector3f.";
        return Vector3f();
    }
    return Vector3f(parts[0].toFloat(), parts[1].toFloat(), parts[2].toFloat());
}

Matrix Data::createMat4x4(const QString& str)
{
    QString s = str;
    s.remove(QRegExp("\\s|\\(|\\)")); //remove brackets and whitespaces
    QStringList parts = s.split(QRegExp(",|;")); //split into single float numbers
    if (parts.size() != 16)
    {
        LOG_ERROR() << "Illegal parameters for Matrix.";
        return Matrix();
    }
    return Matrix(parts[0].toFloat(), parts[4].toFloat(), parts[ 8].toFloat(), parts[12].toFloat(),
                  parts[1].toFloat(), parts[5].toFloat(), parts[ 9].toFloat(), parts[13].toFloat(),
                  parts[2].toFloat(), parts[6].toFloat(), parts[10].toFloat(), parts[14].toFloat(),
                  parts[3].toFloat(), parts[7].toFloat(), parts[11].toFloat(), parts[15].toFloat());
}

float Data::createFloat(const QString& str)
{
    return str.toFloat();
}

float Data::createDouble(const QString& str)
{
    return str.toDouble();
}

int Data::createInt(const QString& str)
{
    return str.toInt();
}

long Data::createLong(const QString& str)
{
    return str.toLong();
}

bool Data::createBool(const QString& str)
{
    return str.compare("false", Qt::CaseInsensitive) == 0 ? false : true;
}

RGBA Data::createRGBA(const QString& str)
{
    QString s = str;
    s.remove(QRegExp("\\s|\\(|\\)")); //remove brackets and whitespaces
    QStringList parts = s.split(QRegExp(",|;")); //split into single float numbers
    if (parts.size() != 4)
    {
        LOG_ERROR() << "Illegal parameters for RGBA.";
        return RGBA();
    }
    return RGBA(parts[0].toFloat(), parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
}

AABB3 Data::createAABB3(const QString& str)
{
    //split at closing bracket
    int pos = str.indexOf(')');
    QString l = str.left(pos);
    QString r = str.mid(pos+1, str.size()-pos);
    
    l.remove(QRegExp("\\s|\\(|\\)")); //remove brackets and whitespaces
    r.remove(QRegExp("\\s|\\(|\\)")); //remove brackets and whitespaces
    QStringList lparts = l.split(QRegExp(",|;")); //split into single float numbers
    QStringList rparts = r.split(QRegExp(",|;")); //split into single float numbers
    if (lparts.size() != 3)
    {
        LOG_ERROR() << "Illegal parameters for first vector of AABB3.";
        return AABB3();
    }
    if (rparts.size() != 3)
    {
        LOG_ERROR() << "Illegal parameters for second vector of AABB3.";
        return AABB3();
    }
    return AABB3(Vector3f(lparts[0].toFloat(), lparts[1].toFloat(), lparts[2].toFloat()), 
                 Vector3f(rparts[0].toFloat(), rparts[1].toFloat(), rparts[2].toFloat()));
}

//--------------------------------------------------------------
// Validator creation functions
//--------------------------------------------------------------

QRegExpValidator* Data::intMatrixValidator()
{
    //Match matrix as vector like "(+1.0,-1.0, .... , 12.34)" with exactly 16 entries, white spaces before and after brackets and numbers, 
    //and with leading + or - for each infinitely big integer number
   return new QRegExpValidator(QRegExp("\\s*\\((\\s*[-+]?\\s*[0-9]*\\s*,){15}(\\s*[-+]?\\s*[0-9]*\\s*)\\)\\s*"), 0); 
}

QRegExpValidator* Data::floatMatrixValidator()
{
   //Match matrix as vector like "(+1.0,-1.0, .... , 12-e22)" with exactly 16 entries, white spaces before and after brackets and numbers, 
   //and with leading + or - for each infinitely big floating point number
    return new QRegExpValidator(QRegExp("\\s*\\((\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*,){15}(\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*)\\)\\s*"), 0); 
}

QRegExpValidator* Data::doubleMatrixValidator()
{
   //Match matrix as vector like "(+1.0,-1.0, .... , 12-e22)" with exactly 16 entries, white spaces before and after brackets and numbers, 
   //and with leading + or - for each infinitely big floating point number
    return new QRegExpValidator(QRegExp("\\s*\\((\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*,){15}(\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*)\\)\\s*"), 0); 
}

QRegExpValidator* Data::vector3iValidator()
{
   //Match vector like "(+1,-1, 1)" with 3 entries, white spaces before and after brackets and numbers, 
   //and with leading + or - for each infinitely big integer number
    return new QRegExpValidator(QRegExp("\\s*\\((\\s*[-+]?\\s*[0-9]*\\s*,){2}(\\s*[-+]?\\s*[0-9]*\\s*)\\)\\s*"), 0); 
}

QRegExpValidator* Data::vector3fValidator()
{
   //Match vector like "(+1.0,-1.5+e3, 1.4-e2)" with 3 entries, white spaces before and after brackets and numbers, 
   //and with leading + or - for each infinitely big floating point number
    return new QRegExpValidator(QRegExp("\\s*\\((\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*,){2}(\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*)\\)\\s*"), 0); 
}

QRegExpValidator* Data::rgbaValidator()
{
   //Match vector like "(+1.0,-1.5+e3, 1.4-e2, 0.0)" with 4 entries, white spaces before and after brackets and numbers, 
   //and with leading + or - for each infinitely big floating point number
    return new QRegExpValidator(QRegExp("\\s*\\((\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*,){3}(\\s*[-+]?[0-9]*((\\.[0-9]+)?([eE][-+]?[0-9]+)?)?\\s*)\\)\\s*"), 0); 
}

QRegExpValidator* Data::stringValidator()
{
    return new QRegExpValidator(QRegExp(".*"), 0);
}

QIntValidator* Data::intValidator(long min, long max)
{
    return new QIntValidator(min, max, 0);
}

QDoubleValidator* Data::doubleValidator(double min, double max, int decimals)
{
    return new QDoubleValidator(min, max, decimals, 0);
}

//--------------------------------------------------------------
// Data-Classes
//--------------------------------------------------------------
// Data
//--------------------------------------------------------------

Data& Data::operator=(const Data& rhs)
{
    if (mDataType != rhs.mDataType)
    {
        LOG_ERROR() << "Data type mismatch.";
        return *this;
    }

    switch(rhs.mDataType)
    {
        case DT_INT   : ((DInt*     )this)->mValue = ((DInt&     )rhs).mValue; break;
        case DT_LONG  : ((DLong*    )this)->mValue = ((DLong&    )rhs).mValue; break;
        case DT_FLOAT : ((DFloat*   )this)->mValue = ((DFloat&   )rhs).mValue; break;
        case DT_DOUBLE: ((DDouble*  )this)->mValue = ((DDouble&  )rhs).mValue; break;
        case DT_BOOL  : ((DBool*    )this)->mValue = ((DBool&    )rhs).mValue; break;
        case DT_STRING: ((DString*  )this)->mValue = ((DString&  )rhs).mValue; break;
        case DT_VEC3F : ((DVector3f*)this)->mValue = ((DVector3f&)rhs).mValue; break;
        case DT_RGBA  : ((DRGBA*    )this)->mValue = ((DRGBA&    )rhs).mValue; break;
        case DT_AABB3 : ((DAABB3*   )this)->mValue = ((DAABB3&   )rhs).mValue; break;
        case DT_MAT4X4: ((DMat4x4*  )this)->mValue = ((DMat4x4&  )rhs).mValue; break;
        default:
            LOG_ERROR() << "Unknown DataType " << rhs.mDataType;
            break;
    }

    return *this;
};

boost::shared_ptr<Data> Data::createData(EDataType type)
{
    switch(type)
    {
        case DT_INT   : return boost::shared_ptr<Data>(new DInt(0)); break;
        case DT_LONG  : return boost::shared_ptr<Data>(new DLong(0)); break;
        case DT_FLOAT : return boost::shared_ptr<Data>(new DFloat(0.0)); break;
        case DT_DOUBLE: return boost::shared_ptr<Data>(new DDouble(0.0)); break;
        case DT_BOOL  : return boost::shared_ptr<Data>(new DBool(false)); break;
        case DT_STRING: return boost::shared_ptr<Data>(new DString(QString(""))); break;
        case DT_VEC3F : return boost::shared_ptr<Data>(new DVector3f(Vector3f())); break;
        case DT_RGBA  : return boost::shared_ptr<Data>(new DRGBA(RGBA())); break;
        case DT_AABB3 : return boost::shared_ptr<Data>(new DAABB3(AABB3())); break;
        case DT_MAT4X4: return boost::shared_ptr<Data>(new DMat4x4(Matrix())); break;
        default:
            LOG_ERROR() << "Unknown DataType " << type;
            break;
    }
    return boost::shared_ptr<Data>();
}

boost::shared_ptr<Data> Data::createData(EDataType type, const QString& dataString)
{
    boost::shared_ptr<Data> dat = createData(type);
    dat->fromString(dataString);
    return dat;
}

//--------------------------------------------------------------
// Others
//--------------------------------------------------------------

QString DBool::toString() const
{
    return formatBool(mValue);
}

QString DInt::toString() const
{
    return formatInt(mValue);
}

QString DLong::toString() const
{
    return formatLong(mValue);
}

QString DFloat::toString() const
{
    return formatFloat(mValue);
}

QString DDouble::toString() const
{
    return formatDouble(mValue);
}

QString DString::toString() const
{
    return mValue.c_str();
}

QString DVector3f::toString() const
{
    return formatVector3f(mValue);
}

QString DAABB3::toString() const
{
    return formatAABB3(mValue);
}

QString DRGBA::toString() const
{
    return formatRGBA(mValue);
}

QString DMat4x4::toString() const
{
    return formatMat4x4(mValue);
}


void DBool::fromString(const QString& str)
{
    mValue = createBool(str);
}

void DInt::fromString(const QString& str)
{
    mValue = createInt(str);
}

void DLong::fromString(const QString& str)
{
    mValue = createLong(str);
}

void DFloat::fromString(const QString& str)
{
    mValue = createFloat(str);
}

void DDouble::fromString(const QString& str)
{
    mValue = createDouble(str);
}

void DString::fromString(const QString& str)
{
    mValue = str.toStdString();
}

void DVector3f::fromString(const QString& str)
{
    mValue = createVector3f(str);
}

void DAABB3::fromString(const QString& str)
{
    mValue = createAABB3(str);
}

void DRGBA::fromString(const QString& str)
{
    mValue = createRGBA(str);
}

void DMat4x4::fromString(const QString& str)
{
    mValue = createMat4x4(str);
}


} //namespace Sparkproperty