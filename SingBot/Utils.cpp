#ifdef _DEBUG
#pragma comment(lib, "..\\cryptopp\\Win32\\Output\\Debug\\cryptlib.lib")
#else
#pragma comment(lib, "..\\cryptopp\\Win32\\Output\\Release\\cryptlib.lib")
#endif


#include "Utils.h"
#include "base64.h"
#include "aes.h"
#include "modes.h"
#include "hex.h"
#include "integer.h"
#include "nbtheory.h"
#include "misc.h"
#include "cqsdk/cqp.h"
#include <locale>
#include <codecvt>
#include <random>
#include <ctime>

using namespace CryptoPP;


int Utils::AuthCode = -1;

std::string Utils::convertGBKtoUTF8(string str)
{
	const char GBK_LOCALE_NAME[] = ".936";
	wstring_convert<codecvt_byname<wchar_t, char, mbstate_t>> cv1(new codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
	wstring tmp_wstr = cv1.from_bytes(str);

	wstring_convert<codecvt_utf8<wchar_t>> cv2;
	string utf8_str = cv2.to_bytes(tmp_wstr);
	return utf8_str;
}

std::string Utils::b64encode(string str)
{
	string encoded;
	CryptoPP::Base64Encoder encoder;
	encoder.Put((byte*)str.data(), str.size());
	encoder.MessageEnd();

	int size = (int)encoder.MaxRetrievable();
	if (size)
	{
		encoded.resize(size);
		encoder.Get((byte*)encoded.data(), encoded.size());
	}
	encoded.erase(std::remove(encoded.begin(), encoded.end(), '\n'), encoded.end());
	return encoded;
}

std::string Utils::b64decode(string str)
{
	string decoded;
	CryptoPP::Base64Decoder decoder;
	decoder.Put((byte*)str.data(), str.size());
	decoder.MessageEnd();

	int size = (int)decoder.MaxRetrievable();
	if (size)
	{
		decoded.resize(size);
		decoder.Get((byte*)decoded.data(), decoded.size());
	}
	
	return decoded;
}

std::string Utils::AESEncrypt(string data, string key, string iv)
{
	string cipher;
	int padding = 16 - data.length() % 16;
	for (int i = 0; i < padding; i++)
		data += (char)padding;
	cipher.resize(data.size());
	CBC_Mode<AES>::Encryption cbcEncrypt((byte*)key.data(), key.size(), (byte*)iv.data());
	cbcEncrypt.ProcessData((byte*)cipher.data(), (byte*)data.data(), data.size());
	cipher = b64encode(cipher);
	return cipher;
}

string Utils::RSAEncrypt(string data, string N, string E)
{
	Integer modulus(N.c_str());
	Integer publicExponent(E.c_str());
	reverse(data.begin(), data.end());
	Integer plain(string("0x" + hexlify(data)).c_str());
	/*
	vector<int> buf;
	
	for (int i = 0; i <= data.length();)
	{
		buf.push_back((data[i++] << 0) + (data[i++] << 8));
	}
	for (int i = 0; i < buf.size(); i++)
	{
		Integer e(buf[i]);
		Integer tmp = Integer::One();
		for (int j = 1; j <= i; j++)
			tmp *= 0x10000;
		e *= tmp;
		n += e;
	}*/

	Integer output = a_exp_b_mod_c(plain, publicExponent, modulus);
	string output_str = IntToString(output, 16);
	if (output_str.length() < 256)
	{
		CQ_addLog(AuthCode, CQLOG_DEBUG, "´¥·¢²¹Áã", output_str.c_str());
		int len = 256 - output_str.length();
		for (int i = 0; i < len; i++)
			output_str.insert(0, "0");
	}
	output_str = toLowerCase(output_str);
	return output_str;
}

std::string Utils::hexlify(string data)
{
	HexEncoder encoder;
	string encoded;
	encoder.Put((byte*)data.data(), data.size());
	encoder.MessageEnd();
	
	int size = (int)encoder.MaxRetrievable();
	if (size)
	{
		encoded.resize(size);
		encoder.Get((byte*)encoded.data(), encoded.size());
	}
	encoded = toLowerCase(encoded);
	return encoded;
}

std::string Utils::toLowerCase(string str)
{
	for (int i = 1; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
	return str;
}

std::string Utils::randomStr(int length)
{
	const char SLOTS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	static default_random_engine generator(time(NULL));
	static uniform_int_distribution<int> distribution(0, strlen(SLOTS) - 1);
	string str("");

	for (int i = 0; i < length; i++)
		str += SLOTS[distribution(generator)];

	return str;
}

size_t Utils::copyDataCallback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t nsize = nmemb*size;
	string* strdata = (string*)stream;
	if (strdata)
	{
		strdata->append((const char*)ptr, nsize);
	}
	return nsize;
}

std::string Utils::UrlEncode(const string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+' || str[i] == '/' || str[i] == '=')
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
		else if (str[i] != '\n')
			strTemp += str[i];
	}
	return strTemp;
}

unsigned char Utils::ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

