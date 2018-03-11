#pragma once
#ifndef _TESTDBCORE_H_
#define _TESTDBCORE_H_
/////////////////////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype								   //
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
* This package provides three classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.
*
* - DbElement provides the value part of key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type.
*
* - Payload privides the name of a category associated with the Payload file
*   and the path to a file in the Repository.
*
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database
*   ----------------------------------------------------------
Database operations
*   ------------------------------------------------------------
*   - contains									: checks if db contain this key?
*   - keys										: returns current key set for db
*   - size										: return number of db elements
*   - operator[]								: extracts value from db with key
*   - operator[]								: extracts value from db with key
*   - showKeys									: displays database key set
*   - showHeader								: shows record header items
*   - showElem                                  : displays DbElements
*   - showDb									: displays all records in database
*   - addChild									: adds a child key to parent key (Addition of relationship)
*   - deleteRecord								: delete the key/value pair associated with the key
(deletion of key/value pairs) by removing the record from the db
*   - deleteChild								: delete all children associated with the key (deletion of relationships)
without deleting the record from the db
*   - getFiles									: finds all the files, matching pattern, in the entire directory tree rooted at repo.storagePath.
*   - getFilesHelper							: private helper function for getFiles function
*

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
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

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{

	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database

	template<typename T>
	class DbElement
	{
	public:
		using Key = std::string;
		using Children = std::vector<Key>;

		// methods to get and set DbElement fields

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		void name(const std::string& name) { name_ = name; }

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		void descrip(const std::string& name) { descrip_ = name; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		void children(const Children& children) { children_ = children; }

		T& payLoad() { return payLoad_; }
		T payLoad() const { return payLoad_; }
		void payLoad(const T& payLoad) { payLoad_ = payLoad; }

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		Children children_;
		T payLoad_;
	};

	/////////////////////////////////////////////////////////////////////
	// DbCore class
	// - provides core NoSql db operations
	// - does not provide editing, querying, or persistance operations

	template <typename T>
	class DbCore
	{
	public:
		//Type Aliases
		using Key = std::string;
		using Keys = std::vector<Key>;
		using Children = Keys;
		using DbStore = std::unordered_map<Key, DbElement<T>>;
		using iterator = typename DbStore::iterator;

		// methods to access database elements

		Keys keys(); // return all keys in db
		bool contains(const Key& key);
		size_t size();
		void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
		DbElement<T>& operator[](const Key& key);
		DbElement<T> operator[](const Key& key) const;
		typename iterator begin() { return dbStore_.begin(); }
		typename iterator end() { return dbStore_.end(); }
		bool addChild(const Key& parentKey, const Key& childKey);
		bool deleteRecord(const Key& key);
		bool deleteChild(const Key& parentKey, const Key& childKey);

		// methods to get and set the private database hash-map storage

		DbStore& dbStore() { return dbStore_; }
		DbStore dbStore() const { return dbStore_; }
		void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

	private:
		DbStore dbStore_;
		bool doThrow_ = false;
	};

	/////////////////////////////////////////////////////////////////////
	// DbCore<T> methods

	//----< does db contain this key? >----------------------------------

	template<typename T>
	bool DbCore<T>::contains(const Key& key)
	{
		iterator iter = dbStore_.find(key);
		return iter != dbStore_.end();
	}

	//----< returns current key set for db >-----------------------------

	template<typename T>
	typename DbCore<T>::Keys DbCore<T>::keys()
	{
		DbCore<T>::Keys dbKeys;
		DbStore& dbs = dbStore();
		size_t size = dbs.size();
		dbKeys.reserve(size);
		for (auto item : dbs)
		{
			dbKeys.push_back(item.first);
		}
		return dbKeys;
	}

	//----< return number of db elements >-------------------------------

	template<typename T>
	size_t DbCore<T>::size()
	{
		return dbStore_.size();
	}

	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes non-const db objects
	*  - In order to create a key-value pair in the database like this:
	*      db[newKey] = newDbElement
	*    we need to index with the new key and assign a default element.
	*    That will be replaced by newDbElement when we return from operator[]
	*  - However, if we want to index without creating new elements, we need
	*    to throw an exception if the key does not exist in the database.
	*  - The behavior we get is determined by doThrow_.  If false we create
	*    a new element, if true, we throw. Creating new elements is the default
	*    behavior.
	*/

	template<typename T>
	DbElement<T>& DbCore<T>::operator[](const Key& key)
	{
		if (!contains(key))
		{
			if (doThrow_)
				throw(std::exception("key does not exist in db"));
			else
				return (dbStore_[key] = DbElement<T>());
		}
		return dbStore_[key];
	}

	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes const db objects
	*/

	template<typename T>
	DbElement<T> DbCore<T>::operator[](const Key& key) const
	{
		if (!contains(key))
		{
			throw(std::exception("key does not exist in db"));
		}
		return dbStore_[key];
	}

	/////////////////////////////////////////////////////////////////////
	// display functions

	//----< display database key set >-----------------------------------

	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << "\n  ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
	}
	//----< show record header items >-----------------------------------

	inline void showHeader(bool showKey = true, std::ostream& out = std::cout)
	{
		out << "\n  ";
		if (showKey)
			out << std::setw(20) << std::left << "Key";
		out << std::setw(25) << std::left << "Name";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(30) << std::left << "Description";
		out << std::setw(25) << std::left << "Payload";
		out << "\n  ";
		if (showKey)
			out << std::setw(15) << std::left << "----------------";
		out << std::setw(25) << std::left << "-----------------------";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(30) << std::left << "------------------------";
		out << std::setw(25) << std::left << "-----------------";
	}
	//----< display DbElements >-----------------------------------------

	template<typename P>
	void showElem(const DbElement<P>& el, std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(25) << std::left << el.name().substr(0, 23);
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(30) << std::left << el.descrip().substr(0, 28);
		out << std::setw(25) << std::left << std::string(el.payLoad()).substr(0, 22);
		typename DbElement<P>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n    child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
	}
	using Key = std::string;
	template<typename P>
	void showRecord(const Key& key, const DbElement<P>& el, std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(20) << std::left << key.substr(0, 18);
		out << std::setw(25) << std::left << el.name().substr(0, 23);
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(30) << std::left << el.descrip().substr(0, 28);
		out << std::setw(25) << std::left << std::string(el.payLoad()).substr(0, 24);
		typename DbElement<P>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n    child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
	}


	//----< display all records in database >----------------------------

	template<typename T>
	void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		showHeader(true, out);
		typename DbCore<T>::DbStore dbs = db.dbStore();
		for (auto item : dbs)
		{
			showRecord(item.first, item.second, out);
		}
	}

	//----< add a child key to parent key (Addition of relationship) >----------------------------

	template <typename T>
	bool DbCore<T>::addChild(const Key& parentKey, const Key& childKey) {
		if (dbStore_.find(parentKey) == dbStore_.end())
			return false;
		if (dbStore_.find(childKey) == dbStore_.end())
			return false;
		DbElement<T>& el = dbStore_[parentKey];
		el.children().push_back(childKey);
		return true;
	}

	/*----< delete the key/value pair associated with the key (deletion of key/value pairs)
	by removing the record from the db >------------------*/

	template <typename T>
	bool DbCore<T>::deleteRecord(const Key& key)
	{
		std::cout << "\n\n  Deleting a db element with key \"" << key << "\":";
		if (dbStore_.find(key) == dbStore_.end())
			return false;
		size_t numErased = dbStore_.erase(key);
		if (numErased == 1)
		{
			// remove key from all other element's children collections
			for (Key k : keys())
			{
				Keys& children = dbStore_[k].children();  // note Keys& - we don't want a copy of children
				Keys::iterator iter;
				for (iter = children.begin(); iter != children.end(); ++iter)
				{
					if ((*iter) == key)
					{
						children.erase(iter);
						break;
					}
				}
			}
			return true;
		}
		return false;
	}

	/*----< delete all children associated with the key (deletion of relationships)
	without deleting the record from the db >----------------------------*/

	template <typename T>
	bool DbCore<T>::deleteChild(const Key& parentKey, const Key& childKey)
	{
		std::cout << "\n\n  Deleting a db element with key \"" << childKey << "\" from \"" << parentKey << "\" :\n";
		if (dbStore_.find(parentKey) == dbStore_.end())
			return false;
		for (Key k : keys())
		{
			if (k == parentKey)
			{
				Keys& children = dbStore_[k].children();  // note Keys& - we don't want copy of children
				Keys::iterator iter;
				for (iter = children.begin(); iter != children.end(); ++iter)
				{
					if ((*iter) == childKey)
					{
						children.erase(iter);
						break;
					}
				}
			}
		}
		return true;
	}
}

#endif
