/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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

InputServer::InputServer() :
    Node(), mModifierState(Input::eUnknown), mScanCodeMap(new ScanCodeMap())
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
    GetScript()->CreateVariable("Input.IC_1", Input::IC_1);
    GetScript()->CreateVariable("Input.IC_2", Input::IC_2);
    GetScript()->CreateVariable("Input.IC_3", Input::IC_3);
    GetScript()->CreateVariable("Input.IC_4", Input::IC_4);
    GetScript()->CreateVariable("Input.IC_5", Input::IC_5);
    GetScript()->CreateVariable("Input.IC_6", Input::IC_6);
    GetScript()->CreateVariable("Input.IC_7", Input::IC_7);
    GetScript()->CreateVariable("Input.IC_8", Input::IC_8);
    GetScript()->CreateVariable("Input.IC_9", Input::IC_9);
    GetScript()->CreateVariable("Input.IC_0", Input::IC_0);

    // function keys
    GetScript()->CreateVariable("Input.IC_F1", Input::IC_F1);
    GetScript()->CreateVariable("Input.IC_F2", Input::IC_F2);
    GetScript()->CreateVariable("Input.IC_F3", Input::IC_F3);
    GetScript()->CreateVariable("Input.IC_F4", Input::IC_F4);
    GetScript()->CreateVariable("Input.IC_F5", Input::IC_F5);
    GetScript()->CreateVariable("Input.IC_F6", Input::IC_F6);
    GetScript()->CreateVariable("Input.IC_F7", Input::IC_F7);
    GetScript()->CreateVariable("Input.IC_F8", Input::IC_F8);
    GetScript()->CreateVariable("Input.IC_F9", Input::IC_F9);
    GetScript()->CreateVariable("Input.IC_F10", Input::IC_F10);
    GetScript()->CreateVariable("Input.IC_F11", Input::IC_F11);
    GetScript()->CreateVariable("Input.IC_F12", Input::IC_F12);

    // alphabet
    GetScript()->CreateVariable("Input.IC_A", Input::IC_A);
    GetScript()->CreateVariable("Input.IC_B", Input::IC_B);
    GetScript()->CreateVariable("Input.IC_C", Input::IC_C);
    GetScript()->CreateVariable("Input.IC_D", Input::IC_D);
    GetScript()->CreateVariable("Input.IC_E", Input::IC_E);
    GetScript()->CreateVariable("Input.IC_F", Input::IC_F);
    GetScript()->CreateVariable("Input.IC_G", Input::IC_G);
    GetScript()->CreateVariable("Input.IC_H", Input::IC_H);
    GetScript()->CreateVariable("Input.IC_I", Input::IC_I);
    GetScript()->CreateVariable("Input.IC_J", Input::IC_J);
    GetScript()->CreateVariable("Input.IC_K", Input::IC_K);
    GetScript()->CreateVariable("Input.IC_L", Input::IC_L);
    GetScript()->CreateVariable("Input.IC_M", Input::IC_M);
    GetScript()->CreateVariable("Input.IC_N", Input::IC_N);
    GetScript()->CreateVariable("Input.IC_O", Input::IC_O);
    GetScript()->CreateVariable("Input.IC_P", Input::IC_P);
    GetScript()->CreateVariable("Input.IC_Q", Input::IC_Q);
    GetScript()->CreateVariable("Input.IC_R", Input::IC_R);
    GetScript()->CreateVariable("Input.IC_S", Input::IC_S);
    GetScript()->CreateVariable("Input.IC_T", Input::IC_T);
    GetScript()->CreateVariable("Input.IC_U", Input::IC_U);
    GetScript()->CreateVariable("Input.IC_V", Input::IC_V);
    GetScript()->CreateVariable("Input.IC_W", Input::IC_W);
    GetScript()->CreateVariable("Input.IC_X", Input::IC_X);
    GetScript()->CreateVariable("Input.IC_Y", Input::IC_Y);
    GetScript()->CreateVariable("Input.IC_Z", Input::IC_Z);

    // keypad
    GetScript()->CreateVariable("Input.IC_KP0", Input::IC_KP0);
    GetScript()->CreateVariable("Input.IC_KP1", Input::IC_KP1);
    GetScript()->CreateVariable("Input.IC_KP2", Input::IC_KP2);
    GetScript()->CreateVariable("Input.IC_KP3", Input::IC_KP3);
    GetScript()->CreateVariable("Input.IC_KP4", Input::IC_KP4);
    GetScript()->CreateVariable("Input.IC_KP5", Input::IC_KP5);
    GetScript()->CreateVariable("Input.IC_KP6", Input::IC_KP6);
    GetScript()->CreateVariable("Input.IC_KP7", Input::IC_KP7);
    GetScript()->CreateVariable("Input.IC_KP8", Input::IC_KP8);
    GetScript()->CreateVariable("Input.IC_KP9", Input::IC_KP9);
    GetScript()->CreateVariable("Input.IC_KP_DECIMAL", Input::IC_KP_DECIMAL);
    GetScript()->CreateVariable("Input.IC_KP_DIVIDE", Input::IC_KP_DIVIDE);
    GetScript()->CreateVariable("Input.IC_KP_MULTIPLY", Input::IC_KP_MULTIPLY);
    GetScript()->CreateVariable("Input.IC_KP_MINUS", Input::IC_KP_MINUS);
    GetScript()->CreateVariable("Input.IC_KP_PLUS", Input::IC_KP_PLUS);
    GetScript()->CreateVariable("Input.IC_KP_ENTER", Input::IC_KP_ENTER);

    // arrows + home/end pad
    GetScript()->CreateVariable("Input.IC_UP", Input::IC_UP);
    GetScript()->CreateVariable("Input.IC_DOWN", Input::IC_DOWN);
    GetScript()->CreateVariable("Input.IC_LEFT", Input::IC_LEFT);
    GetScript()->CreateVariable("Input.IC_RIGHT", Input::IC_RIGHT);
    GetScript()->CreateVariable("Input.IC_INSERT", Input::IC_INSERT);
    GetScript()->CreateVariable("Input.IC_DELETE", Input::IC_DELETE);
    GetScript()->CreateVariable("Input.IC_HOME", Input::IC_HOME);
    GetScript()->CreateVariable("Input.IC_END", Input::IC_END);
    GetScript()->CreateVariable("Input.IC_PAGEUP", Input::IC_PAGEUP);
    GetScript()->CreateVariable("Input.IC_PAGEDOWN", Input::IC_PAGEDOWN);

    // key state modifier keys
    GetScript()->CreateVariable("Input.IC_NUMLOCK", Input::IC_NUMLOCK);
    GetScript()->CreateVariable("Input.IC_CAPSLOCK", Input::IC_CAPSLOCK);
    GetScript()->CreateVariable("Input.IC_SCROLLOCK", Input::IC_SCROLLOCK);
    GetScript()->CreateVariable("Input.IC_LSHIFT", Input::IC_LSHIFT);
    GetScript()->CreateVariable("Input.IC_RSHIFT", Input::IC_RSHIFT);
    GetScript()->CreateVariable("Input.IC_LCTRL", Input::IC_LCTRL);
    GetScript()->CreateVariable("Input.IC_RCTRL", Input::IC_RCTRL);
    GetScript()->CreateVariable("Input.IC_LALT", Input::IC_LALT);
    GetScript()->CreateVariable("Input.IC_RALT", Input::IC_RALT);
    GetScript()->CreateVariable("Input.IC_LSUPER", Input::IC_LSUPER); // Left "Windows" key
    GetScript()->CreateVariable("Input.IC_RSUPER", Input::IC_RSUPER); // Right "Windows" key

    // other keys (cursor control, punctuation)
    GetScript()->CreateVariable("Input.IC_ESCAPE", Input::IC_ESCAPE);
    GetScript()->CreateVariable("Input.IC_PRINT", Input::IC_PRINT);
    GetScript()->CreateVariable("Input.IC_PAUSE", Input::IC_PAUSE);
    GetScript()->CreateVariable("Input.IC_GRAVE", Input::IC_GRAVE);
    GetScript()->CreateVariable("Input.IC_MINUS", Input::IC_MINUS);
    GetScript()->CreateVariable("Input.IC_EQUALS", Input::IC_EQUALS);
    GetScript()->CreateVariable("Input.IC_BACKSLASH", Input::IC_BACKSLASH);
    GetScript()->CreateVariable("Input.IC_BACKSPACE", Input::IC_BACKSPACE);

    GetScript()->CreateVariable("Input.IC_TAB", Input::IC_TAB);
    GetScript()->CreateVariable("Input.IC_LBRACKET", Input::IC_LBRACKET);
    GetScript()->CreateVariable("Input.IC_RBRACKET", Input::IC_RBRACKET);
    GetScript()->CreateVariable("Input.IC_RETURN", Input::IC_RETURN);

    GetScript()->CreateVariable("Input.IC_SEMICOLON", Input::IC_SEMICOLON);
    GetScript()->CreateVariable("Input.IC_APOSTROPHE", Input::IC_APOSTROPHE);

    GetScript()->CreateVariable("Input.IC_OEM_102", Input::IC_OEM_102);    // German <>|
    GetScript()->CreateVariable("Input.IC_COMMA", Input::IC_COMMA);
    GetScript()->CreateVariable("Input.IC_PERIOD", Input::IC_PERIOD);
    GetScript()->CreateVariable("Input.IC_SLASH", Input::IC_SLASH);

    GetScript()->CreateVariable("Input.IC_SPACE", Input::IC_SPACE);

    // mouse buttons
    GetScript()->CreateVariable("Input.IC_MOUSE_LEFT", Input::IC_MOUSE_LEFT);// left
    GetScript()->CreateVariable("Input.IC_MOUSE_RIGHT", Input::IC_MOUSE_RIGHT);// right
    GetScript()->CreateVariable("Input.IC_MOUSE_MIDDLE", Input::IC_MOUSE_MIDDLE);  // middle

    //mouse axis
    GetScript()->CreateVariable("Input.IC_AXISX", Input::IC_AXISX);
    GetScript()->CreateVariable("Input.IC_AXISY", Input::IC_AXISY);
    GetScript()->CreateVariable("Input.IC_AXISZ", Input::IC_AXISZ);

    // timer
    GetScript()->CreateVariable("Input.IC_AXIST", Input::IC_AXIST);

    // create the inputsystem
    boost::shared_ptr<InputSystem> inputSystem =
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

boost::shared_ptr<InputSystem> InputServer::GetInputSystem()
{
    boost::shared_ptr<InputSystem> inputSystem = shared_dynamic_cast<InputSystem>
        (GetChild("inputsystem"));

    return inputSystem;
}


bool InputServer::CreateDevice(const std::string &deviceName)
{
    GetLog()->Normal() << "(InputServer) CreateDevice " << deviceName << "\n";

    boost::shared_ptr<InputSystem> inputSystem = GetInputSystem();

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
    boost::shared_ptr<InputSystem> inputSystem = GetInputSystem();

    if (inputSystem.get() != 0)
        {
            inputSystem->Unlink();
            inputSystem.reset();
        }

    mScanCodeMap->Reset();
}

bool
InputServer::GetInput(Input &input, bool raw)
{
    boost::shared_ptr<InputSystem> inputSystem = GetInputSystem();

    if (inputSystem.get() == 0)
    {
        GetLog()->Error()
            << "(InputServer) ERROR: no InputSystem installed\n";
        input.mId = -1;
        return false;
    }

    if (! inputSystem->GetInput(input))
    {
        input.mId = -1;
        return false;
    }

    if (
        (input.mType == Input::eUser) ||
        (raw)
        )
    {
        // return Input::eUser input
        return true;
    }
    // translate raw input to binding
    TBindMap::iterator bindListIter = mBindings.find(input.mCode);
    if (bindListIter == mBindings.end())
    {
        input.mId = -1;
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

        //printf("Looking at: %d %d %d", (*bind).mCode, (*bind).cmd, (*bind).modifier);
        if (bind.modifier == mModifierState)
        {
#if 0
            if (input.mType == Input::eButton)
            {
                if ((bind.event == eKeyUpDown) ||
                    (bind.event == eKeyUp && input.mData.l == 0) ||
                    (bind.event == eKeyDown && input.mData.l == 1)
                    )
#else
            const Bind& bind = (*bindIter);

            //printf("Looking at: %d %d %d", (*bind).mCode, (*bind).cmd, (*bind).modifier);
            if (
                (bind.modifier == 0 && mModifierState == 0) ||
                (bind.modifier & mModifierState)
                )
#endif
            {
                input.mId = bind.cmd;
                return true;
            }
            else if (input.mType == Input::eAxis)
            {
                input.mId = bind.cmd;
                return true;
            }
        }
    }

    input.mId = -1;
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

void InputServer::AddCode(Input::TInputCode ic, const std::string &name, char noMod,
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
    bind.modifier = Input::eNone;

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
    if (modifier == "lshift")       return Input::eLShift;
    if (modifier == "rshift")       return Input::eRShift;
    if (modifier == "shift")        return Input::eShift;
    if (modifier == "lctrl")        return Input::eLCtrl;
    if (modifier == "rctrl")        return Input::eRCtrl;
    if (modifier == "ctrl")         return Input::eCtrl;
    if (modifier == "lalt")         return Input::eLAlt;
    if (modifier == "ralt")         return Input::eRAlt;
    if (modifier == "alt")          return Input::eAlt;

    return Input::eNone;
}

bool InputServer::TranslateCode(Input::TInputCode code,
                                unsigned long state, char &ch) const
{
    state = mModifierState;
    return mScanCodeMap->TranslateCode(code, state, ch);
}

void InputServer::Invoke(int cmd)
{
    Input input;

    input.mType = Input::eUser;
    input.mCode = -1;
    input.mId   = cmd;
    input.mData.l = 0;

    boost::shared_ptr<InputSystem> inputSystem = GetInputSystem();
    if (inputSystem.get() == 0)
        {
            GetLog()->Error()
                << "(InputServer) ERROR:  no InputSystem installed\n";
            return;
        }

    inputSystem->AddInput(input);
}

unsigned int& InputServer::GetModifierState()
{
    return mModifierState;
}
