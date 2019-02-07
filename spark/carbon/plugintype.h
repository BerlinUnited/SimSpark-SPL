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

#ifndef PLUGINTYPE_H
#define PLUGINTYPE_H

/**
 * \file plugintype.h
 * \brief Contains EPluginType enum and PluginType class, which contains static utility functions for plugin type to string conversion.
 */

#include <QString>

/*!
 \brief Types of plugins.
*/
enum EPluginType
{
    PT_NORMAL, /*!< Normal and simple plugin. */
    PT_SIGNAL, /*!< SignalPlugin derived plugin with signaling.*/
    PT_FRAME,  /*!< AttachableFrame derived plugin with visual representation and signaling. */
    PT_NONE    /*!< Illefal type. */
};

/*!
 \brief Class with static conversion functions for the plugin type data.
*/
class PluginType
{
public:
    //public functions

    /*!
     \brief Convert a Plugin Type to a string.

     \param type type to convert
     \param uppercase if true, return value is in uppercase
     \return type as qstring in uppercase letters
    */
    static QString getTypeString(EPluginType type, bool uppercase = true)
    {
        switch(type)
        {
        case PT_NORMAL:
            return uppercase ? "NORMAL" : "Normal";
            break;
        case PT_SIGNAL:
            return uppercase ? "SIGNAL" : "Signal";
            break;
        case PT_FRAME:
            return uppercase ? "FRAME" : "Frame";
            break;
        default:
            return uppercase ? "NONE" : "None";
            break;
        }
    }

    static const EPluginType mTypes[3]; /*!< List of all plugin types. Useful to iterate. */
    static const int mMaxTypes; /*!< Amount of different plugin types. Useful to iterate. */
};

#endif //PLUGINTYPE_H
