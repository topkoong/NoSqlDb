/////////////////////////////////////////////////////////////////////////////////////
// Queries.h - retrieve NoSqlDb contents										   //
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
* This package provides two classes:
* - Conditions instances hold a DbElement<P> to support compound queries.
*
* - Query instances hold a reference to a database and a vector of keys
*   and returns a set of matching keys against different type of queries
*  which includes compound queries like "AND OR".
* - When constructed those keys are db.keys()
*
*
*
* The package provides functions that demonstrates each of the requirements met.
*   ----------------------------------------------------------
*   - name								: setter and getter of name_
*   - descrip					: setter and getter of name_
*   - dateTime					: setter and getter of descrip_
*   - payLoad				: setter and getter of payLoad_
*   - keys				: returns current key set for db
*   - show				: displays all matching keys
*   - select					: uses conds1 DbElement parts to match db elements for each q1 key
and when done replaces its original keys with the new key set
*   - from						: gets all keys from the resulting query
*   - showSpecifiedKey					: demonstrate the value of a specified key
*   - showChildrenOfSpecifiedKey					: demonstrate the children of a specified key
*   - showMatchKey			:  demonstrate the set of all keys matching a specified regular-expression pattern
*   - selectName                   : demonstrate all keys that contain a name in metadata section, where the specification is based on a regular-expression pattern
*   - selectDesc				: demonstrate all keys that contain a description in metadata section, where the specification is based on a regular-expression pattern
*   - selectTimeInterval		:  demonstrate all keys that match time interval in metadata section, where the specification is based on a regular-expression pattern
*   - selectDate				: demonstrate all keys that match time in metadata section, where the specification is based on a regular-expression pattern
*   - keysUnion								: demonstrate the union of results of one or more previous queries, e.g., an "or"ing of multiple queries., where the specification is based on a regular-expression pattern
*   - selectCategory								: demonstrate all keys that match category in payload section, where the specification is based on a regular-expression pattern
*   - selectFilePath								: all keys that match filepath in payload section, where the specification is based on a regular-expression pattern
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
#include "../DbCore/DbCore.h"
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <regex>
namespace NoSqlDb
{
	/////////////////////////////////////////////////////////
	// Conditions instances hold a DbElement<P>
	// to support compound queries.
	//
	template <typename T>
	class Conditions
	{
	public:
		// methods to get and set DbElement fields
		std::string& name() { return name_; }
		std::string name() const { return name_; }
		Conditions<T>& name(const std::string& name) { name_ = name; return *this; }

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		Conditions<T>& descrip(const std::string& description) { descrip_ = description; return *this; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		Conditions<T>& dateTime(const DateTime& dateTime) { dateTime_ = dateTime; return *this; }


		std::string& payLoad() { return payLoad_; }
		std::string payLoad() const { return payLoad_; }
		Conditions<T>& payLoad(const T& payLoad) { payLoad_ = payLoad; return *this; }

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		T payLoad_;
		DbElement<T> el_;
	};


	///////////////////////////////////////////////////////////////////
	// Query class defines a single type of "query" function.
	// - Query instances hold a reference to a database and a vector of keys
	//   and returns a set of matching keys against different type of queries 
	//  which includes compound queries like "AND OR".
	//
	// - When constructed those keys are db.keys()

	template <typename T>
	class Query
	{
	public:
		using Arg = std::string;
		using Key = std::string;
		using Keys = std::vector<std::string>;

		Query(const DbCore<T>& db) { db_ = db; keys_ = db_.keys(); }
		Query<T>& select(const Conditions<T>& cond);
		template<typename CallObj>
		Query& select(CallObj callObj);
		Query<T>& from(Keys& keys);
		void show();
		Keys& keys();
		Query<T>& showSpecifiedKey(const Key& key);
		Query<T>& showChildrenOfSpecifiedKey(const Key& key);
		Query<T>& showMatchKey(const std::string& arg);
		Query<T>& selectName(const std::string &arg);
		Query<T>& selectDesc(const std::string &arg);
		Query<T>& selectTimeInterval(DateTime dt1, DateTime dt2);
		Query<T>& selectDate(DateTime dt1);
		Query<T>& keysUnion(const Keys& keys);
		Query<T>& selectCategory(const std::string& arg);
		Query<T>& selectFilePath(const std::string& arg);

	private:
		DbCore<T> db_;
		Keys keys_;
	};

	//----< returns current key set for db >-----------------------------

	template<typename T>
	typename Query<T>::Keys& Query<T>::keys()
	{
		return keys_;
	}
	//----< Display all matching keys >-----------------------------------
	template<typename T>
	void Query<T>::show()
	{
		if (keys_.size() > 0)
		{
			std::cout << "\n\nDisplay all matching keys";
			std::cout << "\n====================================================================================\n";
			for (auto key : keys_)
			{
				std::cout << key << " ";
			}
		}
		else
		{
			std::cout << "\n Key didn't match metadata specified in a regular-expression pattern\n";
		}
	}

	/*select uses conds1 DbElement parts to match db elements for each q1 key
	and when done replaces its original keys with the new key set */

	template <typename T>
	Query<T>& Query<T>::select(const Conditions<T>& cond)
	{
		std::string name = cond.name();
		std::string desc = cond.descrip();
		if (!name.empty())
			selectName(name);
		if (!desc.empty())
			selectDesc(desc);
		return *this;
	}

	/*----< supports application defined queries for payload >---------*/
	/*
	*  - CallObj is defined by the application to return results from
	*    application's payload.  See test stub for example uses.
	*/
	template<typename P>
	template<typename CallObj>
	Query<P>& Query<P>::select(CallObj callObj)
	{
		Keys newKeys;
		for (auto item : db_)
		{
			if (callObj(item.second))
				newKeys.push_back(item.first);
		}
		keys_ = newKeys;
		return *this;
	}

	//----<Get all keys from the resulting query  >-----------------------------------
	template <typename T>
	Query<T>& Query<T>::from(Keys& keys)
	{
		keys_.clear();
		keys_ = keys;
		return *this;
	}

	//---- <Demonstrate the value of a specified key >-----------------------------------

	template <typename T>
	Query<T>& Query<T>::showSpecifiedKey(const Key& key)
	{
		std::cout << "\n Demonstrating the value of a specified key";
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		if (db_.contains(key))
		{
			std::cout << "\n\n    Found key \"" << key << "\"";
			std::cout << "\n------------------------------------------------------------------------------------";
			std::cout << "\n\n    The value of key: ";
			showElem(dbStore_[key]);
			std::cout << "\" \n";
		}
		else
			std::cout << "\n\n    didn't find key \"" << key << "\"\n";
		return *this;
	}

	//---- < Demonstrate the children of a specified key >-----------------------------------

	template <typename T>
	Query<T>& Query<T>::showChildrenOfSpecifiedKey(const Key& key)
	{
		std::cout << "\n Demonstrating the children of a specified key";
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		if (db_.contains(key))
		{
			std::cout << "\n\n    Found key: \"" << key << "\"";
			std::cout << "\n------------------------------------------------------------------------------------";
			std::cout << "\n\n    The children of specified key: \"" << key << "\"";
			typename DbElement<T>::Children children = dbStore_[key].children();
			std::cout << "\n------------------------------------------------------------------------------------";
			if (children.size() > 0)
			{
				std::cout << "\n\n    Child keys: ";
				for (auto key : children)
				{
					std::cout << " " << key;
				}
			}
			std::cout << "\n";
		}
		else
			std::cout << "\n\n   didn't find key \"" << key << "\"\n";
		return *this;
	}


	//---- < Demonstrate the set of all keys matching a specified regular-expression pattern >-----------------------------------

	template <typename T>
	Query<T>& Query<T>::showMatchKey(const std::string &arg)
	{
		std::cout << "\n Demonstrating the set of all keys matching a specified regular-expression pattern";
		// get key
		DbCore<std::string>::Keys dbCorekeys = db_.keys();
		Query<std::string>::Keys queryKeys;
		std::regex regx(arg);
		// iterate though those keys 
		std::cout << "\n\n Searching all keys with a specified regular-expression pattern \"" << arg << "\"";
		for (auto key : dbCorekeys)
		{
			if (std::regex_match(key, regx))
				queryKeys.push_back(key);
		}
		if (queryKeys.size() > 0)
		{
			std::cout << "\n\n    The matched keys: ";
			for (auto key : queryKeys)
			{
				std::cout << " " << key;
			}
		}
		else
			std::cout << "\n Key didn't match a specified regular-expression pattern\n";
		return *this;
	}

	//---- < Demonstrate all keys that contain a name in metadata section, where the specification is based on a regular-expression pattern >-----------------------------------

	template <typename T>
	Query<T>& Query<T>::selectName(const std::string &arg)
	{
		std::cout << "\n\n Demonstrating all keys that contain a name in metadata section, where the specification is based on a regular-expression pattern";
		// get key
		DbCore<PayLoad>::Keys queryKeys = keys_;
		DbCore<PayLoad>::DbStore& dbs = db_.dbStore();
		keys_.clear();
		std::regex regx(arg);
		// iterate though those keys 
		std::cout << "\n\n Searching all keys with a name specified in a regular-expression pattern \"" << arg << "\"";
		for (auto key : queryKeys)
		{
			if (std::regex_match(dbs[key].name(), regx))
				keys_.push_back(key);
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n    The matched keys: ";
			for (auto key : keys_)
			{
				std::cout << " " << key;
			}
			std::cout << "\n";
			std::cout << "\n====================================================================================";
			std::cout << "\n    The matched names: ";
			for (auto key : keys_)
			{
				std::cout << " " << dbs[key].name();
			}

		}
		else
			std::cout << "\n Key didn't match a name specified in a regular-expression pattern\n";
		return *this;
	}

	//---- < Demonstrate all keys that contain a description in metadata section, where the specification is based on a regular-expression pattern >-----------------------------------

	template<typename T>
	Query<T>& Query<T>::selectDesc(const std::string &arg)
	{
		std::cout << "\n\n  Demonstrating all keys that contain a description in metadata section, where the specification is based on a regular-expression pattern";
		// get key
		DbCore<PayLoad>::Keys queryKeys = keys_;
		DbCore<PayLoad>::DbStore& dbs = db_.dbStore();
		std::regex regx(arg);
		keys_.clear();
		// iterate though those keys 
		std::cout << "\n\n Searching all keys with a description specified in a regular-expression pattern \"" << arg << "\"";
		std::cout << "\n====================================================================================";
		for (auto key : queryKeys)
		{
			if (std::regex_match(dbs[key].descrip(), regx))
				keys_.push_back(key);
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n    The matched keys: ";
			for (auto key : keys_)
			{
				std::cout << " " << key;
			}
			std::cout << "\n";
			std::cout << "\n====================================================================================";
			std::cout << "\n    The matched description: \n";
			for (auto key : keys_)
			{
				std::cout << "    " << key << ": " << dbs[key].descrip() << "\n";
			}

		}
		else
			std::cout << "\n Key didn't match a description specified in a regular-expression pattern\n";
		std::cout << "\n";
		return *this;
	}

	//---- < Demonstrate all keys that match time interval in metadata section, where the specification is based on a regular-expression pattern >-----------------------------------

	template<typename T>
	Query<T>& Query<T>::selectTimeInterval(DateTime dt1, DateTime dt2)
	{
		std::cout << "\n\n Demonstrating all keys that match time interval in metadata section, where the specification is based on a regular-expression pattern";
		DbCore<PayLoad>::Keys queryKeys = keys_;
		DbCore<PayLoad>::DbStore& dbs = db_.dbStore();
		keys_.clear();
		for (auto key : queryKeys)
		{
			DbElement<PayLoad> dbElem = dbs[key];
			DateTime dt = dbElem.dateTime();
			if (dt2 > dt && dt > dt1)
				keys_.push_back(key);
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n    The matched keys: ";
			for (auto key : keys_)
			{
				std::cout << " " << key;
			}
			std::cout << "\n\n    The matched time interval: \n\n";
			for (auto key : keys_)
			{
				std::cout << "    " << key << ": " << dbs[key].dateTime().time() << "\n";
			}

		}
		else
			std::cout << "\n Key didn't match time interval specified in a regular-expression pattern\n";
		std::cout << "\n";
		return *this;
	}


	//---- < Demonstrate all keys that match time in metadata section, where the specification is based on a regular-expression pattern >-----------------------------------

	template<typename T>
	Query<T>& Query<T>::selectDate(DateTime dt1)
	{
		std::cout << "\n\n Demonstrating all keys that match time in metadata section, where the specification is based on a regular-expression pattern";
		// get key
		DbCore<PayLoad>::Keys queryKeys = keys_;
		DbCore<PayLoad>::DbStore& dbs = db_.dbStore();
		keys_.clear();
		DateTime dt2 = DateTime().now();
		DateTime::Duration dur = DateTime::makeDuration(0, 0, 1, 0);
		dt2 += dur;
		for (auto key : queryKeys)
		{
			DbElement<PayLoad> dbElem = dbs[key];
			DateTime dt = dbElem.dateTime();
			if (dt2 > dt && dt > dt1)
				keys_.push_back(key);
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n    The matched keys: ";
			for (auto key : keys_)
			{
				std::cout << " " << key;
			}
			std::cout << "\n\n    The matched time: \n\n";
			for (auto key : keys_)
			{
				std::cout << "    " << key << ": " << dbs[key].dateTime().time() << "\n";
			}
		}
		else
			std::cout << "\n Key didn't match time specified in a regular-expression pattern\n";
		std::cout << "\n";
		return *this;
	}

	//---- < Demonstrate the union of results of one or more previous queries, e.g., an "or"ing of multiple queries., where the specification is based on a regular-expression pattern >-----------------------------------

	template<typename T>
	Query<T>& Query<T>::keysUnion(const Keys& keys)
	{
		std::cout << "\n\n  Demonstrating the union of results of one or more previous queries, e.g., an \" or \"ing of multiple queries., where the specification is based on a regular-expression pattern \n\n";
		DbCore<std::string>::Keys copiedKeys = keys;
		for (auto ckey : copiedKeys)
		{
			if (std::find(keys_.begin(), keys_.end(), ckey) != keys_.end())
				continue;
			else
				keys_.push_back(ckey);
		}
		return *this;
	}

	//---- < Demonstrate all keys that match category in payload section, where the specification is based on a regular-expression pattern >-----------------------------------

	template <typename T>
	Query<T>& Query<T>::selectCategory(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		DbCore<T>::Keys categorykeys = keys_;
		keys_.clear();
		std::cout << "\n\n Searching all keys with a category specified in a regular-expression pattern \"" << arg << "\"";
		std::cout << "\n====================================================================================";
		for (DbCore<T>::Key key : categorykeys)
		{
			DbElement<T> dbElem = dbStore_[key];
			PayLoad dbElemPayload = dbElem.payLoad();
			PayLoad::Categories categories = dbElemPayload.category();
			for (auto category : categories)
			{
				if (category == arg)
					keys_.push_back(key);
			}
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n Found categories: \"" << arg << "\"\n";
			std::cout << "The matched category keys are:\n";
			for (DbCore<T>::Key categorykey : keys_)
				std::cout << categorykey << " ";
		}
		else
			std::cout << "\n Key didn't match category specified in a regular-expression pattern\n";
		return *this;
	}

	//---- < Demonstrate all keys that match filepath in payload section, where the specification is based on a regular-expression pattern

	template <typename T>
	Query<T>& Query<T>::selectFilePath(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		DbCore<T>::Keys filePathKeys = keys_;
		keys_.clear();
		std::cout << "\n\n Searching all keys with a category specified in a regular-expression pattern \"" << arg << "\"";
		std::cout << "\n====================================================================================";
		std::regex regx(arg);
		for (DbCore<T>::Key filePathKey : filePathKeys)
		{
			DbElement<T> dbElem = dbStore_[filePathKey];
			PayLoad dbElemPayload = dbElem.payLoad();
			std::string filepath = dbElemPayload.filePath();
			if (std::regex_match(filepath, regx))
			{
				keys_.push_back(filePathKey);
			}
		}

		if (keys_.size() > 0)
		{
			std::cout << "\n Found filepaths: \"" << arg << "\"\n";
			std::cout << "The matched filepath keys are:\n";
			for (DbCore<T>::Key filePathKey : keys_)
				std::cout << filePathKey << " ";
		}
		else
			std::cout << "\n Key didn't match filepath specified in a regular-expression pattern\n";
		return *this;
	}
}