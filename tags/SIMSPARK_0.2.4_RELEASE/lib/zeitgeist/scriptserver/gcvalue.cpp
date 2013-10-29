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
#include "rubywrapper.h" // might define STR2CSTR
#include "gcvalue.h"
#include <sstream>

using namespace zeitgeist;
using namespace std;

GCValue::GCValue()
{
    mValue = Qnil;
}

GCValue::GCValue(const GCValue& value)
{
    mValue = Qnil;
    Set(value.mValue);
}

GCValue::GCValue(VALUE v)
{
    mValue = Qnil;
    Set(v);
}

GCValue::GCValue(bool b)
{
    mValue = Qnil;
    Set(b ? Qtrue:Qfalse);
}

GCValue::GCValue(const string& str)
{
    mValue = Qnil;
    Set(rb_str_new2(str.c_str()));
}

GCValue::GCValue(const char* str)
{
    mValue = Qnil;
    Set(rb_str_new2(str));
}

GCValue::GCValue(float f)
{
    mValue = Qnil;
    Set(rb_float_new(f));
}

GCValue::GCValue(int i)
{
    mValue = Qnil;
    Set(rb_int_new(i));
}

GCValue::~GCValue()
{
    GC_Unregister();
}

void GCValue::operator = (const GCValue& value)
{
    mValue = Qnil;
    Set(value.mValue);
}

void GCValue::operator = (const VALUE& value)
{
    mValue = Qnil;
    Set(value);
}

VALUE GCValue::Get()
{
    return mValue;
}

void GCValue::Set(VALUE v)
{
    GC_Unregister();
    mValue = v;
    GC_Register();
}

bool GCValue::IsNil()
{
    return NIL_P(mValue);
}

bool GCValue::GetInt(int& value)
{
    if (IsNil())
        {
            return false;
        }

    value = NUM2INT(mValue);
    return true;
}

bool GCValue::GetFloat(float& value)
{
    if (IsNil())
        {
            return false;
        }

    value = (float)NUM2DBL(mValue);
    return true;
}

bool GCValue::GetBool(bool& value)
{
    if (IsNil())
        {
            return false;
        }

    switch(TYPE(mValue))
        {
        case T_TRUE :
            value = true;
            return true;

        case T_FALSE :
            value = false;
            return true;

        default:
            return false;
        }
}

bool GCValue::GetString(std::string& value)
{
    if (IsNil())
        {
            return false;
        }

    switch(TYPE(mValue))
        {
        case T_STRING:
            {
                value = STR2CSTR(mValue);
                return true;
            }

        case T_FLOAT:
            {
                stringstream ss;
                ss << (float)NUM2DBL(mValue);
                value = ss.str();
                return true;
            }

        case T_FIXNUM:
            {
                stringstream ss;
                ss << NUM2INT(mValue);
                value = ss.str();
                return true;
            }

        default:
            break;
        }

    return false;
}

void GCValue::GC_Unregister()
{
    if (! IsNil())
        {
            rb_gc_unregister_address(&mValue);
        }
}

void GCValue::GC_Register()
{
    if (! IsNil())
        {
            rb_gc_register_address(&mValue);
        }
}
