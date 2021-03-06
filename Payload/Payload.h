#ifndef PAYLOAD_H
#define PAYLOAD_H
///////////////////////////////////////////////////////////////////////
// PayLoad.h - application defined payload                           //
// ver 1.1                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, PayLoad:
*  - holds a payload string and vector of categories
*  - provides means to set and access those values
*  - provides methods used by Persist<PayLoad>:
*    - Sptr toXmlElement();
*    - static PayLoad fromXmlElement(Sptr elem);
*  - provides a show function to display PayLoad specific information
*  - PayLoad processing is very simple, so this package contains only
*    a header file, making it easy to use in other packages, e.g.,
*    just include the PayLoad.h header.
*
*  Required Files:
*  ---------------
*    PayLoad.h, PayLoad.cpp - application defined package
*    DbCore.h, DbCore.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 19 Feb 2018
*  - added inheritance from IPayLoad interface
*  Ver 1.0 : 10 Feb 2018
*  - first release
*
*/

#include <string>
#include <vector>
#include <iostream>
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"
#include "../DbCore/DbCore.h"
#include "IPayLoad.h"

///////////////////////////////////////////////////////////////////////
// PayLoad class provides:
// - a std::string value which, in Project #2, will hold a file path
// - a vector of string categories, which for Project #2, will be 
//   Repository categories
// - methods used by Persist<PayLoad>:
//   - Sptr toXmlElement();
//   - static PayLoad fromXmlElement(Sptr elem);


namespace NoSqlDb
{
	class PayLoad : public IPayLoad<PayLoad>
	{
	public:
		using FilePath = std::string;
		using Categories = std::vector<std::string>;

		PayLoad() = default;
		PayLoad(const FilePath& filePath) : filePath_(filePath) {}
		static void identify(std::ostream& out = std::cout);
		PayLoad& operator=(const FilePath& filePath)
		{
			filePath_ = filePath;
			return *this;
		}
		operator std::string() { return filePath_; }

		FilePath filePath() const { return filePath_; }
		FilePath& filePath() { return filePath_; }
		void filePath(const FilePath& filePath) { filePath_ = filePath; }

		Categories& categories() { return categories_; }
		Categories categories() const { return categories_; }

		bool isOpen() const { return isOpen_; }
		bool& isOpen() { return isOpen_; }
		void isOpen(const bool& status) { isOpen_ = status; }

		bool isClosed() const { return isClosed_; }
		bool& isClosed() { return isClosed_; }
		void isClosed(const bool& status) { isClosed_ = status; }


		bool hasCategory(const std::string& cat)
		{
			return std::find(categories().begin(), categories().end(), cat) != categories().end();
		}

		Sptr toXmlElement();
		static PayLoad fromXmlElement(Sptr elem);

		static void showPayLoadHeaders(std::ostream& out = std::cout);
		static void showElementPayLoad(NoSqlDb::DbElement<PayLoad>& elem, std::ostream& out = std::cout);
		static void showDb(NoSqlDb::DbCore<PayLoad>& db, std::ostream& out = std::cout);
	private:
		FilePath filePath_;
		Categories categories_;
		bool isOpen_ = true;
		bool isClosed_ = false;
	};

	//----< show file name >---------------------------------------------

	inline void PayLoad::identify(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
	}
	//----< create XmlElement that represents PayLoad instance >---------
	/*
	* - Required by Persist<PayLoad>
	*/
	inline Sptr PayLoad::toXmlElement()
	{
		Sptr sPtr = XmlProcessing::makeTaggedElement("payload");
		XmlProcessing::XmlDocument doc(makeDocElement(sPtr));
		Sptr sPtrVal = XmlProcessing::makeTaggedElement("filepath", filePath_);
		sPtr->addChild(sPtrVal);
		Sptr sPtrCats = XmlProcessing::makeTaggedElement("categories");
		sPtr->addChild(sPtrCats);
		for (auto cat : categories_)
		{
			Sptr sPtrCat = XmlProcessing::makeTaggedElement("category", cat);
			sPtrCats->addChild(sPtrCat);
		}
		return sPtr;
	}
	//----< create PayLoad instance from XmlElement >--------------------
	/*
	* - Required by Persist<PayLoad>
	*/
	inline PayLoad PayLoad::fromXmlElement(Sptr pElem)
	{
		PayLoad pl;
		for (auto pChild : pElem->children())
		{
			std::string tag = pChild->tag();
			std::string val = pChild->children()[0]->value();
			if (tag == "filepath")
			{
				pl.filePath(val);
			}
			if (tag == "categories")
			{
				std::vector<Sptr> pCategories = pChild->children();
				for (auto pCat : pCategories)
				{
					pl.categories().push_back(pCat->children()[0]->value());
				}
			}
		}
		return pl;
	}
	/////////////////////////////////////////////////////////////////////
	// PayLoad display functions

	inline void PayLoad::showPayLoadHeaders(std::ostream& out)
	{
		out << "\n  ";
		out << std::setw(25) << std::left << "Name";
		out << std::setw(30) << std::left << "Payload Value";
		out << std::setw(20) << std::left << "Categories";
		out << std::setw(15) << std::left << "Opened";
		out << std::setw(15) << std::left << "Closed";
		out << "\n  ";
		out << std::setw(25) << std::left << "------------------------";
		out << std::setw(30) << std::left << "-----------------------------";
		out << std::setw(20) << std::left << "-------------------";
		out << std::setw(15) << std::left << "-------------";
		out << std::setw(15) << std::left << "-------------";
	}


	inline void PayLoad::showElementPayLoad(NoSqlDb::DbElement<PayLoad>& elem, std::ostream& out)
	{
		out << "\n  ";
		out << std::setw(25) << std::left << elem.name().substr(0, 23);
		out << std::setw(30) << std::left << elem.payLoad().filePath().substr(0, 38);
		for (auto cat : elem.payLoad().categories())
		{
			out << std::setw(20) << std::left << cat.substr(0, 18) << " ";
		}
		out << std::setw(15) << std::left << elem.payLoad().isOpen();
		out << std::setw(15) << std::left << elem.payLoad().isClosed();
	}

	inline void PayLoad::showDb(NoSqlDb::DbCore<PayLoad>& db, std::ostream& out)
	{
		showPayLoadHeaders(out);
		for (auto item : db)
		{
			NoSqlDb::DbElement<PayLoad> temp = item.second;
			PayLoad::showElementPayLoad(temp, out);
		}
	}
}
#endif
