/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.3 2004/04/09 21:06:31 fruit Exp $

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
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

#ifdef HAVE_KEROSIN_H
#include <kerosin/kerosin.h>
#endif

#include <iostream>
#include <fstream>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;
using namespace zeitgeist;

std::string outFile = "zeitgeist.dot";
std::string inFile = "include.dot";

void
printHelp()
{
    cout << "\nusage: gendot [options]" << endl;
    cout << "\noptions:" << endl;
    cout << " -o\t name of the outfile, default zeitgeist.dot" << endl;
    cout << " -i\t name of the dot include file, default include.dot" << endl;
    cout << " -h\t print this help message" << endl;
    cout << endl;
    cout << "gendot generates a graphviz dot file showing the zeitgeist " << endl;
    cout << "class hierarchy, including all classes that are visible at runtime. " << endl;
    cout << "You can add additional classes using 'importbundle <bundle>'" << endl;
    cout << "in the 'gendot.rb' file. To view the output as an ps-file invoke dot" << endl;
    cout << "with 'dot -Tps zeitgeist.dot -o zeitgeist.ps'" << endl;
    cout << "\n";
}

void
processInput(int argc, char* argv[])
{
    for( int i = 0; i < argc; i++)
    {
        if( strcmp( argv[i], "-o" ) == 0 )
        {
            if( i+1 < argc)
            {
                outFile = argv[i+1];
                ++i;
            }
        }
        if( strcmp( argv[i], "-i" ) == 0 )
        {
            if( i+1 < argc)
            {
                inFile = argv[i+1];
                ++i;
            }
        }
        else if
            (
                ( strcmp( argv[i], "--help" ) == 0 ) ||
                ( strcmp( argv[i], "-h" ) == 0 )
                )
        {
            printHelp();
            exit(0);
        }
    }
}

void
printGreeting()
{
    cout << "zeitgeist gendot version 0.1" << endl
         << "Universität Koblenz." << endl
         << "Copyright (C) 2004, "
         << "The RoboCup Soccer Server Maintenance Group." << endl;
}

int
main(int argc, char* argv[])
{
    printGreeting();
    processInput(argc,argv);

    Zeitgeist zg("." PACKAGE_NAME);
    oxygen::Oxygen kOxygen(zg);
#ifdef HAVE_KEROSIN_H
    kerosin::Kerosin kerosin(zg);
#endif

    // require any additional classes in the gendot.rb
    zg.GetCore()->GetScriptServer()->Run("gendot.rb");

    shared_ptr<Leaf> classRoot = zg.GetCore()->Get("/classes");
    if (classRoot.get() == 0)
    {
        cout << "ERROR: unable to get the class root node" << endl;
        return 1;
    }

    ofstream out(outFile.c_str());
    cout << "writing to " << outFile << endl;
    out << "digraph G {" << endl;

    // import the include file
    ifstream header("include.dot");

    while (! header.eof())
    {
        static char buffer[4095];
        header.getline(buffer, sizeof(buffer));
        out << buffer << endl;
    }


    // list all classes
    Leaf::TLeafList classes;
    classRoot->ListChildrenSupportingClass<Class>(classes,true);

    for (
        Leaf::TLeafList::iterator iter = classes.begin();
        iter != classes.end();
        ++iter
        )
    {
        string name = (*iter)->GetFullPath();

        // remove '/classes/' prefix
        name.erase(name.begin(),name.begin()+9);

        cout << "class " << name << endl;

        shared_ptr<Class> theClass = shared_dynamic_cast<Class>(*iter);
        if (theClass.get() == 0)
        {
            continue;
        }

        const Class::TStringList& baseClasses = theClass->GetBaseClasses();

        out << "\"" << name << "\"";

        for (
            Class::TStringList::const_iterator baseIter = baseClasses.begin();
            baseIter != baseClasses.end();
            ++baseIter
            )
        {
            out << " -> \"" << (*baseIter) << "\"";
        }

        out << ";" << endl;
    }

    out << "}" << endl;
}
