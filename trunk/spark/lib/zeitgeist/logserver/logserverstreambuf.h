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
#ifndef ZEITGEIST_LOGSERVERSTREAMBUF_H
#define ZEITGEIST_LOGSERVERSTREAMBUF_H

/*! \class LogServerStreamBuf
  $Id$

  ForwarderStreamBuf

  The ForwarderStreamBuf is a special std::streambuf derived class, which can
  stream to a list of streams. In addition to the multiplexing functionality
  this class also supports priorization of streamed data. Every registered
  stream is associated with a priority mask. This is a bitfield mask which
  tells the system which kind of messages should be muxed to a particular
  stream. The ForwarderStreamBuf also manages the current priority level to
  be used for incoming data. If the state is changed, the buffers are flushed!

  HISTORY:
  The forwarder subsystem was taken from a student project at the AI
  Research Group, Koblenz University. Original development by
  Marco Koegler <koegler@uni-koblenz.de>,
  Marcus Rollmann <rollmark@uni-koblenz.de>,
  Alexander Fuchs <alexf@uni-koblenz.de>,
  et.al.
  It was built into the rcssserver3D, and then converted back into the diploma
  thesis of Marco Koegler, which was the base for rcssserver3D.
*/
#include <functional>
#include <vector>
#include <streambuf>

namespace zeitgeist
{

class LogServerStreamBuf : public std::streambuf
{
    struct MaskStream
    {
        MaskStream(unsigned int mask, std::ostream* stream, bool sync) :
            mMask(mask), mStream(stream), mSync(sync) {};
        MaskStream(const MaskStream& obj) :
            mMask(obj.mMask), mStream(obj.mStream), mSync(obj.mSync) {};
            MaskStream& operator=(const MaskStream& obj){mMask = obj.mMask; mStream = obj.mStream; mSync = obj.mSync; return *this;}

        unsigned int mMask;
        std::ostream* mStream;
        bool mSync;
    };

    // types
    //
protected:
    typedef std::char_traits<char>      TTraitsType;
    typedef traits_type::int_type       TIntType;

private:
    //typedef std::pair<unsigned int, std::ostream*> TMaskStream;
    typedef std::vector<MaskStream> TMaskStreams;

    // functions
    //
public:
    LogServerStreamBuf(unsigned int size);
    virtual ~LogServerStreamBuf();

    /*! Add a stream to the list of streams.

        First, it is checked if the stream is already in. If the
        stream is found, we only add a new priority mask to the
        existing ones, so no stream can be added twice.

        @param stream   the stream to add
        @param mask     the (new) priority mask for the stream
    */
    void AddStream(std::ostream *stream, unsigned int mask, bool syncStream = false);

    /*! Remove a stream from the list of streams.
        @param stream   the stream to remove
        @return         true if the stream was found (and thus removed)
    */
    bool RemoveStream(const std::ostream *stream);

    /*! Remove all streams from the list of streams */
    void RemoveAllStreams();

    /*! Set priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @param mask     the new priority mask
        @return         true if the stream was found
    */
    bool SetPriorityMask(const std::ostream *stream, unsigned int mask);

    /*! Get priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @return         the priority mask; 0 if stream was not found
    */
    unsigned int GetPriorityMask(const std::ostream *stream) const;

    /*! Set the current priority level.

        All data which is streamed into the forwarder after this point
        will use the current priority level. Before the priority level
        is adjusted, all buffered data is flushed.

        @param priority current priority level (see EPriorityLevel)
    */
    void SetCurrentPriority(unsigned int priority);

    void SyncStreams();

protected:
    // these functions implement the streambuf interface ... handle with care
    TIntType    overflow(TIntType c);
    int         sync();

private:
    LogServerStreamBuf(const LogServerStreamBuf &obj);
    LogServerStreamBuf& operator=(const LogServerStreamBuf &obj);

    //! multiplex to all registered streams
    void Forward(const char *buffer, unsigned int length);
    //! stream out complete internal buffer
    void PutBuffer();
    //! stream out a single character
    void PutChar(TIntType chr);

    //! A predicate to compare streams in a MaskStream list (or vector).
    class MaskStreamEQ : public std::unary_function<MaskStream, bool>
    {
    private:
        const std::ostream *stream;
    public:
        explicit MaskStreamEQ(const std::ostream *str) : stream(str) {}
        bool operator ()(const MaskStream &ms)
        { return ms.mStream == stream; }
    };

    // members
    //
private:
    //! size of the internal buffer to use
    unsigned int    mSize;
    unsigned int    mCurrentPriority;
    TMaskStreams     mStreams;
};

} //namespace

#endif // ZEITGEIST_LOGSERVERSTREAMBUF_H
