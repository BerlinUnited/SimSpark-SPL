/* -*-c++-*- ***************************************************************
                             consolebindings.cc
                           ------------------------
    begin                : Aug 16 2002  Oliver Obst
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software   *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/
#include "consolebindings.h"

#if 0
#include <inputserver.h>
#include <baseinputdevice.h>
#endif

#include <algorithm>
#include <functional>

using namespace std;

ConsoleBindings::ConsoleBindings()
{
    // Initialize the number of the next user defined keybinding id
    M_lastBinding = -1;
}

ConsoleBindings::~ConsoleBindings()
{
}

bool
ConsoleBindings::bind(const string& key, const string& value)
{
    // decrease the binding id for this next console binding
    --M_lastBinding;

    return doBind(key, M_lastBinding, value);
}

bool
ConsoleBindings::bind(const std::string& key, int id, const string& name)
{
    // a binding to a command must be > 0
    if (id < 0)
    {
        return false;
    }

    return doBind(key, id, name);
}

bool
ConsoleBindings::exists(const std::string& key) const
{
    /*
       // the STL way: ConsoleBinding::isKeyEqualTo (no const& argument) 
       // has to be modified for this to work
       Bindings::const_iterator iter =
       find_if(M_bindings.begin(), M_bindings.end(), 
               bind2nd(mem_fun_ref(&ConsoleBinding::isKeyEqualTo), key));

       return iter != M_bindings.end();
     */

    for (Bindings::const_iterator iter = M_bindings.begin();
         iter != M_bindings.end(); ++iter)
    {
        if ((*iter).isKeyEqualTo(key))
        {
            return true;
        }
    }

    return false;
}

bool
ConsoleBindings::exists(int id) const
{
    // the STL way:
    Bindings::const_iterator iter =
        find_if(M_bindings.begin(), M_bindings.end(),
                bind2nd(mem_fun_ref(&ConsoleBinding::isIdEqualTo), id));

    return iter != M_bindings.end();

    /*
       for (Bindings::const_iterator iter = M_bindings.begin(); 
            iter != M_bindings.end(); ++iter)
       {
           if ((*iter).isIdEqualTo(id))
           {
               return true;
           }
       }

       return false;
    */
}

string
ConsoleBindings::get(const std::string& key) const
{
    for (Bindings::const_iterator iter = M_bindings.begin(); 
         iter != M_bindings.end();
         ++iter)
    {
        if ((*iter).isKeyEqualTo(key))
        {
            return (*iter).getValue();
        }
    }

    return "";
}

string
ConsoleBindings::get(int id) const
{
    // the STL way:
    Bindings::const_iterator iter =
        find_if(M_bindings.begin(), M_bindings.end(),
                bind2nd(mem_fun_ref(&ConsoleBinding::isIdEqualTo), id));

    if (iter == M_bindings.end())
    {
        return "";
    }

    return (*iter).getValue();

    /*
       for (Bindings::const_iterator iter = M_bindings.begin(); 
            iter != M_bindings.end(); ++iter)
       {
           if ((*iter).isIdEqualTo(id))
           {
               return (*iter).getValue();
           }
       }

       return false;
     */
}

ConsoleBindings::StringList 
ConsoleBindings::getBindings() const
{
    StringList bindings;
    for (Bindings::const_iterator iter = M_bindings.begin(); 
         iter != M_bindings.end();
         ++iter)
    {
        bindings.push_back((*iter).getKey());
    }

    return bindings;
}

bool
ConsoleBindings::unbind(const std::string& key)
{
    bool deleted = false;
#if 0
    for (Bindings::iterator iter = M_bindings.begin(); 
         iter != M_bindings.end(); )
    {
        // we can't check for string equality as a 'unbind +a' 
        // must be deleted by a 'unbind a'
        if ((*iter).overlaps(key))
        {
            // unbind the binding from all devices
            BaseInputServer::DeviceList::iterator iter_device;
            if (InputServer::instance().getFirstDevice(iter_device))
            {
                do
                {
                    // only consider binding devices
                    BaseInputDevice::eClass deviceClass =
                        (*iter_device)->getDeviceClass();
                    if ((deviceClass == BaseInputDevice::IDC_KEYBOARD) ||
                        (deviceClass == BaseInputDevice::IDC_MOUSE) ||
                        (deviceClass == BaseInputDevice::IDC_JOYSTICK))
                    {
                        (*iter_device)->unbindKey((*iter).getKey().c_str());
                    }
                }
                while (InputServer::instance().getNextDevice(iter_device));
            }

            iter = M_bindings.erase(iter);
            deleted = true;
        }
        else
        {
            ++iter;
        }
    }
#endif
    return deleted;
}

bool
ConsoleBindings::doBind(const string& key, int id, const string& name)
{
    if (!bindToDevice(key, id))
    {
        return false;
    }

    store(id, key, name);

    return true;
}

bool
ConsoleBindings::bindToDevice(const string& key, int id) const
{
    bool success = false;
#if 0
    BaseInputServer::DeviceList::iterator iter_device;
    if (InputServer::instance().getFirstDevice(iter_device))
    {
        do
        {
            // only consider binding devices
            BaseInputDevice::eClass deviceClass =
                (*iter_device)->getDeviceClass();
            if ((deviceClass == BaseInputDevice::IDC_KEYBOARD) ||
                (deviceClass == BaseInputDevice::IDC_MOUSE) ||
                (deviceClass == BaseInputDevice::IDC_JOYSTICK))
            {
                success = (*iter_device)->bind(key.c_str(), id);
            }
        }
        while (!success && InputServer::instance().getNextDevice(iter_device));
    }
#endif
    return success;
}

void
ConsoleBindings::store(int id, const string& key, const string& value)
{
    unbind(key);

    M_bindings.push_back(ConsoleBinding(id, key, value));
}
