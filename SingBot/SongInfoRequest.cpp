#include "SongInfoRequest.h"


std::string SongInfoRequest::getAPIURL() const
{
	return "http://music.163.com/weapi/song/enhance/player/url?csrf_token=";
}

std::string SongInfoRequest::constructBody()
{
	Json::Value dict;
	Json::FastWriter writer;
	string id_str = Json::valueToString(songID);
	dict["ids"] = string("[") + id_str + "]";
	dict["csrf_token"] = "";
	dict["br"] = 128000;
	//dict["ids"] = id_str;
	return writer.write(dict);
}

SongInfoRequest::SongInfoRequest(int songID)
{
	this->songID = songID;
}


SongInfoRequest::~SongInfoRequest()
{
}
