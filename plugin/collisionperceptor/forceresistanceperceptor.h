/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceresistanceperceptor.h,v 1.1 2007/05/31 17:39:54 hedayat Exp $

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

#ifndef FORCERESISTANCEPERCEPTOR_H_
#define FORCERESISTANCEPERCEPTOR_H_

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/physicsserver/odewrapper.h>
#include <list>
#include <utility>

class ForceResistancePerceptor : public oxygen::Perceptor
{
protected:
	typedef std::list<std::pair<dContact, dJointID> > ContactListType;
	
public:
	/** adds new touch information
	 */
	void AddTouchInfo(dContact &contact, dJointID contactJointID);
	
    /** adds touch information to predList
        \return true if data is available
     */
	bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);
	
protected:
    /** removes old information left over from the previous cycles */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
	ContactListType mContactList;
};

DECLARE_CLASS(ForceResistancePerceptor);

#endif /*FORCERESISTANCEPERCEPTOR_H_*/
