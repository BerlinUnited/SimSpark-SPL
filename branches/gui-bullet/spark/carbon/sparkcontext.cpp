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

#include "sparkcontext.h"
#include "cutelogger/logger.h"

#include <zeitgeist/leaf.h>
#include <oxygen/sceneserver/scenedict.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;

SparkContext::SparkContext()
{
}

SparkContext::~SparkContext()
{
}


void SparkContext::setSelection(boost::weak_ptr<zeitgeist::Leaf> node)
{
    mSelectedNode = node;
}

boost::weak_ptr<zeitgeist::Leaf> SparkContext::getSelection() const
{
    return mSelectedNode;
}

void SparkContext::resetSelection()
{
    setSelection(boost::weak_ptr<zeitgeist::Leaf>());
}

bool SparkContext::hasSelection()
{
    return (! mSelectedNode.expired());
}

QMenu* SparkContext::getContextMenu()
{
    LOG_WARNING() << "getContextMenu() not implemented yet";
    return new QMenu();
}

QMenu* SparkContext::getContextMenu(boost::weak_ptr<zeitgeist::Leaf> leaf)
{
    LOG_WARNING() << "getContextMenu() not implemented yet";
    return new QMenu();
}

weak_ptr<Leaf> SparkContext::getContextNode() const
{
    return mContextNode;
}
