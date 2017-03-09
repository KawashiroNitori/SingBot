#include "Session.h"
#include "SearchRequest.h"
#include "SearchResult.h"


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
	string response = "[CQ:music,type=163,id=" + Json::valueToString(result.getTopSongID()) + "]";
	return response;
}
