
#pragma region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018 Bio-Electric Studio
* All Right Reserved
* This C++ header file is for the Kiwi Renderer, which is part of the Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* Author: Biobean Derek
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

#include "UWAPI.h"
#include "UWEvents.h"

//#include <thread>
//#include <mutex>

HINSTANCE AppInstance;

BOOL _END_PROGRAM_ = FALSE;

BOOL LOAD_FINISH = FALSE;

UINT DeltaTime;
UINT FixedDeltaTime;

//RT: Render Thread(fixed frame)
//MT: Main Logic Thread(dynamic loop)

RenderEvent MT_OnGeneralRender = NULL;
RenderEvent MT_OnAnimFinishPlay = NULL;

InteractEvent MT_OnUkagakaTouch = NULL;
InteractEvent MT_OnUkagakaInteract = NULL;
InteractEvent MT_OnElementClick = NULL;

//thread LogicThread;
//mutex threadLock;

map<string, UPUkagaka> LoadingUkagaka = map<string, UPUkagaka>();

ULONG_PTR token;

typedef void (*InitFunc)();

//int WINAPI wWinMain(
//	HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPWSTR cmdLine,
//	int lineNum);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
);

void StartCSharpDLLProgress();

void CSHARP_SIMULATE_Main();

void CSHARP_SIMULATE_LoadUkagakaFilesAndResources();

extern "C" __declspec(dllexport) void InitializeMainRenderThread();

extern "C" __declspec(dllexport) void InitializeLogicThread();

void OnWndLeftDown(POINT pos, HWND hWnd);

void OnWndLeftDrag(POINT pos, HWND hWnd);

void OnWndRightDown(POINT pos, HWND hWnd);

void OnWndLeftClick(POINT pos, HWND hWnd);

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
