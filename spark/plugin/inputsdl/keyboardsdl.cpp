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
#include "keyboardsdl.h"
#include <kerosin/inputserver/inputserver.h>
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;
using namespace std;

KeyboardSDL::KeyboardSDL()
    : InputDeviceSDL()
{
    SetupSymMap();
}

KeyboardSDL::~KeyboardSDL()
{
}

void KeyboardSDL::SetupSymMap()
{
    mSymbols.clear();

    // symbols taken from <SDL/SDL_keysm.h>

    mSymbols[SDLK_BACKSPACE]   = Input::IC_BACKSPACE;
    mSymbols[SDLK_TAB]         = Input::IC_TAB;
    // SDLK_CLEAR ??
    mSymbols[SDLK_RETURN]      = Input::IC_RETURN;
    mSymbols[SDLK_PAUSE]       = Input::IC_PAUSE;
    mSymbols[SDLK_ESCAPE]      = Input::IC_ESCAPE;
    mSymbols[SDLK_SPACE]       = Input::IC_SPACE;
    // SDLK_EXCLAIM
    // SDLK_QUOTEDBL
    // SDLK_HASH
    // SDLK_DOLLAR
    // SDLK_AMPERSAND
    // SDLK_QUOTE
    // SDLK_LEFTPAREN
    // SDLK_RIGHTPAREN
    // SDLK_ASTERISK
    // SDLK_PLUS
    mSymbols[SDLK_COMMA]        = Input::IC_COMMA;
    mSymbols[SDLK_MINUS]        = Input::IC_MINUS;
    mSymbols[SDLK_PERIOD]       = Input::IC_PERIOD;
    mSymbols[SDLK_SLASH]        = Input::IC_SLASH;
    mSymbols[SDLK_0]            = Input::IC_0;
    mSymbols[SDLK_1]            = Input::IC_1;
    mSymbols[SDLK_2]            = Input::IC_2;
    mSymbols[SDLK_3]            = Input::IC_3;
    mSymbols[SDLK_4]            = Input::IC_4;
    mSymbols[SDLK_5]            = Input::IC_5;
    mSymbols[SDLK_6]            = Input::IC_6;
    mSymbols[SDLK_7]            = Input::IC_7;
    mSymbols[SDLK_8]            = Input::IC_8;
    mSymbols[SDLK_9]            = Input::IC_9;
    // SDLK_COLON
    mSymbols[SDLK_SEMICOLON]    = Input::IC_SEMICOLON;
    // SDLK_LESS
    // SDLK_EQUALS
    mSymbols[SDLK_EQUALS]       = Input::IC_EQUALS;
    // SDLK_GREATER
    // SDLK_QUESTION
    // SDLK_AT
    mSymbols[SDLK_LEFTBRACKET]  = Input::IC_LBRACKET;
    mSymbols[SDLK_BACKSLASH]    = Input::IC_BACKSLASH;
    mSymbols[SDLK_RIGHTBRACKET] = Input::IC_RBRACKET;
    // SDLK_CARET
    // SDLK_UNDERSCORE
    // SDLK_BACKQUOTE
    mSymbols[SDLK_a]            = Input::IC_A;
    mSymbols[SDLK_b]            = Input::IC_B;
    mSymbols[SDLK_c]            = Input::IC_C;
    mSymbols[SDLK_d]            = Input::IC_D;
    mSymbols[SDLK_e]            = Input::IC_E;
    mSymbols[SDLK_f]            = Input::IC_F;
    mSymbols[SDLK_g]            = Input::IC_G;
    mSymbols[SDLK_h]            = Input::IC_H;
    mSymbols[SDLK_i]            = Input::IC_I;
    mSymbols[SDLK_j]            = Input::IC_J;
    mSymbols[SDLK_k]            = Input::IC_K;
    mSymbols[SDLK_l]            = Input::IC_L;
    mSymbols[SDLK_m]            = Input::IC_M;
    mSymbols[SDLK_n]            = Input::IC_N;
    mSymbols[SDLK_o]            = Input::IC_O;
    mSymbols[SDLK_p]            = Input::IC_P;
    mSymbols[SDLK_q]            = Input::IC_Q;
    mSymbols[SDLK_r]            = Input::IC_R;
    mSymbols[SDLK_s]            = Input::IC_S;
    mSymbols[SDLK_t]            = Input::IC_T;
    mSymbols[SDLK_u]            = Input::IC_U;
    mSymbols[SDLK_v]            = Input::IC_V;
    mSymbols[SDLK_w]            = Input::IC_W;
    mSymbols[SDLK_x]            = Input::IC_X;
    mSymbols[SDLK_y]            = Input::IC_Y;
    mSymbols[SDLK_z]            = Input::IC_Z;
    mSymbols[SDLK_DELETE]       = Input::IC_DELETE;
    // SDLK_WORLD_0
    // [...]
    // SDLK_WORLD_95
    mSymbols[SDLK_KP0]          = Input::IC_KP0;
    mSymbols[SDLK_KP1]          = Input::IC_KP1;
    mSymbols[SDLK_KP2]          = Input::IC_KP2;
    mSymbols[SDLK_KP3]          = Input::IC_KP3;
    mSymbols[SDLK_KP4]          = Input::IC_KP4;
    mSymbols[SDLK_KP5]          = Input::IC_KP5;
    mSymbols[SDLK_KP6]          = Input::IC_KP6;
    mSymbols[SDLK_KP7]          = Input::IC_KP7;
    mSymbols[SDLK_KP8]          = Input::IC_KP8;
    mSymbols[SDLK_KP9]          = Input::IC_KP9;
    mSymbols[SDLK_KP_PERIOD]    = Input::IC_KP_DECIMAL;
    mSymbols[SDLK_KP_DIVIDE]    = Input::IC_KP_DIVIDE;
    mSymbols[SDLK_KP_MULTIPLY]  = Input::IC_KP_MULTIPLY;
    mSymbols[SDLK_KP_MINUS]     = Input::IC_KP_MINUS;
    mSymbols[SDLK_KP_PLUS]      = Input::IC_KP_PLUS;
    mSymbols[SDLK_KP_ENTER]     = Input::IC_KP_ENTER;
    // SDLK_KP_EQUALS
    mSymbols[SDLK_UP]           = Input::IC_UP;
    mSymbols[SDLK_DOWN]         = Input::IC_DOWN;
    mSymbols[SDLK_RIGHT]        = Input::IC_RIGHT;
    mSymbols[SDLK_LEFT]         = Input::IC_LEFT;
    mSymbols[SDLK_INSERT]       = Input::IC_INSERT;
    mSymbols[SDLK_HOME]         = Input::IC_HOME;
    mSymbols[SDLK_END]          = Input::IC_END;
    mSymbols[SDLK_PAGEUP]       = Input::IC_PAGEUP;
    mSymbols[SDLK_PAGEDOWN]     = Input::IC_PAGEDOWN;
    mSymbols[SDLK_F1]           = Input::IC_F1;
    mSymbols[SDLK_F2]           = Input::IC_F2;
    mSymbols[SDLK_F3]           = Input::IC_F3;
    mSymbols[SDLK_F4]           = Input::IC_F4;
    mSymbols[SDLK_F5]           = Input::IC_F5;
    mSymbols[SDLK_F6]           = Input::IC_F6;
    mSymbols[SDLK_F7]           = Input::IC_F7;
    mSymbols[SDLK_F8]           = Input::IC_F8;
    mSymbols[SDLK_F9]           = Input::IC_F9;
    mSymbols[SDLK_F10]          = Input::IC_F10;
    mSymbols[SDLK_F11]          = Input::IC_F11;
    mSymbols[SDLK_F12]          = Input::IC_F12;
    // SDLK_F13
    // SDLK_F14
    // SDLK_F15
    mSymbols[SDLK_NUMLOCK]      = Input::IC_NUMLOCK;
    mSymbols[SDLK_CAPSLOCK]     = Input::IC_CAPSLOCK;
    mSymbols[SDLK_SCROLLOCK]    = Input::IC_SCROLLOCK;
    mSymbols[SDLK_RSHIFT]       = Input::IC_RSHIFT;
    mSymbols[SDLK_LSHIFT]       = Input::IC_LSHIFT;
    mSymbols[SDLK_RCTRL]        = Input::IC_RCTRL;
    mSymbols[SDLK_LCTRL]        = Input::IC_LCTRL;
    mSymbols[SDLK_RALT]         = Input::IC_RALT;
    mSymbols[SDLK_LALT]         = Input::IC_LALT;
    // SDLK_RMETA
    // SDLK_LMETA
    mSymbols[SDLK_LSUPER]       = Input::IC_LSUPER;
    mSymbols[SDLK_RSUPER]       = Input::IC_RSUPER;
    // SDLK_MODE
    // SDLK_COMPOSE
    // SDLK_HELP
    mSymbols[SDLK_PRINT]        = Input::IC_PRINT;
    // SDLK_SYSREQ
    // SDLK_BREAK
    // SDLK_MENU
    // SDLK_POWER
    // SDLK_EURO
    // SDLK_UNDO
}

bool
KeyboardSDL::Init(kerosin::InputSystem *inputSystem)
{
    return InputDevice::Init(inputSystem);
}

bool
KeyboardSDL::TranslateSymbol(int& sym)
{
    TSymMap::iterator iter = mSymbols.find(sym);
    if (iter == mSymbols.end())
    {
        return false;
    }

    sym = (*iter).second;
    return true;
}

int
KeyboardSDL::EventFilter(const SDL_Event* event)
{
    // we only want keyboard events
    if (
        (event->type != SDL_KEYDOWN) &&
        (event->type != SDL_KEYUP)
        )
    {
        return 1;
    }

    unsigned int modState;

    modState = Input::eNone;

    if (event->key.keysym.mod & KMOD_LSHIFT)
    {
        modState |= Input::eLShift;
    }

    if (event->key.keysym.mod & KMOD_RSHIFT)
    {
        modState |= Input::eRShift;
    }

    if (event->key.keysym.mod & KMOD_LALT)
    {
        modState |= Input::eLAlt;
    }

    if (event->key.keysym.mod & KMOD_RALT)
    {
        modState |= Input::eRAlt;
    }

    if (event->key.keysym.mod & KMOD_LCTRL)
    {
        modState |= Input::eLCtrl;
    }

    if (event->key.keysym.mod & KMOD_RCTRL)
    {
        modState |= Input::eRCtrl;
    }

    if (event->key.keysym.sym == 0)
    {
        return 1;
    }

    int sym = event->key.keysym.sym;
    if (! TranslateSymbol(sym))
    {
        GetLog()->Warning()
            << "(KeyboardSDL) WARNING: encountered unknown SDL key symbol "
            << sym << "\n";
        return 1;
    }

    Input input(Input::eButton, sym);
    input.mData.l = (event->type == SDL_KEYDOWN);
    input.mModState = modState;
    mInputSystem->AddInput(input);

    return 0;
}
