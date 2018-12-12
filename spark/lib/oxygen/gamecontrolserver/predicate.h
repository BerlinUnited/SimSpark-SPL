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
#ifndef OXYGEN_PREDICATE_H
#define OXYGEN_PREDICATE_H

#include <list>
#include <string>
#include <functional>
#ifndef Q_MOC_RUN
#include <boost/any.hpp>
#endif
#include <salt/vector.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
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
class OXYGEN_API Predicate
{
 public:
    /** \class Iterator encapsulates a
        ParameterList::TVectr::const_iterator together with a
        reference to the ParameterList the iterator belongs to. It
        tries to mimic the behaviour of an STL iterator as much as
        possible.

        This class is necessary because it is not sufficient for the
        FindParameter method to return a single iterator without the
        corresponding list as it is possible for list to be nested. A
        subsequent GetValue method would not be able to test if the
        end of a TParameterList is reached without knowing the list an
        iterator belongs to.
     */
    struct OXYGEN_API Iterator
    {
    public:
        /** constructs an Iterator pointing to element i of list l */
        Iterator(const zeitgeist::ParameterList* l,
                 zeitgeist::ParameterList::TVector::const_iterator i);

        /** constructs an Iterator pointing to the first element of
            list l */
        Iterator(const zeitgeist::ParameterList* l);

        /** constructs an Iterator pointing to the first element of
            predicate::parameter */
        Iterator(const Predicate& predicate);

        /** construct an Iterator pointing to
            Predicate::nullParamList. This empty static parameter list
            acts as a null element and avoids special cases for
            list==0 in the Iterator implementation.
        */
        Iterator();

        /** aeturns the element this Iterator points to */
        const boost::any& operator * () const;

        /** advances this Iterator on element if possible */
        void operator ++ ();

        /** constructs an Iterator pointing to the first element of
            the associated list */
        Iterator begin() const;

        /** constructs an Iterator pointing to the end() element of
            the associated list */
        Iterator end() const;

        /** \returns true if this Iterator and i are not identical */
        bool operator != (const Iterator& i) const;

        /** \returns true if this Iterator and i are identical */
        bool operator == (const Iterator& i) const;

        /** \returns a reference to the encapuslated iterator */
        const zeitgeist::ParameterList::TVector::const_iterator&
        GetIterator() const;

        /** \returns a reference to the encapsulated iterator */
        zeitgeist::ParameterList::TVector::const_iterator& GetIterator();

        /** \returns a pointer to the associated list */
        const zeitgeist::ParameterList* GetList() const;

    protected:
        /** the associated list */
        const zeitgeist::ParameterList* list;

        /** the encapsulated iterator */
        zeitgeist::ParameterList::TVector::const_iterator iter;
    };

public:
    const Iterator begin() const
    {
        return Iterator(&parameter,parameter.begin());
    }

    /** find a parameter with a given name.

        For this method, we assume that parameters are represented by
        lists consisting of a name (a string) and (a number of)
        values. An example for the predicate init:
        init.name = "init";
        init.parameter = [["teamname", "RoboLog"], ["unum", 1]];

        The part after init is the list of parameters. To extract the
        teamname, do something like
        Predicate::iterator iter(myPredicate);
        if (FindParameter(iter, "teamname"))
        {
          GetValue(iter,name);
        }

        \param iter on success, iter will point to the first value of
        the parameter you are looking for. It will be unchanged
        otherwise.
        \param name Name of the parameter to find.
        \return true if parameter name was found.
    */
    bool FindParameter(Iterator& iter, const std::string& name) const;

    /** finds and returns the first value of a parameter with a given
        name.

        This method tries to cover the most common use case of a
        ParameterList. It assumes that the parameters are a set lists
        consisting of name value pairs, e.g. for the predicate init

        init.name = "init";
        init.parameter = [["teamname", "RoboLog"], ["unum", 1]];

        For parameter entries with more than one value per parameter
        use the generic FindParameter/GetValue functions.

        \param name Name of the parameter to find.
        \param value reference to the value that will receive the
        parameter value on success
        \return true if parameter name was found.
     */
    template<typename T> f_inline bool
    GetValue(const Iterator& iter, const std::string& name, T& value) const
    {
        Iterator tmp(iter);
        return AdvanceValue(tmp,name,value);
   }

    /** finds and returns the first value of a parameter with a given
        name.  On success value contains the first parameter value and
        iter points to the second parameter value
     */
    template<typename T> f_inline bool
    AdvanceValue(Iterator& iter, const std::string&name, T& value) const
    {
        if (! FindParameter(iter,name))
            {
                return false;
            }

        return AdvanceValue(iter,value);
    }

    /** AdvanceValue tries to cast the parameter at iter to a value of
        type T. If successful it returns true, assigns the casted
        parameter to value and increments the iterator iter. Otherwise
        false is returned and value and iter are unchanged.
    */
    template<typename T> f_inline bool
    AdvanceValue(Iterator& iter, T& value) const
    {
        return iter.GetList()->AdvanceValue(iter.GetIterator(),value);
    }

    /** GetValue has the same semantics as AdvanceValue except that it
        takes a const reference to iter that it does not increment.
    */
    template<typename T> f_inline bool
    GetValue(const Iterator& iter, T& value) const
    {
        return iter.GetList()->GetValue(iter.GetIterator(),value);
    }

    template<typename T> f_inline bool
    GetAnyValue(const Iterator& iter, T& value) const
    {
        return iter.GetList()->GetAnyValue(iter.GetIterator(),value);
    }

    /** tries to inteprete the value iter points to as a ParameterList
        and points iter to it's first element. Returns true on success
     */
    bool DescentList(Iterator& iter) const;

 public:
    /** the name of this predicate */
    std::string name;

    /** the list of parameter values */
    zeitgeist::ParameterList parameter;

protected:
    static const zeitgeist::ParameterList nullParamList;
};

/** A functional class to find a specific parameter for use as STL
    predicate.
*/
class ParameterName : public std::binary_function<boost::any,std::string,bool>
{
public:
    bool operator()(const boost::any& param, const std::string& pred) const;
};

class OXYGEN_API PredicateList
{
public:
    typedef std::list<Predicate> TList;
public:
    PredicateList();
    virtual ~PredicateList();

    /** returns an iterator pointing at the first contained Predicate
     */
    TList::const_iterator begin() const;

    /** returns an iterator pointing at the last contained
        Predicate */
    TList::const_iterator end() const;

    /** returns the number of values contained in the managed
        sequence */
    int GetSize() const;

    /** removes all contained Predicates */
    void Clear();

    Predicate& AddPredicate();

protected:
    TList mList;
};

}  // namespace oxygen

#endif // OXYGEN_PREDICATE_H
