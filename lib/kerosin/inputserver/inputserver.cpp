/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: inputserver.cpp,v 1.6 2004/04/11 11:22:56 rollmark Exp $

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
#include "inputserver.h"
#include "inputsystem.h"
#include "inputdevice.h"
#include "scancodemap.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <sstream>

using namespace boost;
using namespace kerosin;
using namespace std;
using namespace zeitgeist;

const InputServer::TInputCode InputServer::IC_1 = 0x02;
const InputServer::TInputCode InputServer::IC_2 = 0x03;
const InputServer::TInputCode InputServer::IC_3 = 0x04;
const InputServer::TInputCode InputServer::IC_4 = 0x05;
const InputServer::TInputCode InputServer::IC_5 = 0x06;
const InputServer::TInputCode InputServer::IC_6 = 0x07;
const InputServer::TInputCode InputServer::IC_7 = 0x08;
const InputServer::TInputCode InputServer::IC_8 = 0x09;
const InputServer::TInputCode InputServer::IC_9 = 0x0a;
const InputServer::TInputCode InputServer::IC_0 = 0x0b;

// function keys
const InputServer::TInputCode InputServer::IC_F1 = 0x3b;
const InputServer::TInputCode InputServer::IC_F2 = 0x3c;
const InputServer::TInputCode InputServer::IC_F3 = 0x3d;
const InputServer::TInputCode InputServer::IC_F4 = 0x3e;
const InputServer::TInputCode InputServer::IC_F5 = 0x3f;
const InputServer::TInputCode InputServer::IC_F6 = 0x40;
const InputServer::TInputCode InputServer::IC_F7 = 0x41;
const InputServer::TInputCode InputServer::IC_F8 = 0x42;
const InputServer::TInputCode InputServer::IC_F9 = 0x43;
const InputServer::TInputCode InputServer::IC_F10 = 0x44;
const InputServer::TInputCode InputServer::IC_F11 = 0x57;
const InputServer::TInputCode InputServer::IC_F12 = 0x58;

// alphabet
const InputServer::TInputCode InputServer::IC_A = 0x1e;
const InputServer::TInputCode InputServer::IC_B = 0x30;
const InputServer::TInputCode InputServer::IC_C = 0x2e;
const InputServer::TInputCode InputServer::IC_D = 0x20;
const InputServer::TInputCode InputServer::IC_E = 0x12;
const InputServer::TInputCode InputServer::IC_F = 0x21;
const InputServer::TInputCode InputServer::IC_G = 0x22;
const InputServer::TInputCode InputServer::IC_H = 0x23;
const InputServer::TInputCode InputServer::IC_I = 0x17;
const InputServer::TInputCode InputServer::IC_J = 0x24;
const InputServer::TInputCode InputServer::IC_K = 0x25;
const InputServer::TInputCode InputServer::IC_L = 0x26;
const InputServer::TInputCode InputServer::IC_M = 0x32;
const InputServer::TInputCode InputServer::IC_N = 0x31;
const InputServer::TInputCode InputServer::IC_O = 0x18;
const InputServer::TInputCode InputServer::IC_P = 0x19;
const InputServer::TInputCode InputServer::IC_Q = 0x10;
const InputServer::TInputCode InputServer::IC_R = 0x13;
const InputServer::TInputCode InputServer::IC_S = 0x1f;
const InputServer::TInputCode InputServer::IC_T = 0x14;
const InputServer::TInputCode InputServer::IC_U = 0x16;
const InputServer::TInputCode InputServer::IC_V = 0x2f;
const InputServer::TInputCode InputServer::IC_W = 0x11;
const InputServer::TInputCode InputServer::IC_X = 0x2d;
const InputServer::TInputCode InputServer::IC_Y = 0x15;
const InputServer::TInputCode InputServer::IC_Z = 0x2c;

// keypad
const InputServer::TInputCode InputServer::IC_KP0 = 0x52;
const InputServer::TInputCode InputServer::IC_KP1 = 0x4f;
const InputServer::TInputCode InputServer::IC_KP2 = 0x50;
const InputServer::TInputCode InputServer::IC_KP3 = 0x51;
const InputServer::TInputCode InputServer::IC_KP4 = 0x4b;
const InputServer::TInputCode InputServer::IC_KP5 = 0x4c;
const InputServer::TInputCode InputServer::IC_KP6 = 0x4d;
const InputServer::TInputCode InputServer::IC_KP7 = 0x47;
const InputServer::TInputCode InputServer::IC_KP8 = 0x48;
const InputServer::TInputCode InputServer::IC_KP9 = 0x49;
const InputServer::TInputCode InputServer::IC_KP_DECIMAL  = 0x53;
const InputServer::TInputCode InputServer::IC_KP_DIVIDE   = 0xb5;
const InputServer::TInputCode InputServer::IC_KP_MULTIPLY = 0x37;
const InputServer::TInputCode InputServer::IC_KP_MINUS    = 0x4a;
const InputServer::TInputCode InputServer::IC_KP_PLUS     = 0x4e;
const InputServer::TInputCode InputServer::IC_KP_ENTER    = 0x8d;

// arrows + home/end pad
const InputServer::TInputCode InputServer::IC_UP        = 0xc8;
const InputServer::TInputCode InputServer::IC_DOWN      = 0xd0;
const InputServer::TInputCode InputServer::IC_LEFT      = 0xcb;
const InputServer::TInputCode InputServer::IC_RIGHT     = 0xcd;
const InputServer::TInputCode InputServer::IC_INSERT    = 0xd2;
const InputServer::TInputCode InputServer::IC_DELETE    = 0xd3;
const InputServer::TInputCode InputServer::IC_HOME      = 0xc7;
const InputServer::TInputCode InputServer::IC_END       = 0xcf;
const InputServer::TInputCode InputServer::IC_PAGEUP    = 0xc9;
const InputServer::TInputCode InputServer::IC_PAGEDOWN  = 0xd1;

// key state modifier keys
const InputServer::TInputCode InputServer::IC_NUMLOCK   = 0x45;
const InputServer::TInputCode InputServer::IC_CAPSLOCK  = 0x3a;
const InputServer::TInputCode InputServer::IC_SCROLLOCK = 0x46;
const InputServer::TInputCode InputServer::IC_LSHIFT    = 0x2a;
const InputServer::TInputCode InputServer::IC_RSHIFT    = 0x36;
const InputServer::TInputCode InputServer::IC_LCTRL     = 0x1d;
const InputServer::TInputCode InputServer::IC_RCTRL     = 0x9d;
const InputServer::TInputCode InputServer::IC_LALT      = 0x38;
const InputServer::TInputCode InputServer::IC_RALT      = 0xb8;
const InputServer::TInputCode InputServer::IC_LSUPER    = 0xdb; // Left "Windows" key
const InputServer::TInputCode InputServer::IC_RSUPER    = 0xdc; // Right "Windows" key

// other keys (cursor control, punctuation)
const InputServer::TInputCode InputServer::IC_ESCAPE    = 0x01;
const InputServer::TInputCode InputServer::IC_PRINT     = 0xb7;
const InputServer::TInputCode InputServer::IC_PAUSE     = 0xc5;
const InputServer::TInputCode InputServer::IC_GRAVE     = 0x29;
const InputServer::TInputCode InputServer::IC_MINUS     = 0x0c;
const InputServer::TInputCode InputServer::IC_EQUALS    = 0x0d;
const InputServer::TInputCode InputServer::IC_BACKSLASH = 0x2b;
const InputServer::TInputCode InputServer::IC_BACKSPACE = 0x0e;

const InputServer::TInputCode InputServer::IC_TAB       = 0x0f;
const InputServer::TInputCode InputServer::IC_LBRACKET  = 0x1a;
const InputServer::TInputCode InputServer::IC_RBRACKET  = 0x1b;
const InputServer::TInputCode InputServer::IC_RETURN    = 0x1c;

const InputServer::TInputCode InputServer::IC_SEMICOLON = 0x27;
const InputServer::TInputCode InputServer::IC_APOSTROPHE= 0x28;

const InputServer::TInputCode InputServer::IC_OEM_102   = 0x56; // German <>|
const InputServer::TInputCode InputServer::IC_COMMA     = 0x33;
const InputServer::TInputCode InputServer::IC_PERIOD    = 0x34;
const InputServer::TInputCode InputServer::IC_SLASH     = 0x35;

const InputServer::TInputCode InputServer::IC_SPACE     = 0x39;

// mouse buttons
const InputServer::TInputCode InputServer::IC_MOUSE_LEFT        = 0x1000;       // left
const InputServer::TInputCode InputServer::IC_MOUSE_RIGHT       = 0x1001;       // right
const InputServer::TInputCode InputServer::IC_MOUSE_MIDDLE      = 0x1002;       // middle

//mouse axis
const InputServer::TInputCode InputServer::IC_AXISX     = 0x2000;
const InputServer::TInputCode InputServer::IC_AXISY     = 0x2001;
const InputServer::TInputCode InputServer::IC_AXISZ     = 0x2002;

// timer
const InputServer::TInputCode InputServer::IC_AXIST = 0x3000;

InputServer::InputServer() :
    Node(), mModifierState(eNone), mScanCodeMap(new ScanCodeMap())
{
    // default to a german keyboard layout
    mScanCodeScript = "german.scan.rb";
}

InputServer::~InputServer()
{
    Reset();
}

bool InputServer::Init(const std::string &inputSysName)
{
    GetLog()->Normal() << "(InputServer) Init " << inputSysName << "\n";
    Reset();

    // push our variables into the scriptserver
    GetScript()->CreateVariable("Input.IC_1", IC_1);
    GetScript()->CreateVariable("Input.IC_2", IC_2);
    GetScript()->CreateVariable("Input.IC_3", IC_3);
    GetScript()->CreateVariable("Input.IC_4", IC_4);
    GetScript()->CreateVariable("Input.IC_5", IC_5);
    GetScript()->CreateVariable("Input.IC_6", IC_6);
    GetScript()->CreateVariable("Input.IC_7", IC_7);
    GetScript()->CreateVariable("Input.IC_8", IC_8);
    GetScript()->CreateVariable("Input.IC_9", IC_9);
    GetScript()->CreateVariable("Input.IC_0", IC_0);

    // function keys
    GetScript()->CreateVariable("Input.IC_F1", IC_F1);
    GetScript()->CreateVariable("Input.IC_F2", IC_F2);
    GetScript()->CreateVariable("Input.IC_F3", IC_F3);
    GetScript()->CreateVariable("Input.IC_F4", IC_F4);
    GetScript()->CreateVariable("Input.IC_F5", IC_F5);
    GetScript()->CreateVariable("Input.IC_F6", IC_F6);
    GetScript()->CreateVariable("Input.IC_F7", IC_F7);
    GetScript()->CreateVariable("Input.IC_F8", IC_F8);
    GetScript()->CreateVariable("Input.IC_F9", IC_F9);
    GetScript()->CreateVariable("Input.IC_F10", IC_F10);
    GetScript()->CreateVariable("Input.IC_F11", IC_F11);
    GetScript()->CreateVariable("Input.IC_F12", IC_F12);

    // alphabet
    GetScript()->CreateVariable("Input.IC_A", IC_A);
    GetScript()->CreateVariable("Input.IC_B", IC_B);
    GetScript()->CreateVariable("Input.IC_C", IC_C);
    GetScript()->CreateVariable("Input.IC_D", IC_D);
    GetScript()->CreateVariable("Input.IC_E", IC_E);
    GetScript()->CreateVariable("Input.IC_F", IC_F);
    GetScript()->CreateVariable("Input.IC_G", IC_G);
    GetScript()->CreateVariable("Input.IC_H", IC_H);
    GetScript()->CreateVariable("Input.IC_I", IC_I);
    GetScript()->CreateVariable("Input.IC_J", IC_J);
    GetScript()->CreateVariable("Input.IC_K", IC_K);
    GetScript()->CreateVariable("Input.IC_L", IC_L);
    GetScript()->CreateVariable("Input.IC_M", IC_M);
    GetScript()->CreateVariable("Input.IC_N", IC_N);
    GetScript()->CreateVariable("Input.IC_O", IC_O);
    GetScript()->CreateVariable("Input.IC_P", IC_P);
    GetScript()->CreateVariable("Input.IC_Q", IC_Q);
    GetScript()->CreateVariable("Input.IC_R", IC_R);
    GetScript()->CreateVariable("Input.IC_S", IC_S);
    GetScript()->CreateVariable("Input.IC_T", IC_T);
    GetScript()->CreateVariable("Input.IC_U", IC_U);
    GetScript()->CreateVariable("Input.IC_V", IC_V);
    GetScript()->CreateVariable("Input.IC_W", IC_W);
    GetScript()->CreateVariable("Input.IC_X", IC_X);
    GetScript()->CreateVariable("Input.IC_Y", IC_Y);
    GetScript()->CreateVariable("Input.IC_Z", IC_Z);

    // keypad
    GetScript()->CreateVariable("Input.IC_KP0", IC_KP0);
    GetScript()->CreateVariable("Input.IC_KP1", IC_KP1);
    GetScript()->CreateVariable("Input.IC_KP2", IC_KP2);
    GetScript()->CreateVariable("Input.IC_KP3", IC_KP3);
    GetScript()->CreateVariable("Input.IC_KP4", IC_KP4);
    GetScript()->CreateVariable("Input.IC_KP5", IC_KP5);
    GetScript()->CreateVariable("Input.IC_KP6", IC_KP6);
    GetScript()->CreateVariable("Input.IC_KP7", IC_KP7);
    GetScript()->CreateVariable("Input.IC_KP8", IC_KP8);
    GetScript()->CreateVariable("Input.IC_KP9", IC_KP9);
    GetScript()->CreateVariable("Input.IC_KP_DECIMAL", IC_KP_DECIMAL);
    GetScript()->CreateVariable("Input.IC_KP_DIVIDE", IC_KP_DIVIDE);
    GetScript()->CreateVariable("Input.IC_KP_MULTIPLY", IC_KP_MULTIPLY);
    GetScript()->CreateVariable("Input.IC_KP_MINUS", IC_KP_MINUS);
    GetScript()->CreateVariable("Input.IC_KP_PLUS", IC_KP_PLUS);
    GetScript()->CreateVariable("Input.IC_KP_ENTER", IC_KP_ENTER);

    // arrows + home/end pad
    GetScript()->CreateVariable("Input.IC_UP", IC_UP);
    GetScript()->CreateVariable("Input.IC_DOWN", IC_DOWN);
    GetScript()->CreateVariable("Input.IC_LEFT", IC_LEFT);
    GetScript()->CreateVariable("Input.IC_RIGHT", IC_RIGHT);
    GetScript()->CreateVariable("Input.IC_INSERT", IC_INSERT);
    GetScript()->CreateVariable("Input.IC_DELETE", IC_DELETE);
    GetScript()->CreateVariable("Input.IC_HOME", IC_HOME);
    GetScript()->CreateVariable("Input.IC_END", IC_END);
    GetScript()->CreateVariable("Input.IC_PAGEUP", IC_PAGEUP);
    GetScript()->CreateVariable("Input.IC_PAGEDOWN", IC_PAGEDOWN);

    // key state modifier keys
    GetScript()->CreateVariable("Input.IC_NUMLOCK", IC_NUMLOCK);
    GetScript()->CreateVariable("Input.IC_CAPSLOCK", IC_CAPSLOCK);
    GetScript()->CreateVariable("Input.IC_SCROLLOCK", IC_SCROLLOCK);
    GetScript()->CreateVariable("Input.IC_LSHIFT", IC_LSHIFT);
    GetScript()->CreateVariable("Input.IC_RSHIFT", IC_RSHIFT);
    GetScript()->CreateVariable("Input.IC_LCTRL", IC_LCTRL);
    GetScript()->CreateVariable("Input.IC_RCTRL", IC_RCTRL);
    GetScript()->CreateVariable("Input.IC_LALT", IC_LALT);
    GetScript()->CreateVariable("Input.IC_RALT", IC_RALT);
    GetScript()->CreateVariable("Input.IC_LSUPER", IC_LSUPER); // Left "Windows" key
    GetScript()->CreateVariable("Input.IC_RSUPER", IC_RSUPER); // Right "Windows" key

    // other keys (cursor control, punctuation)
    GetScript()->CreateVariable("Input.IC_ESCAPE", IC_ESCAPE);
    GetScript()->CreateVariable("Input.IC_PRINT", IC_PRINT);
    GetScript()->CreateVariable("Input.IC_PAUSE", IC_PAUSE);
    GetScript()->CreateVariable("Input.IC_GRAVE", IC_GRAVE);
    GetScript()->CreateVariable("Input.IC_MINUS", IC_MINUS);
    GetScript()->CreateVariable("Input.IC_EQUALS", IC_EQUALS);
    GetScript()->CreateVariable("Input.IC_BACKSLASH", IC_BACKSLASH);
    GetScript()->CreateVariable("Input.IC_BACKSPACE", IC_BACKSPACE);

    GetScript()->CreateVariable("Input.IC_TAB", IC_TAB);
    GetScript()->CreateVariable("Input.IC_LBRACKET", IC_LBRACKET);
    GetScript()->CreateVariable("Input.IC_RBRACKET", IC_RBRACKET);
    GetScript()->CreateVariable("Input.IC_RETURN", IC_RETURN);

    GetScript()->CreateVariable("Input.IC_SEMICOLON", IC_SEMICOLON);
    GetScript()->CreateVariable("Input.IC_APOSTROPHE", IC_APOSTROPHE);

    GetScript()->CreateVariable("Input.IC_OEM_102", IC_OEM_102);    // German <>|
    GetScript()->CreateVariable("Input.IC_COMMA", IC_COMMA);
    GetScript()->CreateVariable("Input.IC_PERIOD", IC_PERIOD);
    GetScript()->CreateVariable("Input.IC_SLASH", IC_SLASH);

    GetScript()->CreateVariable("Input.IC_SPACE", IC_SPACE);

    // mouse buttons
    GetScript()->CreateVariable("Input.IC_MOUSE_LEFT", IC_MOUSE_LEFT);// left
    GetScript()->CreateVariable("Input.IC_MOUSE_RIGHT", IC_MOUSE_RIGHT);// right
    GetScript()->CreateVariable("Input.IC_MOUSE_MIDDLE", IC_MOUSE_MIDDLE);  // middle

    //mouse axis
    GetScript()->CreateVariable("Input.IC_AXISX", IC_AXISX);
    GetScript()->CreateVariable("Input.IC_AXISY", IC_AXISY);
    GetScript()->CreateVariable("Input.IC_AXISZ", IC_AXISZ);

    // timer
    GetScript()->CreateVariable("Input.IC_AXIST", IC_AXIST);

    // create the inputsystem
    shared_ptr<InputSystem> inputSystem =
        shared_dynamic_cast<InputSystem>(GetCore()->New(inputSysName));

    if(inputSystem.get() == 0)
        {
            // could not create InputSystem
            GetLog()->Error() << "(InputServer) ERROR: unable to create "
                              << inputSysName << "\n";
            return false;
        }

    inputSystem->SetName("inputsystem");

    if (inputSystem->Init(this) == false)
        {
            GetLog()->Error() << "(InputServer) ERROR: unable to initialize "
                              << inputSysName << "\n";
            Reset();
            return false;
        }

    AddChildReference(inputSystem);

    // import the scan code map
    GetScript()->RunInitScript
        (
         mScanCodeScript,
         "lib/kerosin/inputserver",
         ScriptServer::IS_COMMON
         );

    return true;
}

shared_ptr<InputSystem> InputServer::GetInputSystem()
{
    shared_ptr<InputSystem> inputSystem = shared_dynamic_cast<InputSystem>
        (GetChild("inputsystem"));

    return inputSystem;
}


bool InputServer::CreateDevice(const std::string &deviceName)
{
    GetLog()->Normal() << "(InputServer) CreateDevice " << deviceName << "\n";

    shared_ptr<InputSystem> inputSystem = GetInputSystem();

    if (inputSystem.get() == 0)
        {
            GetLog()->Error()
                << "(InputSystem) ERROR: no InputSystem installed\n";
            return false;
        }

    if (inputSystem.get() == 0)
        {
            return false;
        }

    return inputSystem->CreateDevice(deviceName);
}

void InputServer::Reset()
{
    shared_ptr<InputSystem> inputSystem = GetInputSystem();

    if (inputSystem.get() != 0)
        {
            inputSystem->Unlink();
            inputSystem.reset();
        }

    mScanCodeMap->Reset();
}

bool InputServer::GetInput(Input &input, bool raw)
{
    shared_ptr<InputSystem> inputSystem = GetInputSystem();

    if (inputSystem.get() == 0)
        {
            GetLog()->Error()
                << "(InputServer) ERROR: no InputSystem installed\n";
            input.id = -1;
            return false;
        }

    if (! inputSystem->GetInput(input))
        {
            input.id = -1;
            return false;
        }

    if (
        (input.type == eUser) ||
        (raw)
        )
        {
            // return eUser input
            return true;
        }

    // translate raw input to binding
    TBindMap::iterator bindListIter = mBindings.find(input.code);
    if (bindListIter == mBindings.end())
        {
            input.id = -1;
            return false;
        }

    // we have an entry for the scan code
    TBindList& bindList = (*bindListIter).second;

    for (
         TBindList::const_iterator bindIter = bindList.begin();
         bindIter != bindList.end();
         ++bindIter
         )
        {
            const Bind& bind = (*bindIter);

            //printf("Looking at: %d %d %d", (*bind).code, (*bind).cmd, (*bind).modifier);
            if (bind.modifier == mModifierState)
                {
                    if (input.type == eButton)
                        {
                            if ((bind.event == eKeyUpDown) ||
                                (bind.event == eKeyUp && input.data.l == 0) ||
                                (bind.event == eKeyDown && input.data.l == 1)
                                )
                                {
                                    input.id = bind.cmd;
                                    return true;
                                }
                        } else if (input.type == eAxis)
                            {
                                input.id = bind.cmd;
                                return true;
                            }
                }
        }


    input.id = -1;
    return false;
}

bool InputServer::BindCommand(const std::string &desc, int cmd)
{
    // first we have to translate the description to a correct Bind
    // struct
    Bind bind;
    if (! ParseBindDescription(bind, desc))
        {
            return false;
        }

    // GetLog()->Normal() << "Binding " << cmd << endl;
    // GetLog()->Normal() << "  code: " << bind.code << endl;
    // GetLog()->Normal() << "  modifier: " << bind.modifier << endl;
    // GetLog()->Normal() << "  event: " << bind.event << endl;

    bind.cmd = cmd;

    mBindings[bind.code].push_front(bind);

    return true;
}

void InputServer::SetScanCodeMapping(const std::string &name)
{
    mScanCodeScript = name;
}

void InputServer::AddCode(TInputCode ic, const std::string &name, char noMod,
                          char shiftMod, char altMod)
{
    mScanCodeMap->AddCode(ic, name, noMod, shiftMod, altMod);
}

bool InputServer::ParseBindDescription(Bind &bind, const std::string &desc)
{
    stringstream s(desc);
    string current;
    list<string> tokens;

    while(!s.eof())
        {
            getline(s, current,' ');
            if (current.size())
                {
                    tokens.push_back(current);
                }
        }

    if (tokens.size() == 0)
        {
            GetLog()->Error() << "(InputServer) ERROR: Empty bind description? '"
                              << desc << "'" << endl;
            return false;
        }

    // separated string is in tokens first we handle all the modifiers
    bind.modifier = eNone;

    while (tokens.size() > 1)
        {
            current = tokens.front();
            tokens.pop_front();
            bind.modifier |= ParseModifier(current);
        }

    // at this point only a single string is in the tokenlist
    current = tokens.front();
    tokens.pop_front();

    // current now holds the event to which to bind to, plus (maybe)
    // its pressed/release modifier
    bind.event = eKeyUpDown;

    if (current[0]=='+')
        {
            bind.event = eKeyDown;
        } else if (current[0]=='-')
            {
                bind.event = eKeyUp;
            }

    if (bind.event != eKeyUpDown)
        {
            current = current.substr(1);
        }

    bind.code = mScanCodeMap->GetCode(current);

    if (bind.code == 0)
        {
            GetLog()->Error() << "ERROR: Erroneous code description '"
                              << current << "'" << endl;
            return false;
        }

    return true;
}

int InputServer::ParseModifier(const std::string &modifier) const
{
    if (modifier == "lshift")       return eLShift;
    if (modifier == "rshift")       return eRShift;
    if (modifier == "shift")        return eShift;
    if (modifier == "lctrl")        return eLCtrl;
    if (modifier == "rctrl")        return eRCtrl;
    if (modifier == "ctrl")         return eCtrl;
    if (modifier == "lalt")         return eLAlt;
    if (modifier == "ralt")         return eRAlt;
    if (modifier == "alt")          return eAlt;

    return eNone;
}

bool InputServer::TranslateCode(TInputCode code,
                                unsigned long state, char &ch) const
{
    state = mModifierState;
    return mScanCodeMap->TranslateCode(code, state, ch);
}

void InputServer::Invoke(int cmd)
{
    Input input;

    input.type = eUser;
    input.code = -1;
    input.id   = cmd;
    input.data.l = 0;

    shared_ptr<InputSystem> inputSystem = GetInputSystem();
    if (inputSystem.get() == 0)
        {
            GetLog()->Error()
                << "(InputServer) ERROR:  no InputSystem installed\n";
            return;
        }

    inputSystem->AddInput(input);
}
