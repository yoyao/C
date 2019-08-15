
#pragma once

#include <string>
#include <iostream>
#include <atldbcli.h>
#include <atldbsch.h>
#include <memory>

#include "datatable.h"

class AccessHelper
{
public:
	AccessHelper();
	AccessHelper(const char *connstr);
	~AccessHelper();
	
public:
	bool Open();
	void BeginTransaction();
	void RollBack();
	void Commit();
	int ExecQuery(std::string &query, DataTable *table=NULL);
	int ExecQuery(const char *query, DataTable *table=NULL);
	void setConnectString(const char *conn);
	//非查询 返回影响记录的行数
	int ExecNoneQuery(const char *query);
	void Close();
	static char* ws2cs(const wchar_t* WStr, char *dest, int destlen);
	static wchar_t* cs2ws(const char* CStr, wchar_t *dest, int dstlen);

private:
	
private:
	bool isInited;
	
	HRESULT hr;
	//LPCOLESTR lpcOleConnect;
	WCHAR *lpcOleConnect;
	WCHAR * p_connect;
	CDataSource dbDataSource;
	CSession dbSession;
};

