/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: predicate.cpp 3 2008-11-21 02:38:08Z hedayat $

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

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace std;

/** The null element for Predicate::Iterator */
const zeitgeist::ParameterList Predicate::nullParamList;

Predicate::Iterator::Iterator(const ParameterList* l,
                              ParameterList::TVector::const_iterator i)
    : list(l),iter(i) {};

Predicate::Iterator::Iterator(const ParameterList* l)
    : list(l),iter(l->begin()) {}

Predicate::Iterator::Iterator(const Predicate& predicate)
    : list(&predicate.parameter), iter(predicate.parameter.begin()) {}

Predicate::Iterator::Iterator()
    : list(&nullParamList), iter(nullParamList.begin()) {};

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

const ParameterList::TVector::const_iterator&
Predicate::Iterator::GetIterator() const
{
    return iter;
}

ParameterList::TVector::const_iterator& Predicate::Iterator::GetIterator()
{
    return iter;
}

const ParameterList* Predicate::Iterator::GetList() const
{
    return list;
}

/** implementation of class ParameterName */

bool
ParameterName::operator()(const boost::any& param, const string& pred) const
{
    try
    {
        // try get a ParameterList as an element
        const any* v = &param;
        const ParameterList* lst = any_cast<ParameterList>(v);

        if ( (lst == 0) || (lst->IsEmpty()))
            {
                return false;
            }

        string s;
        lst->GetValue(lst->begin(),s);

        return (pred == s);
    }

    catch(const boost::bad_any_cast &)
    {
        return false;
    }
}

bool
Predicate::FindParameter(Iterator& iter, const string& name) const
{
    const ParameterList* list = iter.GetList();

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
    const ParameterList* paramList
        = boost::any_cast<ParameterList>(&(*test));
    if (
        (paramList == 0) ||
        (paramList->GetSize() < 2)
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

bool Predicate::DescentList(Iterator& iter) const
{
    try
        {
            const any* v = &(*iter.GetIterator());
            const ParameterList* l = any_cast<ParameterList>(v);
            iter = Iterator(l);
            return true;
        }

    catch(const std::bad_cast&)
        {
            return false;
        }

}

/** implementation of class PredicateList */
PredicateList::PredicateList()
{
}

PredicateList::~PredicateList()
{
}

PredicateList::TList::const_iterator PredicateList::begin() const
{
    return mList.begin();
}

PredicateList::TList::const_iterator PredicateList::end() const
{
    return mList.end();
}

Predicate& PredicateList::AddPredicate()
{
    mList.push_back(Predicate());
    return mList.back();
}

int PredicateList::GetSize() const
{
    return static_cast<int>(mList.size());
}

void PredicateList::Clear()
{
    mList.clear();
}




