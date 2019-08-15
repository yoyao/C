#pragma once

#define _WIN32_DCOM
#ifndef UNICODE
#define UNICODE
#endif
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include <wincred.h>
#include <vector>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "credui.lib")
#pragma comment(lib, "comsuppw.lib")

class WMIHelper
{
public:
	WMIHelper();
	~WMIHelper();

	typedef IWbemClassObject* WHV_WbemClassObject;

	int ConnectServer(const char*ip, const char* domain, const char*user, const char *pwd);
	int Query(const char *q_string, WHV_WbemClassObject *ppClassObj);
	int Query(std::string &q_string, WHV_WbemClassObject *ppClassObj);
	int GetValue(WHV_WbemClassObject pclsObj, const char *key, std::string &value);
	int GetValue(WHV_WbemClassObject pclsObj, const char *key,  double *value);
	int GetValue(WHV_WbemClassObject pclsObj, const char *key, int *value);
	int GetValue(WHV_WbemClassObject pclsObj, const char *key, std::vector<std::string> &values);
	void ReleaseWbemClassObject(WHV_WbemClassObject *pclsobj);
	int GetNextWbemClassObject(WHV_WbemClassObject *ppclsobj);

private:
	int Init();
	int CreateCoauthidentityAndProxyBlanket();
	int RealQuery(const char *q_string, WHV_WbemClassObject *webClassObj);
	int RealQuery(std::string &q_string, WHV_WbemClassObject *webClassObj);

private:
	char ip[24];
	char username[CREDUI_MAX_PASSWORD_LENGTH + 1];
	char pwd[CREDUI_MAX_PASSWORD_LENGTH + 1];
	char domain[CREDUI_MAX_USERNAME_LENGTH + 1];
	wchar_t pszAuthority[CREDUI_MAX_USERNAME_LENGTH + 1];

	IEnumWbemClassObject* pEnumerator;
	HRESULT hres;
	IWbemServices *pSvc = NULL;
	IWbemLocator *pLoc = NULL;
	COAUTHIDENTITY userAcct;
	
};

