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
#include "scanner.h"

Scanner::Scanner(InitInfo* init_info)
{
    M_state.input = 0;
    M_state.input_pos = 0;
    M_state.old_char = 0;
    M_state.input_length = 0;

    if (init_info != 0)
        M_info = *init_info;
    else
        defaultInitInfo(M_info);
}

Scanner::~Scanner()
{
    // destroy current state
    if (M_state.input)
        delete[] M_state.input;

    // destroy the state stack
    StateList::iterator i;
    for (i = M_state_stack.begin(); i != M_state_stack.end(); ++i)
    {
        if ((*i)->input)
            delete[](*i)->input;
        delete(*i);
    }

    M_state_stack.clear();
}

void
Scanner::defaultInitInfo(InitInfo& init_info)
{
    init_info.string_quote = "\"";
    init_info.white_space = " \t\n\r";
    init_info.separator = 0;
    init_info.escape = '\\';
}

void
Scanner::setInitInfo(const InitInfo& init_info)
{
    M_info = init_info;
}

void
Scanner::defineToken(const char* token, long l)
{
    TokenId id;
    memset(&id, 0, sizeof(id));

    id.l = l;
    M_tokens[token] = id;
}

void
Scanner::defineToken(const char* token, int i)
{
    TokenId id;
    memset(&id, 0, sizeof(id));

    id.i = i;
    M_tokens[token] = id;
}

void
Scanner::defineToken(const char* token, void *p)
{
    TokenId id;
    memset(&id, 0, sizeof(id));

    id.p = p;
    M_tokens[token] = id;
}

void
Scanner::setInput(const char* input)
{
    if (M_state.input)
        delete[] M_state.input;
    M_state.input = 0;
    M_state.input_length = 0;

    if (input)
    {
        M_state.input_length = strlen(input);

        if (M_state.input_length)
        {
            M_state.input = new char[M_state.input_length + 1];
            strcpy(M_state.input, input);
        }
        else
            M_state.input = 0;

        M_state.input_pos = M_state.input;
        M_state.old_char = 0;
    }
}

void
Scanner::rewindInput()
{
    M_state.input_pos = M_state.input;
    M_state.old_char = 0;
}

bool
Scanner::scanNumber(char* pos, Token& token)
{
    if (pos == 0)
        return false;

    // only a '.' is no number
    if ((*pos == '.') && (*(pos + 1) == 0))
    {
        return false;
    }

    enum
    {
        S_UNKNOWN,
        S_INTEGER,
        S_FLOAT,
        S_ERROR,
        S_EXP,
        S_SIGNEDEXP
    }
    state;

    char* start_pos;

    start_pos = pos;
    state = S_UNKNOWN;

    for (; (state != S_ERROR) && ((*pos) != 0); ++pos)
    {
        char &ch = *pos;

        if ((ch == '+') || (ch == '-'))
        {
            switch (state)
            {
                case S_UNKNOWN:
                    state = S_INTEGER;
                    break;
                case S_EXP:
                    state = S_SIGNEDEXP;
                    break;
                default:
                    state = S_ERROR;
                    break;
            }
            continue;
        }

        if ((ch >= '0') && (ch <= '9'))
        {
            if (state == S_UNKNOWN)
                state = S_INTEGER;
            continue;
        }

        if (ch == '.')
        {
            switch (state)
            {
                case S_UNKNOWN:
                case S_INTEGER:
                    state = S_FLOAT;
                    break;
                case S_FLOAT:
                    state = S_ERROR;
                    break;
            }
            continue;
        }

        if (ch == 'e')
        {
            if ((state != S_INTEGER) && (state != S_FLOAT))
                state = S_ERROR;
            else
                state = S_EXP;
            continue;
        }

        state = S_ERROR;
    }

    switch (state)
    {
        case S_INTEGER:
            sscanf(start_pos, "%d", &token.data.i);
            token.type = S_TT_INT;
            return true;
        case S_FLOAT:
        case S_EXP:
        case S_SIGNEDEXP:
            sscanf(start_pos, "%f", &token.data.f);
            token.type = S_TT_FLOAT;
            return true;
        default:
            token.type = S_TT_INVALID;
            return false;
    }
}

char* 
Scanner::skipWhiteSpace(char* pos)
{
    char* white_space, *separator;

    if ((!M_state.input) || (!pos))
        return 0;

    for (; (*pos) != 0; ++pos)
    {
        // hold on seperators
        if (M_info.separator)
        {
            for (separator = M_info.separator; (*separator) != 0; ++separator)
            {
                if ((*pos) == (*separator))
                    break;
            }
        }
        // skip whitespace
        if (M_info.white_space)
        {
            for (white_space = M_info.white_space; (*white_space) != 0;
                 white_space++)
            {
                if ((*pos) == (*white_space))
                    break;
            }
            if ((*white_space) != 0)
                continue;
        }

        // non white_space read
        break;
    }

    return pos;
}

char* 
Scanner::skipToken(char* pos, char& oldchar)
{
    bool in_string, first_char, in_escape;
    char* white_space; 
    char* quote; 
    char* separator;
    char string_quote;

    if ((!M_state.input) || (!pos))
        return 0;

    first_char = true;
    in_string = false;
    oldchar = 0;
    in_escape = false;

    for (; (*pos) != 0; first_char = false, ++pos)
    {
        if ((*pos) == M_info.escape)
        {
            in_escape = !in_escape;
            continue;
        }

        // ensure that we don't interprete a char immediately 
        // after an escape char
        if (!in_escape)
        {
            // check for start or end of a string
            if (M_info.string_quote)
            {
                if (!in_string)
                    // check for the start of a string
                {
                    for (quote = M_info.string_quote; (*quote) != 0; ++quote)
                    {
                        if ((*pos) == (*quote))
                        {
                            string_quote = *quote;
                            break;
                        }
                    }

                    if ((*quote) != 0)
                    {
                        // we've found the beginning of a string
                        in_string = true;        
                    }
                }
                else
                {
                    // check for the end of a string
                    if ((*pos) == string_quote)
                    {
                        break;
                    }
                }
            }

            // only check for separators or whitespace if we're not 
            // inside a string
            if (!in_string)
            {
                // check for separator
                if (M_info.separator)
                {
                    for (separator = M_info.separator; (*separator) != 0;
                         separator++)
                    {
                        if ((*pos) == (*separator))
                            break;
                    }

                    if ((*separator) != 0)
                    {
                        /* If this is the first char, then recognize the 
                           seperator as a token otherwise a delimiter for 
                           the preceding string.
                        */
                        if (first_char)
                        {
                            pos++;
                            oldchar = (*pos);
                        }
                        else
                            oldchar = (*separator);
                        break;
                    }
                }

                // hold on first whitespace if we're outside a string
                if (M_info.white_space)
                {
                    for (white_space = M_info.white_space; (*white_space) != 0;
                         white_space++)
                    {
                        if ((*pos) == (*white_space))
                            break;
                    }
                    if ((*white_space) != 0)
                        break;
                }
            }
        }

        // an escape sequence ends always after the second char
        in_escape = false;
    }

    if ((*pos) != 0)
    {
        (*pos) = 0;             // mark end of token
        return pos;
    }
    else
        return 0;
}

void
Scanner::applyEscapeSequences(char** start)
{
    // are escape sequences disabled ?
    if (M_info.escape == 0)
        return;

    // now scan for escape sequences
    for (char* pos = (*start); (*pos) != 0; pos++)
    {
        if ((*pos) == M_info.escape)
        {
            // we recognized an escape sequence
            memmove((*start) + 1, (*start), pos - (*start));
            (*start)++;
            pos++;
        }
    }
}

bool
Scanner::getNextToken(Token& token)
{
    if ((!M_state.input) || (!M_state.input_pos))
        return false;

    // if this isn't the first token step over last end-of-token marker
    if (M_state.input != M_state.input_pos)
    {
        // restore a separator
        if (M_state.old_char)
            (*M_state.input_pos) = M_state.old_char;
        else
            M_state.input_pos++;
    }

    // check for end of input
    if ((*M_state.input_pos) == 0)
        return false;

    // skip Whitespace    
    M_state.input_pos = skipWhiteSpace(M_state.input_pos);

    // check end of input
    if ((*M_state.input_pos) == 0)
        return false;

    char* last_input_pos = M_state.input_pos;

    // advance to next token
    M_state.input_pos = skipToken(M_state.input_pos, M_state.old_char);

    applyEscapeSequences(&last_input_pos);

    // first check it against our known separators
    if (M_info.separator)
    {
        char* separator;
        for (separator = M_info.separator; (*separator) != 0; ++separator)
        {
            if ((*last_input_pos) == (*separator))
                break;
        }
        if ((*separator) != 0)
        {
            // ok, we've found a separator
            token.type = S_TT_SEPARATOR;
            token.data.separator = (*separator);

            return true;
        }
    }

    // now try to interprete it as a number
    if (scanNumber(last_input_pos, token))
        return true;

    // now check it against our map of known strings    
    TokenMap::iterator i = M_tokens.find(last_input_pos);
    if (i != M_tokens.end())
    {
        // ok we got a match
        token.data.id = (*i).second;
        token.type = S_TT_TOKEN;

        return true;
    }

    // now try to interprete it as a string
    if (M_info.string_quote)
    {
        char* pos, *quote;
        pos = last_input_pos;

        for (quote = M_info.string_quote; (*quote) != 0; ++quote)
        {
            if ((*pos) == (*quote))
                break;
        }
        if ((*quote) != 0)
        {
            // we've found a string
            token.type = S_TT_STRING;
            pos++;
            token.data.string = pos;

            return true;
        }
    }

    // we've found a label
    token.type = S_TT_LABEL;
    token.data.label = last_input_pos;

    return true;
}

void
Scanner::printToken(Token& token)
{
    switch (token.type)
    {
        case S_TT_INVALID:
            printf("S_TT_INVALID\n");
            break;
        case S_TT_INT:
            printf("S_TT_INT %d\n", token.data.i);
            break;
        case S_TT_FLOAT:
            printf("S_TT_FLOAT %f\n", token.data.f);
            break;
        case S_TT_STRING:
            printf("S_TT_STRING ");
            printf(token.data.string);
            printf("\n");
            break;
        case S_TT_LABEL:
            printf("S_TT_LABEL ");
            printf(token.data.label);
            printf("\n");
            break;
        case S_TT_TOKEN:
            printf("S_TT_TOKEN %d\n", token.data.id.i);
            break;
        case S_TT_SEPARATOR:
            printf("S_TT_SEPARATOR %c\n", token.data.separator);
            break;
        default:
            printf("unknown token type.\n");
            break;
    }
}

void
Scanner::pushState()
{
    // create a new scanner state und copy the
    // current state of the scanner

    State *state = new State;

    if (M_state.input_length)
    {
        state->input = new char[M_state.input_length + 1];
        memcpy(state->input, M_state.input, M_state.input_length + 1);
    }
    else
        state->input = 0;

    if (M_state.input_pos)
    {
        state->input_pos = state->input + (M_state.input_pos - M_state.input);
    }
    else
        state->input_pos = 0;

    state->old_char = M_state.old_char;
    state->input_length = M_state.input_length;

    // put the saved state on the stack
    M_state_stack.push_back(state);
}

bool
Scanner::popState()
{
    if (M_state_stack.empty())
        return false;

    // take a saved scanner state from the stack
    // and make it the current state
    if (M_state.input)
        delete[]M_state.input;

    State *state = M_state_stack.back();

    M_state = *state;

    delete state;
    M_state_stack.pop_back();

    return true;
}

const char*
Scanner::lookupToken(int id)
{
    TokenMap::iterator i;
    for (i = M_tokens.begin(); i != M_tokens.end(); ++i)
    {
        if ((*i).second.i == id)
            return (*i).first.c_str();
    }

    return 0;
}
