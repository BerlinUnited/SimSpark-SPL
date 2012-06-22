/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sexpparser.h 264 2011-03-26 02:16:36Z hedayat $

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
#ifndef SEXPPARSER_H
#define SEXPPARSER_H

#include <sstream>
#include <sfsexp/sexp.h>
#include <oxygen/gamecontrolserver/baseparser.h>
#include <zeitgeist/class.h>

class SexpParser : public oxygen::BaseParser
{
public:
    SexpParser();
    virtual ~SexpParser();

    virtual boost::shared_ptr<oxygen::PredicateList> Parse(const std::string& input);
    virtual std::string Generate(boost::shared_ptr<oxygen::PredicateList> input);

private:
    void SexpToList(zeitgeist::ParameterList& arguments,
                    const sexp_t* const sexp);

    void SexpToPredicate(boost::shared_ptr<oxygen::PredicateList>& predicate,
                         const sexp_t* const sexp);

    void ListToString(std::stringstream& ss,
                      const zeitgeist::ParameterList& lst);

    void PredicateToString(std::stringstream& ss,
                           const oxygen::Predicate& predicate);

private:
    /** the s-expression library memory management object */
    sexp_mem_t *mSexpMemory;
};

DECLARE_CLASS(SexpParser);


#endif // SEXPPARSER_H
