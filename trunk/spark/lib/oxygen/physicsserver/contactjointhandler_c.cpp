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
#include "contactjointhandler.h"

using namespace oxygen;

FUNCTION(ContactJointHandler,setContactBounceMode)
{
    bool inSet;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSet))
        )
        {
            return false;
        }

    obj->SetContactBounceMode(inSet);
    return true;
}

FUNCTION(ContactJointHandler,setMinBounceVel)
{
    double inVel;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inVel))
        )
        {
            return false;
        }

    obj->SetMinBounceVel(static_cast<float>(inVel));
    return true;
}

FUNCTION(ContactJointHandler,setContactBounceValue)
{
    double inValue;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inValue))
        )
        {
            return false;
        }

    obj->SetBounceValue(static_cast<float>(inValue));
    return true;
}

FUNCTION(ContactJointHandler,setContactSoftERPMode)
{
    bool inSet;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSet))
        )
        {
            return false;
        }

    obj->SetContactSoftERPMode(inSet);
    return true;
}

FUNCTION(ContactJointHandler,setContactSoftERP)
{
    double inERP;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inERP))
        )
        {
            return false;
        }

    obj->SetContactSoftERP(static_cast<float>(inERP));
    return true;
}

FUNCTION(ContactJointHandler,setContactSoftCFMMode)
{
    bool inSet;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSet))
        )
        {
            return false;
        }

    obj->SetContactSoftCFMMode(inSet);
    return true;
}

FUNCTION(ContactJointHandler,setContactSoftCFM)
{
    double inCFM;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inCFM))
        )
        {
            return false;
        }

    obj->SetContactSoftCFM(static_cast<float>(inCFM));
    return true;
}

FUNCTION(ContactJointHandler,setContactSlipMode)
{
    bool inSet;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSet))
        )
        {
            return false;
        }

    obj->SetContactSlipMode(inSet);
    return true;
}

FUNCTION(ContactJointHandler,setContactSlip)
{
    double inSlip;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSlip))
        )
        {
            return false;
        }

    obj->SetContactSlip(static_cast<float>(inSlip));
    return true;
}

FUNCTION(ContactJointHandler,setContactMu)
{
    double inMu;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inMu))
        )
        {
            return false;
        }

    obj->SetContactMu(static_cast<float>(inMu));
    return true;

}

void CLASS(ContactJointHandler)::DefineClass()
{
  DEFINE_BASECLASS(oxygen/CollisionHandler);
  DEFINE_FUNCTION(setContactBounceMode);
  DEFINE_FUNCTION(setContactBounceValue);
  DEFINE_FUNCTION(setMinBounceVel);
  DEFINE_FUNCTION(setContactSoftERPMode);
  DEFINE_FUNCTION(setContactSoftERP);
  DEFINE_FUNCTION(setContactSoftCFMMode);
  DEFINE_FUNCTION(setContactSoftCFM);
  DEFINE_FUNCTION(setContactSlipMode);
  DEFINE_FUNCTION(setContactSlip);
  DEFINE_FUNCTION(setContactMu);
}
