/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: parameterlist.cpp,v 1.4 2004/04/11 17:03:53 rollmark Exp $

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

using namespace boost;
using namespace zeitgeist;
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

bool
ParameterList::AdvanceValue(TVector::const_iterator& iter, std::string& value) const
{
    char* str;
    if (GetValueInternal<char*, char*>(iter,str))
        {
            value = str;
            return true;
        }

    return GetValueInternal<std::string,std::string>(iter,value);
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
    return
        (
        (GetValueInternal<bool,bool>(iter,value)) ||
        (GetValueInternal<int,bool>(iter,value))
        );
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
};







