#ifndef FILECLASSES_H__
#define FILECLASSES_H__

#include <stdio.h>
#include <stdlib.h>

namespace salt
{

//------------------------------------------------------------------------------------------------
// RFile
//------------------------------------------------------------------------------------------------
class RFile
{
public:
	virtual ~RFile(){};

	// a bunch of pure virtual functions which a file must implement
	virtual bool	Open(const char*fn=NULL, char*mode="rb") = 0;
	virtual void	Close() = 0;
	virtual void	Destroy() = 0;
	virtual int		Eof() = 0;
	virtual long	Tell() = 0;
	virtual int		GetPos(fpos_t *pos) = 0;
	virtual int		Seek(long offset, int origin) = 0;
	virtual void	Rewind() = 0;
	virtual long	Size() = 0;
	virtual char*	Gets(char*buffer,int n) = 0;
	virtual int		Getc() = 0;

	virtual void *	GetHandle() = 0;
	virtual size_t	Read(void *buffer,size_t size,size_t count) = 0;
	virtual size_t	Read(void *buffer,size_t bytes) = 0;

	// Get 2-byte int - Intel ordering
	int Igetw()
	{
		int b1, b2;

		if ((b1 = Getc()) != EOF)
			if ((b2 = Getc()) != EOF)
				return ((b2 << 8) | b1);
		return EOF;
	}
	
	// Get 4-byte int - Intel ordering
	long Igetl()
	{
		int b1, b2, b3, b4;

		if ((b1 = Getc()) != EOF)
			if ((b2 = Getc()) != EOF)
				if ((b3 = Getc()) != EOF)
					if ((b4 = Getc()) != EOF)
						return (((long)b4<<24) | ((long)b3<<16) | ((long)b2<<8) | (long)b1);
		return EOF;
	}
	
	// Get 2-byte int - Motorola ordering
	int Mgetw()
	{
		int b1, b2;

		if ((b1 = Getc()) != EOF)
			if ((b2 = Getc()) != EOF)
				return ((b1 << 8) | b2);
		return EOF;
	}

	// Get 4-byte int - Motorola ordering
	long Mgetl(){
		int b1, b2, b3, b4;

		if ((b1 = Getc()) != EOF)
			if ((b2 = Getc()) != EOF)
				if ((b3 = Getc()) != EOF)
					if ((b4 = Getc()) != EOF)
						return (((long)b1<<24) | ((long)b2<<16) | ((long)b3<<8) | (long)b4);
		return EOF;
	}
};

//------------------------------------------------------------------------------------------------
// MemFile
//------------------------------------------------------------------------------------------------
class MemFile : public RFile
{
public:
	MemFile(const char*fn=NULL, char*mode="rb");
	MemFile(FILE*f);
	MemFile(RFile *f);
	~MemFile();

	bool	Open(const char*fn=NULL, char*mode="rb");
	bool	Open(void*buffer, long s);
	void	Close();
	void	Destroy();

	int		Eof();
	long	Tell();
	int		GetPos(fpos_t *pos);
	int		Seek(long offset, int origin);
	void	Rewind();
	long	Size();

	char*	Gets(char*buffer,int n);
	int		Getc();

	void*	GetHandle()	{	return mHandle;	}

	size_t	Read(void *buffer,size_t size,size_t count);
	size_t	Read(void *buffer,size_t count)	{	return Read(buffer, 1,count);	}
private:
	void*			mHandle;
	unsigned char*	mCharHandle;
	long			mSize;
	long			mPosition;
};

//------------------------------------------------------------------------------------------------
// WFile
//------------------------------------------------------------------------------------------------

class WFile : public RFile
{
public:
	virtual ~WFile() {};

	virtual int	Puts(const char*s) = 0;
	virtual int	Putc(int c) = 0;

	// Write 2-byte int - Intel ordering
	int Iputw(int w)
	{
		int b1, b2;

		b1 = (w & 0xFF00) >> 8;
		b2 = w & 0x00FF;

		if (Putc(b2)==b2)
			if (Putc(b1)==b1)
				return w;
		return EOF;
	}
	
	// Write 4-byte int - Intel ordering
	long Iputl(long l)
	{
		int b1, b2, b3, b4;

		b1 = (int)((l & 0xFF000000L) >> 24);
		b2 = (int)((l & 0x00FF0000L) >> 16);
		b3 = (int)((l & 0x0000FF00L) >> 8);
		b4 = (int)l & 0x00FF;

		if (Putc(b4)==b4)
			if (Putc(b3)==b3)
				if (Putc(b2)==b2)
					if (Putc(b1)==b1)
						return l;
		return EOF;
	}
  
	// Write 2-byte int - Motorola ordering
	int Mputw(int w)
	{
		int b1, b2;

		b1 = (w & 0xFF00) >> 8;
		b2 = w & 0x00FF;

		if (Putc(b1)==b1)
			if (Putc(b2)==b2)
				return w;
		return EOF;
	}

	// Write 4-byte int - Motorola ordering
	long Mputl(long l)
	{
		int b1, b2, b3, b4;

		b1 = (int)((l & 0xFF000000L) >> 24);
		b2 = (int)((l & 0x00FF0000L) >> 16);
		b3 = (int)((l & 0x0000FF00L) >> 8);
		b4 = (int)l & 0x00FF;

		if (Putc(b1)==b1)
			if (Putc(b2)==b2)
				if (Putc(b3)==b3)
					if (Putc(b4)==b4)
						return l;
		return EOF;
	}

	virtual size_t	Write(void *buffer,size_t size,size_t count) = 0;
	virtual size_t	Write(void *buffer,size_t count) = 0;
	
	virtual void*	GetHandle() = 0;
};

//------------------------------------------------------------------------------------------------
// StdFile
//------------------------------------------------------------------------------------------------

class StdFile : public WFile
{
public:
	StdFile(FILE*f);
	StdFile(const char*fn=NULL, char*mode="rb");
	virtual ~StdFile();

	bool	Open(const char*fn=NULL, char*mode="rb");
	void	Close();
	void	Destroy();

	int		Eof();
	long	Tell();
	int		GetPos(fpos_t *pos);
	int		Seek(long offset, int origin);
	void	Rewind();
	long	Size();

	char*	Gets(char*buffer,int n);
	int		Getc();

	int		Puts(const char*s);
	int		Putc(int c);

	// Write 2-byte int - Intel ordering
	int Iputw(int w)
	{
		int b1, b2;

		b1 = (w & 0xFF00) >> 8;
		b2 = w & 0x00FF;

		if (Putc(b2)==b2)
			if (Putc(b1)==b1)
				return w;
		return EOF;
	}
	
	// Write 4-byte int - Intel ordering
	long Iputl(long l)
	{
		int b1, b2, b3, b4;

		b1 = (int)((l & 0xFF000000L) >> 24);
		b2 = (int)((l & 0x00FF0000L) >> 16);
		b3 = (int)((l & 0x0000FF00L) >> 8);
		b4 = (int)l & 0x00FF;

		if (Putc(b4)==b4)
			if (Putc(b3)==b3)
				if (Putc(b2)==b2)
					if (Putc(b1)==b1)
						return l;
		return EOF;
	}
  
	// Write 2-byte int - Motorola ordering
	int Mputw(int w)
	{
		int b1, b2;

		b1 = (w & 0xFF00) >> 8;
		b2 = w & 0x00FF;

		if (Putc(b1)==b1)
			if (Putc(b2)==b2)
				return w;
		return EOF;
	}

	// Write 4-byte int - Motorola ordering
	long Mputl(long l)
	{
		int b1, b2, b3, b4;

		b1 = (int)((l & 0xFF000000L) >> 24);
		b2 = (int)((l & 0x00FF0000L) >> 16);
		b3 = (int)((l & 0x0000FF00L) >> 8);
		b4 = (int)l & 0x00FF;

		if (Putc(b1)==b1)
			if (Putc(b2)==b2)
				if (Putc(b3)==b3)
					if (Putc(b4)==b4)
						return l;
		return EOF;
	}

	size_t	Read(void *buffer,size_t size,size_t count);
	size_t	Read(void *buffer,size_t count)	{	return Read(buffer,1,count);	}

	size_t	Write(void *buffer,size_t size,size_t count);
	size_t	Write(void *buffer,size_t count)	{	return Write(buffer,1,count);	}
	
	void*	GetHandle();
private:
	FILE	*mHandle;
};

} //namespace salt

#endif //FILECLASSES_H__
