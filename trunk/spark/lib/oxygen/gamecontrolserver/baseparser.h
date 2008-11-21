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
#ifndef OXYGEN_BASEPARSER_H
#define OXYGEN_BASEPARSER_H

#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include "predicate.h"

namespace oxygen
{
/** \class BaseParser defines the interface for a parser and generator
    that is used with the GameControlServer. A BaseParser is
    responsible to parse a string into a list of predicates (see
    \class Predicate) and to assemble a string representing a given
    list of predicates.
 */
class BaseParser : public zeitgeist::Leaf
{
public:
    /** parses the \param input string into a list of Predicates */
    virtual boost::shared_ptr<PredicateList> Parse(const std::string& input) = 0;

    /** generates a string representing the given \param input list of
        predicates */
    virtual std::string Generate(boost::shared_ptr<PredicateList> input) = 0;
};

DECLARE_ABSTRACTCLASS(BaseParser);

} // namespace oxygen

#endif // OXYGEN_BASEPARSER_H
