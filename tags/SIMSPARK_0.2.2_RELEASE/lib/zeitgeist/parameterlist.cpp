/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "parameterlist.h"
#include <salt/gmath.h>
#include <sstream>

using namespace boost;
using namespace zeitgeist;
using namespace salt;
using namespace std;

ParameterList::ParameterList()
{
}

ParameterList::~ParameterList()
{
}

void
ParameterList::AddValue(const boost::any& value)
{
    mList.push_back(value);
}

ParameterList&
ParameterList::AddList()
{
    mList.push_back(ParameterList());
    return *any_cast<ParameterList>(&mList.back());
}

int
ParameterList::GetSize() const
{
    return static_cast<int>(mList.size());
}

bool
ParameterList::IsEmpty() const
{
    return mList.empty();
}

void
ParameterList::Clear()
{
    mList.clear();
}

void ParameterList::Pop_Front()
{
    if (! mList.empty())
        {
            mList.erase(mList.begin());
        }
}

void ParameterList::Pop_Back()
{
    if (! mList.empty())
        {
            mList.erase(mList.end());
        }
}

ParameterList::TVector::const_iterator
ParameterList::begin() const
{
    return mList.begin();
}

ParameterList::TVector::const_iterator
ParameterList::end() const
{
    return mList.end();
}

ParameterList::TVector::const_iterator
ParameterList::operator[] (int n) const
{
    if (
        (n <0) ||
        (n >= static_cast<int>(mList.size()))
        )
        {
            return mList.end();
        }

    return (mList.begin() + n);
}

ParameterList::TVector::iterator
ParameterList::begin()
{
    return mList.begin();
}

ParameterList::TVector::iterator
ParameterList::end()
{
    return mList.end();
}

ParameterList::TVector::iterator
ParameterList::operator[] (int n)
{
    if (
        (n <0) ||
        (n >= static_cast<int>(mList.size()))
        )
        {
            return mList.end();
        }

    return (mList.begin() + n);
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, std::string& value) const
{
    char* character;
    if (GetValueInternal<char*, char*>(iter,character))
        {
            value = character;
            return true;
        }

    string str;
    if (GetValueInternal<std::string,std::string>(iter,str))
        {
            value = str;
            return true;
        }

    // try to generate a string from a float, double or int

    double d;
    if (
        (GetValueInternal<float,double>(iter,d)) ||
        (GetValueInternal<double,double>(iter,d))
        )
        {
            stringstream ss;
            ss << d;
            value = ss.str();
            return true;
        }

    int i;
    if (GetValueInternal<int,int>(iter,i))
        {
            stringstream ss;
            ss << i;
            value = ss.str();
            return true;
        }

    return false;
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, float& value) const
{
    return
        (
         (GetValueInternal<float,float>(iter,value)) ||
         (GetValueInternal<double,float>(iter,value)) ||
         (GetValueInternal<int,float>(iter,value)) ||
         (ConvertStringValue<float>(iter, value))
         );
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, double& value) const
{
    return
        (
         (GetValueInternal<double,double>(iter,value)) ||
         (GetValueInternal<float,double>(iter,value)) ||
         (GetValueInternal<int,double>(iter,value)) ||
         (ConvertStringValue<double>(iter, value))
         );
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, int& value) const
{
    return
        (
         (GetValueInternal<int,int>(iter,value)) ||
         (GetValueInternal<unsigned int,int>(iter,value)) ||
         (ConvertStringValue<int>(iter, value))
         );
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, bool& value) const
{
    if
        (
        (GetValueInternal<bool,bool>(iter,value)) ||
        (GetValueInternal<int,bool>(iter,value))
        )
        {
            return true;
        }

    if (iter == mList.end())
    {
        return false;
    }

    const any& param = (*iter);
    string str;
    if (param.type() == typeid(std::string))
        {
            str = boost::any_cast<std::string>(param);
        } else if (param.type() == typeid(char*))
        {
            str = boost::any_cast<char*>(param);
        } else
        {
            return false;
        }

    if (str == "true")
        {
            value = true;
            return true;
        }

    if (str == "false")
        {
            value = false;
            return true;
        }

    return false;
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, unsigned int& value) const
{
    return (
            (GetValueInternal<unsigned int,unsigned int>(iter,value)) ||
            (GetValueInternal<int,unsigned int>(iter,value)) ||
            (ConvertStringValue<unsigned int>(iter, value))
            );
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, salt::Vector3f& value) const
{
    return GetVectorValue(iter, value);
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, salt::Vector2f& value) const
{
    return GetVectorValue(iter, value);
}

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, Matrix& value) const
{
    // try to cast to Vector from a single value
    if (GetValueInternal<Matrix,Matrix>(iter,value))
    {
        return true;
    }
    
    // a direct cast faild. try to construct a matrix from
    // 16 consecutive values
    TVector::const_iterator test = iter;
    float m[16];
    int i=0;

    while (
            (i<16) &&
            (test != mList.end())
            )
    {
        if (! AdvanceValue(test,m[i]))
            {
                break;
            }
        ++i;
        // iterator test is incremented within the
        // call to GetValue()
    }
    
    if (i == 16)
    {
        value = Matrix(m);
        iter = test;
        return true;
    }
    
    // constructing matrix from 16 values failed.
    // try to construct a matrix from position and rotation vectors
    test = iter;
    Vector3f pos, rot;
    if (AdvanceValue(test, pos) && AdvanceValue(test, rot))
    {
        value = Matrix();
        value.RotationX(gDegToRad(rot[0]));
        value.RotateY(gDegToRad(rot[1]));
        value.RotateZ(gDegToRad(rot[2]));
        value.Pos() = pos;
        iter = test;
        return true;
    }
    
    return false;
}

/*
  The following declarations are here to force the compiler to
  generate a type_info object for the template classes
  boost::holder<>. Several plugins, e.g. PerfectVisionPerceptor and
  SexpParser rely on the runtime type information to generate and
  parse ParameterLists.

  If these declarations are omitted the linker does not resolve the
  separately generated type_info classes for each module and a
  comparison between otherwise identical types fails.

  Strictly it would be sufficient to typedef two instances of the
  boost::holder<> class. But as this class is an implementation detail
  of boost I chose the more portable way and went with it's public
  wrapper boost:any.

  The following declarations are unfortunately needed for all
  structured types used inside a TParameterList. Currently this only
  is std::string and the TParameterList itself to allow nesting of
  parameter lists.
*/
namespace
{
  const zeitgeist::ParameterList paramList;
  const boost::any anyDummyParamList(paramList);

  const std::string str;
  const boost::any anyStr(str);
  
  const salt::Matrix matrix;
  const boost::any anyDummyMatrix(matrix);
};







