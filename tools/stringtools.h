#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <wchar.h>
#include <regex>
#include <algorithm>
#include <string.h>

using namespace std;

class StringTools
{
public:
    static std::string StringTrim(std::string &pstr);
    static void StringSplit(std::string str, std::string &split, std::vector<std::string> *result);
    static void StringSplit(std::string str, const char *split, std::vector<std::string> *result);
    static bool DomainValid(const string &domain);//域名验证
    static bool DomainValid(const char *domain);//域名验证
    static bool IpAddressValid(const std::string &address);//IP验证
    static bool IpAddressValid(const char *address);//IP验证
    static int  IsGsmrDomain(const char* pName, unsigned int uLen);
    static int  IsGsmrDomain(const string &domain);
    static bool DotIpToCharArray(string domain, char *result);
    static bool DotIpToCharArray(const char *address,char *result=NULL);
    static char* strnstr(const char *s, const char *find, size_t slen);
    static unsigned char* ustrnstr(unsigned char* s, unsigned char* find, size_t slen);
    static unsigned short calc_crc16(unsigned char* pucFrame, unsigned int usLen);

    StringTools();
    ~StringTools();
};

