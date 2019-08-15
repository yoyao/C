#include "DataColumn.h"

DataColumn::DataColumn():column(NULL)
{
	column = new Column;
	if (!column)
	{
		return;
	}
}

DataColumn::DataColumn(const DataColumn &pcolumn)
{
	if (!column)
	{
		delete column;
	}
	this->column = pcolumn.column;
	this->key = pcolumn.key;
}

DataColumn::DataColumn(std::string &header) : column(NULL)
{
	column = new Column;
	std::pair<std::string, ColValue> par = std::make_pair(header, ColValue());
	column->insert(par);
	key = header;
}

DataColumn::DataColumn(const char* header) : column(NULL)
{
	column = new Column;
	std::pair<std::string, ColValue> par = std::make_pair(header, ColValue());
	column->insert(par);
	key = header;
}

void DataColumn::PushBack(std::string &value)
{
	if (this->column->size() < 1)
	{
		return;
	}

	ColValue &values = this->column->at(key);
	//	int coutn = values.size();
	values.push_back(value);

}

void DataColumn::PushBack(const char *value)
{
	if (this->column->size() < 1)
	{
		return;
	}

	ColValue &values=this->column->at(key);
//	int coutn = values.size();
	values.push_back(value);
int count=	values.size();

}

void DataColumn::setHeader(std::string &header)
{
	if (this->column->size() > 0)
	{
		return;
	}
	key = header;
	std::pair<std::string, ColValue> par = std::make_pair(header, ColValue());
	column->insert(par);
}

void DataColumn::setHeader(const char *header)
{
	if (this->column->size() > 0)
	{
		return;
	}
	key = header;
	std::pair<std::string, ColValue> par = std::make_pair(header, ColValue());
	column->insert(par);
}

std::string DataColumn::getHeader()
{
	return this->key;
}

std::string DataColumn::operator[](int index) const
{
	if (this->column->size() >0)
	{
		if (index > -1 && index < this->column->at(key).size())
		{
			return this->column->at(key)[index];
		}
	}

	return "";
}

DataColumn& DataColumn::operator=(const DataColumn &pcolumn)
{
	if (!column)
	{
		delete column;
	}
	this->column = pcolumn.column;
	this->key = pcolumn.key;

	return *this;
}

bool DataColumn::operator<(const DataColumn &pcolumn) const
{
	if (this->column < pcolumn.column)
	{
		return true;
	}
	return false;
}

bool DataColumn::operator>(const DataColumn &pcolumn) const
{
	if (this->column < pcolumn.column)
	{
		return false;
	}
	return true ;
}


int DataColumn::size() const
{
	if (this->column->size() > 0)
	{
		return this->column->at(key).size();
	}
	return 0;
}

DataColumn::~DataColumn()
{
	if (!column)
	{
		delete column;
		column = NULL;
	}
}
