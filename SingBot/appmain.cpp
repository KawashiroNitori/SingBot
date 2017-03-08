#include "stdafx.h"
#include "string"
#include "cqsdk/cqp.h"
#include "appmain.h" 

using namespace std;

int ac = -1; 
bool enabled = false;


CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
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
	string str = "[CQ:music,type=163,id=" + string(msg) + "]";
	CQ_sendPrivateMsg(ac, fromQQ, str.c_str());
	return EVENT_BLOCK;
}


CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	return EVENT_IGNORE; 
}


CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; 
}
