/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Wed Nov 9 2005
   Copyright (C) 2005 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef JOINTEFFECTOR_H
#define JOINTEFFECTOR_H

#include <oxygen/agentaspect/effector.h>

namespace oxygen
{

template<typename _JOINT>
class JointEffector : public oxygen::Effector
{
public:
    JointEffector(const std::string& name)
        : Effector()
    {
        SetName(name);
    }

    virtual ~JointEffector()
    {
    }

    virtual void UpdateCached()
    {
        Effector::UpdateCached();
        mJoint = FindParentSupportingClass<_JOINT>().lock();

        if (mJoint.get() == 0)
        {
            GetLog()->Error()
                << "(" << GetClass()->GetName()
                << ") ERROR: found no Joint parent\n";
        }
    }

protected:
    /** setup the reference to the HingeJoint parent node */
    virtual void OnLink()
    {
        Effector::OnLink();
        UpdateCached();
    }

    /** remove the reference to the HingeJoint parent node */
    virtual void OnUnlink()
    {
        Effector::OnUnlink();
        mJoint.reset();
    }

protected:
    /** cached reference to the joint */
    boost::shared_ptr<_JOINT> mJoint;
};

} // namespace oxygen

#endif // JOINTEFFECTOR_H
