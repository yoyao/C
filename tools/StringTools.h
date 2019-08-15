#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <wchar.h>

class StringTools
{
public:
	static std::string StringTrim(std::string pstr);
	static void StringSplit(std::string str, std::string &split, std::vector<std::string> *result);
	StringTools();
	~StringTools();
};

