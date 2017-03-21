#ifdef _DEBUG
#pragma comment(lib, "..\\curl\\lib\\Debug\\libcurl.lib")
#pragma comment(lib, "..\\Debug\\lib_json.lib")
#else
#pragma comment(lib, "..\\curl\\builds\\libcurl-vc-x86-release-static-ipv6-sspi-winssl\\lib\\libcurl_a.lib")
//#pragma comment(lib, "..\\curl\\lib\\Release\\libcurl.lib")
#pragma comment(lib, "..\\Release\\lib_json.lib")
#endif // _DEBUG

#include "Request.h"
#include "Utils.h"
#include "cqsdk/cqp.h"
#include "curl/curl.h"


Request::Request()
{
}


Request::~Request()
{
}


void Request::encryptBody()
{
	const string RSA_E = "0x010001";
	const string RSA_N = "0x00e0b509f6259df8642dbc35662901477df22677ec152b5ff68ace615bb7b725152b3ab17a876aea8a5aa76d2e417629ec4ee341f56135fccf695280104e0312ecbda92557c93870114af6c9d05c4f7f0c3685b7a46bee255932575cce10b424d813cfe4875d3e82047b97ddef52741d546b8e289dc6935b3ece0462db0a22b8e7";
	const string AES_FIRST_KEY = "0CoJUm6Qyw8W8jud";
	const string AES_IV = "0102030405060708";
	this->aesSecondKey = Utils::randomStr();
	this->params = Utils::AESEncrypt(Utils::AESEncrypt(body, AES_FIRST_KEY, AES_IV), aesSecondKey, AES_IV);
	this->encSecKey = Utils::RSAEncrypt(aesSecondKey, RSA_N, RSA_E);
}


Json::Value Request::perform()
{
	this->body = this->constructBody();
	this->encryptBody();
	string post = "params=" + Utils::UrlEncode(this->params) + "&encSecKey=" + this->encSecKey;
	string response_str;
	Json::Value response;
	Json::Reader reader;

	CURL* hnd = curl_easy_init();
	curl_easy_setopt(hnd, CURLOPT_POST, 1);
	curl_easy_setopt(hnd, CURLOPT_URL, getAPIURL().c_str());
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, "SingBot/0.0.1");
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, post.c_str());
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, Utils::copyDataCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response_str);
	curl_easy_setopt(hnd, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_perform(hnd);

	curl_easy_cleanup(hnd);
	if (!reader.parse(response_str, response))
	{
		CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "response", response_str.c_str());
		CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "Plain", this->body.c_str());
		CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "params", this->params.c_str());
		CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "AESKey", this->aesSecondKey.c_str());
		CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "encSecKey", this->encSecKey.c_str());
		CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "post", post.c_str());
	}
	return response;
}
