/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _SCANNER_H_
#define _SCANNER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(WIN32)
#pragma warning (disable : 4786) // truncation of debug information
#endif

#include <stdio.h>
#include <map>
#include <list>
#include <string>

/*! \class Scanner
  $Id: scanner.h,v 1.1 2002/08/14 09:24:52 fruit Exp $

    Scanner - a simple tokenizer

    This scanner implentation manages a registry of known tokens. It is able
    to tokenize ints, floats, quoted strings and the registered tokens.

    TODO:
    Throw away the char pointers and replace by C++ strings

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Marcus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class Scanner  
{
public:
    enum ETokenType
    {
        eTT_INVALID,
        eTT_INT,        
        eTT_FLOAT,
        eTT_STRING,    // unrecognized quoted string
        eTT_LABEL,     // unrecognized unquoted string
        eTT_SEPARATOR, // recognized separator
        eTT_TOKEN      // recognized token
    };

    union UTokenId
    {
        void *p;
        long l;
        int  i;
    };

    struct SToken
    {
        ETokenType    type;
        union 
   {
       int      i;
            float    f;
            char     separator;
            char     *string;
            char     *label;            
            UTokenId id;
        } data;
    };

    struct SInitInfo
    {
            //! delimiter for a quoted string                [default "\""]
       char *stringQuote;   
            //! ignored characters used to seperate tokens   [default " \t\n"] 
       char *whiteSpace;    
            //! chars, that seperate tokens                  [default NULL]
       char *separator;        
            //! char, that starts an escape sequence         [default '\']
       char escape;            
       // Only recognized escape is '\[stringQuote-Char]' 
       // inside a quoted string. Every other sequence defaults 
       // to the escaped character (i.e. '\a' to 'a')
    };

protected:    
    struct SState
    {
   char *input;      // start of referenced input
        int inputLength;  // length of the complete input
        char *inputPos;   // current processed position
        char oldChar;     // remembered char replaced by \0
    };

    typedef std::map<std::string, UTokenId> TTokenMap; 
    typedef std::list<SState*> TStateList;

    TStateList mStateStack;    // list of pushed states
    TTokenMap  mTokens;        // map from token to the assigned id
    SState     mState;         // current state of the scanner
    SInitInfo  mInfo;          // parameters used to initiliaze the scanner

protected:
    bool scanNumber(char *pos, SToken &token);
    
    char* skipWhiteSpace(char *pos);
    char* skipToken(char *pos, char &oldchar);

    void applyEscapeSequences(char **start);

public:
    Scanner(SInitInfo *initInfo = NULL);
    virtual ~Scanner();
    
    static void defaultInitInfo(SInitInfo &initInfo);
    void setInitInfo (const SInitInfo &initInfo);
    void defineToken(const char *token, long l);
    void defineToken(const char *token, int i);
    void defineToken(const char *token, void *p);
    
    void setInput(const char* input);
    void rewindInput();

    bool getNextToken(SToken &token);

    static void printToken(SToken &token);

    void pushState();
    bool popState();

    const char* Scanner::lookupToken(int id);
};

#endif // _SCANNER_H_
