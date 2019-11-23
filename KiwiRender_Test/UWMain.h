
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

#include "stdafx.h"

#include <thread>
#include <mutex>

#include "Ukagaka.h"
#include "UWEvents.h"

HINSTANCE AppInstance;

BOOL _END_PROGRAM_ = FALSE;

UINT DeltaTime;
UINT FixedDeltaTime;

thread LogicThread;
mutex threadLock;

map<string, UPUkagaka> LoadingUkagaka = map<string, UPUkagaka>();

map<string, SPUkagaka> UkagakaInstances_ID = map<string, SPUkagaka>();
map<HWND, SPUkagaka> UkagakaInstances_HWND = map<HWND, SPUkagaka>();;

ULONG_PTR token;


//int WINAPI wWinMain(
//	HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPWSTR cmdLine,
//	int lineNum);

void StartCSharpDLLProgress();

void CSHARP_SIMULATE_Main();

void CSHARP_SIMULATE_LoadUkagakaFilesAndResources();

void OnWndLeftDown(POINT pos, HWND hWnd);

void OnWndLeftHold(POINT pos, HWND hWnd);

void OnWndRightDown(POINT pos, HWND hWnd);

void Initialize();

void SetUkagakaLoading(LPCSTR ID, LPCWSTR Name);

void FinishLoading(LPCSTR ID);

HWND CreateUkagakaInstance(LPCWSTR name, LPCSTR id);

void FetchData(LPCSTR tag, void* resource);

WORD RegisterWNDClass(LPCWSTR id);

LRESULT CALLBACK WndProc(
	HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam
);

void InnerUpdate();

void InnerFixedUpdate();

//just for test render
void DrawOnDC(HWND hwnd);

void GdiInit();

void GdiEnd();
