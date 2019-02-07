/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "inputqt.h"
#include <QKeyEvent>

using namespace kerosin;
using namespace Qt;

QTInput::QTInput(const QKeyEvent& e, bool keyPress)
    : Input(eButton)
{
    if (keyPress)
        SetKeyPress();
    else
        SetKeyRelease();

    //modifier
    Qt::KeyboardModifiers mod = e.modifiers();
    mModState = Input::eNone;
    if (mod & Qt::ShiftModifier)   mModState |= Input::eShift;
    if (mod & Qt::ControlModifier) mModState |= Input::eCtrl;
    if (mod & Qt::AltModifier)     mModState |= Input::eAlt;

    //key
    int qtKey = e.key();
    switch (qtKey)
    {
    // numbers
    case '0': mCode = Input::IC_0; break;
    case '1': mCode = Input::IC_1; break;
    case '2': mCode = Input::IC_2; break;
    case '3': mCode = Input::IC_3; break;
    case '4': mCode = Input::IC_4; break;
    case '5': mCode = Input::IC_5; break;
    case '6': mCode = Input::IC_6; break;
    case '7': mCode = Input::IC_7; break;
    case '8': mCode = Input::IC_8; break;
    case '9': mCode = Input::IC_9; break;

    // function keys
    case Qt::Key_F1 : mCode = Input::IC_F1;  break;
    case Qt::Key_F2 : mCode = Input::IC_F2;  break;
    case Qt::Key_F3 : mCode = Input::IC_F3;  break;
    case Qt::Key_F4 : mCode = Input::IC_F4;  break;
    case Qt::Key_F5 : mCode = Input::IC_F5;  break;
    case Qt::Key_F6 : mCode = Input::IC_F6;  break;
    case Qt::Key_F7 : mCode = Input::IC_F7;  break;
    case Qt::Key_F8 : mCode = Input::IC_F8;  break;
    case Qt::Key_F9 : mCode = Input::IC_F9;  break;
    case Qt::Key_F10: mCode = Input::IC_F10; break;
    case Qt::Key_F11: mCode = Input::IC_F11; break;
    case Qt::Key_F12: mCode = Input::IC_F12; break;

    //Letter keys
    case 'A': mCode = Input::IC_A; break;
    case 'B': mCode = Input::IC_B; break;
    case 'C': mCode = Input::IC_C; break;
    case 'D': mCode = Input::IC_D; break;
    case 'E': mCode = Input::IC_E; break;
    case 'F': mCode = Input::IC_F; break;
    case 'G': mCode = Input::IC_G; break;
    case 'H': mCode = Input::IC_H; break;
    case 'I': mCode = Input::IC_I; break;
    case 'J': mCode = Input::IC_J; break;
    case 'K': mCode = Input::IC_K; break;
    case 'L': mCode = Input::IC_L; break;
    case 'M': mCode = Input::IC_M; break;
    case 'N': mCode = Input::IC_N; break;
    case 'O': mCode = Input::IC_O; break;
    case 'P': mCode = Input::IC_P; break;
    case 'Q': mCode = Input::IC_Q; break;
    case 'R': mCode = Input::IC_R; break;
    case 'S': mCode = Input::IC_S; break;
    case 'T': mCode = Input::IC_T; break;
    case 'U': mCode = Input::IC_U; break;
    case 'V': mCode = Input::IC_V; break;
    case 'W': mCode = Input::IC_W; break;
    case 'X': mCode = Input::IC_X; break;
    case 'Y': mCode = Input::IC_Y; break;
    case 'Z': mCode = Input::IC_Z; break;

    // keypad
    // we can't distinguish between 0 and 'numpad 0' with Qt constants
    // case : mCode = Input::IC_KP0; break
    // case : mCode = Input::IC_KP1; break
    // case : mCode = Input::IC_KP2; break
    // case : mCode = Input::IC_KP3; break
    // case : mCode = Input::IC_KP4; break
    // case : mCode = Input::IC_KP5; break
    // case : mCode = Input::IC_KP6; break
    // case : mCode = Input::IC_KP7; break
    // case : mCode = Input::IC_KP8; break
    // case : mCode = Input::IC_KP9; break

    //case Qt::Key_Period:      mCode = Input::IC_KP_DECIMAL; break; //in non-kp
    case Qt::Key_division:    mCode = Input::IC_KP_DIVIDE; break;
    case Qt::Key_multiply:    mCode = Input::IC_KP_MULTIPLY; break;
    //case Qt::Key_Minus:       mCode = Input::IC_KP_MINUS; break; //in non-kp
    case Qt::Key_Plus:        mCode = Input::IC_KP_PLUS; break;
    case Qt::Key_Enter:       mCode = Input::IC_KP_ENTER; break;

    // arrows + home/end pad
    case Qt::Key_Up:       mCode = Input::IC_UP;       break;
    case Qt::Key_Down:     mCode = Input::IC_DOWN;     break;
    case Qt::Key_Left:     mCode = Input::IC_LEFT;     break;
    case Qt::Key_Right:    mCode = Input::IC_RIGHT;    break;
    case Qt::Key_Insert:   mCode = Input::IC_INSERT;   break;
    case Qt::Key_Delete:   mCode = Input::IC_DELETE;   break;
    case Qt::Key_Home:     mCode = Input::IC_HOME;     break;
    case Qt::Key_End:      mCode = Input::IC_END;      break;
    case Qt::Key_PageUp:   mCode = Input::IC_PAGEUP;   break;
    case Qt::Key_PageDown: mCode = Input::IC_PAGEDOWN; break;

    // key state modifier keys
    case Qt::Key_NumLock:    mCode = Input::IC_NUMLOCK; break;
    case Qt::Key_CapsLock:   mCode = Input::IC_CAPSLOCK; break;
    case Qt::Key_ScrollLock: mCode = Input::IC_SCROLLOCK; break;
    case Qt::Key_Shift:      mCode = Input::IC_LSHIFT; break;
    //case Qt::Key_:         mCode = Input::IC_RSHIFT; break; //no right shift in qt
    case Qt::Key_Control:    mCode = Input::IC_LCTRL; break;
    //case Qt::Key_:         mCode = Input::IC_RCTRL; break; //no right control in qt
    case Qt::Key_Alt:        mCode = Input::IC_LALT; break;
    case Qt::Key_AltGr:      mCode = Input::IC_RALT; break;
    case Qt::Key_Super_L:    mCode = Input::IC_LSUPER; break;
    case Qt::Key_Super_R:    mCode = Input::IC_RSUPER; break;

    // other keys (cursor control, punctuation)
    case Qt::Key_Escape: mCode = Input::IC_ESCAPE; break;
    case Qt::Key_Print: mCode = Input::IC_PRINT; break;
    case Qt::Key_Pause: mCode = Input::IC_PAUSE; break;
    case Qt::Key_Agrave: mCode = Input::IC_GRAVE; break;
    case Qt::Key_Minus: mCode = Input::IC_MINUS; break;
    case Qt::Key_Equal: mCode = Input::IC_EQUALS; break;

    case Qt::Key_Backslash: mCode = Input::IC_BACKSLASH; break;
    case Qt::Key_Backspace: mCode = Input::IC_BACKSPACE; break;
    case Qt::Key_Tab: mCode = Input::IC_TAB; break;
    case Qt::Key_BracketLeft : mCode = Input::IC_LBRACKET; break;
    case Qt::Key_BracketRight: mCode = Input::IC_RBRACKET; break;
    case Qt::Key_Return: mCode = Input::IC_RETURN; break;

    case Qt::Key_Semicolon: mCode = Input::IC_SEMICOLON; break;
    case Qt::Key_Apostrophe: mCode = Input::IC_APOSTROPHE; break;
    case Qt::Key_Comma: mCode = Input::IC_COMMA; break;
    case Qt::Key_Period: mCode = Input::IC_PERIOD; break;
    case Qt::Key_Slash: mCode = Input::IC_SLASH; break;
    case Qt::Key_Space: mCode = Input::IC_SPACE; break;
    }
}
