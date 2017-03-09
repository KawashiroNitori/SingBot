#include "Session.h"
#include "SearchRequest.h"
#include "SearchResult.h"


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
	string response = "[CQ:music,type=163,id=" + Json::valueToString(result.getTopSongID()) + "]";
	return response;
}
