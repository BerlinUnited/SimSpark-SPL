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
#include "consoleparser.h"
#include "console.h"
#include "consolealiases.h"
#include "commands/commandserver.h"

#include <classserver.h>
#include <algorithm>

using namespace std;

ConsoleParser::ConsoleParser()
{
    // Initialize the scanner
    Scanner::DefaultInitInfo(mScannerInfo);
    mScannerInfo.stringQuote = "\"'";
    mScannerInfo.whiteSpace  = " \t\n";

    string separators
   = SP_SEPARATOR
   + SP_COMMENT
   + SP_VECTOR_BEGIN + SP_VECTOR_SEP + SP_VECTOR_END;

    mScannerInfo.separator = new char [strlen (separators.c_str()) + 1];
    strcpy (mScannerInfo.separator, separators.c_str());
}

ConsoleParser::~ConsoleParser()
{
    delete [] mScannerInfo.separator;
}

pair<bool, ConsoleParser::TStatementList> 
ConsoleParser::scan(const string& toScan, bool removeEmptyStatements) const
{
    pair<bool, ConVar::TConVars> scanResult;
    ConsoleParser::TStatementList statements;
    
    // try to scan the input string
    scanResult = doScan(toScan);
    if (!scanResult.first)
    {
   return make_pair(false, statements);
    }

    // adapt the tokens
    statements = 
   replaceVectors(removeComments(splitStatements(scanResult.second)));

    if (removeEmptyStatements)
    {
        statements = removeEmptyStatements(statements);
    }
   
    return make_pair(true, statements);
}

bool 
ConsoleParser::parse(const TStatementList& statements)
{
    TStatementList parsed;

    for (TStatementList::const_iterator iter = statements.begin(); 
    iter != statements.end(); 
    ++iter)
    {
        // skip emtpy statements
        if ((*iter).size() == 0)
        {
            return false;
        }
        
        // does the statement start with a command?
        ConVar* commandVar = (*iter)[0];
        if (!commandVar->isConvertibleTo(ConVar::eCVT_COMMAND))
        {
            gDispatchWarning("the statement does not begin with a command.\n");
            return false;
        }

        parsed.push_back(*iter);
    }
    return true;
}

pair<bool, ConVar::TConVars> 
ConsoleParser::doScan(const string& input) const
{
    TStrings usedAliases;
    return scan(input, usedAliases);
}
    
pair<bool, ConVar::TConVars> 
ConsoleParser::scan(const string& input, TStrings usedAliases) const
{
    // initialize the scanner
    Scanner scanner;    
    scanner.setInitInfo(mScannerInfo);
    scanner.setInput(input.c_str());

    ConsoleAliases& aliases = gConsole.getAliases();

    // the scanned tokens
    ConVar::TConVars tokens;

    // scan the input
    Scanner::Token token;
    while (scanner.getNextToken(token))
    {
        switch (token.type)
   {
   case Scanner::eTT_INVALID: 
       gDispatchWarning("The scanner returned token eTT_INVALID "
              "in string %s\n", input.c_str());
       deleteStatement(tokens);
       return make_pair(false, tokens);
   case Scanner::eTT_SEPARATOR :
       tokens.push_back(ConVar::createSeparator(token.data.separator));
       break;
   case Scanner::eTT_INT:
       tokens.push_back(ConVar::createInt(token.data.i));
       break;
   case Scanner::eTT_FLOAT:
       tokens.push_back(ConVar::createFloat(token.data.f));
       break;
   case Scanner::eTT_STRING:
       tokens.push_back(ConVar::createString(token.data.string));
       break;
   case Scanner::eTT_LABEL:
       // a label is something special as it may be used 
            // for a command, a variable, a constant, ...
       string label = token.data.label;
       // is the label an alias?
       if (aliases.exists(label))
       {
      // did we already expand this alias, i.e. do 
                // we have a cyclic alias definition?
      if (find(usedAliases.begin(), usedAliases.end(), label) != 
          usedAliases.end())
      {
          // yes, -> return
          gDispatchNormal("alias loop.\n");
          deleteStatement(tokens);
          return make_pair (false, tokens);
      }
                        
      // try to expand the alias
      usedAliases.push_back(label);
      pair<bool, ConVar::TConVars> 
          aliasExpanded = scan(aliases.get(label), usedAliases);
      if (!aliasExpanded.first)
      {
          // something failed; probably an invalid 
                    // syntax or a cyclic alias definition
          deleteStatement (tokens);
          return make_pair(false, tokens);
      }

      // append the expanded alias
      copy(aliasExpanded.second.begin(), 
           aliasExpanded.second.end(), 
           back_inserter(tokens));
       } 
       // is it a command?
       else if (CommandServer::instance().get(label) != NULL)
       {
      ConVarAttributes attr;
      attr.setName(label);
      tokens.push_back(
          ConVar::createCommand(CommandServer::instance().get(label),
                 attr));
      // ok, we take it as a label and let the command 
      // interpret this argument
       } else {
      // a label is used for variables and for everything else
      // like parameters to a command, constants, modifiers, ...
      tokens.push_back(ConVar::createLabel(label));
       }
       break;
   default:
       gDispatchWarning("The scanner returned the unknown token type "
              "%s in string %s\n", token.type, input.c_str());
       deleteStatement(tokens);
       return make_pair(false, tokens);
        }
    }
    return make_pair(true, tokens);
}

ConsoleParser::TStatementList 
ConsoleParser::splitStatements(const ConVar::TConVars& tokens) const
{
    TStatementList statements;
    ConVar::TConVars statement;

    string separator;
    for (ConVar::TConVars::const_iterator iter = tokens.begin(); 
    iter != tokens.end(); 
    ++iter)
    {
        // if this is not a separator, ignore it
        if (!(*iter)->isSeparator())
        {
            statement.push_back (*iter);
            continue;
        }

        // if this is not the statement separator, ignore it
        (*iter)->getSeparator (separator);
        if (!(separator == SP_SEPARATOR))
        {
            statement.push_back (*iter);
            continue;
        }

        // ok, the statement separator
        // -> remove it and start a new statement
        delete *iter;
        statements.push_back (statement);
        statement.clear();
    }

    // append the last statement
    statements.push_back (statement);

    return statements;
}

ConsoleParser::TStatementList 
ConsoleParser::removeComments(const ConsoleParser::TStatementList& statements) const
{
    TStatementList shrinkedStatements;
    ConVar::TConVars shrinkedStatement;

    for (ConsoleParser::TStatementList::const_iterator iter = statements.begin(); 
    iter != statements.end(); 
    ++iter)
    {
        // keep empty statements
        if ((*iter).size() == 0)
        {
            shrinkedStatements.push_back(*iter);
            continue;
        }

        // remove a comment
        shrinkedStatement = removeComment(*iter);
        if (shrinkedStatement.size() > 0)
        {
            shrinkedStatements.push_back(shrinkedStatement);
        }
    }

    return shrinkedStatements;
}

ConVar::TConVars 
ConsoleParser::removeComment(const ConVar::TConVars& statement) const
{
    ConVar::TConVars shrinkedStatement;
    string separator;

    ConVar::TConVars::const_iterator iter = statement.begin();

    for (; iter != statement.end(); ++iter)
    {
        // if this is not a separator, ignore it
        if (!(*iter)->isSeparator())
        {
            shrinkedStatement.push_back (*iter);
            continue;
        }
        
        // if this is not the comment separator, ignore it
        (*iter)->getSeparator(separator);
        if (!(separator == SP_COMMENT))
        {
            shrinkedStatement.push_back(*iter);
            continue;
        }
        
        // ok, the comment separator -> no more copying of the tokens
        break;
    }

    // remove the remaining tokens
    for (iter; iter != statement.end(); ++iter)
    {
        delete *iter;
    }
    
    return shrinkedStatement;
}

ConsoleParser::TStatementList 
ConsoleParser::replaceVectors(const ConsoleParser::TStatementList& statements) const
{
    TStatementList modifiedStatements;

    for (ConsoleParser::TStatementList::const_iterator iter = statements.begin(); 
    iter != statements.end(); 
    ++iter)
    {
        modifiedStatements.push_back(replaceVector(*iter));
    }
    
    return modifiedStatements;
}

ConVar::TConVars 
ConsoleParser::replaceVector(const ConVar::TConVars& statement) const
{
    ConVar::TConVars modifiedStatement;
    ConVar::TConVars vectorStatement;
    
    for (ConVar::TConVars::const_iterator iter = statement.begin(); 
    iter != statement.end(); )
    {
        vectorStatement = matchVector(iter, statement.end());
        copy(vectorStatement.begin(), 
        vectorStatement.end(), 
        back_inserter(modifiedStatement));
    }
    
    return modifiedStatement;
}

ConVar::TConVars 
ConsoleParser::matchVector(ConVar::TConVars::const_iterator& iter, 
            ConVar::TConVars::const_iterator iterEnd) const
{
    ConVar::TConVars firstToken;

    // try to find a sequence of < float , float, float >
    // store the matched tokens; return them, if a mismatch occurs
    // otherwise delete them and return a new token of type vector
    firstToken.push_back(*iter);
    
    // assure that there are at least 7 tokens remaining
    if (((iter + 1) == iterEnd)
        || ((iter + 2) == iterEnd)
        || ((iter + 3) == iterEnd)
        || ((iter + 4) == iterEnd)
        || ((iter + 5) == iterEnd)
        || ((iter + 6) == iterEnd))
    {
        ++iter;
        return firstToken;
    }

    // check the tokens
    if ((!isItThisSeparator (*iter, SP_VECTOR_BEGIN))
        || (!isItThisSeparator (*(iter + 2), SP_VECTOR_SEP))
        || (!isItThisSeparator (*(iter + 4), SP_VECTOR_SEP))
        || (!isItThisSeparator (*(iter + 6), SP_VECTOR_END)))
    {
        ++iter;
        return firstToken;
    }
    
    // check the floats
    if ((!(*(iter + 1))->isConvertibleTo(ConVar::eCVT_FLOAT))
        || (!(*(iter + 3))->isConvertibleTo(ConVar::eCVT_FLOAT))
        || (!(*(iter + 5))->isConvertibleTo(ConVar::eCVT_FLOAT)))
    {
        ++iter;
        return firstToken;
    }
    
    // get the values
    float x, y, z;
    (*(iter + 1))->getFloat (x);
    (*(iter + 3))->getFloat (y);
    (*(iter + 5))->getFloat (z);

    // delete the old tokens
    for (int i = 0; i < 6; ++i)
    {
        delete (*(iter + i));
    }

    // move the iterator
    iter += 7;
    
    // create the vector and return it
    firstToken.clear();
    firstToken.push_back(ConVar::createVector(x, y, z));
    return firstToken;
}


ConsoleParser::TStatementList 
ConsoleParser::removeEmptyStatements(const ConsoleParser::TStatementList& statements) const
{
    TStatementList purifiedStatements;
    
    for (ConsoleParser::TStatementList::const_iterator iter = statements.begin(); 
    iter != statements.end(); 
    ++iter)
    {
        if ((*iter).size() != 0)
        {
            purifiedStatements.push_back(*iter);
        }
    }
    
    return purifiedStatements;
}

bool 
ConsoleParser::isItThisSeparator(const ConVar* conVar, const string& separator)
{
    string value;
    
    if (!conVar->getSeparator(value))
    {
        return false;
    }
    
    if (value != separator)
    {
        return false;
    }
    
    return true;
}

void 
ConsoleParser::deleteStatements(const ConsoleParser::TStatementList& statements)
{
    for_each(statements.begin(), 
        statements.end(), 
        ConsoleParser::deleteStatement);
}

void 
ConsoleParser::deleteStatement(ConVar::TConVars statement)
{
    for (ConVar::TConVars::iterator iter = statement.begin(); 
    iter != statement.end(); 
    ++iter)
    {
        delete *iter;
    }
}
