#pragma once
#include "stdafx.h"
#include "json/json.h"

using namespace std;

class Request
{
protected:
	string body;
private:
	string aesSecondKey;
	string params;
	string encSecKey;
protected:
	virtual string constructBody() = 0;
	void encryptBody();
public:
	Request();
	~Request();
	virtual string getAPIURL() const = 0;
	Json::Value perform();
};

