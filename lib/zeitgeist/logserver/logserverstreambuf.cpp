#include <algorithm>
#include <iostream>
#include "logserverstreambuf.h"

using namespace std;
using namespace zeitgeist;

LogServerStreamBuf::LogServerStreamBuf(unsigned int size) :
streambuf(), mSize(size), mCurrentPriority(0xffffffff)
{
    // Here we set up the 'put' area, where data is streamed in
    if (mSize)
    {
        char* ptr = new char[mSize];
        setp(ptr, ptr + mSize);
    }
    else
    {
        setp(0, 0);
    }

    // we don't support a 'get' area ... no istream support
    setg(0, 0, 0);
}

LogServerStreamBuf::~LogServerStreamBuf()
{
    // flush buffer
    sync();

    // delete mask-stream elements. The streams will not be deleted.
    while (mStreams.size() != 0)
    {
                if (mStreams.back().second != &std::cout && mStreams.back().second != &std::cerr)
                        delete mStreams.back().second;
        mStreams.pop_back();
    }

    // delete buffer
    delete[] pbase();
}

void LogServerStreamBuf::AddStream(std::ostream *stream, unsigned int mask)
{
    TMaskStreams::iterator i;
    i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

    if (i == mStreams.end())
    {
        TMaskStream pstream(mask, stream);
        mStreams.push_back(pstream);
    }   
    else
    {
        i->first |= mask;
    }
}

bool LogServerStreamBuf::RemoveStream(const std::ostream *stream)
{
    // flush buffer
    sync();

        TMaskStreams::iterator i;
        i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

        if (i != mStreams.end())
        {
                mStreams.erase(i);
                return true;
        }
        return false;
}

bool
LogServerStreamBuf::SetPriorityMask(const std::ostream *stream, unsigned int mask)
{
        // flush buffer
        sync();

        TMaskStreams::iterator i;
        i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

        if (i != mStreams.end())
        {
                i->first = mask;
                return true;
        }
        return false;
}

unsigned int LogServerStreamBuf::GetPriorityMask(const std::ostream *stream) const
{
        TMaskStreams::const_iterator i;
        i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

        if (i != mStreams.end())
        {
                return i->first;
        }
        return 0;
}

void LogServerStreamBuf::SetCurrentPriority(unsigned int priority)
{
    sync();
    mCurrentPriority = priority;
}

/*!
    This routine is called by the iostream library if our internal buffer is
    overflowing (the put area is full).
*/
LogServerStreamBuf::TIntType LogServerStreamBuf::overflow(TIntType c)
{
    // write out the buffered content
    PutBuffer();

    // handle the extra character if it isn't eof
    if (c != TTraitsType::eof())
    {
        // if we don't do buffering
        if (pbase() == epptr())
        {
            // write out the character directly
            PutChar(c);
        }
        else
        {
            // buffer it
            sputc(c);
        }
    }
    return 0;
}

/*!
    This routine synchronizes the internal state with the external state. It is
    used to flush the streambuf object.
*/
int LogServerStreamBuf::sync()
{
        PutBuffer();

        return 0;
}

void LogServerStreamBuf::Forward(const char *buffer, unsigned int length)
{
        TMaskStreams::iterator i;
        for (i=mStreams.begin(); i!= mStreams.end(); ++i)
        {
                if ((*i).first & mCurrentPriority)
                {
                        (*i).second->write(buffer, length);
                }
        }
}

void LogServerStreamBuf::PutBuffer()
{
        // if we have data to stream out
        if (pbase() != pptr())
        {
                int len = (pptr() - pbase());

                // pbase() = buffer address
                Forward(pbase(), len);

                // reset pointers == put area is empty
                setp(pbase(), epptr());
        }
}

void
LogServerStreamBuf::PutChar(TIntType chr)
{
        char a[1];
        a[0] = chr;
        Forward(a, 1);
}
