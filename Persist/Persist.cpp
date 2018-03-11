/////////////////////////////////////////////////////////////////////////////////////
// Persist.cpp - store and retrieve NoSqlDb contents 							   //
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
* -------------------------------------------------------------
* This package defines class DbProvider that provides mechanism
* to share a test database between test functions without explicitly passing as a function argument.
*  - Creates XmlDocument from a new database
*  - Extracts Xml string representing database
*  - Restores db, e.g., builds new db from Xml String
*  - Persists to file and restore from file
*
* The package also provides functions that demonstrates each of the requirements met.
*   ----------------------------------------------------------
*   - putLine								: reduce the number of characters to type
*   - testR3a								: demonstrate firsst part of requirement #3
*   - testR3b								: demonstrate second part of requirement #3 and first part of requirement #4
*   - testXml								: demonstrate first part of requirement #8
*   - testCreateNewDbFromXmlFile			: demonstrate second part of requirement #8
*
* Required Files:
* ---------------
* Persist.h, Persist.cpp,
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* StringUtilities.h, StringUtilities.cpp
* TestUtilities.h, TestUtilities.cpp
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


#include "Persist.h"
#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

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

///////////////////////////////////////////////////////////////////////
// Demonstration of prototype persistance processing
//

//----< demonstrate first part of requirement #3 >----------------------------

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

//----< demonstrate second part of requirement #3 and first part of requirement #4>----------------------------

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

//----< demonstrate first part of requirement #8>----------------------------

bool testXml()
{
	Utilities::title("Demonstrating Requirement #8a - persisting a colllection of database contents, defined by a collection of keys, to an XML file");
	putLine(2);
	DbCore<PayLoad> db;
	DbProvider dbp;
	db = dbp.db();
	Persist<PayLoad> persist(db);
	persist.createXml();
	std::cout << "Persisting db to file Persist/DbXml.xml\n";
	if (persist.saveToFile("DbXml.xml"))
	{
		std::cout << "\n  db saved";
		return true;
	}
	std::cout << "\n  filesave failed";
	putLine();
	return false;
}

//----< demonstrate second part of requirement #8>----------------------------

bool testCreateNewDbFromXmlFile()
{
	Utilities::title("Demonstrating Requirement #8b - creating new db from persisted XML");
	putLine();
	std::cout << "\nThe database can be restored or augmented from an existing XML file\n";
	DbCore<PayLoad> newDb;
	Persist<PayLoad> persist2(newDb);
	persist2.restoreFromFile("DbXml.xml");
	std::cout << "\n  showing all the database elements:";
	showDb(newDb);
	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(newDb);
	putLine();
	return true;
}
#ifdef TEST_PERSIST
using namespace Utilities;
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;
	TestExecutive::TestStr ts1{ testR3a, "Creating DbElement" };
	TestExecutive::TestStr ts2{ testR3b, "Creating DbCore and adding element with key" };
	TestExecutive::TestStr ts3{ testXml, "testing XML" };
	TestExecutive::TestStr ts4{ testCreateNewDbFromXmlFile, "testing XML" };

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts4);

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(3);
	return 0;
}
#endif
