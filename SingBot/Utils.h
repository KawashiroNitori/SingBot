#pragma once
#include "stdafx.h"

using namespace std;

class Utils
{
public:
	static int AuthCode;
public:
	static string convertGBKtoUTF8(string str);
	static string b64encode(string str);
	static string b64decode(string str);
	static string AESEncrypt(string data, string key, string iv);
	static string RSAEncrypt(string data, string N, string E);
	static string hexlify(string data);
	static string toLowerCase(string str);
	static string randomStr(int length = 16);
	static size_t copyDataCallback(void* ptr, size_t size, size_t nmemb, void* stream);
	static string UrlEncode(const string& str);
	static unsigned char ToHex(unsigned char x);
};

