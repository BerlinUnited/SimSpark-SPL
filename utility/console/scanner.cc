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

Scanner::Scanner(SInitInfo *initInfo)
{
    mState.input  = NULL;
    mState.inputPos = NULL;
    mState.oldChar = 0;
    mState.inputLength = 0;

    if (initInfo != NULL)
        mInfo = *initInfo;
    else
   defaultInitInfo(mInfo);
}

Scanner::~Scanner()
{
    // destroy current state
    if (mState.input) delete[] mState.input;

    // destroy the state stack
    TStateList::iterator i;
    for(i=mStateStack.begin(); i!=mStateStack.end(); ++i) 
    {
        if ((*i)->input) delete[] (*i)->input;
        delete (*i);
    }
    
    mStateStack.clear();
}

void 
Scanner::defaultInitInfo(SInitInfo &initInfo)
{
    initInfo.stringQuote = "\"";
    initInfo.whiteSpace = " \t\n\r";
    initInfo.separator = NULL;
    initInfo.escape = '\\';
}

void 
Scanner::setInitInfo(const SInitInfo &initInfo)
{
    mInfo = initInfo;
}

void 
Scanner::defineToken(const char *token, long l)
{
    UTokenId id;
    memset(&id,0,sizeof(id));

    id.l = l;
    mTokens[token] = id;
}

void 
Scanner::defineToken(const char *token, int i)
{
    UTokenId id;
    memset(&id,0,sizeof(id));

    id.i = i;
    mTokens[token] = id;
}

void 
Scanner::defineToken(const char *token, void *p)
{
    UTokenId id;
    memset(&id,0,sizeof(id));

    id.p = p;
    mTokens[token] = id;
}

void 
Scanner::setInput(const char *input)
{
    if (mState.input) delete[] mState.input;
    mState.input = NULL;
    mState.inputLength = 0;

    if (input)
    {
        mState.inputLength = strlen(input);
        
        if (mState.inputLength) 
        {
            mState.input = new char[mState.inputLength+1];
            strcpy(mState.input, input);
        } else mState.input = NULL;    

        mState.inputPos    = mState.input;
        mState.oldChar    = 0;
    }
}

void 
Scanner::rewindInput()
{
    mState.inputPos = mState.input;
    mState.oldChar = 0;
}

bool 
Scanner::scanNumber(char *pos, SToken &token)
{
    if (pos == NULL) return false;

    // only a '.' is no number
    if ((*pos == '.') && (*(pos + 1) == 0)) 
    {
        return false;
    }

    enum { eUNKNOWN, eINTEGER, eFLOAT, eERROR, eEXP, eSIGNEDEXP} state;
    char *startPos;

    startPos = pos;
    state = eUNKNOWN;

    for (; (state != eERROR) && ((*pos) != 0); ++pos)
    {
        char &ch = *pos;

        if ((ch == '+') || (ch == '-'))
        {
            switch (state)
            {
       case eUNKNOWN : state = eINTEGER;   break;
       case eEXP     : state = eSIGNEDEXP; break;
       default       : state = eERROR;     break;
            }
            continue;
        }
        
        if ((ch >= '0') && (ch <= '9'))
        {
            if (state == eUNKNOWN) state = eINTEGER;
            continue;
        }

        if (ch == '.')
        {
            switch(state)
            {            
            case eUNKNOWN :
            case eINTEGER :
      state = eFLOAT;
      break;
            case eFLOAT :
      state = eERROR;
      break;
            }
            continue;
        }

        if (ch == 'e')
        {
            if ((state != eINTEGER) &&  (state != eFLOAT)) 
      state = eERROR;
            else state = eEXP;
            continue;
        }

        state = eERROR;
    }

    switch (state)
    {
    case eINTEGER:
   sscanf(startPos,"%d",&token.data.i);
   token.type = eTT_INT;
   return true;
    case eFLOAT:
    case eEXP:
    case eSIGNEDEXP:
   sscanf(startPos,"%f",&token.data.f);
   token.type = eTT_FLOAT;
   return true;
    default:
   token.type = eTT_INVALID;
   return false;
    }
}

char* 
Scanner::skipWhiteSpace(char *pos) 
{
    char *whiteSpace,*separator;

    if ((!mState.input) || (!pos)) return NULL;

    for ( ; (*pos) != 0; ++pos)
    {    
        // hold on seperators
        if (mInfo.separator)
        {
            for (separator = mInfo.separator;(*separator)!=0;separator++)
            {
                if ((*pos) == (*separator)) break;
            }
        }
        // skip whitespace
        if (mInfo.whiteSpace) 
        {
            for (whiteSpace = mInfo.whiteSpace;(*whiteSpace)!=0;whiteSpace++)
            {
                if ((*pos) == (*whiteSpace)) break;
            }
            if ((*whiteSpace) != 0) continue;
        }

        // non whiteSpace read
        break;
    }

    return pos;
}

char* 
Scanner::skipToken(char *pos, char &oldchar)
{
    bool inString,firstChar,inEscape;
    char *whiteSpace,*quote,*separator;
    char stringQuote;

    if ((!mState.input) || (!pos)) return NULL;

    firstChar    = true;
    inString    = false;
    oldchar        = 0;
    inEscape    = false;
    
    for ( ; (*pos) != 0; firstChar=false,++pos)
    {
        if ((*pos) == mInfo.escape) 
   {
            inEscape = !inEscape;
            continue;
        }

        // ensure that we don't interprete a char immediately 
        // after an escape char
        if (!inEscape) 
        {        
            // check for start or end of a string
            if (mInfo.stringQuote)
            {
                if (!inString) 
                // check for the start of a string
                {
                    for (quote = mInfo.stringQuote; (*quote) !=0 ; ++quote)
                    {
                        if ((*pos) == (*quote)) 
                        {
                            stringQuote = *quote;
                            break;
                        }
                    }

                    if ((*quote) != 0)
                    {
                        inString=true; // we've found the beginning of a string
                    }
                } else {
                    // check for the end of a string
                    if ((*pos) == stringQuote) 
                    {
                        break;
                    }
                }
            }

            // only check for separators or whitespace if we're not 
            // inside a string
            if (!inString) 
            {
                // check for separator
                if (mInfo.separator)
                {
                    for (separator = mInfo.separator;(*separator)!=0;separator++)
                    {
                        if ((*pos) == (*separator)) break;
                    }

                    if ((*separator) != 0) {
                        // if this is the first char, then recognize the seperator
                        // as a token otherwise a delimiter for the preceding string                        
                        if (firstChar) {                            
                            pos++;
                            oldchar = (*pos);
                        } else oldchar = (*separator);
                        break;
                    }
                }

                // hold on first whitespace if we're outside a string
                if (mInfo.whiteSpace)
                {
                    for (whiteSpace = mInfo.whiteSpace;(*whiteSpace)!=0;whiteSpace++)
                    {
                        if ((*pos) == (*whiteSpace)) break;
                    }
                    if ((*whiteSpace) != 0) break;
                }
            }
        }

        // an escape sequence ends always after the second char
        inEscape = false;
    }
    
    if ((*pos) != 0)
    {
        (*pos)    = 0;        // mark end of token
        return pos;
    } 
    else return NULL;
}

void 
Scanner::applyEscapeSequences(char **start)
{
    // are escape sequences disabled ?
    if (mInfo.escape == 0) return;

    // now scan for escape sequences
    for (char *pos=(*start);(*pos)!=0;pos++)
    {
        if ((*pos) == mInfo.escape)
        {
            // we recognized an escape sequence
            memmove((*start) + 1,(*start),pos-(*start));
            (*start)++;
            pos++;
        }
    }
}

bool 
Scanner::getNextToken(SToken &token)
{        
    if ((!mState.input) || (!mState.inputPos)) return false;    

    // if this isn't the first token step over last end-of-token marker
    if (mState.input != mState.inputPos) 
    {
        // restore a separator
        if (mState.oldChar) (*mState.inputPos) = mState.oldChar;
            else mState.inputPos++;
    }

    // check for end of input
    if ((*mState.inputPos) == 0) return false;

    // skip Whitespace    
    mState.inputPos = skipWhiteSpace(mState.inputPos);

    // check end of input
    if ((*mState.inputPos) == 0) return false;

    char *lastInputPos = mState.inputPos;

    // advance to next token
    mState.inputPos = skipToken(mState.inputPos,mState.oldChar);

    applyEscapeSequences(&lastInputPos);

    // first check it against our known separators
    if (mInfo.separator) 
    {
        char* separator;
        for (separator=mInfo.separator;(*separator)!=0;separator++) 
        {
            if ((*lastInputPos) == (*separator)) break;
        }
        if ((*separator)!=0)
        {
            // ok, we've found a separator
            token.type                = eTT_SEPARATOR;
            token.data.separator    = (*separator);

            return true;
        }
    }

    // now try to interprete it as a number
    if (scanNumber(lastInputPos,token)) return true;

    // now check it against our map of known strings    
    TTokenMap::iterator i = mTokens.find(lastInputPos);
    if (i != mTokens.end()) 
    {
        // ok we got a match
        token.data.id    = (*i).second;
        token.type        = eTT_TOKEN;
        
        return true;
    }

    // now try to interprete it as a string
    if (mInfo.stringQuote)
    {
        char *pos,*quote;
        pos = lastInputPos;

        for (quote = mInfo.stringQuote;(*quote)!=0;quote++)
        {
            if ((*pos) == (*quote)) break;
        }
        if ((*quote) !=0) 
        {
            // we've found a string
            token.type            = eTT_STRING;
            pos++;
            token.data.string    = pos;

            return true;
        }
    }

    // we've found a label
    token.type            = eTT_LABEL;
    token.data.label    = lastInputPos;

    return true;
}

void 
Scanner::printToken(SToken &token)
{
    switch (token.type)
    {
    case eTT_INVALID : 
   printf("eTT_INVALID\n");
   break;
    case eTT_INT :
   printf("eTT_INT %d\n",token.data.i);
   break;
    case eTT_FLOAT :
   printf("eTT_FLOAT %f\n",token.data.f);
   break;
    case eTT_STRING :
   printf("eTT_STRING ");
   printf(token.data.string);
   printf("\n");
   break;
    case eTT_LABEL :
   printf("eTT_LABEL ");
   printf(token.data.label);
   printf("\n");
   break;
    case eTT_TOKEN :
   printf("eTT_TOKEN %d\n",token.data.id.i);
   break;
    case eTT_SEPARATOR :
   printf("eTT_SEPARATOR %c\n",token.data.separator);
   break;
    default :
   printf("unknown token type.\n");
   break;
    }
}

void 
Scanner::pushState()
{
    // create a new scanner state und copy the
    // current state of the scanner

    SState *state = new SState;

    if (mState.inputLength)
    {
        state->input = new char[mState.inputLength+1];
        memcpy(state->input, mState.input, mState.inputLength+1);
    } else state->input = NULL;

    if (mState.inputPos)
    {
        state->inputPos = state->input + (mState.inputPos - mState.input);
    } else state->inputPos = NULL;

    state->oldChar        = mState.oldChar;
    state->inputLength    = mState.inputLength;

    // put the saved state on the stack
    mStateStack.push_back(state);
}

bool 
Scanner::popState()
{
    if (mStateStack.empty()) return false;

    // take a saved scanner state from the stack
    // and make it the current state
    if (mState.input) delete[] mState.input;    

    SState *state = mStateStack.back();
    
    mState = *state;

    delete state;
    mStateStack.pop_back();

    return true;
}

const char* 
Scanner::lookupToken(int id)
{
    TTokenMap::iterator i;
    for(i=mTokens.begin(); i!=mTokens.end(); i++)
    {
        if ((*i).second.i == id) return (*i).first.c_str();
    }
    
    return NULL;
}
