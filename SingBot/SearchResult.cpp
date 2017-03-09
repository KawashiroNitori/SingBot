#include "SearchResult.h"


SearchResult::SearchResult(const Json::Value& dict)
{
	this->dict = dict;
	this->code = dict.get("code", 0).asInt();
	this->count = dict.get("result", Json::Value(Json::objectValue)).get("songCount", 0).asInt();
	this->songs = dict.get("result", Json::Value(Json::objectValue)).get("songs", Json::Value(Json::arrayValue));
}


SearchResult::SearchResult()
{
	code = 0;
}

SearchResult::~SearchResult()
{
}

bool SearchResult::isSuccess() const
{
	return this->code == 200;
}

int SearchResult::getTopSongID() const
{
	if (count)
		return this->songs[0]["id"].asInt();
	else
		return 0;
}
