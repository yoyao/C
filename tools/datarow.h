#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

class DataRow
{
	typedef std::pair<std::string, std::string> RowCell;
	typedef std::vector<RowCell> Row;

public:
	DataRow();
	~DataRow();
	 DataRow(const DataRow &datarow);

public:
	std::string operator[](int index) const;
	std::string operator[](char *key) const;
	std::string operator[](std::string &key) const;

public:
	void PushBack(RowCell cell);
	void PushBack(std::string &key,std::string &value);
	void PushBack(const char*key, char *value);

	int CellCount() const;

protected:

private:
	Row *row;
};

