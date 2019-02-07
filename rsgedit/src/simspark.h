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
#ifndef SIMSPARK_H__
#define SIMSPARK_H__

#include <spark/spark.h>
#include <wx/string.h>
#include "constants.h"
#include <sstream>

class SimSpark : public spark::Spark
{
public:
    SimSpark();
    ~SimSpark();

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    /** print command line options */
    void PrintHelp();

    /** print a greeting */
    void PrintGreeting();

    /** process command line options */
    bool ProcessCmdLine(int argc, char* argv[]);

    /** get the current simulation state */
    ESimState GetSimState() const;

    /** resets the simulation, i.e. empties the scene */
    void ResetSimulation();

    /** set the current simulationm state */
    void SetSimState(ESimState state);

    /** returns true iff the log buffer is filled */
    static bool HasLogContents();

    /** returns the current log buffer */
    static wxString GetLogBuffer();

    /** clears the log buffer */
    static void ClearLogBuffer();

    /** change the priority filter of the managed log stream */
    void SetLogPriority(unsigned int mask);

    /** returns the current priority filter of the managed log stream */
    unsigned int GetLogPriority();

protected:
    //! the state of the current simulation
    ESimState mState;

    //! log buffer
    static std::ostringstream* mLogStream;
};

#endif // SIMSPARK_H__
