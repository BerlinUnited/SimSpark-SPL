/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: keyboardsdl.cpp,v 1.5 2004/04/23 20:43:54 fruit Exp $

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

KeyboardSDL::KeyboardSDL()
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

    mSymbols[SDLK_BACKSPACE]   = InputServer::IC_BACKSPACE;
    mSymbols[SDLK_TAB]         = InputServer::InputServer::IC_TAB;
    // SDLK_CLEAR ??
    mSymbols[SDLK_RETURN]      = InputServer::IC_RETURN;
    mSymbols[SDLK_PAUSE]       = InputServer::IC_PAUSE;
    mSymbols[SDLK_ESCAPE]      = InputServer::IC_ESCAPE;
    mSymbols[SDLK_SPACE]       = InputServer::IC_SPACE;
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
    mSymbols[SDLK_COMMA]        = InputServer::IC_COMMA;
    mSymbols[SDLK_MINUS]        = InputServer::IC_MINUS;
    mSymbols[SDLK_PERIOD]       = InputServer::IC_PERIOD;
    mSymbols[SDLK_SLASH]        = InputServer::IC_SLASH;
    mSymbols[SDLK_0]            = InputServer::IC_0;
    mSymbols[SDLK_1]            = InputServer::IC_1;
    mSymbols[SDLK_2]            = InputServer::IC_2;
    mSymbols[SDLK_3]            = InputServer::IC_3;
    mSymbols[SDLK_4]            = InputServer::IC_4;
    mSymbols[SDLK_5]            = InputServer::IC_5;
    mSymbols[SDLK_6]            = InputServer::IC_6;
    mSymbols[SDLK_7]            = InputServer::IC_7;
    mSymbols[SDLK_8]            = InputServer::IC_8;
    mSymbols[SDLK_9]            = InputServer::IC_9;
    // SDLK_COLON
    mSymbols[SDLK_SEMICOLON]    = InputServer::IC_SEMICOLON;
    // SDLK_LESS
    // SDLK_EQUALS
    mSymbols[SDLK_EQUALS]       = InputServer::IC_EQUALS;
    // SDLK_GREATER
    // SDLK_QUESTION
    // SDLK_AT
    mSymbols[SDLK_LEFTBRACKET]  = InputServer::IC_LBRACKET;
    mSymbols[SDLK_BACKSLASH]    = InputServer::IC_BACKSLASH;
    mSymbols[SDLK_RIGHTBRACKET] = InputServer::IC_RBRACKET;
    // SDLK_CARET
    // SDLK_UNDERSCORE
    // SDLK_BACKQUOTE
    mSymbols[SDLK_a]            = InputServer::IC_A;
    mSymbols[SDLK_b]            = InputServer::IC_B;
    mSymbols[SDLK_c]            = InputServer::IC_C;
    mSymbols[SDLK_d]            = InputServer::IC_D;
    mSymbols[SDLK_e]            = InputServer::IC_E;
    mSymbols[SDLK_f]            = InputServer::IC_F;
    mSymbols[SDLK_g]            = InputServer::IC_G;
    mSymbols[SDLK_h]            = InputServer::IC_H;
    mSymbols[SDLK_i]            = InputServer::IC_I;
    mSymbols[SDLK_j]            = InputServer::IC_J;
    mSymbols[SDLK_k]            = InputServer::IC_K;
    mSymbols[SDLK_l]            = InputServer::IC_L;
    mSymbols[SDLK_m]            = InputServer::IC_M;
    mSymbols[SDLK_n]            = InputServer::IC_N;
    mSymbols[SDLK_o]            = InputServer::IC_O;
    mSymbols[SDLK_p]            = InputServer::IC_P;
    mSymbols[SDLK_q]            = InputServer::IC_Q;
    mSymbols[SDLK_r]            = InputServer::IC_R;
    mSymbols[SDLK_s]            = InputServer::IC_S;
    mSymbols[SDLK_t]            = InputServer::IC_T;
    mSymbols[SDLK_u]            = InputServer::IC_U;
    mSymbols[SDLK_v]            = InputServer::IC_V;
    mSymbols[SDLK_w]            = InputServer::IC_W;
    mSymbols[SDLK_x]            = InputServer::IC_X;
    mSymbols[SDLK_y]            = InputServer::IC_Y;
    mSymbols[SDLK_z]            = InputServer::IC_Z;
    mSymbols[SDLK_DELETE]       = InputServer::IC_DELETE;
    // SDLK_WORLD_0
    // [...]
    // SDLK_WORLD_95
    mSymbols[SDLK_KP0]          = InputServer::IC_KP0;
    mSymbols[SDLK_KP1]          = InputServer::IC_KP1;
    mSymbols[SDLK_KP2]          = InputServer::IC_KP2;
    mSymbols[SDLK_KP3]          = InputServer::IC_KP3;
    mSymbols[SDLK_KP4]          = InputServer::IC_KP4;
    mSymbols[SDLK_KP5]          = InputServer::IC_KP5;
    mSymbols[SDLK_KP6]          = InputServer::IC_KP6;
    mSymbols[SDLK_KP7]          = InputServer::IC_KP7;
    mSymbols[SDLK_KP8]          = InputServer::IC_KP8;
    mSymbols[SDLK_KP9]          = InputServer::IC_KP9;
    mSymbols[SDLK_KP_PERIOD]    = InputServer::IC_KP_DECIMAL;
    mSymbols[SDLK_KP_DIVIDE]    = InputServer::IC_KP_DIVIDE;
    mSymbols[SDLK_KP_MULTIPLY]  = InputServer::IC_KP_MULTIPLY;
    mSymbols[SDLK_KP_MINUS]     = InputServer::IC_KP_MINUS;
    mSymbols[SDLK_KP_PLUS]      = InputServer::IC_KP_PLUS;
    mSymbols[SDLK_KP_ENTER]     = InputServer::IC_KP_ENTER;
    // SDLK_KP_EQUALS
    mSymbols[SDLK_UP]           = InputServer::IC_UP;
    mSymbols[SDLK_DOWN]         = InputServer::IC_DOWN;
    mSymbols[SDLK_RIGHT]        = InputServer::IC_RIGHT;
    mSymbols[SDLK_LEFT]         = InputServer::IC_LEFT;
    mSymbols[SDLK_INSERT]       = InputServer::IC_INSERT;
    mSymbols[SDLK_HOME]         = InputServer::IC_HOME;
    mSymbols[SDLK_END]          = InputServer::IC_END;
    mSymbols[SDLK_PAGEUP]       = InputServer::IC_PAGEUP;
    mSymbols[SDLK_PAGEDOWN]     = InputServer::IC_PAGEDOWN;
    mSymbols[SDLK_F1]           = InputServer::IC_F1;
    mSymbols[SDLK_F2]           = InputServer::IC_F2;
    mSymbols[SDLK_F3]           = InputServer::IC_F3;
    mSymbols[SDLK_F4]           = InputServer::IC_F4;
    mSymbols[SDLK_F5]           = InputServer::IC_F5;
    mSymbols[SDLK_F6]           = InputServer::IC_F6;
    mSymbols[SDLK_F7]           = InputServer::IC_F7;
    mSymbols[SDLK_F8]           = InputServer::IC_F8;
    mSymbols[SDLK_F9]           = InputServer::IC_F9;
    mSymbols[SDLK_F10]          = InputServer::IC_F10;
    mSymbols[SDLK_F11]          = InputServer::IC_F11;
    mSymbols[SDLK_F12]          = InputServer::IC_F12;
    // SDLK_F13
    // SDLK_F14
    // SDLK_F15
    mSymbols[SDLK_NUMLOCK]      = InputServer::IC_NUMLOCK;
    mSymbols[SDLK_CAPSLOCK]     = InputServer::IC_CAPSLOCK;
    mSymbols[SDLK_SCROLLOCK]    = InputServer::IC_SCROLLOCK;
    mSymbols[SDLK_RSHIFT]       = InputServer::IC_RSHIFT;
    mSymbols[SDLK_LSHIFT]       = InputServer::IC_LSHIFT;
    mSymbols[SDLK_RCTRL]        = InputServer::IC_RCTRL;
    mSymbols[SDLK_LCTRL]        = InputServer::IC_LCTRL;
    mSymbols[SDLK_RALT]         = InputServer::IC_RALT;
    mSymbols[SDLK_LALT]         = InputServer::IC_LALT;
    // SDLK_RMETA
    // SDLK_LMETA
    mSymbols[SDLK_LSUPER]       = InputServer::IC_LSUPER;
    mSymbols[SDLK_RSUPER]       = InputServer::IC_RSUPER;
    // SDLK_MODE
    // SDLK_COMPOSE
    // SDLK_HELP
    mSymbols[SDLK_PRINT]        = InputServer::IC_PRINT;
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

    unsigned int& modState =
        mInputSystem->GetInputServer()->mModifierState;

    if (event->key.keysym.mod & KMOD_LSHIFT)
    {
        modState |= InputServer::eLShift;
    }

    if (event->key.keysym.mod & KMOD_RSHIFT)
    {
        modState |= InputServer::eRShift;
    }

    if (event->key.keysym.mod & KMOD_LALT)
    {
        modState |= InputServer::eLAlt;
    }

    if (event->key.keysym.mod & KMOD_RALT)
    {
        modState |= InputServer::eRAlt;
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

    InputServer::Input input(InputServer::eButton, sym);
    input.data.l = (event->type == SDL_KEYDOWN);
    mInputSystem->AddInputInternal(input);

    return 0;
}
