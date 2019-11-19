
#pragma region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018 Bio-Electric Studio
* All Right Reserved
* This C++ source file is for the Kiwi Renderer, which is part of the Project Ukagaka_W.
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
#include "UWMain.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR cmdLine,
	int lineNum) {
	
	UNREFERENCED_PARAMETER(hPrevInstance);

	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr;

	AppInstance = hInstance;
	hr = CoInitialize(NULL);
	GdiInit();

	if (FAILED(hr))
		return 0;

	Initialize();

	MSG msg;
		
	while (!_END_PROGRAM_) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//threadLock.try_lock();
		InnerUpdate();
		//threadLock.unlock();
	}
	
	GdiEnd();
	CoUninitialize();

	return 1;
}

void Initialize() {

	StartCSharpDLLProgress();

	RegisterMouseEvent(OnWndLeftDown, MSE_LEFT_DOWN);
	RegisterMouseEvent(OnWndLeftHold, MSE_LEFT_HOLD);
	RegisterMouseEvent(OnWndRightDown, MSE_RIGHT_DOWN);
	
	auto threadLoop = [] (mutex &locker) {
		while (!_END_PROGRAM_) {
			//clock_t start, end;
			//start = clock();
			//InnerFixedUpdate();
			//end = clock();
			//UINT deltaTime = end - start;
			//UINT sleepTime = (MILLISECONDS_PER_SEC / FRAMES_PER_SEC) - deltaTime;
			//Sleep(sleepTime);
			//FixedDeltaTime = deltaTime;
			//locker.try_lock();
			
			for (pair<string, SPUkagaka> uki : UkagakaInstances_ID) {
				uki.second->renderer->pDirect2DRenderer->RenderOnScreen();
			}
			
			//----------------------Frame End-------------------------------

			//----------------------Frame Start-----------------------------
			Milliseconds deltaTime = 0;
			for (pair<string, SPUkagaka> uki : UkagakaInstances_ID) {
				Milliseconds dt;
				uki.second->renderer->pDirect2DRenderer->OnDrawFrame(dt);
				deltaTime += dt;
			}

			WCHAR chInput[256];
			wsprintf(chInput, L"delta Time: %d \n", deltaTime);
			Log(wstring(chInput));

			int sleeptime = 40 - deltaTime;
			if (sleeptime > 2) {
				Sleep(sleeptime);
			}
			else if (sleeptime < 0) {
				WCHAR chInput[256];
				wsprintf(chInput, L"WTF delta time too long! delta Time: %d \n", deltaTime);
				Warning(wstring(chInput));
			}
			//_CrtDumpMemoryLeaks();
			//locker.unlock();
		}
	};

	if (UkagakaInstances_ID["TEST"]->renderer != NULL) {
		UkagakaInstances_ID["TEST"]->renderer->PlayAnimation("Anim1", AnimationState::EndWithLastFrame);
		UkagakaInstances_ID["TEST"]->renderer->MainLogicUpdate();
	}

	LogicThread = std::thread(threadLoop, std::ref(threadLock));
	LogicThread.detach();
}

void StartCSharpDLLProgress() {
	CSHARP_SIMULATE_Main();
}

void CSHARP_SIMULATE_Main() {
	CSHARP_SIMULATE_LoadUkagakaFilesAndResources();
}

void CSHARP_SIMULATE_LoadUkagakaFilesAndResources() {
	FetchData("NewUkagakaID:TEST", (VOID*)&L"Name:TestUkagaka");
	FetchData("UkagakaConfig:TEST", (VOID*)&L"GivenName:Test-Chan");
	FetchData("RES:TEST_IDLE0", (VOID*)&L"PATH:.\\Resources\\Images\\1499589181009.jpeg");
	FetchData("FinishLoadResources:TEST", (VOID*)&L"NONE:NONE");
}

void OnWndLeftDown(POINT pos, HWND hWnd) {
	SPUkagaka ukw = UkagakaInstances_HWND[hWnd];
	if (ukw != NULL) {
		ukw->renderer->pDirect2DRenderer->OnLeftDown(pos);
	}
}

void OnWndLeftHold(POINT pos, HWND hWnd) {
	SPUkagaka ukw = UkagakaInstances_HWND[hWnd];
	if (ukw != NULL) {
		ukw->renderer->pDirect2DRenderer->OnLeftDrag_Global(pos);
	}
}

void FetchData(LPCSTR tag, void* data) {
	auto seps = Split(tag, ":");
	LPCSTR tagType = seps[0];
	LPCSTR tagContent = seps[1];
	if (tagType == NULL) {
		strcpy_s((LPSTR)tagContent,strlen(tagContent) + 1, tag);
		tagType = "RES";
	}
	if (static_cast<LPWSTR>(data) != NULL) {
		auto dseps = Split((LPWSTR)data, L":");
		LPCWSTR dataNameOrType = dseps[0];
		LPCWSTR dataContent = dseps[1];
		if (dataNameOrType == NULL) {
			wcscpy_s((LPWSTR)dataContent, wcslen((LPCWSTR)data) + 1, (LPCWSTR)data);
			dataNameOrType = L"Value";
		}

		if (strcmp(tagType, "NewUkagakaID") == 0) {
			LPSTR id = new char[strlen(tagContent) + 1];
			LPWSTR name = new wchar_t[wcslen(dataContent) + 1];
			strcpy_s(id, strlen(tagContent) + 1, tagContent);
			wcscpy_s(name, wcslen(dataContent) + 1, dataContent);

			SetUkagakaLoading(id, name);
		}
		else if (strcmp(tagType, "UkagakaConfig") == 0) {
			LoadingUkagaka.at(tagContent)->SetConfig((LPSTR)dataNameOrType, (LPSTR)dataContent);
		}
		else if (strcmp(tagType, "RES") == 0) {
			if (dataNameOrType == L"PATH") {
				//LoadD2DBitmap(dataContent, tagContent);
			}
		}
		else if (strcmp(tagType, "FinishLoadResources") == 0) {
			FinishLoading(tagContent);
		}
	}
}

void SetUkagakaLoading(LPCSTR Id, LPCWSTR Name) {
	UPUkagaka ukagakaIns = UPUkagaka(new Ukagaka(Id));
	ukagakaIns->name = Name;
	ukagakaIns->id = Id;
	CoCreateGuid(&ukagakaIns->UID);

	LoadingUkagaka[Id] = move(ukagakaIns);
}

void FinishLoading(LPCSTR Id) {
	UkagakaInstances_ID.insert(pair<string, SPUkagaka>(string(Id), SPUkagaka(LoadingUkagaka[Id].release())));
	LoadingUkagaka.at(Id) = NULL;

	CreateUkagakaInstance(UkagakaInstances_ID.at(Id)->name.c_str(), Id);
}

HWND CreateUkagakaInstance(LPCWSTR name, LPCSTR id) {
	USES_CONVERSION;
	LPCWSTR UkagakaID = A2W(id);
	if (!RegisterWNDClass(UkagakaID))
		return NULL;

	HWND hWindow = CreateWindowEx(WS_EX_LAYERED, UkagakaID, name, 
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP
		/*WS_CAPTION*/,
		0, 0, 300, 300, NULL, NULL, AppInstance, NULL);

	UkagakaInstances_ID.at(id)->Initiate(hWindow);
	UkagakaInstances_HWND[hWindow] = UkagakaInstances_ID[id];

	//the second parameter must be cmdline
	ShowWindow(hWindow, 10);
	UpdateWindow(hWindow);

	//SetLayeredWindowAttributes(hWindow, 0, 255, ULW_ALPHA);

	//DrawOnDC(hWindow);

	if (hWindow)
		return hWindow;
	else
		return false;
}

//// just for test render
//void DrawOnDC(HWND hwnd) {
//
//	int ix, iy;
//
//	HDC screenDC = GetDC(NULL);
//	HDC memDC;
//	memDC = CreateCompatibleDC(screenDC);
//
//	Gdiplus::Image* image = Gdiplus::Image::FromFile(L".\\Bitmap\\BM0_Test.png");
//	ix = 300;
//	iy = 300;
//
//	Gdiplus::Pen defPen(Gdiplus::Color::Violet);
//	Gdiplus::SolidBrush defBrush(Gdiplus::Color::Violet);
//
//	HBITMAP bitmap = CreateCompatibleBitmap(screenDC, ix, iy);
//
//	BYTE * pBits;
//	BITMAPINFOHEADER bmih;
//	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
//
//	bmih.biSize = sizeof(BITMAPINFOHEADER);
//	bmih.biWidth = ix;
//	bmih.biHeight = iy;
//	bmih.biPlanes = 1;
//	bmih.biBitCount = 32; //这里一定要是32 16位色没有alpha通道
//	bmih.biCompression = BI_RGB;
//	bmih.biSizeImage = 0;
//	bmih.biXPelsPerMeter = 0;
//	bmih.biYPelsPerMeter = 0;
//	bmih.biClrUsed = 0;
//	bmih.biClrImportant = 0;
//
//	HBITMAP hBitMap = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);
//
//	SelectObject(memDC, hBitMap);
//	Gdiplus::Graphics graph(memDC);
//	graph.DrawImage(image, 0, 0, ix, iy);
//	graph.DrawEllipse(&defPen, Gdiplus::Rect(10, 10, 100, 100));
//	graph.FillEllipse(&defBrush, Gdiplus::Rect(10, 10, 100, 100));
//	graph.DrawRectangle(&defPen, Gdiplus::Rect(10, 10, 100, 100));
//	graph.FillRectangle(&defBrush, Gdiplus::Rect(10, 10, 100, 100));
//
//	POINT	pPos = { 0, 0 };
//	POINT	pSrc = { 0, 0 };
//	SIZE	sizeWnd = { ix, iy };
//
//	BLENDFUNCTION blendFunc;
//	blendFunc.AlphaFormat = AC_SRC_ALPHA;
//	blendFunc.BlendFlags = 0;
//	blendFunc.BlendOp = AC_SRC_OVER;
//	blendFunc.SourceConstantAlpha = 255;
//	//SetLayeredWindowAttributes(hwnd, 0, 255, ULW_ALPHA);
//	bool re = UpdateLayeredWindow(hwnd, screenDC, &pPos, &sizeWnd, memDC, &pSrc, NULL, &blendFunc, ULW_ALPHA);
//	DWORD err = GetLastError();
//
//	//DeleteObject(bitmap);
//	DeleteObject(&defPen);
//	DeleteObject(&defBrush);
//	DeleteObject(image);
//	//DeleteObject(&graph);
//	DeleteDC(memDC);
//	ReleaseDC(hwnd, memDC);
//	ReleaseDC(NULL, screenDC);
//}

void GdiInit() {
	InitCommonControls();
	Gdiplus::GdiplusStartupInput input;
	GdiplusStartup(&token, &input, NULL);
}

void GdiEnd() {
	Gdiplus::GdiplusShutdown(token);
}

ATOM RegisterWNDClass(LPCWSTR id) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = AppInstance;
	wcex.lpszClassName = id;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);
	wcex.hIcon = LoadIcon(AppInstance, NULL);

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	if (LeftHolding) {
		//Log(L"holding");
		OnLeftHold(GetMouseGlobalPos(), hWnd);
	}

	switch (msg)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hWnd, &ps);
		//// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (LeftHolding) {
			OnLeftDragInWindow({ x, y }, hWnd);
		}
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		OnLeftDoubleClick({ x, y }, hWnd);
		break;
	}
	case WM_RBUTTONDBLCLK:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		OnRightDoubleClick({ x, y }, hWnd);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		OnLeftDown({ x, y }, hWnd);
		LeftHolding = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		OnLeftUp({ x , y }, hWnd);
		LeftHolding = false;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		OnRightDown({ x, y }, hWnd);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		OnRightUp({ x, y }, hWnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void InnerUpdate()
{
	for (auto uki : UkagakaInstances_HWND) {
		uki.second->renderer->MainLogicUpdate();
	}
}

void InnerFixedUpdate()
{
}

void OnWndRightDown(POINT pos, HWND hWnd) {
	int iii = 0;
	UkagakaInstances_HWND[hWnd]->renderer->PlayAnimationImmediately("inm", AnimationState::InfinityLoop);
}