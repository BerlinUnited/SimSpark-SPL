/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: predicate.cpp,v 1.1.2.2 2003/12/29 18:28:17 rollmark Exp $

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

bool
ParameterName::operator()(const boost::any& param, const std::string& pred) const
{
    try
    {
        const Predicate::TParameterList* lst =
            boost::any_cast<Predicate::TParameterList>(&param);

        if ( (lst == 0) || (lst->empty()))
            {
                return false;
            }

        std::string s = boost::any_cast<std::string>(lst->front());
        return pred == s;
    }
    catch(const boost::bad_any_cast &)
    {
        return false;
    }
}

bool
Predicate::FindParameter(TParameterList::const_iterator& iter,
                         const std::string& name) const
{
    iter = find_if(parameter.begin(), parameter.end(),
                   std::bind2nd(ParameterName(), name));
    return iter != parameter.end();
}
