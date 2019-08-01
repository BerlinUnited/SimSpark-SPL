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
#ifndef ZEITGEIST_PARAMETERLIST_H
#define ZEITGEIST_PARAMETERLIST_H

#ifndef Q_MOC_RUN
#include <boost/any.hpp>
#endif
#include <vector>
#include <string>
#include <salt/vector.h>
#include <salt/matrix.h>
#include "zeitgeist_defines.h"

namespace zeitgeist
{
class ParameterList;

/** \class ParameterList manages a list of values. Boost::Any is used
    as a typesafe container to realize a sequence of values of
    arbitrary types.
*/
class ZEITGEIST_API ParameterList
{
public:
    typedef std::vector<boost::any> TVector;

protected:
    TVector mList;

public:
    ParameterList();
    virtual ~ParameterList();

    /** inserts a value at the end of the managed sequence */
    void AddValue(const boost::any& value);

    /** inserts an empty ParameterList as a new value at the end of
        the managed sequence and returns a reference to the new
        list. Using AddList instead of AddValue avoids copying a list
        on insertion.
     */
    ParameterList& AddList();

    /** returns the number of values contained in the managed
        sequence */
    int GetSize() const;

    /** returns true if the managed sequence is empty */
    bool IsEmpty() const;

    /** removes all elements from the managed sequence */
    void Clear();

    /** removes the first element from the managed sequence */
    void Pop_Front();

    /** removes the last element from the managed sequence */
    void Pop_Back();

    /** returns an iterator pointing at the begin of the managed
        sequence */
    TVector::const_iterator begin() const;

    /** returns an iterator pointing at the end of the managed
        sequence */
    TVector::const_iterator end() const;

    /** returns an iterator pointing at the nth element of the managed
        sequence */
    TVector::const_iterator operator[] (int n) const;

    /** returns an iterator pointing at the nth element of the managed
        sequence */
    TVector::iterator operator[] (int n);

    /** returns an iterator pointing at the begin of the managed
        sequence */
    TVector::iterator begin();

    /** returns an iterator pointing at the end of the managed
        sequence */
    TVector::iterator end();

    /** AdvanceAnyValue is a generic templated helper function for consumers
        of ParameterLists.

        It tries to cast the parameter at iter to a value of type
        T. If successful it returns true, assigns the casted parameter
        to value and increments the iterator iter. Otherwise false is
        returned and value and iter are unchanged.  Note that
        AdvanceValue increments iter to transparently allow
        specialized functions for types that are represented by more
        than one entry in the ParameterList. An example is a three
        dimensional vector represented by a sequence of three floats.

        Note: the AdvanceAnyValue and AdvanceValue have distinct
        names, as C++ does not support partly specialized template
        function (currently only partly specialized classes) and would
        always call the generic template function instead of a partly
        specialized variant.

        \param iter position of the parameter
        \param value extracted value of the parameter
        \return true if extraction successful, false otherwise
    */
    template<typename T> bool
    AdvanceAnyValue(TVector::const_iterator& iter, T& value) const
    {
        return GetValueInternal<T,T>(iter,value);
    }

    /** GetValue is a generic templated helper function for consumers
        of ParameterLists. It has the same semantics as it's
        corresponding AdvanceValue except that it takes a const
        reference to iter that it does not increment.
    */
    template<typename T> f_inline bool
    GetValue(const TVector::const_iterator& iter, T& value) const
    {
        TVector::const_iterator tmp = iter;
        return AdvanceValue(tmp,value);
    }

    template<typename T> f_inline bool
    GetAnyValue(const TVector::const_iterator& iter, T& value) const
    {
        TVector::const_iterator tmp = iter;
        return AdvanceAnyValue(tmp,value);
    }

    /** Below are AdvanceValue helper functions spezialiced for a type */

    bool AdvanceValue(TVector::const_iterator& iter, std::string& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, float& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, double& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, int& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, bool& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, unsigned int& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, salt::Vector3f& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, salt::Vector2f& value) const;
    bool AdvanceValue(TVector::const_iterator& iter, salt::Matrix& value) const;

protected:
    /** This is a specialized GetValue helper to read any
        salt::TVector from a TParameterList. It first tries to
        interprete a single element of the TParameterList as a
        TVector. If this fails it tries to build a vector from a
        sequence of values in the ParameterList. Depending on the way
        the Vector is built iter is incremented either by one element
        or the number of scalar elements that make up a vector of the
        given type.

        Note: If GetVectorValue builds a vector from a sequence of
        scalars it uses the AdvanceValue function specialized for the
        scalar value type of that TVector. For example in the common
        case of a float vector this enables GetVectorValue to handle
        any mixture of float, int and string representation within the
        TParameterList.

        \param iter
        \param value a vector extracted from a parameter list
        \return true if extraction successful
    */
    template <typename DATATYPE, int ELEMENTS, typename TYPE> f_inline bool
    GetVectorValue(TVector::const_iterator& iter,
                   salt::TVector<DATATYPE,ELEMENTS,TYPE>& value) const
    {
        typedef salt::TVector<DATATYPE,ELEMENTS,TYPE> Vector;

        // try to cast to Vector from a single value
        if (GetValueInternal<Vector,Vector>(iter,value))
            {
                return true;
            }

        // a direct cast faild. try to construct a vector from
        // three consecutive values
        TVector::const_iterator test = iter;
        Vector vec;
        int i=0;

        while (
               (i<ELEMENTS) &&
               (test != mList.end())
               )
            {
                if (! AdvanceValue(test,vec[i]))
                    {
                        break;
                    }
                ++i;
                // iterator test is incremented within the
                // call to GetValue()
            }

        if (i != ELEMENTS)
            {
                // there were not enough components to build
                // the vector
                return false;
            }

        value = vec;
        iter = test;
        return true;
    }

    /** This is a specialized GetValue helper to read a scalar value
        from a TParameterList. It tries to generate the value from a
        string representation
    */
    template<typename TYPE> f_inline bool
    ConvertStringValue(TVector::const_iterator& iter, TYPE& value) const
    {
        if (iter == mList.end())
        {
            return false;
        }

        const boost::any& param = (*iter);

        try
            {
                std::string str;

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

                value = static_cast<TYPE>(atof(str.c_str()));
                ++iter;
            }

        catch(const std::bad_cast&)
            {
                return false;
            }

        return true;
    }

    /** helper that tries to any_cast the TFrom value at iter to a TTo
        Value, on success it returns true and advances the iterator
    */
    template<typename TFrom, typename TTo> f_inline bool
    GetValueInternal(TVector::const_iterator& iter, TTo& value) const
    {
        if (iter == mList.end())
        {
            return false;
        }

        const boost::any& param = (*iter);

        if (param.type() != typeid(TFrom))
            {
                return false;
            }

        try
            {
                value = static_cast<TTo>(boost::any_cast<TFrom>(*iter));
                ++iter;
                return true;
            }

        catch(const std::bad_cast&)
        {
            return false;
        }
    }
};

}
#endif // ZEITGEIST_PARAMETERLIST_H
