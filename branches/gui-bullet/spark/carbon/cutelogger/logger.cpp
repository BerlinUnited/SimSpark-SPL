/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

  Copyright (c) 2010 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  Extended by: Patrick Geib
  This file is part of Carbon
  $Id$
*/

// Local
#include "logger.h"
#include "abstractappender.h"
#include "../carbon.h"

// Qt
#include <QCoreApplication>
#include <QReadWriteLock>
#include <QDateTime>
#include <QIODevice>
#include <QDebug>

// STL
#include <iostream>
#include <sstream>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Local class: LogDevice
//--------------------------------------------------------------

/*!
 \brief QIO log device for the logger. Used to continuously stream data to the logger.
*/
class LogDevice : public QIODevice
{
public:
    //public functions

    /*!
     \brief Constructor.
    */
    LogDevice()
    {}

    /*!
     \brief Lock and open the log device.
    */
    void lock(Logger::LogLevel logLevel, const char* file, int line, const char* function, unsigned int source)
    {
        mSelfMutex.lock();

        if (!isOpen())
          open(QIODevice::WriteOnly);

        mLogLevel = logLevel;
        mFile = file;
        mLine = line;
        mFunction = function;
        mSource = source;
    }

protected:
    //protected functions

    /*!
     \brief Read data from device. Not needed and empty.
    */
    qint64 readData(char*, qint64)
    {
        return 0;
    }

    /*!
     \brief Writes data to the device.

     \param data buffer
     \param maxSize buffer size
     \return maximum size
    */
    qint64 writeData(const char* data, qint64 maxSize)
    {
        if (maxSize > 0)
          Logger::write(mLogLevel, mFile, mLine, mFunction, mSource, QString::fromLocal8Bit(QByteArray(data, maxSize)));

        mSelfMutex.unlock();
        return maxSize;
    }

private:
    //private members

    QMutex mSelfMutex;          /*!< Mutex to lock device. */
    Logger::LogLevel mLogLevel; /*!< Message log level. */
    const char* mFile;          /*!< Message source file. */
    int mLine;                  /*!< Message source line. */
    const char* mFunction;      /*!< Message source function. */
    unsigned int mSource;       /*!< Message source id. */
};

//--------------------------------------------------------------
// Local class: LoggerPrivate
//--------------------------------------------------------------

// Forward declarations
static void cleanupLoggerPrivate();
static void qtLoggerMessageHandler(QtMsgType type, const char* msg);

/**
 * \brief LoggerPrivate implements the public interface of the Logger class with the Singleton pattern.
 *
 * LoggerPrivate class implements the Singleton pattern in a thread-safe way. It uses a static pointer to itself
 * protected by QReadWriteLock.
 *
 * The appender list inside the LoggerPrivate class is also protected by QReadWriteLock so this class could be safely
 * used in a multi-threaded application.
 */
class LoggerPrivate
{
public:
    //public functions

    /*!
     \brief Instance a LoggerPrivate.

     \return new logger
    */
    static LoggerPrivate* instance()
    {
        LoggerPrivate* result = 0;
        {        
            QReadLocker locker(&mSelfLock);
            result = mSelf;
        }

        if (!result)
        {
            QWriteLocker locker(&mSelfLock);
            mSelf = new LoggerPrivate;
            qInstallMsgHandler(qtLoggerMessageHandler);
            qAddPostRoutine(cleanupLoggerPrivate);
            result = mSelf;
        }

        return result;
    }


    /*!
     \brief Default Constructor.
    */
    LoggerPrivate()
      : mLogDevice(0),
        mLogSources(),
        mEmptyLogSourceName("unknown source")
    {}


    /*!
     \brief Destructor. Cleans appenders and the log device.
    */
    ~LoggerPrivate()
    {
        // Cleanup appenders
        QReadLocker appendersLocker(&mAppendersLock);
        foreach (AbstractAppender* appender, mAppenders)
            delete appender;

        // Cleanup device
        QReadLocker deviceLocker(&mLogDeviceLock);
        delete mLogDevice;
    }


    /*!
     \brief Register an appender to the logger.

     All messages matching the appenders specification will be sent to him.
    */
    void registerAppender(AbstractAppender* appender)
    {
        QWriteLocker locker(&mAppendersLock);

        if (!mAppenders.contains(appender))
            mAppenders.append(appender);
        else
            std::cerr << "Trying to register appender that was already registered." << std::endl;
    }

    /*!
     \brief Unregister an appender to stop sending messages to it.

     The appender is not deleted.
    */
    void unregisterAppender(AbstractAppender* appender)
    {
        QWriteLocker locker(&mAppendersLock);

        if (!mAppenders.contains(appender))
        {
            // do nothing
        }
        else
            mAppenders.removeOne(appender);
    }

    /*!
     \brief Returns the log device.
    */
    LogDevice* logDevice()
    {
        LogDevice* result = 0;
        {
            QReadLocker locker(&mLogDeviceLock);
            result = mLogDevice;
        }

        if (!result)
        {
            QWriteLocker locker(&mLogDeviceLock);
            mLogDevice = new LogDevice;
            result = mLogDevice;
        }

        return result;
    }


    /*!
     \brief Writes a message to the logger. The message is sent to all appenders.
    */
    void write(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line, const char* function,
               unsigned int source, const QString& message)
    {
        QReadLocker locker(&mAppendersLock);

        if (!mAppenders.isEmpty())
        {
            foreach (AbstractAppender* appender, mAppenders)
                appender->write(timeStamp, logLevel, file, line, function, source, message);
        }
        else
        {
            // Fallback
            QString result = QString(QLatin1String("[%1] <%2> %3")).arg(Logger::levelToString(logLevel), -7)
                                                                 .arg(function).arg(message);

            std::cerr << qPrintable(result) << std::endl;
        }

        if (logLevel == Logger::Fatal)
            abort();
    }


    /*!
     \brief Writes a message to the logger. The message is sent to all appenders.
    */
    void write(Logger::LogLevel logLevel, const char* file, int line, const char* function, unsigned int source, const QString& message)
    {
        write(QDateTime::currentDateTime(), logLevel, file, line, function, source, message);
    }

    /*!
     \brief Writes a message to the logger. The message is sent to all appenders.
    */
    void write(Logger::LogLevel logLevel, const char* file, int line, const char* function, unsigned int source, const char* message)
    {
        write(logLevel, file, line, function, source, QString::fromAscii(message));
    }

    /*!
     \brief Writes a message to the logger. and returs a QDebug object with the current logdevice to stream additional data to.
    */
    QDebug write(Logger::LogLevel logLevel, const char* file, int line, const char* function, unsigned int source)
    {
        LogDevice* d = logDevice();
        d->lock(logLevel, file, line, function, source);
        return QDebug(d).nospace();
    }

    /*!
     \brief Writes a fatal error message to the logger when the assert failed.
    */
    void writeAssert(const char* file, int line, const char* function, const char* condition, unsigned int source)
    {
        write(Logger::Fatal, file, line, function, source, QString("ASSERT: \"%1\"").arg(condition));
    }

    /*!
      \brief Returns the LoggerPrivate instance pointer as LoggerRegistration.

      The pointer can be handed to external libraries when linked to the main application.
      The library import macros take the logger instance pointer and register it in their namespace using registerStaticLogger.
      If the pointer is already set (on platforms/systems with shared static variables) the re-registration will be ignored).

      \warning After the first log call in an external library it will create its own LoggerPrivate instance and cant be removed anymore.
      This means you must not use the Logger and its macros in a library that does not export plugins.
    */
    static Logger::LoggerRegistration getStaticLoggerRegistration()
    {
        return mSelf;
    }

    /*!
      \brief Registers the LoggerPrivate instance pointer (as LoggerRegistration) if it is still 0.

      The pointer can be handed to external libraries when linked to the main application.
      The library import macros take the logger instance pointer and register it in their namespace using registerStaticLogger.
      If the pointer is already set (on platforms/systems with shared static variables) the re-registration will be ignored).

      \warning After the first log call in an external library it will create its own LoggerPrivate instance and cant be removed anymore.
      This means you must not use the Logger and its macros in a library that does not export plugins.
    */
    static void registerStaticLogger(Logger::LoggerRegistration registration)
    {
        if (!mSelf)
        {
            //re-init not possible because it is platform dependant
            //on linux mSelf may already be initialized, so simply ignore this
            //on windows static variables are not shared so they need to be reinitialized in dlls
            QWriteLocker locker(&mSelfLock);
            mSelf = registration;    
        }
    }

    /*!
     \brief Returns the size of the mLogSources list.
    */
    unsigned int getLogSourceCount()
    {
        return mLogSources.size();
    }

    /*!
     \brief Returns the name of the log source at the given index or mEmptyLogSourceName for illegal index.
    */
    const std::string& getLogSourceName(unsigned int index)
    {
        if (index >= mLogSources.size()) 
            return mEmptyLogSourceName;

        return mLogSources.at(index).second;
    }

    /*!
     \brief Returns the index of the log source with the given name or -1 if name was not found.

     Case sensitive.
    */
    unsigned int getLogSource(const std::string& name)
    {
        return defineLogSource(name);
    }

    /*!
     \brief Defines a new log source with the specified name and returns its index or simply returns the index of an existing source.
    */
    unsigned int defineLogSource(const std::string& name)
    {
        for (unsigned int i = 0; i < mLogSources.size(); i++)
        {
            if (mLogSources.at(i).second.compare(name) == 0)
            {
                return mLogSources.at(i).first;
            }
        }

        mLogSources.push_back(std::make_pair(mLogSources.size(), name));
        if (Carbon::get() != 0)
            Carbon::get()->updateLogSourceAdded(mLogSources.size() -1);

        return mLogSources.size()-1;
    }

public:
    //public members

    static LoggerPrivate* mSelf;     /*!< Self pointer. */
    static QReadWriteLock mSelfLock; /*!< Self locker. */

private:
    //private members

    QList<AbstractAppender*> mAppenders; /*!< Appender list. */
    QReadWriteLock mAppendersLock;       /*!< Locker for appenders. */

    LogDevice* mLogDevice;         /*!< Log device. */
    QReadWriteLock mLogDeviceLock; /*!< Locker for log device. */

    std::vector<std::pair<unsigned int, const std::string> > mLogSources; /*!< Only-expanding list (with fixed indices) of log sources. */
    std::string mEmptyLogSourceName;                                      /*!< Default name for illegal log source. */
};

// Static fields initialization
LoggerPrivate* LoggerPrivate::mSelf = 0;
QReadWriteLock LoggerPrivate::mSelfLock;

static void cleanupLoggerPrivate()
{
    QWriteLocker locker(&LoggerPrivate::mSelfLock);

    delete LoggerPrivate::mSelf;
    LoggerPrivate::mSelf = 0;
}

static void qtLoggerMessageHandler(QtMsgType type, const char* msg)
{
    switch (type)
    {
        case QtDebugMsg:
            LOG_DEBUG(msg);
            break;
        case QtWarningMsg:
            LOG_WARNING(msg);
            break;
        case QtCriticalMsg:
            LOG_ERROR(msg);
            break;
        case QtFatalMsg:
            LOG_FATAL(msg);
            break;
    }
}

//--------------------------------------------------------------
// Logger
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

QString Logger::levelToString(Logger::LogLevel logLevel)
{
    switch (logLevel)
    {
        case Debug:
            return QLatin1String("Debug");
        case Info:
            return QLatin1String("Info");
        case Warning:
            return QLatin1String("Warning");
        case Error:
            return QLatin1String("Error");
        case Fatal:
            return QLatin1String("Fatal");
    }

    return QString();
}

Logger::LogLevel Logger::levelFromString(const QString& s)
{
    QString str = s.trimmed().toLower();

    LogLevel result = Debug;

    if (str == QLatin1String("debug"))
        result = Debug;
    else if (str == QLatin1String("info"))
        result = Info;
    else if (str == QLatin1String("warning"))
        result = Warning;
    else if (str == QLatin1String("error"))
        result = Error;
    else if (str == QLatin1String("fatal"))
        result = Fatal;

    return result;
}

void Logger::registerAppender(AbstractAppender* appender)
{
    LoggerPrivate::instance()->registerAppender(appender);
}

void Logger::unregisterAppender(AbstractAppender* appender)
{
    LoggerPrivate::instance()->unregisterAppender(appender);
}

void Logger::write(const QDateTime& timeStamp, LogLevel logLevel, const char* file, int line, const char* function,
                      unsigned int source, const QString& message)
{
    LoggerPrivate::instance()->write(timeStamp, logLevel, file, line, function, source, message);
}

void Logger::write(LogLevel logLevel, const char* file, int line, const char* function, unsigned int source, const QString& message)
{
    LoggerPrivate::instance()->write(logLevel, file, line, function, source, message);
}

void Logger::write(LogLevel logLevel, const char* file, int line, const char* function, unsigned int source, const char* message)
{
    LoggerPrivate::instance()->write(logLevel, file, line, function, source, message);
}

QDebug Logger::write(LogLevel logLevel, const char* file, int line, const char* function, unsigned int source)
{
    return LoggerPrivate::instance()->write(logLevel, file, line, function, source);
}

void Logger::writeAssert(const char* file, int line, const char* function, const char* condition, unsigned int source)
{
  LoggerPrivate::instance()->writeAssert(file, line, function, condition, source);
}

unsigned int Logger::getLogSourceCount()
{
    return LoggerPrivate::instance()->getLogSourceCount();
}

const std::string& Logger::getLogSourceName(unsigned int index)
{
    return LoggerPrivate::instance()->getLogSourceName(index);
}

unsigned int Logger::getLogSource(const std::string& name)
{
    return LoggerPrivate::instance()->getLogSource(name);
}

unsigned int Logger::defineLogSource(const std::string& name)
{
    return LoggerPrivate::instance()->defineLogSource(name);
}

void Logger::registerStaticLogger(Logger::LoggerRegistration registration)
{
    LoggerPrivate::registerStaticLogger(registration);
}

Logger::LoggerRegistration Logger::getStaticLoggerRegistration()
{
    return LoggerPrivate::getStaticLoggerRegistration();
}

