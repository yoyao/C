#include "datatable.h"

DataTable::DataTable():cols(-1),rows(-1), rowlist(NULL),collist(NULL)
{
	rowlist = new RowList;
	collist = new ColumnList;
}

DataTable::DataTable(int rows, int cols):rowlist(NULL), collist(NULL)
{
	this->rows = rows;
	this->cols = cols;

	rowlist = new RowList;
	collist = new ColumnList;

}

DataTable::DataTable(const DataTable & datatable)
{
	if (NULL != this->rowlist)
	{
		delete this->rowlist;
		rowlist = NULL;
	}
	this->rowlist = datatable.rowlist;

	if (NULL != this->collist)
	{
		delete this->collist;
		this->collist = NULL;
	}
	this->collist = datatable.collist;

	this->headers = datatable.headers;
	this->rows = datatable.rows;
	this->cols = datatable.cols;
}

DataTable& DataTable::operator=(const DataTable& table)
{

	return *this;
}

std::string& DataTable::MoveString(std::string &para)
{
	return para;
}

void DataTable::setHeaders(std::list<std::string>& pheaders)
{
	if (pheaders.size() >0)
	{
		this->headers = pheaders;

		for (std::string &t_header : pheaders)
		{
			DataColumn column(t_header);
			this->collist->push_back(column);
		}
	}

}

void DataTable::setHeaders(std::vector<std::string>& pheaders)
{
	if (pheaders.size() < 1)
	{
		return;
	}

	this->headers.clear();
	this->headers.resize(pheaders.size());

	std::transform(pheaders.begin(), pheaders.end(), this->headers.begin(), MoveString);

	for (std::string &t_header : this->headers)
	{
		DataColumn column(t_header);
		this->collist->push_back(column);
	}
}

void DataTable::setHeaders(const char **pheaders,int count)
{
	if (NULL == pheaders || count < 1)
	{
		return;
	}
	for (size_t i = 0; i < count; i++)
	{
		if (NULL !=pheaders[i])
		{
			DataColumn column(pheaders[i]);
			this->collist->push_back(column);
			this->headers.push_back(pheaders[i]);
		}	
	}
	//std::cout << " column count " << this->collist->size() << std::endl;
}

void DataTable::setHeaders(char **pheaders, int count)
{
	if (NULL == pheaders || count < 1)
	{
		return;
	}
	for (size_t i = 0; i < count; i++)
	{
		if (NULL != pheaders[i])
		{
			DataColumn column(pheaders[i]);
			this->collist->push_back(column);
			this->headers.push_back(pheaders[i]);
			//std::cout << pheaders[i] << std::endl;
		}
	}
	//std::cout << " column count " << this->collist->size() << std::endl;
}

void DataTable::addHeader(char *pheader)
{
	if (NULL != pheader)
	{
		DataColumn column(pheader);
		this->collist->push_back(column);
		this->headers.push_back(pheader);
	}
}

void DataTable::addHeader(const char *pheader)
{
	if (NULL != pheader)
	{
		DataColumn column(pheader);
		this->collist->push_back(column);
		this->headers.push_back(pheader);
	}
}
void DataTable::addHeader(std::string &pheader)
{
	if (!pheader.empty())
	{
		DataColumn column(pheader);
		this->collist->push_back(column);
		this->headers.push_back(pheader);
	}
}

void DataTable::PushBack(DataRow &prow)
{
	this->rowlist->push_back(prow);
}

void DataTable::PushBack(DataRow *prow)
{
	this->rowlist->push_back(*prow);
}

void DataTable::PushBack(DataColumn &pcolumn)
{
	this->collist->push_back(pcolumn);
}

void DataTable::PushBack(DataColumn *pcolumn)
{
	this->collist->push_back(*pcolumn);
}

DataRow DataTable::operator[](const int index) const
{
	return (*rowlist)[index];
}

DataColumn DataTable::operator[](const char* key) const
{
	const char* head = key;
	ColumnList::iterator it;
	it=	std::find_if(this->collist->begin(), this->collist->end(), [&](DataColumn &val)->bool {
		if (val.getHeader() == head)
		{
			return true;
		}
		return false;
	});

	return *it;
}

DataColumn DataTable::operator[]( char* key) const
{
	const char* head = key;
	ColumnList::iterator it;
	it = std::find_if(this->collist->begin(), this->collist->end(), [&](DataColumn &val)->bool {
		if (val.getHeader() == head)
		{
			return true;
		}
		return false;
	});

	return *it;
}

DataColumn DataTable::operator[](const std::string &key) const
{
	const std::string head = key;
	ColumnList::iterator it;
	it = std::find_if(this->collist->begin(), this->collist->end(), [&](DataColumn &val)->bool {
		if (val.getHeader() == head)
		{
			return true;
		}
		return false;
	});
	if (it == this->collist->end())
	{
		return NULL;
	}
	return *it;
}

int DataTable::RowsCount() const
{
	return this->rowlist->size();
}

int DataTable::ColumnsCount() const
{
	return this->collist->size();
}

DataTable::~DataTable()
{
	if (NULL !=rowlist)
	{
		delete rowlist;
		rowlist = NULL;
	}

	if (NULL != collist)
	{
		delete collist;
		collist = NULL;
	}
}
