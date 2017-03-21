#include "Session.h"
#include "SearchRequest.h"
#include "SongInfoRequest.h"
#include "SearchResult.h"
#include "Utils.h"


regex Session::exp(".*(?:点歌|来首)\\s*(\\S.*?)\\s*$"); 

std::string Session::Message(string msg)
{
	string keyword;
	smatch match;
	if (!regex_search(msg, match, exp))
	{
		return "";
	}
	keyword = match[1];
	SearchRequest request(keyword);
	// return "";
	SearchResult result = request.perform();
	if (!result.isSuccess())
	{
		return "由于某种原因，查找失败了，请稍后再试。";
	}
	if (!result.getCount())
	{
		return "并没有找到指定的歌曲，请更换一个关键词再试试。";
	}
	Json::Value songInfo = result.getTopSong();
	SongInfoRequest infoRequest(songInfo["id"].asInt());
	Json::Value urlInfo = infoRequest.perform();
	if (urlInfo.get("data", Json::arrayValue)[0].get("code", 0).asInt() != 200)
	{
		return "由于版权曲或其他的某些原因，无法获取歌曲信息。";
	}
	string response = "[CQ:music,type=custom,url=";
	response += "http://music.163.com/#/song?id=" + Json::valueToString(songInfo["id"].asInt()) + ",audio=";
	response += urlInfo["data"][0]["url"].asString() + ",title=";
	response += Utils::convertUTF8toGBK(songInfo["name"].asString()) + ",content=";
	response += Utils::convertUTF8toGBK(songInfo["ar"][0]["name"].asString()) + ",image=";
	response += songInfo["al"]["picUrl"].asString();
	response += "]";
	return response.c_str();
}
