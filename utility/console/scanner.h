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
#ifndef UTILITY_SCANNER_H
#define UTILITY_SCANNER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(WIN32)
#pragma warning (disable : 4786)        // truncation of debug information
#endif

#include <stdio.h>
#include <map>
#include <list>
#include <string>

/*! \class Scanner
  $Id: scanner.h,v 1.3 2002/08/21 14:11:17 fruit Exp $

    Scanner - a simple tokenizer

    This scanner implentation manages a registry of known tokens. It is able
    to tokenize ints, floats, quoted strings and the registered tokens.

    TODO:
    Throw away the char pointers and replace by C++ strings

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class Scanner
{
public:
    enum TokenType
    {
        S_TT_INVALID,
        S_TT_INT,
        S_TT_FLOAT,
        S_TT_STRING,             // unrecognized quoted string
        S_TT_LABEL,              // unrecognized unquoted string
        S_TT_SEPARATOR,          // recognized separator
        S_TT_TOKEN               // recognized token
    };

    union TokenId
    {
        void* p;
        long l;
        int i;
    };

    struct Token
    {
        TokenType type;
        union
        {
            int i;
            float f;
            char separator;
            char *string;
            char *label;
            TokenId id;
        }
        data;
    };

    struct InitInfo
    {
        //! delimiter for a quoted string                [default "\""]
        char* string_quote;
        //! ignored characters used to seperate tokens   [default " \t\n"] 
        char* white_space;
        //! chars, that seperate tokens                  [default NULL]
        char* separator;
        //! char, that starts an escape sequence         [default '\']
        char escape;
        // Only recognized escape is '\[string_quote-Char]' 
        // inside a quoted string. Every other sequence defaults 
        // to the escaped character (i.e. '\a' to 'a')
    };

protected:
    struct State
    {
        char* input;            // start of referenced input
        int input_length;        // length of the complete input
        char* input_pos;         // current processed position
        char old_char;           // remembered char replaced by \0
    };

    typedef std::map < std::string, TokenId > TokenMap;
    typedef std::list < State* > StateList;

    StateList M_state_stack;     // list of pushed states
    TokenMap M_tokens;          // map from token to the assigned id
    State M_state;              // current state of the scanner
    InitInfo M_info;            // parameters used to initiliaze the scanner

protected:
    bool scanNumber(char* pos, Token& token);

    char* skipWhiteSpace(char* pos);
    char* skipToken(char* pos, char &oldchar);

    void applyEscapeSequences(char** start);

public:
    Scanner(InitInfo* initInfo = 0);
    virtual ~Scanner();

    static void defaultInitInfo(InitInfo& init_info);
    void setInitInfo(const InitInfo& init_info);
    void defineToken(const char* token, long l);
    void defineToken(const char* token, int i);
    void defineToken(const char* token, void* p);

    void setInput(const char* input);
    void rewindInput();

    bool getNextToken(Token& token);

    static void printToken(Token& token);

    void pushState();
    bool popState();

    const char* Scanner::lookupToken(int id);
};

#endif                          // UTILITY_SCANNER_H
