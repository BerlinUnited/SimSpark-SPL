#include <boost/tokenizer.hpp>
#include "path.h"

using namespace std;
using namespace salt;

Path::Path(const std::string &path)
{
	Set(path);
}

void Path::Set(const std::string &path)
{
	if (path[0] == '/' || path[0] == '\\')
	{
		mIsAbsolute = true;
	}
	else
	{
		mIsAbsolute = false;
	}

	Tokenize(path);
}

std::string Path::GetCleanPath(const std::string &sep) const
{
	string path;

	if (mIsAbsolute)
	{
		path += sep;
	}

	unsigned int count = 0;
	for (TStringList::const_iterator i = mPathComponents.begin(); i != mPathComponents.end(); ++i)
	{
		path += (*i);
		++count;
		if ( count != mPathComponents.size() )
		{
			path += sep;
		}
	}

	return path;
}

bool Path::IsAbsolute() const
{
	return mIsAbsolute;
}

bool Path::IsEmpty() const
{
	return mPathComponents.empty();
}

const std::string& Path::Front() const
{
	return mPathComponents.front();
}

void Path::PopFront()
{
	mIsAbsolute = false;
	mPathComponents.pop_front();
}

const std::string& Path::Back() const
{
	return mPathComponents.back();
}

void Path::PopBack()
{
	mPathComponents.pop_back();
}

void Path::Tokenize(const std::string &path)
{
	typedef boost::tokenizer<boost::char_separator<char> > TTokenizer;
	boost::char_separator<char> sep("/\\");
	TTokenizer	tokens(path, sep);

	for (TTokenizer::iterator i = tokens.begin(); i != tokens.end(); ++i)
	{
		mPathComponents.push_back(*i);
	}
}
