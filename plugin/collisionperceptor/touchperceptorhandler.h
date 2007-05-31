/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: touchperceptorhandler.h,v 1.1 2007/05/31 14:48:58 hedayat Exp $

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

#ifndef TOUCHPERCEPTORHANDLER_H_
#define TOUCHPERCEPTORHANDLER_H_

#include <oxygen/physicsserver/contactjointhandler.h>

/** \class TouchPerceptorHandler is a ContactJointHandler that provides
 * enough information for the ExtendedTouchPerceptor
 */
class TouchPerceptorHandler : public oxygen::ContactJointHandler
{
public:
    virtual void HandleCollision
    (boost::shared_ptr<oxygen::Collider> collidee, dContact& contact);
};

DECLARE_CLASS(TouchPerceptorHandler);

#endif /*TOUCHPERCEPTORHANDLER_H_*/
