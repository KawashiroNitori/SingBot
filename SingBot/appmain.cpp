#include "stdafx.h"
#include "string"
#include "cqsdk/cqp.h"
#include "Utils.h"
#include "appmain.h" 
#include "Session.h"

using namespace std;
bool enabled = false;


CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	Utils::AuthCode = AuthCode;
	return 0;
}


CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {
	if (!enabled)
		return EVENT_IGNORE;
	string response = Session::Message(msg);
	if (!response.empty())
	{
		CQ_sendPrivateMsg(Utils::AuthCode, fromQQ, response.c_str());
		return EVENT_BLOCK;
	}
	return EVENT_IGNORE;
}


CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	if (!enabled)
		return EVENT_IGNORE;
	string response = Session::Message(msg);
	if (!response.empty())
	{
		CQ_sendGroupMsg(Utils::AuthCode, fromGroup, response.c_str());
		return EVENT_BLOCK;
	}
	return EVENT_IGNORE; 
}


CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {
	if (!enabled)
		return EVENT_IGNORE;
	string response = Session::Message(msg);
	if (!response.empty())
	{
		CQ_sendDiscussMsg(Utils::AuthCode, fromDiscuss, response.c_str());
		return EVENT_BLOCK;
	}
	return EVENT_IGNORE; 
}
