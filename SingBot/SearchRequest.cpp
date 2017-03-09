#ifdef _DEBUG
#pragma comment(lib, "..\\curl\\lib\\Debug\\libcurl.lib")
#pragma comment(lib, "..\\curlpp\\Debug\\libcurlpp.lib")
#pragma comment(lib, "..\\Debug\\lib_json.lib")
#else
#pragma comment(lib, "..\\curl\\builds\\libcurl-vc-x86-release-static-ipv6-sspi-winssl\\lib\\libcurl_a.lib")
#pragma comment(lib, "..\\curlpp\\Release\\libcurlpp.lib")
#pragma comment(lib, "..\\Release\\lib_json.lib")
#endif // _DEBUG


#include <sstream>
#include "SearchRequest.h"
#include "Utils.h"
#include "cqsdk/cqp.h"


const string SearchRequest::SEARCH_API_URL = "http://music.163.com/weapi/cloudsearch/get/web?csrf_token=";

std::string SearchRequest::constructBody()
{
	Json::Value dict;
	dict["s"] = this->keyword;
	dict["type"] = "1";
	dict["offset"] = "0";
	dict["total"] = "true";
	dict["limit"] = Json::valueToString(this->limit);
	dict["csrf_token"] = "";
	Json::FastWriter writer;
	//return "{\"csrf_token\":\"\",\"limit\":\"30\",\"offset\":\"0\",\"s\":\"" + keyword + "\",\"total\":\"true\",\"type\":\"1\"}";
	string result(writer.write(dict));
	return result.substr(0, result.length() - 1); 
}

void SearchRequest::encryptBody()
{
	const string RSA_E = "0x010001";
	const string RSA_N = "0x00e0b509f6259df8642dbc35662901477df22677ec152b5ff68ace615bb7b725152b3ab17a876aea8a5aa76d2e417629ec4ee341f56135fccf695280104e0312ecbda92557c93870114af6c9d05c4f7f0c3685b7a46bee255932575cce10b424d813cfe4875d3e82047b97ddef52741d546b8e289dc6935b3ece0462db0a22b8e7";
	const string AES_FIRST_KEY = "0CoJUm6Qyw8W8jud";
	const string AES_IV = "0102030405060708";
	this->aesSecondKey = Utils::randomStr();
	this->params = Utils::AESEncrypt(Utils::AESEncrypt(body, AES_FIRST_KEY, AES_IV), aesSecondKey, AES_IV);
	this->encSecKey = Utils::RSAEncrypt(aesSecondKey, RSA_N, RSA_E);
	//CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "Plain", this->body.c_str());
	//CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "params", this->params.c_str());
	//CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "AESKey", this->aesSecondKey.c_str());
	//CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "encSecKey", this->encSecKey.c_str());
}

SearchRequest::SearchRequest(string keyword, int limit)
{
	//CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "Keyword", keyword.c_str());
	this->keyword = Utils::convertGBKtoUTF8(keyword);
	this->limit = limit;
	this->body = this->constructBody();
	this->encryptBody();
}


SearchRequest::~SearchRequest()
{
}

SearchResult SearchRequest::perform()
{
	string post = "params=" + Utils::UrlEncode(this->params) + "&encSecKey=" + this->encSecKey;
	//CQ_addLog(Utils::AuthCode, CQLOG_DEBUG, "post", post.c_str());
	string response_str;
	Json::Value response;
	Json::Reader reader;

	CURL* hnd = curl_easy_init();
	curl_easy_setopt(hnd, CURLOPT_POST, 1);
	curl_easy_setopt(hnd, CURLOPT_URL, SEARCH_API_URL.c_str());
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, "SingBot/0.0.1");
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, post.c_str());
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, Utils::copyDataCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response_str);
	curl_easy_setopt(hnd, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_perform(hnd);

	curl_easy_cleanup(hnd);
	if (!reader.parse(response_str, response))
	{
		CQ_addLog(Utils::AuthCode, CQLOG_WARNING, "´íÎó", response_str.c_str());
		SearchResult result;
		return result;
	}
	else
	{
		SearchResult result(response);
		return result;
	}
}
