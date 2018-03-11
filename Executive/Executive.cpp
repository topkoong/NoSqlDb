/////////////////////////////////////////////////////////////////////////////////////
// Executive.cpp - demonstrate NoSqlDb requirements 								   //
// ver 1.0                                                                         //
//                                                                                 //
// Environment : C++ Console Application                                           //
// Platform    : Windows 10 Home x64, Lenovo IdeaPad 700, Visual Studio 2017       //
// Application :  NoSqlDb Key/Value database prototype for CSE687-OOD, Spring 2018 //  
// Author: Theerut Foongkiatcharoen, EECS Department, Syracuse University          //
//         tfoongki@syr.edu                                                        //
// Source: Dr. Jim Fawcett, EECS Department, CST 4-187, Syracuse University        //
//         jfawcett @twcny.rr.com                                                  //
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* The package provides functions that demonstrates each of the requirements met.
*
* Required Files:
* ---------------
* Executive.h, Executive.cpp,
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* StringUtilities.h, StringUtilities.cpp
* TestUtilities.h, TestUtilities.cpp
* Queries.h, Queries.cpp,
* Persist.h, Persist.cpp,
* XmlDocument.h, XmlDocument.cpp,
* XmlElement.h, XmlElement.cpp,
* XmlUtilities.h, XmlUtilities.cpp
*
* Build Process:
* --------------
* devenv Cpp11-NoSqlDb.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
*/
#include "Executive.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>

using namespace Utilities;
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();
	TestExecutive ex;
	// define test structures with test function and message
	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Creating DbElement" };
	TestExecutive::TestStr ts3b{ testR3b, "Creating DbCore and adding element with key" };
	TestExecutive::TestStr ts4b{ testR4b, "Deletion of key/value pairs" };
	TestExecutive::TestStr ts5a{ testR5a, "Addition of relationships" };
	TestExecutive::TestStr ts5b{ testR5b, "Deletion of relationships" };
	TestExecutive::TestStr ts5c{ testR5c, "Editing text metadata and replacing an existing value's instance with a new instance" };
	TestExecutive::TestStr ts6abcd{ testR6, "Testing queries mechanism" };
	TestExecutive::TestStr ts6e7a7b{ testQuery, "Testing queries mechanism" };
	TestExecutive::TestStr ts6c{ testSpecializedSelectors, "Testing specialized selectors for common queries" };
	TestExecutive::TestStr ts8a{ testXml, "Testing persisting a colllection of database contents, defined by a collection of keys, to an XML file" };
	TestExecutive::TestStr ts8b{ testCreateNewDbFromXmlFile, "Testing the database can be restored or augmented from an existing XML file as well as write its contents out to one or more XML files" };
	TestExecutive::TestStr ts9{ testQueryonPayLoad, "Testing the Query on Payload including Category and FilePath" };
	TestExecutive::TestStr ts11a{ testCreateProjectPackageStructure, "Testing an XML file, that describe my project's package structure and dependency relationships that can be loaded when my project is graded." };
	TestExecutive::TestStr ts11b{ testCreateNewDbFromProjectFile, "Demonstrating Requirement #11b - creating a new db from persisted ProjectPackageStructure.xml" };
	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4b);
	ex.registerTest(ts5a);
	ex.registerTest(ts5b);
	ex.registerTest(ts5c);
	ex.registerTest(ts6abcd);
	ex.registerTest(ts6e7a7b);
	ex.registerTest(ts6c);
	ex.registerTest(ts8a);
	ex.registerTest(ts8b);
	ex.registerTest(ts9);
	ex.registerTest(ts11a);
	ex.registerTest(ts11b);
	// run tests
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n\n  all tests passed";
	else
		std::cout << "\n\n  at least one test failed";

	putLine(5);
	return 0;
}

