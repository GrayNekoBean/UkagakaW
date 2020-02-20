
#pragma region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018 Bio-Electric Studio
* All Right Reserved
* This C++ source file is for the Kiwi Renderer, which is part of the Project Ukagaka_W.
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

#include "stdafx.h"
#include "Utils.h"

int CalculateColorCode(int r, int g, int b, int a) {
	if (r < 256 && g < 256 && b < 256 && a < 256) {
		int code = r * (0x10)*(0x10)*(0x10)
			+ g * (0x10)*(0x10)
			+ b * (0x10)
			+ a;
		return code;
	}

	return -1;
}

vector<LPCSTR> Split(LPCSTR str, LPCSTR spliter) {
	if (str != ""&&spliter != "") {
		vector<LPCSTR> results = vector<LPCSTR>();

		LPSTR buf = NULL;
		char* cp = new char[strlen(str) + 1];
		strcpy_s(cp, strlen(str) + 1, str);
		LPCSTR token = strtok_s(cp, spliter, &buf);
		while (token != NULL)
		{
			results.push_back(token);
			token = strtok_s(NULL, spliter, &buf);
		}

		return results;
	}
}

vector<LPCWSTR> Split(LPCWSTR str, LPCWSTR spliter) {
	if (str != L""&&spliter != L"") {
		vector<LPCWSTR> results = vector<LPCWSTR>();

		LPWSTR buf = NULL;
		wchar_t* cp = new wchar_t[wcslen(str) + 1];
		wcscpy_s(cp, wcslen(str) + 1, str);
		LPCWSTR token = wcstok_s(cp, spliter, &buf);
		while (token != NULL)
		{
			results.push_back(token);
			token = wcstok_s(NULL, spliter, &buf);
		}

		return results;
	}
}

string WString2String(const wstring &wstr)
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
	{
		return "";
	}
	return str;
}


wstring String2WString(const string& str)
{
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	::wstring w_str(wide);
	delete[] wide;
	return w_str;
}

vector<wstring> GetAllFileNamesWithExtName(wstring dirPath, wstring extName) {
	vector<wstring> filesPathes = vector<wstring>();

	FileHandle hFile = NULL;
	_wfinddata_t fileInfo;
	LPCWSTR spec = dirPath.append(L"\\*.").append(extName).c_str();

	hFile = _wfindfirst(spec, &fileInfo);

	if (hFile != -1) {
		int res = 0;
		while (res == 0) {
			filesPathes.push_back(fileInfo.name);
			res = _wfindnext(hFile, &fileInfo);
		}
		_findclose(hFile);
	}

	return filesPathes;
}

POINT GetMouseGlobalPos() {

	POINT globalMousePos;
	GetCursorPos(&globalMousePos);
	return globalMousePos;
}

void Timer::Start() {
	if (start == 0) {
		ms = 0;
		start = clock();
	}
}

void Timer::End() {
	ended = true;
	ms += clock() - start;
	start = 0;
}

void Timer::Restart() {
	ms = 0;
	start = clock();
}

void Timer::Stop() {
	stopping = true;
	ms += clock() - start;
	start = 0;
}

void Timer::Continue() {
	stopping = false;
	start = clock();
}

Milliseconds Timer::GetMilliseconds() {
	if (!stopping || !ended) {
		return 	ms + (clock() - start);
	}
	else {
		return ms;
	}
}

Seconds Timer::Seconds() {
	return GetMilliseconds() / 1000;
}

void Log(wstring info) {
	LPSTR fmt = new char[32];
	struct tm* local = new tm();
	time_t now;

	time(&now);
	localtime_s(local, &now);
	strftime(fmt, 32, "%T", local);

	wstring timeInfo = wstring(String2WString(fmt));
	OutputDebugString(timeInfo.append(wstring(L" [INFO]: ")).append(info).append(L"\n").c_str());
	delete fmt;
}

void Warning(wstring info) {
	LPSTR fmt = new char[32];
	struct tm* local = new tm();
	time_t now;

	time(&now);
	localtime_s(local, &now);
	strftime(fmt, 32, "%T", local);

	wstring timeInfo = wstring(String2WString(fmt));
	OutputDebugString(timeInfo.append(wstring(L" [WARNING]: ")).append(info).append(L"\n").c_str());
	delete fmt;
}

void Error(wstring info) {
	LPSTR fmt = new char[32];
	struct tm* local = new tm();
	time_t now;

	time(&now);
	localtime_s(local, &now);
	strftime(fmt, 32, "%T", local);

	wstring timeInfo = wstring(String2WString(fmt));
	OutputDebugString(timeInfo.append(wstring(L" [ERROR]: ")).append(info).append(L"\n").c_str());
	delete fmt;
}