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

#ifndef SPARKPROPERTYDATA_H
#define SPARKPROPERTYDATA_H
/**
 \file sparkpropertydata.h
 \brief Contains SparkProperty::Data and derived classes as well as SparkProperty::EDataType enum.
*/
#include <QString>

#include <salt/vector.h>
#include <salt/bounds.h>
#include <salt/matrix.h>
#include <kerosin/materialserver/materialsolid.h>

#include <QValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>

namespace SparkProperty
{

class PropertyList;
class Data;

/*!
 \brief Data types for interactions between the Gui and SimSpark.

 \todo There are currently only those DataTypes implemented that are needed for some basic class properties.
 This list has to be extended to create new Properties.

 \sa Data
*/
enum EDataType
{
    DT_UNDEFINED = 0, /*!< Illegal or unknown type. */
    DT_INT,           /*!< Integer wrapper. */
    DT_LONG,          /*!< Long wrapper. */
    DT_FLOAT,         /*!< Float wrapper. */
    DT_DOUBLE,        /*!< Double wrapper. */
    DT_BOOL,          /*!< Boolean wrapper. */
    DT_STRING,        /*!< std::string wrapper. */
    DT_VEC3F,         /*!< salt::Vector3f wrapper. */
    DT_RGBA,          /*!< kerosin::RGBA wrapper. */
    DT_AABB3,         /*!< salt::AABB3 wrapper. */
    DT_MAT4X4         /*!< salt::Matrix wrapper. */
};

/*!
 \brief Abstract parent class for Data-Wrappers used to interact with SimSpark in SparkProperty::Property objects.

 <b>General:</b>\n
 Data derived classes act as wrapper for data types used to interact with SimSpark.
 They have conversion functions to and from data Strings.\n

 <b>Extension with new types:</b>\n
 Add new data type id to EDataType.\n
 Implement data type class derived from Data.\n
 Implement creation of the data class in createData().\n

 \todo There are currently only those DataTypes implemented that are needed for some basic class properties.
 This list has to be extended to create new Properties.

 \todo <b>Data conversion and Validators:</b>\n
 Data conversion from and to string as well as the creation of validators for strings are done by static functions.
 Currently there are only basic RegExp validators for complex data types like matrices or vectors.
 Additional validators could be created to provide more control about the contents of data types that are not simple number values (e.g. having a
 normalized vector, having a diagonal matrix, having positive matrix or vector fields and so on).
*/
class Data
{
protected:
    //protected constructor

    /*!
     \brief Protected constructor.

     Create new data objects with createData().
     \param type data type of derived class
    */
    explicit Data(EDataType type) :
         mDataType(type){}

public:
    //public functions

    /*!
     \brief Copy operator.

     Will fail if data types do not match.
     \param rhs data to read from
     \return self reference
    */
    Data& operator=(const Data& rhs);

    /*!
     \brief Static creation function for data objects.

     \param type type of data to create
    */
    static boost::shared_ptr<Data> createData(EDataType type);
    /*!
     \brief Static creation function for data objects. Initializes data using a data string.

     \param type type of data to create
     \param dataString string containing data
    */
    static boost::shared_ptr<Data> createData(EDataType type, const QString& dataString);

public:
    //public virtual functions

    /*!
     \brief Return contained data as string.
    */
    virtual QString toString() const = 0;
    /*!
     \brief Update contained data by parsing and converting a string.

     \param str data string
    */
    virtual void fromString(const QString& str) = 0;

public:
    //public static data conversion functions

    /*!
     \brief Data object creation from a data string.
    */
    static salt::Vector3f createVector3f(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static salt::Matrix createMat4x4(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static float createFloat(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static float createDouble(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static int createInt(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static long createLong(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static bool createBool(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static kerosin::RGBA createRGBA(const QString& str);
    /*!
     \brief Data object creation from a data string.
    */
    static salt::AABB3 createAABB3(const QString& str);

    /*!
     \brief Data string validator creation for integer matrices.
    */
    static QRegExpValidator* intMatrixValidator();
    /*!
     \brief Data string validator creation for float matrices.
    */
    static QRegExpValidator* floatMatrixValidator();
    /*!
     \brief Data string validator creation for double matrices.
    */
    static QRegExpValidator* doubleMatrixValidator();
    /*!
     \brief Data string validator creation for 3d integer vectors.
    */
    static QRegExpValidator* vector3iValidator();
    /*!
     \brief Data string validator creation for 3d floating point vectors.
    */
    static QRegExpValidator* vector3fValidator();
    /*!
     \brief Data string validator creation for rgba (4d) vectors.
    */
    static QRegExpValidator* rgbaValidator();
    /*!
     \brief Data string validator creation for strings.
    */
    static QRegExpValidator* stringValidator();
    /*!
     \brief Data string validator creation for integer values in a range that can be specified.

     \param min minimum int value
     \param max maximum int value
    */
    static QIntValidator* intValidator(long min = INT_MIN, long max = INT_MAX);
    /*!
     \brief Data string validator creation for float values in a range and with a decimal count that can be specified.

     \param min minimum value
     \param max maximum value
     \param decimals maximum number of decimals
    */
    static QDoubleValidator* doubleValidator(double min = D_NINF, double max = D_INF, int decimals = 6);

public:
    //public static members

    static float F_NAN;   /*!< Float NAN (not a number). Platform independant. */
    static float F_INF;   /*!< Float INF (infinite). Platform independant. */
    static float F_NINF;  /*!< Float NINF (negative infinite). Platform independant. */

    static double D_NAN;  /*!< Double NAN (not a number). Platform independant. */
    static double D_INF;  /*!< Double INF (infinite). Platform independant. */
    static double D_NINF; /*!< Double NINF (negative infinite). Platform independant. */

public:
    //public members

    const EDataType mDataType; /*!< Data type of the data wrapper. */
};


/*!
 \brief Boolean wrapper used in Properties.
*/
class DBool : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DBool() :
      Data(DT_BOOL), mValue(false){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DBool(bool val) :
      Data(DT_BOOL), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    bool mValue; /*!< TODO */
};

/*!
 \brief Integer wrapper used in Properties.
*/
class DInt : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DInt() :
      Data(DT_INT), mValue(0){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DInt(int val) :
      Data(DT_INT), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    int mValue; /*!< TODO */
};

/*!
 \brief Long wrapper used in Properties.
*/
class DLong : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DLong() :
      Data(DT_LONG), mValue(0){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DLong(long val) :
      Data(DT_LONG), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    long mValue; /*!< TODO */
};

/*!
 \brief Float wrapper used in Properties.
*/
class DFloat : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DFloat() :
      Data(DT_FLOAT), mValue(0.0){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DFloat(float val) :
      Data(DT_FLOAT), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    float mValue; /*!< TODO */
};

/*!
 \brief Double wrapper used in Properties.
*/
class DDouble : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DDouble() :
      Data(DT_DOUBLE), mValue(0.0){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DDouble(double val) :
      Data(DT_DOUBLE), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    double mValue; /*!< TODO */
};

/*!
 \brief std::string wrapper used in Properties.
*/
class DString : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DString() :
      Data(DT_STRING), mValue(""){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DString(const QString& val) :
      Data(DT_STRING), mValue(val.toStdString()){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DString(const std::string& val) :
      Data(DT_STRING), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    std::string mValue; /*!< TODO */
};

/*!
 \brief salt::Vector3f wrapper used in Properties.
*/
class DVector3f : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DVector3f() :
      Data(DT_VEC3F), mValue(salt::Vector3f()){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DVector3f(const salt::Vector3f& val) :
      Data(DT_VEC3F), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    salt::Vector3f mValue; /*!< TODO */
};

/*!
 \brief kerosin::RGBA wrapper used in Properties.
*/
class DRGBA : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DRGBA() :
      Data(DT_RGBA), mValue(kerosin::RGBA()){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DRGBA(const kerosin::RGBA& val) :
      Data(DT_RGBA), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    kerosin::RGBA mValue; /*!< TODO */
};

/*!
 \brief salt::AABB3 wrapper used in Properties.
*/
class DAABB3 : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DAABB3() :
      Data(DT_AABB3), mValue(salt::AABB3()){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DAABB3(const salt::AABB3& val) :
      Data(DT_AABB3), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    salt::AABB3 mValue; /*!< TODO */
};

/*!
 \brief salt::Matrix wrapper used in Properties.
*/
class DMat4x4 : public Data
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    DMat4x4() :
      Data(DT_MAT4X4), mValue(salt::Matrix()){}
    /*!
     \brief Complete constructor.

     \param val initial value
    */
    DMat4x4(const salt::Matrix& val) :
      Data(DT_MAT4X4), mValue(val){}

public:
    //public virtual functions

    /*!
     \brief Conversion to data string.
    */
    virtual QString toString() const;
    /*!
     \brief Read data from data string.
    */
    virtual void fromString(const QString& str);

public:
    //public members

    salt::Matrix mValue; /*!< TODO */
};

} //namespace SparkProperty

#endif //SPARKPROPERTYDATA_H
