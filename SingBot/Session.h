#pragma once
#include "stdafx.h"
#include <regex>

using namespace std;

class Session
{
private:
	static regex exp;
public:
	static string Message(string msg);
};

