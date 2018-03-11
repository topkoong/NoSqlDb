#include "Test.h"
//#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>

using namespace Utilities;
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();
	//TestDB testDb;;
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
	TestExecutive::TestStr ts6{ testR6, "Testing queries mechanism" };
	TestExecutive::TestStr ts6b{ testQuery, "Testing queries mechanism" };
	TestExecutive::TestStr ts6c{ testSpecializedSelectors, "Testing specialized selectors for common queries" };
	TestExecutive::TestStr ts7a{ testXml, "Testing persisting a colllection of database contents, defined by a collection of keys, to an XML file" };
	TestExecutive::TestStr ts7b{ testCreateNewDbFromXmlFile, "Testing the database can be restored or augmented from an existing XML file as well as write its contents out to one or more XML files" };
	TestExecutive::TestStr ts11{ testCreateProjectPackageStructure, "Testing an XML file, that describe my project's package structure and dependency relationships that can be loaded when my project is graded." };
	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4b);
	ex.registerTest(ts5a);
	ex.registerTest(ts5b);
	ex.registerTest(ts5c);
	ex.registerTest(ts6);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts7a);
	ex.registerTest(ts7b);
	ex.registerTest(ts11);
	// run tests
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n\n  all tests passed";
	else
		std::cout << "\n\n  at least one test failed";

	putLine(5);
	return 0;
}

