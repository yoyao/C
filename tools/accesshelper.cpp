#include "accesshelper.h"

using namespace std;

AccessHelper::AccessHelper() :lpcOleConnect(NULL), isInited(false)
{
	// Uses ATL's string conversion macros to convert between character encodings.
	USES_CONVERSION;
	CoUninitialize();
	CoInitialize(NULL);         //初始化OLE/COM库环境 
}

AccessHelper::AccessHelper(const char *connstr) :lpcOleConnect(NULL), isInited(false)
{
	// Uses ATL's string conversion macros to convert between character encodings.
	USES_CONVERSION;
	CoUninitialize();
	CoInitialize(NULL);         //初始化OLE/COM库环境 
	setConnectString(connstr);
}

void AccessHelper::setConnectString(const char *conn)
{
	//Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Northwind 2007.accdb;User Id=admin;Password=;
	if (!isInited)
	{
		if (NULL != this->lpcOleConnect)
		{
			delete[] this->lpcOleConnect;
		}
		size_t len = strlen(conn) + 1;
		WCHAR *connstr = new WCHAR[len];
		this->lpcOleConnect = connstr;
		if (this->lpcOleConnect == NULL)
		{
			return;
		}
		p_connect = connstr;

		this->cs2ws(conn, (wchar_t*)lpcOleConnect, len);
	}
	
}

bool AccessHelper::Open()
{
	DWORD lerr = GetLastError();
	try
	{
		hr = dbDataSource.OpenFromInitializationString(lpcOleConnect);
		if (FAILED(hr))
		{
			wcout << ": Unable to connect to data source " << OLE2T((LPOLESTR)lpcOleConnect) << "\n errno 0x" << hex << hr << endl;
			std::cout << "是否安装数据库依赖？请尝试安装dependent目录下AccessDatabaseEngine.exe文件" << std::endl;
			lerr = GetLastError();
			return false;
		}
		else
		{
			hr = dbSession.Open(dbDataSource);
			if (FAILED(hr))
			{
				wcout << ":Couldn't create session on data source " << OLE2T((LPOLESTR)lpcOleConnect) << endl;
				return false;
			}
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return false;
	}
	

	CComVariant var;
	try
	{
		hr = dbDataSource.GetProperty(DBPROPSET_DATASOURCEINFO, DBPROP_DATASOURCENAME, &var);
		if (FAILED(hr) || (var.vt == VT_EMPTY))
		{
			//wcout << DAM << ": No Data Source Name Specified." << endl;
			return false;
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return false;
	}
	
	isInited = true;
	return true;
}

int AccessHelper::ExecQuery(std::string &query, DataTable *table)
{
	int ret = 0;
	try
	{
		ret = ExecQuery(query.c_str(), table);
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}
	
	return ret;
}

int AccessHelper::ExecQuery(const char *query,DataTable *table)
{
	int ret = 0;
	if (NULL == query || strlen(query) < 1 || !isInited)
	{
		return -1;
	}
	CCommand<CDynamicStringAccessor> cmd;
	try
	{
		hr = cmd.Open(dbSession, query);
		if (!SUCCEEDED(hr))
		{
			return -1;
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}


	DBORDINAL colCount = cmd.GetColumnCount();
	if (SUCCEEDED(hr) && 0 < colCount)
	{
	//	wcout << DAM << ": Retrieve schema info for the given result set: " << endl;
		if (table != NULL)
		{
			DBORDINAL cColumns;
			DBCOLUMNINFO* rgInfo = NULL;
			OLECHAR* pStringsBuffer = NULL;
			cmd.GetColumnInfo(&cColumns, &rgInfo, &pStringsBuffer);
			for (int col = 0; col < (int)colCount; col++)
			{
				//std::cout << " | " << OLE2T(rgInfo[col].pwszName);
				//此处全是字段名称 也就是列名
				char colName[64] = { 0 };
				ws2cs(rgInfo[col].pwszName, colName, sizeof(colName));
				table->addHeader(colName);
			}
			//wcout << endl;

			//wcout << DAM << ": Fetch the actual data: " << endl;
			int rowCount = 0;
			CRowset<CDynamicStringAccessor>* pRS = (CRowset<CDynamicStringAccessor>*)&cmd;
			// Loop through the rows in the result set.
			while (pRS->MoveNext() == S_OK)//一次一行
			{
				DataRow row;
				for (int col = 1; col <= (int)colCount; col++)
				{
					WCHAR* szValue = cmd.GetString(col);
					//此处全是每个单元格的值
					//wcout << " | " << szValue;
					char cellValue[256] = { 0 };
					ws2cs(szValue, cellValue, sizeof(cellValue));

					char colName[64] = { 0 };
					ws2cs(rgInfo[col-1].pwszName, colName, sizeof(colName));

					std::pair<std::string, std::string> pair_ = std::make_pair(colName,cellValue);
					row.PushBack(pair_);
				}
				//std::cout << std::endl;
				table->PushBack(row);
				rowCount++;

			}
		}
		//wcout << ": Total Row Count: " << rowCount << endl;
	}
	else
	{
		//wcout << DAM << ": Error: Number of fields in the result set is 0." << endl;
	}

	return ret;
}

int AccessHelper::ExecNoneQuery(const char *query)
{
	if (NULL == query || strlen(query) < 1 || !isInited)
	{
		return -1;
	}
	int ret = 0;
	long row = 0;
	CCommand<CDynamicStringAccessor> cmd;
	try
	{
		hr = cmd.Open(dbSession, query, NULL, &row);
		if (SUCCEEDED(hr) || RPC_E_TOO_LATE == hr)
		{
			ret = row;
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}

	return ret;
}

char* AccessHelper::ws2cs(const wchar_t* WStr, char *dest, int destlen)
{
	/*
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), dest, len, NULL, NULL);
	dest[len] = '\0';
	*/
	size_t len = wcslen(WStr)+1;
	size_t converted = 0;
	char *CStr=dest;
	wcstombs_s(&converted, CStr, len, WStr, _TRUNCATE);

	return dest;
}

wchar_t* AccessHelper::cs2ws(const char* CStr, wchar_t *dest, int dstlen)
{
	size_t len = strlen(CStr);
	size_t converted = 0;
	wchar_t *WStr=dest;
	mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
	//其结果是WStr中储存了CStr的wchar_t版本。

	return dest;
}

#define SAFE_DELETE(p) if(p !=NULL) { delete p; (p)  = NULL;}  
#define SAFE_DELETE_ARRAY(p) if(p !=NULL) { delete[] p; (p)  = NULL;}  

AccessHelper::~AccessHelper()
{
	if (lpcOleConnect != NULL)
	{
		delete[] lpcOleConnect;
	}
	
}

void AccessHelper::Close()
{
	if (lpcOleConnect != NULL)
	{
		delete[] lpcOleConnect;
	}
	dbDataSource.Close();
	dbSession.Close();

	CoUninitialize();
}

void AccessHelper::BeginTransaction()
{
	dbSession.StartTransaction();
}

void AccessHelper::Commit()
{
	this->dbSession.Commit();
}

void  AccessHelper::RollBack()
{
	
}
