
#pragma once
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <iostream>
#include <list>

#include "datacolumn.h"
#include "datarow.h"

class DataTable
{
	typedef std::vector<DataRow> RowList;
	typedef std::vector<DataColumn> ColumnList;
	typedef std::list<std::string> Headers;

public:
	DataTable();
	DataTable(int rows,int cols);
	DataTable(const DataTable & datatable);
	virtual ~DataTable();

public:
	void setHeaders(std::list<std::string>& pheaders);
	void setHeaders(std::vector<std::string>& pheaders);
	void setHeaders(const char **pheaders, int count);
	void setHeaders( char **pheaders, int count);
	void addHeader(char *pheader);
	void addHeader(const char *pheader);
	void addHeader(std::string &pheader);

	void PushBack(DataRow &prow);
	void PushBack(DataRow *prow);
	void PushBack(DataColumn &pcolumn);
	void PushBack(DataColumn *pcolumn);
	int RowsCount() const;
	int ColumnsCount() const;

public:
	DataRow operator[](const int index)const;
	DataColumn operator[](const char* key) const;
	DataColumn operator[](char* key) const;
	DataColumn operator[](const std::string &key) const;
	DataTable& operator=(const DataTable& table);

protected:

private:
	static std::string& MoveString(std::string &para);

private:
	RowList *rowlist;
	ColumnList *collist;
	Headers headers;
	int rows;
	int cols;
};

