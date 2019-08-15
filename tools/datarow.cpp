#include "datarow.h"



DataRow::DataRow():row(NULL)
{
	try
	{
		row = new Row;
		if (!row)
		{
			row = NULL;
		}
	}
	catch (const std::exception&)
	{
		row = NULL;
	}

	
}

void DataRow::PushBack(RowCell cell)
{
	row->push_back(cell);
}

DataRow::DataRow(const DataRow &datarow)
{
	if (!row)
	{
		delete row;
		row = NULL;
	}
	if (NULL != datarow.row)
	{
		this->row = new Row(*(datarow.row));
	}
}


void DataRow::PushBack(std::string &key, std::string &value)
{
	row->push_back(std::pair<std::string,std::string>(key,value));
}

void DataRow::PushBack(const char*key, char *value)
{
	row->push_back(std::pair<std::string, std::string>(key, value));
}

int DataRow::CellCount() const
{
	return row->size();
}

std::string DataRow::operator[](int index) const
{
	if (index > -1 && index < row->size())
	{
		return (*row)[index].second;
	}
	
	return "";
}

std::string DataRow::operator[](char *key) const
{
	Row::iterator it;
	size_t len = this->row->size();
	if ( len> 0)
	{
		Row::iterator it_begin= this->row->begin();
		
		it = std::find_if(this->row->begin(), this->row->end(), [&](RowCell &val)->bool {
			if (val.first == key)
			{
				return true;
			}
			return false;
		});

		if (it == this->row->end())
		{
			return "";
		}
		return it->second;
	}
	return "";
}

std::string DataRow::operator[](std::string &key) const
{
	Row::iterator it;
	it = std::find_if(this->row->begin(), this->row->end(), [&](RowCell &val)->bool {
		if (val.first == key)
		{
			return true;
		}
		return false;
	});

	if (it == this->row->end())
	{
		return "";
	}
	return it->second;
}

DataRow::~DataRow()
{
	if (!row)
	{
		delete row;
		row = NULL;
	}
}
