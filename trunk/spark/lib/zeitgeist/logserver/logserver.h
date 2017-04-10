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

        LogServer


        HISTORY:
                22.08.2002 MK
                        - initial version - reclaimed from rcssserver3D :)

*/
#ifndef ZEITGEIST_LOGSERVER_H
#define ZEITGEIST_LOGSERVER_H

#include <iostream>
#include "../node.h"

namespace zeitgeist
{

class LogServerStreamBuf;

/** The log server is responsible for holding a bunch of ostreams,
    which data can be streamed to. It is THE logging facility used
    within zeitgeist.
*/
class ZEITGEIST_API LogServer : public Node , public std::ostream
{
    // types
    //
public:

    /** defines different priority levels assigned to a log message. The
        values are designed as a bitmap and can be combined, to trigger
        different filters */
    enum EPriorityLevel
    {
        eNone    = 0,
        eDebug   = 1,
        eNormal  = 2,
        eWarning = 4,
        eError   = 8,
        eAll     = 0xffffffff
    };

    //
    // functions
    //
public:
    /** constructs a logserver with a an internal stream buffer of size */
    LogServer(unsigned int size = 1024);
    virtual ~LogServer();

    /** adds a stream to the list of streams. First, it is checked if
        the stream is already in. If the stream is found, we only
        install a new priority mask, so no stream can be added twice.

        @param stream   the stream to add
        @param mask     the (new) priority mask for the stream
        @param syncStream if true, stream will be synced too
    */
    void AddStream(std::ostream *stream, unsigned int mask = eAll, bool syncStream = false);

    /** removes a stream from the list of streams.
        @param stream   the stream to remove
        @return         true if the stream was found (and thus removed)
    */
    bool RemoveStream(const std::ostream *stream);


    /** removes all streams from the list of streams */
    void RemoveAllStreams();

    /** sets the priority mask of a stream in the list.

    @param stream   the stream for which we want to set the priority mask
    @param mask     the new priority mask
    @return         true if the stream was found
    */
    bool SetPriorityMask(const std::ostream *stream, unsigned int mask);

    /** gets priority mask of a stream in the list.

    @param stream   the stream for which we want to set the priority mask
    @return         the priority mask; 0 if stream was not found
    */
    unsigned int GetPriorityMask(const std::ostream *stream) const;

    /** selects the priority for the messages to be written. It
        returns a reference to this logserver instance, allowing
        multiple priority changes in one stream expression,
        e.g. log << Priority(eNormal) << "normal msg" <<
        Priority(eDbug) << "debug msg"
    */
    LogServer& Priority(unsigned int prio);

    /** selects the debug priority and returns a reference to this
        logserver */
    LogServer& Debug()    { return Priority(eDebug); }

    /** selects the normal priority and returns a reference to this
        logserver */
    LogServer& Normal()   { return Priority(eNormal); }

    /** selects the warning priority and returns a reference to this
        logserver */
    LogServer& Warning()  { return Priority(eWarning); }

    /** selects the error priority and returns a reference to this
        logserver */
    LogServer& Error()    { return Priority(eError); }

    /** provides an printf-style interface. */
    void Printf(const char *inFormat, ...);

private:
    LogServer(const LogServer& obj);
    LogServer& operator=(const LogServer& obj);

    const LogServerStreamBuf& GetStreamBuf() const;
    LogServerStreamBuf& GetStreamBuf();
};

DECLARE_CLASS(LogServer);

} //namespace zeitgeist

#endif //ZEITGEIST_LOGSERVER_H
