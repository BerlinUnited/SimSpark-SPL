/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: objectstate.h,v 1.1.2.2 2004/02/08 22:18:22 fruit Exp $

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
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H

#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/transform.h>

class ObjectState : public oxygen::BaseNode
{
public:
    typedef enum TPerceptType
    {
        PT_Default,
        PT_TooFar
    };

public:
    ObjectState();
    virtual ~ObjectState();

    /** set the object name for perceptors */
    virtual void SetPerceptName(const std::string& name,
                                TPerceptType pt = PT_Default);

    /** returns the object name for perceptors */
    virtual std::string GetPerceptName(TPerceptType pt = PT_Default) const;

    /** set the object id for perceptors */
    virtual void SetID(const std::string& id, TPerceptType pt = PT_Default);

    /** returns the object id */
    virtual std::string GetID(TPerceptType pt = PT_Default) const;

    boost::shared_ptr<oxygen::Transform> GetTransformParent() const;

protected:
    typedef std::map<TPerceptType, std::string> TPerceptStringMap;

    virtual void OnLink();

    virtual void OnUnlink();

    /** object names */
    TPerceptStringMap mPerceptNames;

    /** object ids */
    TPerceptStringMap mIDs;

    /** reference to the parent transform node*/
    boost::shared_ptr<oxygen::Transform> mTransformParent;
};

DECLARE_CLASS(ObjectState);

#endif // OBJECTSTATE_H
