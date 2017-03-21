#pragma once
#include "Request.h"
class SongInfoRequest :
	public Request
{
public:
	virtual string getAPIURL() const;
private:
	int songID;
private:
	virtual string constructBody();
public:
	SongInfoRequest(int songID);
	~SongInfoRequest();
};

