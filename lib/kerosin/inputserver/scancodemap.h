#ifndef SCANCODEMAP_H__
#define SCANCODEMAP_H__

/*	\class ScanCodeMap
	$Id: scancodemap.h,v 1.2 2003/04/30 14:21:28 fruit Exp $

	ScanCodeMap

	This object can be used to translate from characters to scancodes and vice
	versa. It is necessary to allow for country specific keyboard layouts.

	NOTE:

	HISTORY:
		30.09.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <map>
#include "inputserver.h"

namespace kerosin
{

class ScanCodeMap
{
	//
	// types
	//	
public:
	// character representations of the scancode this struct is associated with
	// the altModifier is only really used on German keyboards
	struct TScanCodeEntry
	{
		std::string name;			// name of the key
		char		noModifier;		// no modifiers		'<'
		char		shiftModifier;	// shift modifier	'>'
		char		altModifier;	// alt modifier		'|'

		void Set(const std::string &keyName, char noMod, char shiftMod, char altMod)
		{
			name = keyName;
			noModifier = noMod;
			shiftModifier = shiftMod;
			altModifier = altMod;
		}
	};
protected:
	typedef std::map<kerosin::InputServer::TInputCode, TScanCodeEntry*>	TScanCodeEntryMap;
	//
	// functions
	//	
public:
	ScanCodeMap();
	~ScanCodeMap();

	InputServer::TInputCode	GetCode(const std::string &name) const;
	bool					TranslateCode(InputServer::TInputCode code, unsigned long state, char &ch) const;

	void					Reset();
	void					AddCode(InputServer::TInputCode ic, const std::string &name, char noMod, char shiftMod, char altMod);
protected:
	//! this is the map which the translation process is based on
	TScanCodeEntryMap	mScanCodes;
};

} //namespace kerosin

#endif //SCANCODEMAP_H__