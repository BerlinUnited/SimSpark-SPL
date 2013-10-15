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
#ifndef KEROSIN_TEXTURESERVER_H
#define KEROSIN_TEXTURESERVER_H

#ifdef HAVE_CONFIG_H
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#include <sparkconfig.h>
#endif

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/imageserver/imageserver.h>

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

class Texture;

class KEROSIN_API TextureServer : public zeitgeist::Leaf
{
    //
    // types
    //
private:
#if HAVE_HASH_MAP
    typedef std::hash_map<std::string, boost::shared_ptr<Texture> > TTextureCache;
#else
    typedef std::map<std::string, boost::shared_ptr<Texture> > TTextureCache;
#endif
    //
    // functions
    //
public:
    TextureServer();
    virtual ~TextureServer();

    /** load (or returned cached) texture  */
    boost::shared_ptr<Texture> GetTexture(const std::string &name);

protected:
    /** set up the OpenGLServer and ImageServer reference */
    virtual void OnLink();

    //
    // members
    //
private:
    /** reference to the ImageServer */
    CachedPath<ImageServer> mImageServer;
    /** reference to the OpenGLServer */
    CachedPath<OpenGLServer> mOpenGLServer;
    /** registry of cached textures */
    TTextureCache mTextureCache;
};

DECLARE_CLASS(TextureServer);

} //kerosin

#endif //KEROSIN_TEXTURESERVER_H
