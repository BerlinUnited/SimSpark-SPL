/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: predicate.cpp,v 1.3 2004/02/12 14:07:22 fruit Exp $

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
#include "predicate.h"

using namespace oxygen;
using namespace std;

/** The null elemeent for Predicate::Iterator */
const Predicate::TParameterList Predicate::nullParamList;

/** implementation of class Predicate::Iterator */

Predicate::Iterator::Iterator(const TParameterList* l, TParameterList::const_iterator i)
    : list((TParameterList*)l),iter(i) {};

Predicate::Iterator::Iterator(const Predicate& predicate)
    : list((TParameterList*)&predicate.parameter), iter(predicate.parameter.begin()) {}

Predicate::Iterator::Iterator()
    : list((TParameterList*)&nullParamList), iter(nullParamList.begin()) {};

const boost::any& Predicate::Iterator::operator * () const
{
    return (*iter);
}

void Predicate::Iterator::operator ++ ()
{
    if (iter != list->end())
        {
            ++iter;
        }
}

Predicate::Iterator Predicate::Iterator::begin() const
{
    return Iterator(list, list->begin());
}

Predicate::Iterator Predicate::Iterator::end() const
{
    return Iterator(list, list->end());
}

bool Predicate::Iterator::operator != (const Iterator& i) const
{
    return (
            (list != i.list) ||
            (iter != i.iter)
            );
}

bool Predicate::Iterator::operator == (const Iterator& i) const
{
    return (
            (list == i.list) &&
            (iter == i.iter)
            );
}

Predicate::TParameterList::const_iterator Predicate::Iterator::GetIterator() const
{
    return iter;
}

Predicate::TParameterList* Predicate::Iterator::GetList() const
{
    return list;
}


/** implementation of class ParameterName */

bool
ParameterName::operator()(const boost::any& param, const string& pred) const
{
    try
    {
        // try to cast the element to a TParameterList
        const Predicate::TParameterList* lst =
            boost::any_cast<Predicate::TParameterList>(&param);

        if ( (lst == 0) || (lst->empty()))
            {
                return false;
            }

        string s = boost::any_cast<string>(lst->front());

        return pred == s;
    }

    catch(const boost::bad_any_cast &)
    {
        return false;
    }
}

bool
Predicate::FindParameter(Iterator& iter, const string& name) const
{
    const TParameterList* list = iter.GetList();

    Iterator test
        (
         list,
         find_if(
                 list->begin(), list->end(),
                 bind2nd(ParameterName(), name)
                 )
         );

    if (test == test.end())
        {
            return false;
        }

    // try to extract the first element as a parameter list
    const TParameterList* paramList
        = boost::any_cast<Predicate::TParameterList>(&(*test));
    if (
        (paramList == 0) ||
        (paramList->size() < 2)
        )
        {
            return false;
        }

    // return an iterator to the second element of the list, i.e. the
    // first parameter value
    iter = Iterator(paramList,paramList->begin());
    ++iter;

    return true;
}
