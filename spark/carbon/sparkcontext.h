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

#ifndef SPARKCONTEXT_H
#define SPARKCONTEXT_H
/**
 \file sparkcontext.h
 \brief Contains SparkContext class.
*/

#include <QMenu>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif

namespace zeitgeist
{
    class Leaf;
}

/*!
 \brief Manages a selection of a zeitgeist::Leaf tht can be shared and creates a context menu.

 \todo Class is incomplete because it was not needed yet. The only selection sharing is done between monitor and scene graph plugin via
 the relative path. For more complex management of selections and creation of context menus for example in the gui monitor or for
 management (storing/loading/searching) of different selections this class should be completed with multiple selection, selection types
 and so on.
*/
class SparkContext
{
public:
    //public functions

    /*!
     \brief Default constructor.
    */
    SparkContext();

    /*!
     \brief Destructor.
    */
    ~SparkContext();

    /*!
     \brief Set the selected node.
    */
    void setSelection(boost::weak_ptr<zeitgeist::Leaf> node);
    /*!
     \brief Returns the selected node.
    */
    boost::weak_ptr<zeitgeist::Leaf> getSelection() const;
    /*!
     \brief Resets the selection.
    */
    void resetSelection();
    /*!
     \brief Returns true if there is a selection.
    */
    bool hasSelection();

    /*!
     \brief Creates a context menu for the current selection.
    */
    QMenu* getContextMenu();
    /*!
     \brief Creates a context menu for the given leaf.
    */
    static QMenu* getContextMenu(boost::weak_ptr<zeitgeist::Leaf> leaf);

    /*!
     \brief Returns the node the current context menu refers to.
    */
    boost::weak_ptr<zeitgeist::Leaf> getContextNode() const;

protected:
    //protected members

    boost::weak_ptr<zeitgeist::Leaf> mSelectedNode; /*!< The current selection. */
    boost::weak_ptr<zeitgeist::Leaf> mContextNode;  /*!< The node that context menu commands refer to. */
};

#endif // SPARKCONTEXT_H
