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

#include <forwarder.h>

#include <classserver.h>
#include <algorithm>

using namespace std;
using namespace Utility;

ConsoleParser::ConsoleParser()
{
    // Initialize the scanner
    Scanner::defaultInitInfo(M_scanner_info);
    M_scanner_info.string_quote = "\"'";
    M_scanner_info.white_space = " \t\n";

    string separators
        = SP_SEPARATOR
        + SP_COMMENT + SP_VECTOR_BEGIN + SP_VECTOR_SEP + SP_VECTOR_END;

    M_scanner_info.separator = new char[strlen(separators.c_str()) + 1];
    strcpy(M_scanner_info.separator, separators.c_str());
}

ConsoleParser::~ConsoleParser()
{
    delete[] M_scanner_info.separator;
}

pair < bool, ConsoleParser::StatementList >
ConsoleParser::scan(const string& to_scan, bool remove_empty_statements) const
{
    pair < bool, ConVar::ConVars > scan_result;
    ConsoleParser::StatementList statements;

    // try to scan the input string
    scan_result = doScan(to_scan);
    if (!scan_result.first)
    {
        return make_pair(false, statements);
    }

    // adapt the tokens
    statements =
        replaceVectors(removeComments(splitStatements(scan_result.second)));

    if (remove_empty_statements)
    {
        statements = removeEmptyStatements(statements);
    }

    return make_pair(true, statements);
}

bool 
ConsoleParser::parse(const StatementList& statements)
{
    StatementList parsed;

    for (StatementList::const_iterator iter = statements.begin();
         iter != statements.end(); ++iter)
    {
        // skip emtpy statements
        if ((*iter).size() == 0)
        {
            return false;
        }

        // does the statement start with a command?
        ConVar* commandVar = (*iter)[0];
        if (!commandVar->isConvertibleTo(ConVar::S_CVT_COMMAND))
        {
            smux.warning() << "the statement does not begin with a command.\n";
            return false;
        }

        parsed.push_back(*iter);
    }
    return true;
}

pair < bool, ConVar::ConVars > 
ConsoleParser::doScan(const string& input) const
{
    TStrings used_aliases;
    return scan(input, used_aliases);
}

pair < bool, ConVar::ConVars >
ConsoleParser::scan(const string& input, TStrings used_aliases) const
{
    // initialize the scanner
    Scanner scanner;
    scanner.setInitInfo(M_scanner_info);
    scanner.setInput(input.c_str());

    ConsoleAliases& aliases = Console::instance().getAliases();

    // the scanned tokens
    ConVar::ConVars tokens;

    // scan the input
    Scanner::Token token;
    while (scanner.getNextToken(token))
    {
        switch (token.type)
        {
            case Scanner::S_TT_INVALID:
                smux.warning() << "The scanner returned token S_TT_INVALID "
                               << "in string " << input << "\n";
                deleteStatement(tokens);
                return make_pair(false, tokens);
            case Scanner::S_TT_SEPARATOR:
                tokens.push_back(ConVar::createSeparator(token.data.separator));
                break;
            case Scanner::S_TT_INT:
                tokens.push_back(ConVar::createInt(token.data.i));
                break;
            case Scanner::S_TT_FLOAT:
                tokens.push_back(ConVar::createFloat(token.data.f));
                break;
            case Scanner::S_TT_STRING:
                tokens.push_back(ConVar::createString(token.data.string));
                break;
            case Scanner::S_TT_LABEL:
            {
            
                // a label is something special as it may be used 
                // for a command, a variable, a constant, ...
                string label = token.data.label;
                // is the label an alias?
                if (aliases.exists(label))
                {
                    // did we already expand this alias, i.e. do 
                    // we have a cyclic alias definition?
                    if (find(used_aliases.begin(), 
                             used_aliases.end(), label) != used_aliases.end())
                    {
                        // yes, -> return
                        smux.normal() << "alias loop.\n";
                        deleteStatement(tokens);
                        return make_pair(false, tokens);
                    }

                    // try to expand the alias
                    used_aliases.push_back(label);
                    pair < bool, ConVar::ConVars > alias_expanded = 
                        scan(aliases.get(label), used_aliases);
                    if (!alias_expanded.first)
                    {
                        // something failed; probably an invalid 
                        // syntax or a cyclic alias definition
                        deleteStatement(tokens);
                        return make_pair(false, tokens);
                    }

                    // append the expanded alias
                    copy(alias_expanded.second.begin(),
                         alias_expanded.second.end(), back_inserter(tokens));
                }
                // is it a command?
                else if (CommandServer::instance().get(label) != 0)
                {
                    ConVarAttributes attr;
                    attr.setName(label);
                    tokens.push_back(
                        ConVar::createCommand(
                            CommandServer::instance().get(label), attr));
                    // ok, we take it as a label and let the command 
                    // interpret this argument
                }
                else
                {
                    // a label is used for variables and for everything else
                    // like parameters to a command, constants, modifiers, ...
                    tokens.push_back(ConVar::createLabel(label));
                }
                break;
            }
            default:
                smux.warning() << "The scanner returned the unknown token type "
                               << token.type << " in string " << input << "\n";
                deleteStatement(tokens);
                return make_pair(false, tokens);
        }
    }
    return make_pair(true, tokens);
}

ConsoleParser::StatementList
ConsoleParser::splitStatements(const ConVar::ConVars& tokens) const
{
    StatementList statements;
    ConVar::ConVars statement;

    string separator;
    for (ConVar::ConVars::const_iterator iter = tokens.begin();
         iter != tokens.end(); ++iter)
    {
        // if this is not a separator, ignore it
        if (!(*iter)->isSeparator())
        {
            statement.push_back(*iter);
            continue;
        }

        // if this is not the statement separator, ignore it
        (*iter)->getSeparator(separator);
        if (!(separator == SP_SEPARATOR))
        {
            statement.push_back(*iter);
            continue;
        }

        // ok, the statement separator
        // -> remove it and start a new statement
        delete *iter;
        statements.push_back(statement);
        statement.clear();
    }

    // append the last statement
    statements.push_back(statement);

    return statements;
}

ConsoleParser::StatementList
ConsoleParser::removeComments(const ConsoleParser::StatementList& slist) const
{
    StatementList shrinked_statements;
    ConVar::ConVars shrinked_statement;

    for (ConsoleParser::StatementList::const_iterator iter =
         slist.begin(); iter != slist.end(); ++iter)
    {
        // keep empty statements
        if ((*iter).size() == 0)
        {
            shrinked_statements.push_back(*iter);
            continue;
        }

        // remove a comment
        shrinked_statement = removeComment(*iter);
        if (shrinked_statement.size() > 0)
        {
            shrinked_statements.push_back(shrinked_statement);
        }
    }

    return shrinked_statements;
}

ConVar::ConVars
ConsoleParser::removeComment(const ConVar::ConVars& statement) const
{
    ConVar::ConVars shrinked_statement;
    string separator;

    ConVar::ConVars::const_iterator iter = statement.begin();

    for (; iter != statement.end(); ++iter)
    {
        // if this is not a separator, ignore it
        if (!(*iter)->isSeparator())
        {
            shrinked_statement.push_back(*iter);
            continue;
        }

        // if this is not the comment separator, ignore it
        (*iter)->getSeparator(separator);
        if (!(separator == SP_COMMENT))
        {
            shrinked_statement.push_back(*iter);
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

    return shrinked_statement;
}

ConsoleParser::StatementList
ConsoleParser::replaceVectors(const ConsoleParser::StatementList& slist) const
{
    StatementList modified_statements;

    for (ConsoleParser::StatementList::const_iterator iter =
         slist.begin(); iter != slist.end(); ++iter)
    {
        modified_statements.push_back(replaceVector(*iter));
    }

    return modified_statements;
}

ConVar::ConVars
ConsoleParser::replaceVector(const ConVar::ConVars& statement) const
{
    ConVar::ConVars modified_statement;
    ConVar::ConVars vector_statement;

    for (ConVar::ConVars::const_iterator iter = statement.begin();
         iter != statement.end();)
    {
        vector_statement = matchVector(iter, statement.end());
        copy(vector_statement.begin(),
             vector_statement.end(), back_inserter(modified_statement));
    }

    return modified_statement;
}

ConVar::ConVars
ConsoleParser::matchVector(ConVar::ConVars::const_iterator& iter,
                           ConVar::ConVars::const_iterator iter_end) const
{
    ConVar::ConVars first_token;

    // try to find a sequence of < float , float, float >
    // store the matched tokens; return them, if a mismatch occurs
    // otherwise delete them and return a new token of type vector
    first_token.push_back(*iter);

    // assure that there are at least 7 tokens remaining
    if (((iter + 1) == iter_end)
        || ((iter + 2) == iter_end)
        || ((iter + 3) == iter_end)
        || ((iter + 4) == iter_end)
        || ((iter + 5) == iter_end) || ((iter + 6) == iter_end))
    {
        ++iter;
        return first_token;
    }

    // check the tokens
    if ((!isItThisSeparator(*iter, SP_VECTOR_BEGIN))
        || (!isItThisSeparator(*(iter + 2), SP_VECTOR_SEP))
        || (!isItThisSeparator(*(iter + 4), SP_VECTOR_SEP))
        || (!isItThisSeparator(*(iter + 6), SP_VECTOR_END)))
    {
        ++iter;
        return first_token;
    }

    // check the floats
    if ((!(*(iter + 1))->isConvertibleTo(ConVar::S_CVT_FLOAT))
        || (!(*(iter + 3))->isConvertibleTo(ConVar::S_CVT_FLOAT))
        || (!(*(iter + 5))->isConvertibleTo(ConVar::S_CVT_FLOAT)))
    {
        ++iter;
        return first_token;
    }

    // get the values
    TFloat x, y, z;
    (*(iter + 1))->getFloat(x);
    (*(iter + 3))->getFloat(y);
    (*(iter + 5))->getFloat(z);

    // delete the old tokens
    for (int i = 0; i < 6; ++i)
    {
        delete(*(iter + i));
    }

    // move the iterator
    iter += 7;

    // create the vector and return it
    first_token.clear();
    first_token.push_back(ConVar::createVector(x, y, z));
    return first_token;
}

ConsoleParser::StatementList
ConsoleParser::removeEmptyStatements(const ConsoleParser::StatementList& 
                                     slist) const
{
    StatementList purified_statements;

    for (ConsoleParser::StatementList::const_iterator iter =
         slist.begin(); iter != slist.end(); ++iter)
    {
        if ((*iter).size() != 0)
        {
            purified_statements.push_back(*iter);
        }
    }

    return purified_statements;
}

bool 
ConsoleParser::isItThisSeparator(const ConVar* con_var, const string& separator)
{
    string value;

    if (!con_var->getSeparator(value))
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
ConsoleParser::deleteStatements(const ConsoleParser::StatementList& slist)
{
    for_each(slist.begin(), slist.end(), ConsoleParser::deleteStatement);
}

void
ConsoleParser::deleteStatement(ConVar::ConVars statement)
{
    for (ConVar::ConVars::iterator iter = statement.begin();
         iter != statement.end(); ++iter)
    {
        delete *iter;
    }
}
