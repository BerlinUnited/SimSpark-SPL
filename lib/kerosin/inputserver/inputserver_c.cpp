#include "inputserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(init)
{
	if (in.size() == 1)
	{
		InputServer *is = static_cast<InputServer*>(obj);
		is->Init(any_cast<char*>(in[0]));
	}
}

FUNCTION(createDevice)
{
	if (in.size() == 1)
	{
		InputServer *is = static_cast<InputServer*>(obj);
		is->CreateDevice(any_cast<char*>(in[0]));
	}
}

FUNCTION(reset)
{
	if (in.size() == 0)
	{
		InputServer *is = static_cast<InputServer*>(obj);
		is->Reset();
	}
}

FUNCTION(importScanCodeMapping)
{
	if (in.size() == 1)
	{
		InputServer *is = static_cast<InputServer*>(obj);
		is->ImportScanCodeMapping(any_cast<char*>(in[0]));
	}
}

FUNCTION(addCode)
{
	if (in.size() == 5)
	{
		InputServer *is = static_cast<InputServer*>(obj);
		is->AddCode(any_cast<int>(in[0]), any_cast<char*>(in[1]), any_cast<int>(in[2]), any_cast<int>(in[3]), any_cast<int>(in[4]));
	}
}

void CLASS(InputServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Node);
	DEFINE_FUNCTION(init);
	DEFINE_FUNCTION(createDevice);
	DEFINE_FUNCTION(reset);
	DEFINE_FUNCTION(importScanCodeMapping);
	DEFINE_FUNCTION(addCode);
}
