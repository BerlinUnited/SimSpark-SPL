#include "scancodemap.h"

using namespace kerosin;

ScanCodeMap::ScanCodeMap()
{
}

ScanCodeMap::~ScanCodeMap()
{
}

kerosin::InputServer::TInputCode ScanCodeMap::GetCode(const std::string &name) const
{
	// loop through all entries in the map and find the one, which matches the symbolic name
	for (TScanCodeEntryMap::const_iterator i = mScanCodes.begin(); i != mScanCodes.end(); ++i)
	{
		if ((*i).second->name == name) return (*i).first;
	}

	return 0;
}

bool ScanCodeMap::TranslateCode(kerosin::InputServer::TInputCode code, unsigned long state, char &ch) const
{
	if ((state & InputServer::eShift) && (state & InputServer::eAlt)) return false;

	TScanCodeEntryMap::const_iterator entry = mScanCodes.find(code);

	if (entry != mScanCodes.end())
	{
		if (state & InputServer::eShift)
		{
			ch = (*entry).second->shiftModifier;
			return ch != 0;
		}
		if (state & InputServer::eAlt)
		{
			ch = (*entry).second->altModifier;
			return ch != 0;
		}
		ch = (*entry).second->noModifier;
		return ch != 0;
	}

	return false;
}

void ScanCodeMap::Reset()
{
	for (TScanCodeEntryMap::iterator i = mScanCodes.begin(); i != mScanCodes.end(); ++i)
	{
		delete (*i).second;
	}
}

void ScanCodeMap::AddCode(InputServer::TInputCode ic, const std::string &name, char noMod, char shiftMod, char altMod)
{
	TScanCodeEntry *entry = new TScanCodeEntry();
	entry->Set(name, noMod, shiftMod, altMod);
	mScanCodes[ic] = entry;
}
