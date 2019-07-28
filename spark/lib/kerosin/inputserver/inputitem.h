/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef KEROSIN_INPUTITEM_H
#define KEROSIN_INPUTITEM_H

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include "inputserver.h"
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{

/** InputItem is the base class for all nodes installed below the
    Spark InputControl node. This serves as a hook mechanism to
    register custom command ids to the InputServer and to process them
*/
class KEROSIN_API InputItem : public zeitgeist::Leaf
{
public:
    InputItem();
    virtual ~InputItem();

    /** this method is called every time an input unknown to the
        parent InputControl node is received
    */
    virtual void ProcessInput(const Input& input) = 0;
};

DECLARE_ABSTRACTCLASS(InputItem);

} // namespace kerosin

#endif // KEROSIN_INPUTITEM_H
