#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally useful, helper classes             //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.2 : 22 Feb 2015
* - changed default underline char in Title(const std::string&, Char)
* ver 1.1 : 06 Feb 2015
* - fixed bug in split which turns a comma separated string into
*   a vector of tokens.
* - added comments
* ver 1.0 : 05 Feb 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"	

namespace XmlProcessing
{
	template<typename T>
	class XmlConvert
	{
	public:
		using SPtr = std::shared_ptr<AbstractXmlElement>;

		static SPtr toXmlElement(const T& t, const std::string& tag);
		static T fromXmlElement(SPtr sElem);
	};

	template<typename T>
	typename XmlConvert<T>::SPtr XmlConvert<T>::toXmlElement(const T& t, const std::string& tag)
	{
		using SPtr = XmlConvert<T>::SPtr;
		SPtr pElem = XmlProcessing::makeTaggedElement(tag);
		SPtr pText = XmlProcessing::makeTextElement(Utilities::Converter<T>::toString(t));
		pElem->addChild(pText);
		return pElem;
	}

	template<typename T>
	T XmlConvert<T>::fromXmlElement(SPtr pElem)
	{
		std::vector<SPtr> children = pElem->children();
		if (children.size() == 0)
			throw std::exception("text node does not exist");
		SPtr pText = children[0];
		std::string text = pText->value();
		T t = Utilities::Convert<T>::fromString(text);
		return t;
	}
}

#endif
