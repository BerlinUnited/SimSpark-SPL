/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: input.cpp,v 1.1 2008/02/22 16:48:19 hedayat Exp $

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
#include "input.h"

using namespace kerosin;

Input::Input(EType t, TInputCode c, int i)
    : mType(t),mCode(c),mId(i)
{
}

void Input::SetKeyPress()
{
    mData.l = 1;
}

void Input::SetKeyRelease()
{
    mData.l = 0;
}

bool Input::GetKeyPress() const
{
    return (mData.l == 1);
}

bool Input::GetKeyRelease() const
{
    return (mData.l == 0);
}


// numbers
const Input::TInputCode Input::IC_1 = 0x02;
const Input::TInputCode Input::IC_2 = 0x03;
const Input::TInputCode Input::IC_3 = 0x04;
const Input::TInputCode Input::IC_4 = 0x05;
const Input::TInputCode Input::IC_5 = 0x06;
const Input::TInputCode Input::IC_6 = 0x07;
const Input::TInputCode Input::IC_7 = 0x08;
const Input::TInputCode Input::IC_8 = 0x09;
const Input::TInputCode Input::IC_9 = 0x0a;
const Input::TInputCode Input::IC_0 = 0x0b;

// function keys
const Input::TInputCode Input::IC_F1 = 0x3b;
const Input::TInputCode Input::IC_F2 = 0x3c;
const Input::TInputCode Input::IC_F3 = 0x3d;
const Input::TInputCode Input::IC_F4 = 0x3e;
const Input::TInputCode Input::IC_F5 = 0x3f;
const Input::TInputCode Input::IC_F6 = 0x40;
const Input::TInputCode Input::IC_F7 = 0x41;
const Input::TInputCode Input::IC_F8 = 0x42;
const Input::TInputCode Input::IC_F9 = 0x43;
const Input::TInputCode Input::IC_F10 = 0x44;
const Input::TInputCode Input::IC_F11 = 0x57;
const Input::TInputCode Input::IC_F12 = 0x58;

// alphabet
const Input::TInputCode Input::IC_A = 0x1e;
const Input::TInputCode Input::IC_B = 0x30;
const Input::TInputCode Input::IC_C = 0x2e;
const Input::TInputCode Input::IC_D = 0x20;
const Input::TInputCode Input::IC_E = 0x12;
const Input::TInputCode Input::IC_F = 0x21;
const Input::TInputCode Input::IC_G = 0x22;
const Input::TInputCode Input::IC_H = 0x23;
const Input::TInputCode Input::IC_I = 0x17;
const Input::TInputCode Input::IC_J = 0x24;
const Input::TInputCode Input::IC_K = 0x25;
const Input::TInputCode Input::IC_L = 0x26;
const Input::TInputCode Input::IC_M = 0x32;
const Input::TInputCode Input::IC_N = 0x31;
const Input::TInputCode Input::IC_O = 0x18;
const Input::TInputCode Input::IC_P = 0x19;
const Input::TInputCode Input::IC_Q = 0x10;
const Input::TInputCode Input::IC_R = 0x13;
const Input::TInputCode Input::IC_S = 0x1f;
const Input::TInputCode Input::IC_T = 0x14;
const Input::TInputCode Input::IC_U = 0x16;
const Input::TInputCode Input::IC_V = 0x2f;
const Input::TInputCode Input::IC_W = 0x11;
const Input::TInputCode Input::IC_X = 0x2d;
const Input::TInputCode Input::IC_Y = 0x15;
const Input::TInputCode Input::IC_Z = 0x2c;

// keypad
const Input::TInputCode Input::IC_KP0 = 0x52;
const Input::TInputCode Input::IC_KP1 = 0x4f;
const Input::TInputCode Input::IC_KP2 = 0x50;
const Input::TInputCode Input::IC_KP3 = 0x51;
const Input::TInputCode Input::IC_KP4 = 0x4b;
const Input::TInputCode Input::IC_KP5 = 0x4c;
const Input::TInputCode Input::IC_KP6 = 0x4d;
const Input::TInputCode Input::IC_KP7 = 0x47;
const Input::TInputCode Input::IC_KP8 = 0x48;
const Input::TInputCode Input::IC_KP9 = 0x49;
const Input::TInputCode Input::IC_KP_DECIMAL  = 0x53;
const Input::TInputCode Input::IC_KP_DIVIDE   = 0xb5;
const Input::TInputCode Input::IC_KP_MULTIPLY = 0x37;
const Input::TInputCode Input::IC_KP_MINUS    = 0x4a;
const Input::TInputCode Input::IC_KP_PLUS     = 0x4e;
const Input::TInputCode Input::IC_KP_ENTER    = 0x8d;

// arrows + home/end pad
const Input::TInputCode Input::IC_UP        = 0xc8;
const Input::TInputCode Input::IC_DOWN      = 0xd0;
const Input::TInputCode Input::IC_LEFT      = 0xcb;
const Input::TInputCode Input::IC_RIGHT     = 0xcd;
const Input::TInputCode Input::IC_INSERT    = 0xd2;
const Input::TInputCode Input::IC_DELETE    = 0xd3;
const Input::TInputCode Input::IC_HOME      = 0xc7;
const Input::TInputCode Input::IC_END       = 0xcf;
const Input::TInputCode Input::IC_PAGEUP    = 0xc9;
const Input::TInputCode Input::IC_PAGEDOWN  = 0xd1;

// key state modifier keys
const Input::TInputCode Input::IC_NUMLOCK   = 0x45;
const Input::TInputCode Input::IC_CAPSLOCK  = 0x3a;
const Input::TInputCode Input::IC_SCROLLOCK = 0x46;
const Input::TInputCode Input::IC_LSHIFT    = 0x2a;
const Input::TInputCode Input::IC_RSHIFT    = 0x36;
const Input::TInputCode Input::IC_LCTRL     = 0x1d;
const Input::TInputCode Input::IC_RCTRL     = 0x9d;
const Input::TInputCode Input::IC_LALT      = 0x38;
const Input::TInputCode Input::IC_RALT      = 0xb8;
const Input::TInputCode Input::IC_LSUPER    = 0xdb; // Left "Windows" key
const Input::TInputCode Input::IC_RSUPER    = 0xdc; // Right "Windows" key

// other keys (cursor control, punctuation)
const Input::TInputCode Input::IC_ESCAPE    = 0x01;
const Input::TInputCode Input::IC_PRINT     = 0xb7;
const Input::TInputCode Input::IC_PAUSE     = 0xc5;
const Input::TInputCode Input::IC_GRAVE     = 0x29;
const Input::TInputCode Input::IC_MINUS     = 0x0c;
const Input::TInputCode Input::IC_EQUALS    = 0x0d;
const Input::TInputCode Input::IC_BACKSLASH = 0x2b;
const Input::TInputCode Input::IC_BACKSPACE = 0x0e;

const Input::TInputCode Input::IC_TAB       = 0x0f;
const Input::TInputCode Input::IC_LBRACKET  = 0x1a;
const Input::TInputCode Input::IC_RBRACKET  = 0x1b;
const Input::TInputCode Input::IC_RETURN    = 0x1c;

const Input::TInputCode Input::IC_SEMICOLON = 0x27;
const Input::TInputCode Input::IC_APOSTROPHE= 0x28;

const Input::TInputCode Input::IC_OEM_102   = 0x56; // German <>|
const Input::TInputCode Input::IC_COMMA     = 0x33;
const Input::TInputCode Input::IC_PERIOD    = 0x34;
const Input::TInputCode Input::IC_SLASH     = 0x35;

const Input::TInputCode Input::IC_SPACE     = 0x39;

// mouse buttons
const Input::TInputCode Input::IC_MOUSE_LEFT        = 0x1000;       // left
const Input::TInputCode Input::IC_MOUSE_RIGHT       = 0x1001;       // right
const Input::TInputCode Input::IC_MOUSE_MIDDLE      = 0x1002;       // middle

//mouse axis
const Input::TInputCode Input::IC_AXISX     = 0x2000;
const Input::TInputCode Input::IC_AXISY     = 0x2001;
const Input::TInputCode Input::IC_AXISZ     = 0x2002;

// timer
const Input::TInputCode Input::IC_AXIST = 0x3000;



