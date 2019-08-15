#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>

class DataColumn
{
	typedef std::vector<std::string> ColValue;
	typedef std::map<std::string,ColValue> Column;

public:
	DataColumn();
	DataColumn(std::string &header);
	DataColumn(const char* header);
	DataColumn(const DataColumn & pcolumn);
	~DataColumn();
	
public:
	void PushBack(std::string &value);
	void PushBack(const char *value);
	void setHeader(std::string &header);
	void setHeader(const char *header);
	std::string getHeader();

	int  size()const;

	std::string operator[](int index) const;
	DataColumn& operator=(const DataColumn &pcolumn);
	bool operator<(const DataColumn &pcolumn) const;
	bool operator>(const DataColumn &pcolumn) const;

protected:

private:
	Column *column;
	std::string key;
};

