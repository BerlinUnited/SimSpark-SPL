#ifndef LOGSERVERSTREAMBUF_H__
#define LOGSERVERSTREAMBUF_H__

/*! \class LogServerStreamBuf
  $Id: logserverstreambuf.h,v 1.1 2003/04/30 11:29:35 fruit Exp $

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
  It was built into the rcssserver3D, and then converted back into this diploma
  thesis.
*/

#include <functional>
#include <vector>
#include <streambuf>

namespace zeitgeist
{

class LogServerStreamBuf : public std::streambuf
{
    // types
    //
protected:
    typedef std::char_traits<char>	TTraitsType;
    typedef traits_type::int_type	TIntType;

private:
    typedef std::pair<unsigned int, std::ostream*>	TMaskStream;
    typedef std::vector<TMaskStream>				TMaskStreams;

    // functions
    //
public:
    LogServerStreamBuf(unsigned int size);
    virtual ~LogServerStreamBuf();

    /*! Add a stream to the list of streams.

        First, it is checked if the stream is already in. If the stream
        is found, we only install a new priority mask, so no stream can
        be added twice.

        @param stream   the stream to add
        @param mask     the (new) priority mask for the stream
    */
    void AddStream(std::ostream *stream, unsigned int mask);
    /*! Remove a stream from the list of streams.
        @param stream   the stream to remove
        @return         true if the stream was found (and thus removed)
    */
    bool RemoveStream(const std::ostream *stream);
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

protected:
    // these functions implement the streambuf interface ... handle with care
    TIntType	overflow(TIntType c);
    int			sync();

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
    class MaskStreamEQ : public std::unary_function<TMaskStream, bool>
    {
    private:
        const std::ostream	*stream;
    public:
        explicit MaskStreamEQ(const std::ostream *str) : stream(str) {}
        bool operator ()(const TMaskStream &ms)
        { return ms.second == stream; }
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

#endif // LOGSERVERSTREAMBUF_H
