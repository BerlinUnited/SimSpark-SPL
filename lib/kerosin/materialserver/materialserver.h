/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialserver.h,v 1.4 2003/11/14 14:05:52 fruit Exp $

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
#ifndef KEROSIN_MATERIALSERVER_H
#define KEROSIN_MATERIALSERVER_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace kerosin
{

class Material;

class MaterialServer : public zeitgeist::Leaf
{
    //
    // types
    //
private:
#ifdef HAVE_HASH_MAP
    typedef std::hash_map<std::string, boost::shared_ptr<Material> > TMaterialCache;
#else
    typedef std::map<std::string, boost::shared_ptr<Material> > TMaterialCache;
#endif

    //
    // functions
    //
public:
    MaterialServer();
    virtual ~MaterialServer();

    //! load (or returned cached) material
    boost::shared_ptr<Material> GetMaterial(const std::string &name);

    //! make sure that a material is in memory
    bool Touch(const std::string &name);
protected:
    //! sets up the script variables used for loading
    virtual bool ConstructInternal();

    //
    // members
    //
private:
    TMaterialCache  mMaterialCache;         // registry of cached textures
};

DECLARE_CLASS(MaterialServer);

} //kerosin

#endif //KEROSIN_MATERIALSERVER_H
