/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perceptor.h,v 1.3 2003/08/29 06:55:29 fruit Exp $

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
#ifndef KEROSIN_PERCEPTOR_H
#define KEROSIN_PERCEPTOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../sceneserver/basenode.h"

#if HAVE_HASH_MAP
#include <hash_map>
#else
#include <map>
#endif

namespace kerosin
{
#if 0
}
#endif

class Perceptor : public kerosin::BaseNode
{
    //
    // types
    //
public:
#if HAVE_HASH_MAP
    typedef std::hash_map<std::string, boost::any> TDictionary;
#else
    typedef std::map<std::string, boost::any> TDictionary;
#endif
    //
    // functions
    //
public:
    /*!
      This is called by agents to trigger the percept event implemented by
      this perceptor. The perceptor can return data through the dictionary,
      which is passed as a parameter. Returns true, if valid data is
      available and false otherwise.
    */
    virtual bool Percept(TDictionary &dictionary) = 0;
};

DECLARE_ABSTRACTCLASS(Perceptor);

} // namespace kerosin

#endif //KEROSIN_PERCEPTOR_H
