#include "Session.h"
#include "SearchRequest.h"
#include "SongInfoRequest.h"
#include "SearchResult.h"
#include "Utils.h"


regex Session::exp(".*(?:���|����)\\s*(\\S.*?)\\s*$"); 

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
		return "����ĳ��ԭ�򣬲���ʧ���ˣ����Ժ����ԡ�";
	}
	if (!result.getCount())
	{
		return "��û���ҵ�ָ���ĸ����������һ���ؼ��������ԡ�";
	}
	Json::Value songInfo = result.getTopSong();
	SongInfoRequest infoRequest(songInfo["id"].asInt());
	Json::Value urlInfo = infoRequest.perform();
	if (urlInfo.get("data", Json::arrayValue)[0].get("code", 0).asInt() != 200)
	{
		return "���ڰ�Ȩ����������ĳЩԭ���޷���ȡ������Ϣ��";
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
