/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitortest.h,v 1.5 2004/12/21 19:42:04 rollmark Exp $

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

   FileSystemSTD
*/
#ifndef MONITORTEST_H__
#define MONITORTEST_H__

#include <string>
#include <oxygen/monitorserver/monitorsystem.h>

/**     \class MonitorTest

        MonitorTest - A simple monitor plugin to use with app/spadestest
*/
class MonitorTest : public oxygen::MonitorSystem
{
public:
    MonitorTest();
    virtual ~MonitorTest();

    /** If a monitor sends information to the world model, this
     * function is called to process it.
     * @param data data sent from monitor to monitorsystem via SPADES.
     */
    virtual void ParseMonitorMessage(const std::string& data);

    /** This function will be called periodically to get information
     * about the current state of the world.
     * \param items holds a list of additional name value pairs. These
     * predicates are collected from MonitorItem objects registered to
     * the MonitorServer. The monitor should transfer them to the
     * client if possible.
     */
    virtual std::string GetMonitorInfo(const oxygen::PredicateList& pList);

    /** This function is called once for every MonitorSystem each time
     *  a new client connects. It should return any header/setup
     *  information that is needed.
     *  \param items holds a list of additional name value
     *  pairs. These predicates are collected from MonitorItem objects
     *  registered to the MonitorServer. The monitor should transfer
     *  them to the client if possible.
     */
    virtual std::string GetMonitorHeaderInfo(const oxygen::PredicateList& pList);

protected:

    /** called after the object has been created and attached to a
        core to allow for object dependent internal
        construction */
  virtual bool ConstructInternal();
};

DECLARE_CLASS(MonitorTest);

#endif // MONITORTEST_H__

