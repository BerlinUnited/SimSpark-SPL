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

#ifndef FUNCTIONAPPENDER_H
#define FUNCTIONAPPENDER_H
/**
 * \file functionappender.h
 * \brief Contains FunctionAppender class, which formats log messages and calls a registered function with the message as parameter.
 */

#include "abstractstringappender.h"

#include <QString>

/*!
 \brief Sends the log records to a registered (static/global) function.

 The target function is specified using a function pointer. The caller has to make sure he unregisters the FunctionAppender using
 Logger::unregisterAppender() before the function pointer becomes invalid.

 In case a more detailed message (with more meta information like timestamp etc.) or to call a non-static function instead a
 MessageReceiverAppender should be used.

 \sa MessageReceiverAppender
*/
class FunctionAppender : public AbstractStringAppender
{
  public:
    /*!
     \brief Basic constructor.

     \param callback callback function
    */
    FunctionAppender(void (*callback)(QString, Logger::LogLevel));

  protected:
    //! Calls the target function
    /**
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, unsigned int source, const QString& message);

  private:

    void (*mFunction)(QString, Logger::LogLevel); /*!< function to call */
};

#endif // FUNCTIONAPPENDER_H
