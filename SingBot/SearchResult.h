#pragma once
#include "stdafx.h"
#include "json/json.h"
#include <vector>

using namespace std;

class SearchResult
{
private:
	Json::Value dict;
	Json::Value songs;
	int code;
	int count;

public:
	SearchResult(const Json::Value& dict);
	SearchResult();
	~SearchResult();
	bool isSuccess() const;
	Json::Value getSongs() const { return songs; }
	int getCount() const { return count; }
	int getCode() const { return code; }
	int getTopSongID() const;
	Json::Value getTopSong() const;
};

