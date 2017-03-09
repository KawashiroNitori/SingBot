#pragma once
#include "stdafx.h"
#include "SearchResult.h"
#include "json/json.h"
#include "curl/curl.h"

using namespace std;


class SearchRequest
{
private:
	const static string SEARCH_API_URL;
private:
	string keyword;
	string aesSecondKey;
	string body;
	string params;
	string encSecKey;
	int limit;
private:
	string constructBody();
	void encryptBody();
public:
	SearchRequest(string keyword, int limit = 30);
	~SearchRequest();
	SearchResult perform();
};

