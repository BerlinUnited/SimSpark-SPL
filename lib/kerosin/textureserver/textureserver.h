/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: textureserver.h,v 1.4 2003/08/29 07:04:58 fruit Exp $

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
#include <config.h>
#endif

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

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

class OpenGLServer;
class Texture;

class TextureServer : public zeitgeist::Leaf
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

    //! initializes the TextureServer ... checks for the existance of the OpenGLServer
    bool Init(const std::string &openglPath);
    //! retrieve pointer to the OpenGL server ... used by Textures to check extensions
    boost::shared_ptr<OpenGLServer> GetOpenGLServer() const;
    //! load (or returned cached) texture
    boost::shared_ptr<Texture> GetTexture(const std::string &name);

    //
    // members
    //
private:
    boost::shared_ptr<OpenGLServer> mOpenGLServer;
    TTextureCache mTextureCache;          // registry of cached textures
};

DECLARE_CLASS(TextureServer);

} //kerosin

#endif //KEROSIN_TEXTURESERVER_H
