/* -*-c++-*- ***************************************************************
                             consolebinding.cc
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
#include "consolebinding.h"

using namespace std;

ConsoleBinding::ConsoleBinding()
{
}

ConsoleBinding::ConsoleBinding(int id, const std::string& key, 
                               const std::string& value) :
    M_id(id), M_key(key), M_value(value)
{
}

int
ConsoleBinding::getId() const
{
    return M_id;
}

void
ConsoleBinding::setId(int id)
{
    M_id = id;
}

string
ConsoleBinding::getKey() const
{
    return M_key;
}

void
ConsoleBinding::setKey(const string& key)
{
    M_key = key;
}

string
ConsoleBinding::getValue() const
{
    return M_value;
}

void
ConsoleBinding::setValue(const string& value)
{
    M_value = value;
}

bool
ConsoleBinding::isIdEqualTo(int id) const
{
    return M_id == id;
}

bool
ConsoleBinding::isKeyEqualTo(const string& key) const
{
    return M_key == key;
}

bool
ConsoleBinding::overlaps(const string& key)
{
    if (M_key == key)
    {
        // keys are equal
        return true;
    }
    else
    {

        if ((M_key.length() == 0) || (key.length() == 0))
        {
            // one key is the empty string???
            return false;
        }

        // switch based on M_key's prefix
        switch (M_key.at(0))
        {
            case '+':
                switch (key.at(0))
                {
                    case '+':
                        // we did already check that the two keys are not equal
                        return false;   
                    case '-':
                        return false;
                    default:
                        return M_key.substr(1) == key;
                }
            case '-':
                switch (key.at(0))
                {
                    case '+':
                        return false;
                    case '-':
                        // we did already check that the two keys are not equal
                        return false;   
                    default:
                        return M_key.substr(1) == key;
                }
            default:
                switch (key.at(0))
                {
                    case '+':
                        return M_key == key.substr(1);
                    case '-':
                        return M_key == key.substr(1);
                    default:
                        // we did already check that the two keys are not equal
                        return false;  
                }
        }
    }
}
