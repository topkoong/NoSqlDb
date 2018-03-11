/////////////////////////////////////////////////////////////////////////////////////
// Queries.cpp - retrieve NoSqlDb contents										   //
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
* This package defines class DbProvider that provides mechanism
* to share a test database between test functions without explicitly passing as a function argument.
*
** The package provides functions that demonstrates each of the requirements met.
*   ----------------------------------------------------------
*   - putLine								: reduce the number of characters to type
*   - testR3a								: demonstrate firsst part of requirement #3
*   - testR3b								: demonstrate second part of requirement #3 and first part of requirement #4
*   - testR6								: demonstrate first, second, third, and fourth part of requirement #6
*   - testQuery								: demonstrate fifth part of requirement #6, and first and second part of requirement #7
*   - testSpecializedSelectors				: demonstrate specialized selectors for common queries
*
* Required Files:
* ---------------
* Queries.h, Queries.cpp,
* DbCore.h, DbCore.cpp
* StringUtilities.h, StringUtilities.cpp
* TestUtilities.h, TestUtilities.cpp
*
* Build Process:
* --------------
* devenv Cpp11-NoSqlDb.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
*/

#include "Queries.h"
#include "../DbCore/DbCore.h"
#include "../Payload/Payload.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<PayLoad>& db() { return db_; }
private:
	static DbCore<PayLoad> db_;
};
DbCore<PayLoad> DbProvider::db_;

/////////////////////////////////////////////////////////////////////////
//// test functions
//
////----< demo requirement #1 >------------------------------------------
//

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n\n  Creating a db element with key \"Fawcett\":\n";
	// create database to hold std::string payload
	DbCore<PayLoad> db;
	DbProvider dbp;
	dbp.db() = db;
	// check whether db contains this key already?
	if (db.contains("Fawcett"))
	{
		std::cout << "\n  we cannot insert the key named " << "Fawcett" << " to the db\n";
		return false;
	}
	// create some demo elements and insert into db
	DbElement<PayLoad> demoElem;
	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	PayLoad pL;
	pL.filePath("C:/InsertDataDemo.txt");
	pL.categories().push_back("Instructor");
	demoElem.payLoad(pL);
	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad().filePath() != "C:/InsertDataDemo.txt")
		return false;
	if (!(std::find(demoElem.payLoad().categories().begin(), demoElem.payLoad().categories().end(), "Instructor") != demoElem.payLoad().categories().end()))
		return false;
	showHeader();
	showElem(demoElem);
	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b&4a - creating DbCore and adding element with key");
	DbProvider dbp;
	DbCore<PayLoad> db = dbp.db();
	DbElement<PayLoad> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	PayLoad pL;
	pL.filePath("C:/AdditionKVDemo.txt");
	pL.categories().push_back("TA");
	demoElem.payLoad(pL);
	db["Salman"] = demoElem;
	demoElem.name("Jianan");
	PayLoad pL2;
	pL2.filePath("C:/AdditionKVDemo2.txt"); pL2.categories().push_back("TA"); demoElem.payLoad(pL2);
	db["Sun"] = demoElem;
	PayLoad pL3;
	pL3.filePath("C:/AdditionKVDemo3.txt"); pL3.categories().push_back("TA"); demoElem.payLoad(pL3);
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;
	PayLoad pL4;
	pL4.filePath("C:/AdditionKVDemo4.txt"); pL4.categories().push_back("TA"); demoElem.payLoad(pL4);
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;
	PayLoad pL5;
	pL5.filePath("C:/AdditionKVDemo5.txt"); pL5.categories().push_back("TA"); demoElem.payLoad(pL5);
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;
	std::cout << "\n\n  after adding elements with keys: \n";
	DbCore<PayLoad>::Keys keys = db.keys();
	showKeys(db);
	std::cout << "\n\n  make all the new elements children of element with key \"Fawcett\"\n";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");
	showHeader();
	showElem(db["Fawcett"]);
	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");
	dbp.db() = db;
	std::cout << "\n\n  showing all the database elements:";
	showDb(db);
	std::cout << "\n\n  database keys are: ";
	showKeys(db);
	return true;
}

bool testR6()
{
	DbCore<PayLoad> db;
	DbProvider dbp;
	db = dbp.db();
	Query<PayLoad> q(db);
	Utilities::title("Demonstrating Requirement #6a - support queries for: ");
	std::cout << "\n\nThe value of a specified key.\n";
	q.showSpecifiedKey("Fawcett");
	putLine(2);
	Utilities::title("Demonstrating Requirement #6b - support queries for: ");
	putLine(2);
	std::cout << "\n\nThe children of a specified key.\n";
	q.showChildrenOfSpecifiedKey("Salman");
	Utilities::title("Demonstrating Requirement #6c - support queries for: ");
	putLine(2);
	std::cout << "\nThe set of all keys matching a specified regular-expression pattern.\n";
	q.showMatchKey(".*a.*");
	putLine(2);
	Utilities::title("Demonstrating Requirement #6d - support queries for: ");
	putLine(2);
	return true;
}

bool testQuery()
{
	DbCore<PayLoad> db;
	DbProvider dbp;
	db = dbp.db();
	std::cout << "\nAll keys that contain a specified string in their metadata section, where the specification is based on a regular-expression pattern.\n";
	Query<PayLoad> q1(db);
	Conditions<PayLoad> conds1;
	conds1.name(".*a.*").descrip(".*E.*");
	std::cout << "\n\nQuery command: conds1.name(\".*a.*\").descrip(\".*E.*\");\n";
	std::cout << "\n\nQuery command: q1.select(conds1);\n";
	q1.select(conds1);
	putLine(2);
	Utilities::title("Demonstrating Requirement #6e - support queries for: ");
	putLine(2);
	std::cout << "\nAll keys that contain values written within a specified time-date interval. If only one end of the interval is provided shall take the present as the other end of the interval.\n";
	DateTime dt = DateTime().now();
	DateTime::Duration dur = DateTime::makeDuration(1, 0, 0, 0);
	Query<PayLoad> q2(db);
	std::cout << "\n\nQuery command: q2.selectTimeInterval(dt - dur, dt + dur);\n";
	q2.selectTimeInterval(dt - dur, dt + dur);
	putLine(2);
	std::cout << "\n\nQuery command: q2.selectTimeInterval(dt - dur, dt + dur);\n";
	q2.selectDate(dt - dur);
	Utilities::title("Demonstrating Requirement #7a - queries on the set of keys returned by a previous query");
	putLine(2);
	std::cout << "\n\nQuery command: conds2.name(\".*P.*\").descrip(\".*TA.*\");\n";
	std::cout << "\n\nQuery command: q3.from(q1.keys()).select(conds2).show();\n";
	Query<PayLoad> q3(db);
	Conditions<PayLoad> conds2;
	conds2.name(".*P.*").descrip(".*TA.*");
	q3.from(q1.keys()).select(conds2).show();
	putLine();
	DbCore<PayLoad>::DbStore& dbs = db.dbStore();
	Conditions<PayLoad> conds3;
	Utilities::title("Demonstrating Requirement #7a - queries on the set of keys returned by a previous query");
	std::cout << "\n\nQuery command: q2.from(q1.keys()).select(conds2).show();\n";
	q2.from(q1.keys()).select(conds2).show();
	putLine(2);
	Utilities::title("Demonstrating Requirement #7b - queries on the union of results of one or more previous queries, e.g., an \" or \"ing of multiple queries");
	std::cout << "\n\nQuery command: q2.keysUnion(q1.keys()).show();\n";
	q2.keysUnion(q1.keys()).show();
	putLine(2);
	return true; // would not compile unless C++11

}
bool testSpecializedSelectors() {
	DbCore<PayLoad> db;
	DbProvider dbp;
	db = dbp.db();
	Query<PayLoad> q4(db);
	Utilities::title("\n\nDemonstrating specialized selectors for common queries\n");
	std::cout << "\n\nQuery command: q4.selectName(\".*i.*\").show();\n";
	q4.selectName(".*i.*").show();
	putLine(2);
	std::cout << "\n\nQuery command: q4.selectDesc(\".*E.*\").show();\n";
	q4.selectDesc(".*E.*").show();
	putLine(2);
	return true;
}
#ifdef TEST_QUERY
using namespace Utilities;
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();
	TestExecutive ex;
	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testR3a, "Creating DbElement" };
	TestExecutive::TestStr ts2{ testR3b, "Creating DbCore and adding element with key" };
	TestExecutive::TestStr ts3{ testR6, "Testing queries" };
	TestExecutive::TestStr ts4{ testQuery, "Testing Query mechanisms" };
	TestExecutive::TestStr ts5{ testSpecializedSelectors, "Testing Query mechanisms" };
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts4);
	ex.registerTest(ts5);

	// run tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}
#endif

