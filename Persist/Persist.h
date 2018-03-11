/////////////////////////////////////////////////////////////////////////////////////
// Persist.h - store and retrieve NoSqlDb contents 								   //
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
* This package defines a single class, Persist, that provides methods
* save and restore to save db contents in an XML string, and restore
* or create a new db from the XML.
*
*   ----------------------------------------------------------
* The Persist class also provides methods to save and restore db.
*   ------------------------------------------------------------
*   - createXml									: create XML representation of Database using XmlDocument
*   - saveToFile								: saves db contents to file, using save
*   - save										: save db contents to XML string
*   - restoreChildren							: retrive db children from XML string
*   - restorePayLoad							: retrive db payload from XML string
*   - restore									: retrive db contents from XML string
*   - restoreFromFile							: restores db contents from file, using restore

* Required Files:
* ---------------
* Persist.h, Persist.cpp,
* DbCore.h, DbCore.cpp
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

#include <string>
#include <iostream>
#include <iomanip>
#include "../DbCore/DbCore.h"
#include "../Payload/Payload.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"
#include "../XmlUtilities/XmlUtilities.h"
#include "../DateTime/DateTime.h"
#include <fstream>

using namespace XmlProcessing;
namespace NoSqlDb
{
	/////////////////////////////////////////////////////////
	// Persist class holds a reference to db and uses
	// that to save and restore the db data.
	//

	template<typename T>
	class Persist
	{
	public:
		using Xml = std::string;
		using SPtr = std::shared_ptr<AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;
		using Key = std::string;
		using Keys = std::vector<std::string>;

		Persist(DbCore<T>& db) :db_(db) {}
		void createXml();
		Xml save();
		void restore(const Xml& xml);
		bool saveToFile(const std::string& fileSpec);
		bool restoreFromFile(const std::string& fileSpec);
		DbElement<PayLoad>::Children restoreChildren(SPtr pValueChild);
		PayLoad restorePayLoad(SPtr pValueChild);
	private:
		DbCore<T> &db_;
		Xml xml_;
		std::string getTextBodyFromElement(SPtr sPtr);
		std::string getTextBodyFromElement(SPtrs sPtrs);
	};
	template<typename T>
	//----< create XML representation of Database using XmlDocument >--------------------
	void Persist<T>::createXml()
	{
		std::cout << "\n  Creating XML representation of Database using XmlDocument";
		std::cout << "\n ----------------------------------------------------------------";
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		Sptr pDb = makeTaggedElement("db");
		pDb->addAttrib("type", "testDb");
		Sptr pDocElem = makeDocElement(pDb);
		XmlDocument xDoc(pDocElem);
		for (auto item : db_)
		{
			Sptr pRecord = makeTaggedElement("dbRecord");
			pDb->addChild(pRecord);
			Sptr pKey = makeTaggedElement("key", item.first);
			pRecord->addChild(pKey);

			Sptr pValue = makeTaggedElement("value");
			pRecord->addChild(pValue);
			Sptr pName = makeTaggedElement("name", item.second.name());
			pValue->addChild(pName);
			Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
			pValue->addChild(pDescrip);
			Sptr pDate = makeTaggedElement("date", item.second.dateTime());
			pValue->addChild(pDescrip);
			SPtr pChildren = XmlProcessing::makeTaggedElement("children");
			Keys childKeys = item.second.children();
			for (Key child : childKeys)
			{
				SPtr pChild = XmlProcessing::makeTaggedElement("childKey", static_cast<std::string>(child));
				pChildren->addChild(pChild);
			}
			pValue->addChild(pChildren);
			Sptr pPayLoad = item.second.payLoad().toXmlElement();
			pValue->addChild(pPayLoad);

		}
		Persist<PayLoad> persist(db_);
		Persist<PayLoad>::Xml xml = persist.save();
		std::string Xml = xDoc.toString();
	}

	//----< saves db contents to file, using save >--------------------

	template<typename T>
	bool Persist<T>::saveToFile(const std::string& fileSpec)
	{
		std::ofstream out(fileSpec);
		if (!out.good())
			return false;
		Persist<T> persist(db_);
		Xml xml = persist.save();
		out << xml;
		out.close();
		return true;
	}
	//----< save db contents to XML string >---------------------------

	template<typename T>
	typename Persist<T>::Xml Persist<T>::save()
	{
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		Sptr pDb = makeTaggedElement("db");
		pDb->addAttrib("type", "testDb");
		Sptr pDocElem = makeDocElement(pDb);
		XmlDocument xDoc(pDocElem);
		// This works too.
		// XmlProcessing::XmlDocument doc(XmlProcessing::makeDocElement(pDbRoot));
		for (auto item : db_)
		{
			Sptr pRecord = makeTaggedElement("dbRecord");
			pDb->addChild(pRecord);
			Sptr pKey = makeTaggedElement("key", item.first);
			pRecord->addChild(pKey);
			Sptr pValue = makeTaggedElement("value");
			pRecord->addChild(pValue);
			Sptr pName = makeTaggedElement("name", item.second.name());
			pValue->addChild(pName);
			Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
			pValue->addChild(pDescrip);
			Sptr pDate = makeTaggedElement("date", item.second.dateTime());
			pValue->addChild(pDate);
			SPtr pChildren = XmlProcessing::makeTaggedElement("children");
			Keys childKeys = item.second.children();
			for (Key child : childKeys)
			{
				SPtr pChild = XmlProcessing::makeTaggedElement("childKey", static_cast<std::string>(child));
				pChildren->addChild(pChild);
			}

			pValue->addChild(pChildren);
			Sptr pPayLoad = item.second.payLoad().toXmlElement();
			pValue->addChild(pPayLoad);
		}
		return xDoc.toString();
	}


	//----< retrive db children from XML string >---------------------------
	/*
	* Private helper function - provides safe extraction of element text
	* from single element pointer.
	*/

	template<typename T>
	DbElement<PayLoad>::Children Persist<T>::restoreChildren(SPtr pValueChild)
	{
		DbElement<PayLoad>::Children children;
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		std::vector<Sptr> pChildren = pValueChild->children();
		for (SPtr pChild : pChildren)
			children.push_back(pChild->children()[0]->value());
		return children;
	}

	//----< retrive db contents from XML string >----------------------
	/*
	* Private helper function - provides safe extraction of element text
	* from single element pointer.
	*/
	template<typename T>
	void Persist<T>::restore(const Xml& xml)
	{
		std::cout << "\n  Building a new Db from XmlDocument";
		std::cout << "\n ----------------------------------";
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		XmlDocument doc(xml, XmlDocument::str);
		std::vector<Sptr> records = doc.descendents("dbRecord").select();
		for (auto pRecord : records)
		{
			Key key;
			DbElement<PayLoad> elem;
			std::vector<Sptr> pChildren = pRecord->children();
			for (auto pChild : pChildren)
			{
				// record's children are key and value
				if (pChild->tag() == "key")
				{
					// get value of TextElement child
					key = pChild->children()[0]->value();
				}
				else
				{
					// value represents a MockDbElem with children name and description
					std::vector<Sptr> pValueChildren = pChild->children();
					for (auto pValueChild : pValueChildren)
					{
						if (pValueChild->tag() == "name")
							elem.name(pValueChild->children()[0]->value());
						if (pValueChild->tag() == "description")
							elem.descrip(pValueChild->children()[0]->value());
						if (pValueChild->tag() == "date")
							elem.dateTime(pValueChild->children()[0]->value());
						if (pValueChild->tag() == "children")
						{
							DbElement<PayLoad>::Children children;
							restoreChildren(pValueChild);
							elem.children(children);
						}
						if (pValueChild->tag() == "payload")
						{
							PayLoad const payload = PayLoad::fromXmlElement(pValueChild);
							elem.payLoad(payload);
						}
					}
				}
			}
			db_[key] = elem;
		}
	}

	//----< restores db contents from file, using restore >------------

	template<typename T>
	bool Persist<T>::restoreFromFile(const std::string& fileSpec)
	{
		//std::string newFS = "C:\\Users\\James\\Desktop\\InstrSolProject1S2017\\Project1.xml";
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  failed to open file";
			return false;
		}
		Xml xml;

		// build XML string from file

		while (in.good())
		{
			char ch = in.get();

			/////////////////////////////////////////////////////
			// This code removes whitespace from XML contents
			// but is unnecessart and adds a small CPU load.
			// I left it here because I intent to use it in
			// another package, for display.
			/////////////////////////////////////////////////////
			// if (ch == '\n' || ch == '\t' || ch == ' ')
			//   continue;
			/////////////////////////////////////////////////////

			if (!in.good())
				break;
			xml += ch;
		}
		in.close();

		restore(xml);
		return true;
	}
}
