
#pragma region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018 Bio-Electric Studio
* All Right Reserved
* This C++ header file is for the Kiwi Renderer, which is part of the Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* Author: Gray_Neko_Bean
*
* Overall Description:
* None
*
* Update Description:
* None
*
*
* ---------------------------------------------------
*/
#pragma endregion

#pragma once

#include <string>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <queue>
#include <time.h>
#include <io.h>

#include <comdef.h>

using namespace std;

typedef string tag;

typedef ULONG Milliseconds;
typedef FLOAT Seconds;

typedef vector<pair<float, string>> TIMELINE_FRM;
typedef vector<pair<Milliseconds, string>> TIMELINE_SEC;

typedef struct Timer {

private:
	Milliseconds ms = 0;
	Milliseconds start = 0;

	bool stopping = false;
	bool ended = false;;

public:

	Seconds Seconds();

	Milliseconds GetMilliseconds();

	void Start();

	void Stop();

	void Continue();

	void End();

	void Restart();
};

vector<LPCSTR> Split(LPCSTR str, LPCSTR spliter);

vector<LPCWSTR> Split(LPCWSTR str, LPCWSTR spliter);

string WString2String(const std::wstring &wstr);

wstring String2WString(const string& str);

vector<wstring> GetAllFileNamesWithExtName(wstring dirPath, wstring extName);

POINT GetMouseGlobalPos();

void Log(wstring info);

void Warning(wstring info);

void Error(wstring info);