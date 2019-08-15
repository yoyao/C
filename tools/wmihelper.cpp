
#define _CRT_SECURE_NO_WARNINGS

#include "wmihelper.h"
#include "mainmonitor.h"
#include <strsafe.h>

char* ws2cs(const wchar_t* wc, char *dest, int destlen)
{
	if (NULL == wc || NULL == dest || wcslen(wc) < 1)
	{
		return NULL;
	}
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), dest, len, NULL, NULL);
	dest[len] = '\0';
	return dest;
}

wchar_t* cs2ws(const char* s, wchar_t *dest, int dstlen)
{
	if (NULL == s || NULL == dest || strlen(s) < 1)
	{
		return NULL;
	}
	int len = MultiByteToWideChar(CP_ACP, 0, s, strlen(s), NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, s, strlen(s), dest, len);
	dest[len] = '\0';
	return dest;
}

WMIHelper::WMIHelper()
{
	memset(this->domain, 0, sizeof(this->domain));
	memset(this->username, 0, sizeof(this->username));
	memset(this->ip, 0, sizeof(this->ip));
	memset(this->pwd, 0, sizeof(this->pwd));

}

int WMIHelper::Init()
{
	int ret = 0;
	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------
	CoUninitialize();
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. Error code = 0x" << hex << hres << endl;
		return 1;                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IDENTIFY,    // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres) && RPC_E_TOO_LATE != hres)
	{
		cout << "Failed to initialize security. Error code = 0x"<< hex << hres << endl;
		CoUninitialize();
		return 1;                    // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object." << " Err code = 0x" << hex << hres << endl;

		CoUninitialize();
		return 1;                 // Program has failed.
	}

	return ret;
}

int WMIHelper::ConnectServer(const char*ip, const char* domain, const char*user, const char *pwd)
{
	int ret = 0;

	if (!ip || !user || !pwd)
	{
		return -1;
	}
	std::cout << "connect wmi server hostip:" << ip;
	if (NULL != domain && strlen(domain)>0)
	{
		std::cout  << " domain:" << domain;
	}
	std::cout << " username:" << user << " password:" << pwd << std::endl;

	try
	{
		//初始化
		ret = this->Init();
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}
	if (ret != 0)
	{
		return -1;
	}
	wchar_t pszName[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszPwd[CREDUI_MAX_PASSWORD_LENGTH + 1] = { 0 };

	memcpy(this->ip, ip, strlen(ip));
	memcpy(this->username, user, strlen(user));
	memcpy(this->pwd, pwd, strlen(pwd));
	if (domain != NULL && strlen(domain)>0)
	{
		memcpy(this->domain, domain, strlen(domain));
	}
	
	if (NULL != domain && strlen(domain)>0)
	{
		std::string tuser(domain);
		tuser.append("\\");
		tuser.append(user);
		cs2ws(tuser.c_str(), pszName, sizeof(pszName));
	}
	else
	{
		cs2ws(user, pszName, sizeof(pszName));
	}

	cs2ws(pwd, pszPwd, sizeof(pszPwd));

	char linkPath[64] = { 0 };
	wchar_t wLinkPath[64] = { 0 };
	strcat(linkPath, "\\\\");
	strcat(linkPath, ip);
	strcat(linkPath, "\\root\\cimv2");

	cs2ws(linkPath, wLinkPath, sizeof(wLinkPath));
	try
	{
		hres = pLoc->ConnectServer(
			//_bstr_t(L"\\\\192.168.3.107\\root\\cimv2"),
			_bstr_t(wLinkPath),
			_bstr_t(pszName),    // User name
			_bstr_t(pszPwd),     // User password
			NULL,                              // Locale             
			NULL,                              // Security flags
			NULL,// Authority        
			NULL,                              // Context object 
			&pSvc                              // IWbemServices proxy
			);

		if (FAILED(hres))
		{
			std::string messg;
			messg += " hostip:"; 
			messg+=ip;
			if (NULL != domain)
			{
				messg += " domain:";
				messg += domain;
			}
			messg += " username:";
			messg += user;
			messg += " password:";
			messg += pwd; 
			messg += " connect wmi server failed\n";
			messg += "Could not connect. Error code = 0x";
			LogScreen(messg.c_str());

			pLoc->Release();
			CoUninitialize();
			return 1;                // Program has failed.
		}

	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}
	return ret;
}

int WMIHelper::CreateCoauthidentityAndProxyBlanket()
{
	wchar_t pszDomain[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszUserName[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszName[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszPwd[CREDUI_MAX_PASSWORD_LENGTH + 1] = { 0 };
	// step 5: --------------------------------------------------
	// Create COAUTHIDENTITY that can be used for setting security on proxy
	cs2ws(this->username, pszName, sizeof(pszName));
	cs2ws(this->pwd, pszPwd, sizeof(pszPwd));

	memset(&userAcct, 0, sizeof(COAUTHIDENTITY));
	userAcct.PasswordLength = wcslen(pszPwd);
	userAcct.Password = (USHORT*)pszPwd;

	LPWSTR slash = wcschr(pszName, L'\\');
	if (slash != NULL)
	{
		StringCchCopy(pszUserName, CREDUI_MAX_USERNAME_LENGTH + 1, slash + 1);
		//cs2ws(pwd, pszPwd, sizeof(pszPwd));
	}
	else
	{
		StringCchCopy(pszUserName, CREDUI_MAX_USERNAME_LENGTH + 1, pszName);
	}

	userAcct.User = (USHORT*)pszUserName;
	userAcct.UserLength = wcslen(pszUserName);

	userAcct.Domain = NULL;
	userAcct.DomainLength = 0;

	if (domain != NULL && strlen(this->domain) > 0)
	{
		memcpy(this->domain, domain, strlen(domain));
		cs2ws(domain, pszDomain, sizeof(pszDomain));
		userAcct.Domain = (USHORT*)pszDomain;
		userAcct.DomainLength = wcslen(pszDomain);
	}

	userAcct.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;


	// Step 6: --------------------------------------------------
	// Set security levels on a WMI connection ------------------
	try
	{
		hres = CoSetProxyBlanket(
			pSvc,                           // Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&userAcct,                       // client identity
			EOAC_NONE                       // proxy capabilities 
			);

		if (FAILED(hres))
		{
			cout << "Could not set proxy blanket. Error code = 0x" << hex << hres << endl;
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}

	}
	catch (std::exception ex)
	{
		throw ex;
	}

	return 0;
}

int WMIHelper::Query(const char *q_string, WHV_WbemClassObject *ppClassObj)
{
	int ret = 0;

	wchar_t pszDomain[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszUserName[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszName[CREDUI_MAX_USERNAME_LENGTH + 1] = { 0 };
	wchar_t pszPwd[CREDUI_MAX_PASSWORD_LENGTH + 1] = { 0 };

	// step 5: --------------------------------------------------
	// Create COAUTHIDENTITY that can be used for setting security on proxy
   #pragma region Create COAUTHIDENTITY

	cs2ws(this->username, pszName, sizeof(pszName));
	cs2ws(this->pwd, pszPwd, sizeof(pszPwd));

	memset(&userAcct, 0, sizeof(COAUTHIDENTITY));
	userAcct.PasswordLength = wcslen(pszPwd);
	userAcct.Password = (USHORT*)pszPwd;

	LPWSTR slash = wcschr(pszName, L'\\');
	if (slash != NULL)
	{
		StringCchCopy(pszUserName, CREDUI_MAX_USERNAME_LENGTH + 1, slash + 1);
		//cs2ws(pwd, pszPwd, sizeof(pszPwd));
	}
	else
	{
		StringCchCopy(pszUserName, CREDUI_MAX_USERNAME_LENGTH + 1, pszName);
	}

	userAcct.User = (USHORT*)pszUserName;
	userAcct.UserLength = wcslen(pszUserName);

	userAcct.Domain = NULL;
	userAcct.DomainLength = 0;

	if (domain != NULL && strlen(this->domain) > 0)
	{
		memcpy(this->domain, domain, strlen(domain));
		cs2ws(domain, pszDomain, sizeof(pszDomain));
		userAcct.Domain = (USHORT*)pszDomain;
		userAcct.DomainLength = wcslen(pszDomain);
	}

	userAcct.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

#pragma endregion

	// Step 6: --------------------------------------------------
	// Set security levels on a WMI connection ------------------
   #pragma region Set security levels on a WMI connection

	try
	{

		hres = CoSetProxyBlanket(
			pSvc,                           // Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&userAcct,                       // client identity
			EOAC_NONE                       // proxy capabilities 
			);

		if (FAILED(hres))
		{
			cout << "Could not set proxy blanket. Error code = 0x" << hex << hres << endl;
			//pSvc->Release();
			//pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}

	}
	catch (std::exception ex)
	{
		throw ex;
	}

#pragma endregion

	try
	{
		ret = RealQuery(q_string, ppClassObj);
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}

	return ret;
}

int WMIHelper::Query(std::string &q_string, WHV_WbemClassObject *ppClassObj)
{
	int ret = 0;
	try
	{
		ret = Query(q_string.c_str(), ppClassObj);
	}
	catch (std::exception ex)
	{
		throw ex;
	}
	return ret;
}

int WMIHelper::RealQuery(std::string &q_string, WHV_WbemClassObject *ppClassObj)
{
	int ret = 0;
	try
	{
		ret = RealQuery(q_string.c_str(), ppClassObj);
	}
	catch (std::exception ex)
	{
		throw ex;
	}
	return ret;
}

int WMIHelper::RealQuery(const char *q_string, WHV_WbemClassObject *ppClassObj)
{
	int ret = 0;

	// Step 7: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	try
	{
		hres = pSvc->ExecQuery(
			bstr_t("WQL"),
			bstr_t(q_string),//查系统的语句
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			cout << "Query for operating system name failed." << " Error code = 0x" << hex << hres << endl;
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			getchar();
			return 1;               // Program has failed.
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}

	// Step 8: -------------------------------------------------
	// Secure the enumerator proxy
	try
	{
		hres = CoSetProxyBlanket(
			pEnumerator,                    // Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&userAcct,                       // client identity
			EOAC_NONE                       // proxy capabilities 
			);

		if (FAILED(hres))
		{
			cout << "Could not set proxy blanket on enumerator. Error code = 0x" << hex << hres << endl;
			pEnumerator->Release();
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}
	// Step 9: -------------------------------------------------
	// Get the data from the query in step 7 -------------------

	/*
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
	HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

	if (0 == uReturn)
	{
	//std::cout << "TMD  break le" << std::endl;
	break;
	}

	VARIANT vtProp;
	wchar_t wkey[64] = { 0 };
	cs2ws(key, wkey, sizeof(wkey));
	// Get the value of the Name property
	hr = pclsObj->Get(wkey, 0, &vtProp, 0, 0);
	//wcout << " OS Name : " << vtProp.bstrVal << endl;
	char buff[1024] = { 0 };
	ws2cs(vtProp.bstrVal, buff, sizeof(buff));
	value = buff;
	// Get the value of the FreePhysicalMemory property
	//hr = pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
	//	wcout << " Free physical memory (in kilobytes): "<< vtProp.uintVal << endl;
	VariantClear(&vtProp);

	pclsObj->Release();
	pclsObj = NULL;
	}
	*/

	// Cleanup
	// ========
	/*
	pEnumerator->Release();
	if (pclsObj)
	{
	pclsObj->Release();
	}
	*/
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	if (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn)
		{
			//std::cout << "TMD  break le" << std::endl;
			//break;
			return 1;
		}
	}
	*ppClassObj = pclsObj;

	return ret;
}

int WMIHelper::GetNextWbemClassObject(WHV_WbemClassObject *ppclsobj)
{
	int ret = 0;

	IWbemClassObject *p_clsObj = NULL;
	ULONG uReturn = 0;
	if (pEnumerator && ppclsobj !=NULL)
	{
		try
		{
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &p_clsObj, &uReturn);
		}
		catch (std::exception ex)
		{
			throw ex;
			return -1;
		}

		if (0 == uReturn)
		{
			//std::cout << "TMD  break le" << std::endl;
			//break;
			*ppclsobj = NULL;
			return 1;
		}
	}

	(*ppclsobj) = p_clsObj;

	return ret;
}

int WMIHelper::GetValue(WHV_WbemClassObject pclsObj, const char *key, std::string &value)
{
	int ret = 0;
	HRESULT hr;
	if (pclsObj == NULL)
	{
		return -1;
	}
	VARIANT vtProp;
	memset(&vtProp, 0, sizeof(vtProp));
	wchar_t wkey[64] = { 0 };
	cs2ws(key, wkey, sizeof(wkey));
	// Get the value of the Name property
	hr = pclsObj->Get(wkey, 0, &vtProp, 0, 0);
	
	//wcout << " OS Name : " << vtProp.bstrVal << endl;
	try
	{
		if (vtProp.bstrVal != NULL && wcslen(vtProp.bstrVal) > 0)
		{
			char buff[1024] = { 0 };
			ws2cs(vtProp.bstrVal, buff, sizeof(buff));
			value = buff;
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}
	
	
	VariantClear(&vtProp);
	
	return ret;
}

int WMIHelper::GetValue(WHV_WbemClassObject pclsObj, const char *key, double *value)
{
	int ret = 0;
	HRESULT hr;
	if (pclsObj == NULL)
	{
		return -1;
	}
	VARIANT vtProp;
	memset(&vtProp, 0, sizeof(vtProp));
	wchar_t wkey[64] = { 0 };
	cs2ws(key, wkey, sizeof(wkey));
	// Get the value of the Name property
	hr = pclsObj->Get(wkey, 0, &vtProp, 0, 0);

	//wcout << " OS Name : " << vtProp.bstrVal << endl;

	(*value) = vtProp.fltVal;

	// Get the value of the FreePhysicalMemory property
	//hr = pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
	//	wcout << " Free physical memory (in kilobytes): "<< vtProp.uintVal << endl;
	VariantClear(&vtProp);

	return ret;
}

int WMIHelper::GetValue(WHV_WbemClassObject pclsObj, const char *key, int *value)
{
	int ret = 0;
	HRESULT hr;
	if (pclsObj == NULL)
	{
		return -1;
	}
	VARIANT vtProp;
	memset(&vtProp, 0, sizeof(vtProp));
	wchar_t wkey[64] = { 0 };
	cs2ws(key, wkey, sizeof(wkey));
	// Get the value of the Name property
	hr = pclsObj->Get(wkey, 0, &vtProp, 0, 0);
	try
	{
		//wcout << " OS Name : " << vtProp.bstrVal << endl;
		if (vtProp.bstrVal != NULL && wcslen(vtProp.bstrVal) > 0)
		{
			char buff[1024] = { 0 };
			ws2cs(vtProp.bstrVal, buff, sizeof(buff));
			(*value) = atoi(buff);
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}
	

	// Get the value of the FreePhysicalMemory property
	//hr = pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
	//	wcout << " Free physical memory (in kilobytes): "<< vtProp.uintVal << endl;
	VariantClear(&vtProp);

	return ret;
}

int WMIHelper::GetValue(WHV_WbemClassObject pclsObj, const char *key, std::vector<std::string> &values)
{
	int ret = 0;
	HRESULT hr;
	if (pclsObj == NULL)
	{
		return -1;
	}
	VARIANT vtProp;
	memset(&vtProp, 0, sizeof(vtProp));
	wchar_t wkey[64] = { 0 };
	cs2ws(key, wkey, sizeof(wkey));
	// Get the value of the Name property
	hr = pclsObj->Get(wkey, 0, &vtProp, 0, 0);

	try
	{
		if ((vtProp.vt & VT_ARRAY))
		{
			long lLower = 0, lUpper = 0;
			SAFEARRAY *pSafeArray = vtProp.parray;
			SafeArrayGetLBound(pSafeArray, 1, &lLower);//数组的下界
			SafeArrayGetUBound(pSafeArray, 1, &lUpper);//数组的上界

			for (long i = lLower; i <= lUpper; i++)
			{
				BSTR buf = NULL;
				hres = SafeArrayGetElement(pSafeArray, &i, &buf);
				char buff[1024] = { 0 };
				ws2cs((wchar_t*)buf, buff, sizeof(buff));
				if (strlen(buff)>0)
				{
					values.push_back(buff);
				}
			}

			SafeArrayUnaccessData(vtProp.parray);
		}
	}
	catch (std::exception ex)
	{
		throw ex;
		return -1;
	}


	// Get the value of the FreePhysicalMemory property
	//hr = pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
	//	wcout << " Free physical memory (in kilobytes): "<< vtProp.uintVal << endl;
	VariantClear(&vtProp);

	return ret;
}

void WMIHelper::ReleaseWbemClassObject(WHV_WbemClassObject *pclsobj)
{
	if (pclsobj != NULL && *pclsobj != NULL)
	{
		(*pclsobj)->Release();
		(*pclsobj) = NULL;
	}
		
}

WMIHelper::~WMIHelper()
{
	try
	{
		CoUninitialize();
		//		pSvc->Release();
		//		pLoc->Release();
	}
	catch (std::exception ex)
	{

	}

}
