/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "sexpparser.h"

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace boost;


SexpParser::SexpParser()
    : BaseParser()
{
}

SexpParser::~SexpParser()
{
}

shared_ptr<PredicateList>
SexpParser::Parse(const std::string& input)
{
    size_t len = input.length();

    shared_ptr<PredicateList> predList(new PredicateList);
    if (len == 0)
    {
            return predList;
    }

    char* c = const_cast<char*>(input.c_str());
    pcont_t* pcont = init_continuation(c);
    sexp_t* sexp = iparse_sexp(c,static_cast<int>(input.size()),pcont);

    while (sexp != 0)
    {
        SexpToPredicate(predList,sexp);
        destroy_sexp(sexp);
        sexp = iparse_sexp(c,static_cast<int>(input.size()),pcont);
    }

    destroy_continuation(pcont);
    return predList;
}

string
SexpParser::Generate(boost::shared_ptr<PredicateList> input)
{
    if (input.get() == 0)
    {
            return "";
    }

    stringstream ss;
    PredicateList& list = *input.get();

    for (
         PredicateList::TList::const_iterator i = list.begin();
         i != list.end();
         ++i
         )
        PredicateToString(ss,*i);

    return ss.str();
}

void
SexpParser::SexpToList(ParameterList& arguments, const sexp_t* const sexp)
{
    sexp_t* s = const_cast<sexp_t*>(sexp);
    while (s != 0)
    {
        if (s->ty == SEXP_VALUE)
        {
            arguments.AddValue(string(s->val));
        } else {
            ParameterList& elem = arguments.AddList();
            SexpToList(elem,s->list);
        }
        s = s->next;
    }
}

void
SexpParser::SexpToPredicate
(shared_ptr<PredicateList>& predList, const sexp_t* const sexp)
{
    // throw away outer brackets (i.e. we have a list at the top
    // level)
    if (sexp->ty != SEXP_LIST)
    {
        return;
    }

    sexp_t* s = const_cast<sexp_t*>(sexp->list);

    if (
        (s == 0) ||
        (s->ty != SEXP_VALUE)
        )
        {
            return;
        }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = string(s->val);
    SexpToList(predicate.parameter,s->next);
}

void
SexpParser::ListToString(stringstream& ss, const ParameterList& lst)
{
    string space;

    ss.setf(ios_base::fixed,ios_base::floatfield);
    ss.precision(2);

    for (
         ParameterList::TVector::const_iterator i = lst.begin();
         i != lst.end();
         ++i
         )
    {
        if (i->type() == typeid(string))
        {
            ss << space;
            ss << boost::any_cast<string>(*i);
        }
        else if (i->type() == typeid(float))
        {
            ss << space;
            ss << boost::any_cast<float>(*i);
        }
        else if (i->type() == typeid(int))
        {
            ss << space;
            ss <<boost::any_cast<int>(*i);
        }
        else if (i->type() == typeid(ParameterList))
        {
            const any* v = &(*i);
            const ParameterList* lst = any_cast<ParameterList>(v);
            ss << space;
            ss << '(';
            ListToString(ss,*lst);
            ss << ')';
        }
        else
        {
            ss << space;
            ss << "(error data format unknown)";
        }

        space = " ";
    }
}

void
SexpParser::PredicateToString(stringstream& ss, const Predicate& plist)
{
    ss << '(';
    ss << plist.name;
    ss << ' ';
    ListToString(ss,plist.parameter);
    ss << ')';
}
