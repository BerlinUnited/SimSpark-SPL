/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: baseparser.h,v 1.1.2.2 2003/12/02 16:55:43 rollmark Exp $

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

#include <list>
#include <boost/any.hpp>
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>

namespace oxygen
{

class BaseParser : public zeitgeist::Leaf
{
public:
    typedef std::list<boost::any> TParameterList;

    struct TPredicate
    {
        std::string name;
        TParameterList parameter;
    };

    typedef std::list<TPredicate> TPredicateList;

    virtual TPredicateList Parse(const std::string input) = 0;
};

DECLARE_ABSTRACTCLASS(BaseParser);

} // namespace oxygen

#endif // OXYGEN_BASEPARSER_H
