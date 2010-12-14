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

#ifndef ZEITGEIST_GCVALUE_H
#define ZEITGEIST_GCVALUE_H

typedef unsigned long VALUE;
typedef unsigned long ID;

#include <string>
#include <zeitgeist/zeitgeist_defines.h>

#ifndef STR2CSTR
#define STR2CSTR StringValueCStr
#endif

namespace zeitgeist
{
/** \class GCValue is a ruby VALUE wrapper. It is responsible to
    register and unregister the wrapped value as busy with the ruby
    garbage collector. The ruby garbage collector might be
    transparently invoked on any call to ruby. If a returned VALUE
    must stay valid during a series of ruby function calls you should
    use this wrapper. A simple use case is a VALUE returned from ruby
    that is used as an argument in a subsequent function call. This
    wrapper also provides some helpers to access and convert the
    stored value.
*/
class ZEITGEIST_API GCValue
{
public:
    GCValue();
    GCValue(const GCValue& value);
    GCValue(VALUE v);

    /** constructs the corresponding ruby boolean */
    GCValue(bool b);

    /** constructs a new ruby string object */
    GCValue(const std::string& str);

    /** constructs a new ruby string object */
    GCValue(const char* str);

    /** constructs a new ruby float object */
    GCValue(float f);

    /** constructs a new riby int object */
    GCValue(int i);

    ~GCValue();

    void operator = (const GCValue& value);
    void operator = (const VALUE& value);

    /** returns the stored ruby VALUE */
    VALUE Get();

    /** sets the setored value to v. Any previously stored value is
        properly unregistered
    */
    void Set(VALUE v);

    /** returns true if the stored value equals QNil */
    bool IsNil();

    /** tries to convert the stored value to an integer value, returns
        true on success
    */
    bool GetInt(int& value);

    /** tries to convert the stored value to a float value, returns
        true on success
    */
    bool GetFloat(float& value);

    /** tries to convert the stored value to a bool value, returns
        true on success
    */
    bool GetBool(bool& value);

    /** tries to convert the stored value to a string, returns true on
        success
    */
    bool GetString(std::string& value);

protected:
    /** unregisters the stored VALUE from the ruby garbage collector,
        i.e marks mValue no longer as busy
    */
    void GC_Unregister();

    /** registers the stored VALUE to the ruby garbage collector,
        i.e. marks it as busy
    */
    void GC_Register();

protected:
    /** the wrapped ruby VALUE */
    VALUE mValue;
};

} // namespace zeitgeist

#endif // ZEITGEIST_GCVALUE_H
