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
#ifndef ZEITGEIST_RUBYWRAPPER_H
#define ZEITGEIST_RUBYWRAPPER_H

//
// Both <ruby.h> and "config.h" define PACKAGE_ constants.
// To suppress compiler warnings about redefinitions they
// are #undef'ed in this wrapper
//

#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#ifndef __GNUC__
#define EXTERN extern __declspec(dllimport)
#endif

#ifdef WIN32
#include <winsock2.h>

#ifndef __MINGW32__
// disable compiler warning about type cast from VALUE to RBasic*
#pragma warning (disable : 4312)
#endif
#endif

#include <ruby.h>

#ifdef WIN32
#undef bind
#undef listen
#undef accept
#undef connect
#undef close
#undef fclose
#undef recv
#undef socket
#undef send
#undef read
#undef write
#undef Sleep

#ifndef __MINGW32__
// reenable compiler warning
#pragma warning (default : 4312)
#endif
#endif

#undef EXTERN

#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include <iostream>

namespace zeitgeist
{
    /** RbArguments is a structure that describes a ruby function
        call.
        \param receiver is the ruby object that receives the function call
        \param id is the ruby id of the receiver member function
        \param n is the number of parameters passed
        \param *argv is a pointer to an array containnig the function
        parameters
    */
    struct RbArguments
    {
        VALUE receiver;
        ID id;
        int n;
        VALUE *argv;

        RbArguments(VALUE r, ID id, int n, VALUE *argv) :
            receiver(r), id(id), n(n), argv(argv) {};
    };

    /** a functor for the rb_protect function, used to safely excecute
        ruby code */
    VALUE RbFuncallWrap(VALUE arg);

    /** calls a safe rb_eval_string variant and prints any ruby error
        messages along with a backtrace to stdout
     */
    VALUE RbEvalStringWrap(const std::string& str);

    /** calls a safe rb_eval_string variant and prints any ruby error
        messages along with a backtrace to stdout. The error code
        returned from ruby is stored in the 'error' parameter.
     */
    VALUE RbEvalStringWrap(const std::string& str, int& error);

    /** qeuries ruby for a string that describes the last error */
    std::string RbGetError();

    /** prints the last ruby error to stdout along with a backtrace */
    void RbPrintError();
};

#endif // ZEITGEIST_RUBYWRAPPER_H

