#ifdef _DEBUG
#pragma comment(lib, "..\\curl\\lib\\Debug\\libcurl.lib")
#pragma comment(lib, "..\\Debug\\lib_json.lib")
#else
#pragma comment(lib, "..\\curl\\builds\\libcurl-vc-x86-release-static-ipv6-sspi-winssl\\lib\\libcurl_a.lib")
//#pragma comment(lib, "..\\curl\\lib\\Release\\libcurl.lib")
#pragma comment(lib, "..\\Release\\lib_json.lib")
#endif // _DEBUG


#include <sstream>
#include "SearchRequest.h"
#include "Utils.h"
#include "cqsdk/cqp.h"


std::string SearchRequest::constructBody()
{
	Json::Value dict;
	dict["s"] = Utils::convertGBKtoUTF8(this->keyword);
	dict["type"] = "1";
	dict["offset"] = "0";
	dict["total"] = "true";
	dict["limit"] = Json::valueToString(this->limit);
	dict["csrf_token"] = "";
	Json::FastWriter writer;
	string result(writer.write(dict));
	return result; 
}


std::string SearchRequest::getAPIURL() const
{
	return "http://music.163.com/weapi/cloudsearch/get/web?csrf_token=";
}

SearchRequest::SearchRequest(string keyword, int limit)
{
	this->keyword = keyword;
	this->limit = limit;
}


SearchRequest::~SearchRequest()
{
}
