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
#ifndef KEROSIN_MATERIALSERVER_H
#define KEROSIN_MATERIALSERVER_H

#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{
class Material;

class KEROSIN_API MaterialServer : public zeitgeist::Node
{
    //
    // types
    //
private:
    //
    // functions
    //
public:
    MaterialServer();
    virtual ~MaterialServer();

    //! registers a new material
    void RegisterMaterial(boost::shared_ptr<Material> material);

    /** removes all materials from the server and recreates the
        default material
    */
    void ResetMaterials();

    //! returns a cached material
    boost::shared_ptr<Material> GetMaterial(const std::string &name);

    /** Create an instance of a material exporter and register it to the MaterialServer.
        A MaterialExporter is a class that takes care to additionally register/export
        materials registered at the MaterialServer at another class (potentially
         external to spark). This has to be done only when spark/kerosin
        rendering alone is not enough.
        \param name name of the MaterialExporter class name
        \returns true if successful
    */
    bool InitMaterialExporter(const std::string& name);

    //! export all registered materials via all registered exporters
    void ExportAllMaterial();

protected:
    virtual void OnLink();

    //! export a single material to all registered exporters
    void ExportMaterial(boost::shared_ptr<Material> material);
};

DECLARE_CLASS(MaterialServer);

} //kerosin

#endif //KEROSIN_MATERIALSERVER_H
