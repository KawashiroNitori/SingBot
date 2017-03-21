#pragma once
#include "stdafx.h"
#include "Request.h"
#include "SearchResult.h"
#include "json/json.h"
#include "curl/curl.h"

using namespace std;


class SearchRequest : public Request
{
public:
	virtual string getAPIURL() const;
private:
	string keyword;
	int limit;
private:
	virtual string constructBody();
public:
	SearchRequest(string keyword, int limit = 30);
	~SearchRequest();
};

