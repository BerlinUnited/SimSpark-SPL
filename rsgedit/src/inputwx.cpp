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
n*/
#include "inputwx.h"
#include <wx/event.h>

using namespace kerosin;

wxInput::wxInput(const wxKeyEvent& event, bool keyPress)
    : Input(eButton)
{
    if (keyPress)
        {
            SetKeyPress();
        } else
        {
            SetKeyRelease();
        }

    int wxKey = event.GetKeyCode();

    switch (wxKey)
        {
        case '1':
            mCode = Input::IC_1;
            break;

            // numbers
        case '2':
            mCode = Input::IC_2;
            break;

        case '3':
            mCode = Input::IC_3;
            break;

        case '4':
            mCode = Input::IC_4;
            break;

        case '5':
            mCode = Input::IC_5;
            break;

        case '6':
            mCode = Input::IC_6;
            break;

        case '7':
            mCode = Input::IC_7;
            break;

        case '8':
            mCode = Input::IC_8;
            break;

        case '9':
            mCode = Input::IC_9;
            break;

        case '0':
            mCode = Input::IC_0;
            break;

            // function keys
        case WXK_F1:
            mCode = Input::IC_F1;
            break;


        case WXK_F2:
            mCode = Input::IC_F2;
            break;


        case WXK_F3:
            mCode = Input::IC_F3;
            break;


        case WXK_F4:
            mCode = Input::IC_F4;
            break;


        case WXK_F5:
            mCode = Input::IC_F5;
            break;


        case WXK_F6:
            mCode = Input::IC_F6;
            break;


        case WXK_F7:
            mCode = Input::IC_F7;
            break;


        case WXK_F8:
            mCode = Input::IC_F8;
            break;


        case WXK_F9:
            mCode = Input::IC_F9;
            break;


        case WXK_F10:
            mCode = Input::IC_F10;
            break;


        case WXK_F11:
            mCode = Input::IC_F11;
            break;


        case WXK_F12:
            mCode = Input::IC_F12;
            break;


        case 'A':
            mCode = Input::IC_A;
            break;

        case 'B':
            mCode = Input::IC_B;
            break;

        case 'C':
            mCode = Input::IC_C;
            break;

        case 'D':
            mCode = Input::IC_D;
            break;

        case 'E':
            mCode = Input::IC_E;
            break;

        case 'F':
            mCode = Input::IC_F;
            break;

        case 'G':
            mCode = Input::IC_G;
            break;

        case 'H':
            mCode = Input::IC_H;
            break;

        case 'I':
            mCode = Input::IC_I;
            break;

        case 'J':
            mCode = Input::IC_J;
            break;

        case 'K':
            mCode = Input::IC_K;
            break;

        case 'L':
            mCode = Input::IC_L;
            break;

        case 'M':
            mCode = Input::IC_M;
            break;

        case 'N':
            mCode = Input::IC_N;
            break;

        case 'O':
            mCode = Input::IC_O;
            break;

        case 'P':
            mCode = Input::IC_P;
            break;

        case 'Q':
            mCode = Input::IC_Q;
            break;

        case 'R':
            mCode = Input::IC_R;
            break;

        case 'S':
            mCode = Input::IC_S;
            break;

        case 'T':
            mCode = Input::IC_T;
            break;

        case 'U':
            mCode = Input::IC_U;
            break;

        case 'V':
            mCode = Input::IC_V;
            break;

        case 'W':
            mCode = Input::IC_W;
            break;

        case 'X':
            mCode = Input::IC_X;
            break;

        case 'Y':
            mCode = Input::IC_Y;
            break;

        case 'Z':
            mCode = Input::IC_Z;
            break;

            // keypad
            // we can't distinguish between 0 and 'numpad 0' with the WXK
            // constants
            //     mCode = Input::IC_KP0;
            //     mCode = Input::IC_KP1;
            //     mCode = Input::IC_KP2;
            //     mCode = Input::IC_KP3;
            //     mCode = Input::IC_KP4;
            //     mCode = Input::IC_KP5;
            //     mCode = Input::IC_KP6;
            //     mCode = Input::IC_KP7;
            //     mCode = Input::IC_KP8;
            //     mCode = Input::IC_KP9;

        case WXK_NUMPAD_DECIMAL:
            mCode = Input::IC_KP_DECIMAL;
            break;


        case WXK_NUMPAD_DIVIDE:
            mCode = Input::IC_KP_DIVIDE;
            break;


        case WXK_NUMPAD_MULTIPLY:
            mCode = Input::IC_KP_MULTIPLY;
            break;

        case WXK_NUMPAD_SUBTRACT:
            mCode = Input::IC_KP_MINUS;
            break;

        case WXK_NUMPAD_ADD:
            mCode = Input::IC_KP_PLUS;
            break;


        case WXK_NUMPAD_ENTER:
            mCode = Input::IC_KP_ENTER;
            break;

            // arrows + home/end pad
        case WXK_UP:
            mCode = Input::IC_UP;
            break;

        case WXK_DOWN:
            mCode = Input::IC_DOWN;
            break;

        case WXK_LEFT:
            mCode = Input::IC_LEFT;
            break;

        case WXK_RIGHT:
            mCode = Input::IC_RIGHT;
            break;

        case WXK_INSERT:
            mCode = Input::IC_INSERT;
            break;

        case WXK_DELETE:
            mCode = Input::IC_DELETE;
            break;

        case WXK_HOME:
            mCode = Input::IC_HOME;
            break;

        case WXK_END:
            mCode = Input::IC_END;
            break;

        case WXK_PAGEUP:
            mCode = Input::IC_PAGEUP;
            break;

        case WXK_PAGEDOWN:
            mCode = Input::IC_PAGEDOWN;
            break;


            // key state modifier keys
        case WXK_NUMLOCK:
            mCode = Input::IC_NUMLOCK;
            break;

            // Input::IC_CAPSLOCK;

            // Input::IC_SCROLLOC;

        case WXK_SHIFT:
            mCode = Input::IC_LSHIFT;
            break;

            // Input::IC_RSHIFT;

        case WXK_CONTROL:
            mCode = Input::IC_LCTRL;
            break;

            // Input::IC_RCTRL;

        case WXK_MENU:
            mCode = Input::IC_LALT;
            break;

            // Input::IC_RALT;
            // Input::IC_LSUPER;
            // Input::IC_RSUPER;

            // other keys (cursor control, punctuation)
        case WXK_ESCAPE:
            mCode = Input::IC_ESCAPE;
            break;

        case WXK_PRINT:
            mCode = Input::IC_PRINT;
            break;

        case WXK_PAUSE:
            mCode = Input::IC_PAUSE;
            break;

            // Input::IC_GRAVE;

        case WXK_SUBTRACT:
            mCode = Input::IC_MINUS;
            break;

            // Input::IC_EQUALS;

        case '\\':
            mCode = Input::IC_BACKSLASH;
            break;

            // Input::IC_BACKSPAC;

        case WXK_TAB:
            mCode = Input::IC_TAB;
            break;

            // Input::IC_LBRACKET;
            // Input::IC_RBRACKET;

        case WXK_RETURN:
            mCode = Input::IC_RETURN;
            break;

        case ';':
            mCode = Input::IC_SEMICOLON;
            break;

        case '`':
            mCode = Input::IC_APOSTROPHE;

        case ',':
            mCode = Input::IC_COMMA;
            break;

        case '.':
            mCode = Input::IC_PERIOD;
            break;

        case '/':
            mCode = Input::IC_SLASH;
            break;

        case ' ':
            mCode = Input::IC_SPACE;
            break;
        }
}
