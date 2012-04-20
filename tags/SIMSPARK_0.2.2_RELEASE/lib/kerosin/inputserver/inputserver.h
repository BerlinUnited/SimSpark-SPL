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
#ifndef KEROSIN_INPUTSERVER_H
#define KEROSIN_INPUTSERVER_H

#include <zeitgeist/node.h>
#include <kerosin/kerosin_defines.h>
#include "input.h"

namespace kerosin
{

class InputSystem;
class InputDevice;
class ScanCodeMap;

/*      \class InputServer
        InputServer

        The InputServer of Kerosin is basically the next generation of
        the inputserver which has been developed for AGSPT. It tries
        to do a few more things more cleanly than the InputServer in
        the AGSPT frame- work.

        One major feature is that the input server does not directly
        deal with the API used to handle inputs. This is now done via
        an input system.  This enables a pluggable solution for the
        inputsystem and its devices.

        Another novel approach is that the binding is done directly
        via the input server and not through the individual devices.

        NOTE:

        HISTORY:
                21.08.02 - MK
                        - Initial version

        TODO:

        TOFIX:
*/
class KEROSIN_API InputServer : public zeitgeist::Node
{
    //
    // Types
    //
private:
    typedef std::list<InputDevice*> TDeviceList;

public:
    /** this enumerates different filters that describing which button
        events are translated to user defined values (aka bindings).
    */
    enum EBindEvent
    {
        eKeyUp          = 1,    // the key-release event is bound
        eKeyDown        = 2,    // the key-pressed event is bound
        eKeyUpDown      = 3,    // both events are bound (==
                                // IDB_KEYUP|IDB_KEYDOWN)
    };

    /** this defines a bind, i.e. a mapping from an input (code) to an
        user specific command (cmd), along with a filter (see enum
        eBind)
    */
    struct Bind
    {
        int code;              // a value identifying the button or
                               // axis (see Input::TInputCodes)
        int cmd;               // the associated user defined value
        unsigned int modifier; // a bitmask of modifiers (see
                               // EModifiers)
        EBindEvent event;      // the used filter (see enum EBind)
    };

    typedef std::list<Bind> TBindList;

#ifdef HAVE_HASH_MAP
    typedef std::hash_map<int, TBindList>   TBindMap;
#else
    typedef std::map<int, TBindList>        TBindMap;
#endif

    //
    // Methods
    //
public:
    InputServer();
    ~InputServer();

    //! creates and registers the given InputSystem
    bool Init(const std::string &inputSysName);

    //! returns the current InputSystem in use
    boost::shared_ptr<InputSystem> GetInputSystem();

    //! delegates device creation to the active input system
    bool CreateDevice(const std::string &deviceName);

    /** this function resets the currently active inputsystem (read:
        destroys it)
     */
    void Reset();

    bool GetInput(Input &input, bool raw = false);

    /**  Bind() allows a user specified command id (cmd) to be bound
         to an input event described by a string (desc). Examples for
         desc:

      "b"             b pressed/released
      "+b"            b pressed
      "-b"            b released
      "minus"         - pressed/released
      "shift a"       a pressed/released while shift (left or right) is being held

      The basic syntax is: (modifier ' ')* ['+'|'-'] inputcode
    */
    bool BindCommand(const std::string &desc, int cmd);

    /** sets the name of the input mapping to be imported on init, the
        name of a ruby script is expected
    */
    void SetScanCodeMapping(const std::string &name);

    //! add a code to the scancode map
    void AddCode(Input::TInputCode ic, const std::string &name,
                 char noMod, char shiftMod, char altMod);

    /** convert an inputcode back into a displayable
        character. Untranslatable codes will return 0
    */
    bool TranslateCode(Input::TInputCode code, unsigned long state, char &ch) const;

    //! invoke a certain input event
    void Invoke(int cmd);

    /** returns a reference to the modifier state map */
    unsigned int& GetModifierState();

private:
    bool ParseBindDescription(Bind &bind, const std::string &desc);
    int ParseModifier(const std::string &modifier) const;

    //
    // Members
    //

    // this is the up-to-date state of the modifier keys
    unsigned int    mModifierState;

private:
    /** the name of the scan code script to be imported */
    std::string mScanCodeScript;

    /** the scan code mapping */
    boost::shared_ptr<ScanCodeMap>  mScanCodeMap;

    /** map of active bindings */
    TBindMap mBindings;
};

DECLARE_CLASS(InputServer);

} // kerosin

#endif //KEROSIN_INPUTSERVER_H

