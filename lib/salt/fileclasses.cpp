#include "fileclasses.h"
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "vector.h"
//------------------------------------------------------------------------------------------------
// MemFile
//------------------------------------------------------------------------------------------------

namespace salt
{
static long fsize(FILE *f)
{
	long savepos, end;
	
	savepos = ftell(f);
	fseek(f, 0, SEEK_END);
	end = ftell(f);
	fseek(f, savepos, SEEK_SET);

	Vector3f a;

	
	return end;
}
}

using namespace salt;

MemFile::MemFile(const char *fn, char *mode)
{
	if(fn==NULL)
	{
		mHandle		= NULL;
		mCharHandle	= NULL;
		mSize		= 0;
		mPosition	= 0;
	}
	else
	{
		Open(fn, mode);
	}
}

MemFile::MemFile(FILE *f)
{
	mSize		= fsize(f);
	mHandle		= new unsigned char[mSize];
	mCharHandle	= (unsigned char*)mHandle;
	mPosition	= 0;
	fread(mHandle,1,mSize,f);
}

MemFile::MemFile(RFile *f)
{
	mSize=f->Size();
	mHandle=new unsigned char[mSize];
	mCharHandle=(unsigned char*)mHandle;
	f->Read(mHandle,mSize);
	mPosition=0;
}

MemFile::~MemFile()
{
	if(mHandle!=NULL)
		Destroy();
}

bool MemFile::Open(const char *fn, char *mode)
{
	if(fn==NULL)
		return false;
	else
	{
		StdFile* f = new StdFile(fn, mode);
		if(f->GetHandle())
		{
			mSize		= f->Size();
			mCharHandle	= new unsigned char[mSize];
			mHandle		= mCharHandle;
			mPosition	= 0;
			f->Read(mHandle, mSize);
			delete f;
			return true;
		}
		else
		{
			delete f;
			return false;
		}
	}
}

bool MemFile::Open(void *buffer, long s)
{
	mHandle		= buffer;
	mCharHandle	= (unsigned char*)mHandle;
	mSize		= s;
	mPosition	= 0;
	return true;
}

void MemFile::Close()
{
	Destroy();
}

void MemFile::Destroy()
{
	if(mHandle!=NULL)
	{
		delete []mCharHandle;
		mHandle		= NULL;
		mCharHandle	= NULL;
		mSize		= 0;
		mPosition	= 0;
	}
}

int MemFile::Eof()
{
	return (mPosition>=mSize);
}

long MemFile::Tell()
{
	return mPosition;
}

int MemFile::GetPos(fpos_t *pos)
{
	*pos=Tell();
	return 1;
}

int MemFile::Seek(long offset, int origin)
{
	switch(origin){
		case SEEK_SET:
			return (mPosition=offset);
			break;
		case SEEK_END:
			return (mPosition=mSize-1-offset);
			break;
		default:
			return (mPosition+=offset);
			break;
	}
}

void MemFile::Rewind()
{
	mPosition=0;
}

long MemFile::Size()
{
	return mSize;
}

char* MemFile::Gets(char* buffer, int n)
{
	int i		= 0;
	bool done	= false;

	if(Eof()) return NULL;

	while(!done)
	{
		buffer[i]=mCharHandle[mPosition];
		mPosition++;
		i++;
		if((i==n)||(buffer[i-1]=='\n')||(Eof())) done=true;
	}
	buffer[i]=0;
	return buffer;
}

int MemFile::Getc()
{
	mPosition++;
	if((mPosition-1)>=mSize)
		return EOF;
	else
		return (int)(mCharHandle[(mPosition-1)]);
}

size_t MemFile::Read(void *buffer, size_t size, size_t count)
{
	long oldPos = mPosition;

	mPosition += (size*count);
	if(mPosition >= mSize)
	{
		memcpy(buffer, &mCharHandle[oldPos], (mSize-oldPos));
		return (mSize-oldPos);
	}
	else
	{
		memcpy(buffer, &mCharHandle[oldPos], size*count);
		return (size*count);
	}
}

//------------------------------------------------------------------------------------------------
// StdFile
//------------------------------------------------------------------------------------------------

StdFile::StdFile(FILE* h)
{
	//Make sure object is free
	Destroy();
	//Assign new mHandle
	mHandle=h;
}

StdFile::StdFile(const char* fn,char* mode)
{
	mHandle=NULL;
	//open file
	Open(fn,mode);
}

StdFile::~StdFile()
{
	//Free up memory
	Destroy();
}

void StdFile::Destroy()
{
	//Close file
	Close();
}

void StdFile::Close()
{
	//Close file
	if(mHandle!=NULL)
	{
		fclose(mHandle);
		mHandle = NULL;
	}
}

bool StdFile::Open(const char* fileName,char* mode)
{
	//Open fn with mode mode
	if(fileName==NULL)
		return false;
	else
	{
		mHandle=fopen(fileName, mode);
		if(mHandle==NULL)
			return false;
		else
			return true;
	}
}

int StdFile::Eof()
{
	return feof(mHandle);
}

long StdFile::Tell()
{
	return ftell(mHandle);
}

long StdFile::Size()
{
	return fsize(mHandle);
}

int StdFile::GetPos(fpos_t* pos)
{
	return fgetpos(mHandle, pos);
}

void StdFile::Rewind()
{
	rewind(mHandle);
}

char* StdFile::Gets(char* buffer, int n)
{
	return fgets(buffer, n, mHandle);
}

int StdFile::Getc()
{
	return fgetc(mHandle);
}

int StdFile::Puts(const char*s)
{
	return fputs(s, mHandle);
}

int StdFile::Putc(int c)
{
	return fputc(c, mHandle);
}

int StdFile::Seek(long offset, int origin)
{
	return fseek(mHandle, offset, origin);
}

size_t StdFile::Read(void *buffer,size_t mSize,size_t count)
{
	return fread(buffer, mSize, count, mHandle);
} 

size_t StdFile::Write(void *buffer,size_t mSize,size_t count)
{
	return fwrite(buffer, mSize, count, mHandle);
}

void* StdFile::GetHandle()
{
	return (void*) mHandle;
}
