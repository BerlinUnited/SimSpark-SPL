/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: predicate.h,v 1.4 2004/01/01 18:24:33 fruit Exp $

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
#ifndef OXYGEN_PREDICATE_H
#define OXYGEN_PREDICATE_H

#include <list>
#include <string>
#include <functional>
#include <boost/any.hpp>
#include <salt/vector.h>
#include <zeitgeist/logserver/logserver.h>

namespace oxygen
{

/** A functional class to find a specific parameter for use as STL predicate.
*/
class ParameterName : public std::binary_function<boost::any,std::string,bool>
{
public:
    bool operator()(const boost::any& param, const std::string& pred) const;
};

/** \class Predicate encapsulates a predicate name together with its
    list of parameters.

    It is used as an internal representation for commands issued by
    agents and is interpreted by Effector classes. A BaseParser object
    is responsible to translate the received agent commands into a
    list of Predicates. An Effector is therefore independent from the
    command format used between an agent and the simulator.

    Predicates are further used to collect sensor data from the agents
    perceptors. A BaseParser is then used to translate these percepts
    into the format used to describe them to the corresponding
    agent. Again, the Perceptor is independent from the format used to
    transmit its data to the agent.
*/
class Predicate
{
 public:
    /** This declares the list of parameters of a predicate. Boost::Any
        is used as a typesafe container to realize a sequence of
        values of arbitrary types.
     */
    typedef std::list<boost::any> TParameterList;

    /** This declares a list of predicates. It is used as output
        format of the BaseParser interface.
    */
    typedef std::list<Predicate> TList;

public:
    /** GetValue is a generic templated helper function for consumers
        of TParameterLists.

        It tries to cast the parameter at iter to a value of type
        T. If successful it returns true, assigns the casted parameter
        to value and increments the iterator iter. Otherwise false is
        returned and value and iter are unchanged.  Note the GetValue
        increments iter to transparently allow specialized functions
        for types that are represented by more than one entry in the
        ParameterList. An example is a three dimensional vector
        represented by a sequence of three floats.

        \param iter position of the parameter
        \param value extracted value of the parameter
        \return true if extraction successful, false otherwise
     */
    template<typename T> bool
    GetValue(TParameterList::const_iterator& iter, T& value) const
    {
        try
            {
                value = boost::any_cast<T>(*iter);
                ++iter;
                return true;
            }
        catch(const boost::bad_any_cast &)
            {
                return false;
            }
    }
    /** Below are GetValue helper functions spezialiced for a single type */

    /** GetValue helper for float */
    bool GetValue(TParameterList::const_iterator& iter, float& value) const
    {
        return ConvertStringValue(iter, value);
    }

    /** GetValue helper for double */
    bool GetValue(TParameterList::const_iterator& iter, double& value) const
    {
        return ConvertStringValue(iter, value);
    }

    /** GetValue helper for int */
    bool GetValue(TParameterList::const_iterator& iter, int& value) const
    {
        return ConvertStringValue(iter, value);
    }

    /** GetValue helper for unsigned int */
    bool GetValue(TParameterList::const_iterator& iter, unsigned int& value) const
    {
        return ConvertStringValue(iter, value);
    }

    /** GetValue helper for Vetor3f */
    bool GetValue(TParameterList::const_iterator& iter, salt::Vector3f& value) const
    {
        return GetVectorValue(iter,value);
    }

    /** GetValue helper for Vector2f */
    bool GetValue(TParameterList::const_iterator& iter, salt::Vector2f& value) const
    {
        return GetVectorValue(iter,value);
    }

    /** Find a parameter with a given name.

        For this method, we assume that parameters are represented by lists
        consisting of a name (a string) and a (number of) values. An example
        for the predicate init:
        init.name = "init";
        init.parameter = [["teamname", "RoboLog"], ["unum", 1]];

        The part after init is the list of parameters. To extract the teamname,
        do something like
        FindParameter(iter, "teamname");
        TParameterList myList = boost::any_cast<TParameterList>(*iter);
        iter = myList.begin();
        ++iter;
        if (iter != myList.end())
            GetValue(iter, name);

        \param iter on success, iter will point to the list element you are
                    looking for. Will be unchanged otherwise.
        \param name Name of the parameter to find.
        \return true if parameter name was found.
    */
    bool FindParameter(TParameterList::const_iterator& iter,
                       const std::string& name) const;

 protected:
    /** \note As c++ does not support partially specialized function
        templates (this concept does only apply to template classes),
        the compiler would always call the generic GetValue function
        and not our partially specialized functions. To work around
        this issue the helper functions below are inlined into the set
        of fully specialized functions above.
    */

    /** This is a specialized GetValue helper to read any
        salt::TVector from a TParameterList. It first tries to
        interprete a single element of the TParameterList as a
        TVector. If this fails it tries to build a vector from a
        sequence of values in the TParameterList. Depending on the way
        the Vector is built the \param iter is incremented either by
        one element or the number scalar elements that make up a
        vector of the given type.

        Note: If GetVectorValue builds a vector from a sequence of
        scalars it uses a template GetValue function specialized for
        the scalar value type of that TVector. For example in the
        common case of a float vector this enables GetVectorValue to
        handle any mixture of float and string representation within
        the TParameterList.
     */
    template <typename DATATYPE, int ELEMENTS, typename TYPE> f_inline bool
    GetVectorValue(TParameterList::const_iterator& iter,
                   salt::TVector<DATATYPE,ELEMENTS,TYPE>& value) const
    {
        typedef salt::TVector<DATATYPE,ELEMENTS,TYPE> Vector;

        try
            {
                // try to cast to Vector from a single value
                value = boost::any_cast<Vector>(*iter);
                ++iter;
                return true;
            }
        catch(const boost::bad_any_cast &)
            {
                // a direct cast faild. try to construct a vector from
                // three consecutive values
                TParameterList::const_iterator& test = iter;
                Vector vec;
                int i=0;

                while (
                       (i<ELEMENTS) &&
                       (test != parameter.end())
                       )
                    {
                        if (! GetValue(test,vec[i]))
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
    }

    /** This is a specialized GetValue helper to read a scalar value
        from a TParameterList. It first tries a direct cast to the
        desired type. If this fails it tries to generate the type
        from a string.
    */
    template<typename TYPE> f_inline bool
    ConvertStringValue(TParameterList::const_iterator& iter, TYPE& value) const
    {
        try
            {
                value = boost::any_cast<TYPE>(*iter);
                ++iter;
                return true;
            }
        catch(const boost::bad_any_cast &)
            {
                try
                    {
                        std::string str = boost::any_cast<std::string>(*iter);
                        value = static_cast<TYPE>(atof(str.c_str()));
                        ++iter;
                        return true;
                    }

                catch(const boost::bad_any_cast &)
                    {
                        return false;
                    }
            }
    }

 public:
    std::string name;
    TParameterList parameter;
};

}  // namespace oxygen

#endif // OXYGEN_PREDICATE_H
