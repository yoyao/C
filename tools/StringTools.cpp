#include "StringTools.h"


StringTools::StringTools()
{

}

void StringTools::StringSplit(std::string str, std::string &split, std::vector<std::string> *result)
{
	if (result == NULL)
	{
		return;
	}
	std::string::size_type pos = str.find(split);
	while (pos != std::string::npos)
	{
		std::string sub = str.substr(0, pos);
		if (sub.length() > 0)
		{
			result->push_back(sub);
		}
		str = str.substr(sub.length() + 1);
		pos = str.find(split);
	}
	if (str.length() > 0)
	{
		result->push_back(str);
	}

}

std::string StringTools::StringTrim(std::string pstr)
{
	if (pstr.length() < 1)
	{
		return;
	}
	std::string::iterator it = pstr.begin();

	while (it != pstr.end())
	{
		if (*it != ' ')
		{
			break;
		}
		it = pstr.erase(it);
	}

	std::string::reverse_iterator rit = pstr.rbegin();
	while (rit != pstr.rend())
	{
		if (*rit != ' ')
		{
			break;
		}
		rit = std::string::reverse_iterator(pstr.erase((++rit).base()));
	}

	return pstr;
}


StringTools::~StringTools()
{
}
