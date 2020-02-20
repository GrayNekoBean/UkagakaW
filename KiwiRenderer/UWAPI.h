
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

#include "Ukagaka.h"

#define EXPORT_PASS_RENDER_EVENT_API(name) extern "C" _declspec(dllexport) void name(RenderEvent _event);
#define EXPORT_PASS_GENERAL_EVENT_API(name) extern "C" _declspec(dllexport) void name(GeneralEvent _event);
#define EXPORT_UKAGAKA_FUNCTION_API(name, ... ) extern "C" _declspec(dllexport) void name(LPCSTR ukagakaID, __VA_ARGS__);

EXPORT_PASS_GENERAL_EVENT_API(PassPostInitializeEvent)

extern "C" _declspec(dllexport) void PassGeneralRenderEvent(RenderEvent _event);

extern "C" _declspec(dllexport) void PassAnimPlayFinishEvent(RenderEvent _event);

extern "C" _declspec(dllexport) void PassSpeakFinishEvent(RenderEvent _event);

extern "C" _declspec(dllexport) void PassWaitFinishEvent(RenderEvent _event);

extern "C" _declspec(dllexport) void PassUkagakaTouchEvent(InteractEvent _event);

extern "C" _declspec(dllexport) void PassElementClickEvent(InteractEvent _event);

extern "C" _declspec(dllexport) void PassUkagakaInteractEvent(InteractEvent _event);

extern "C" _declspec(dllexport) void PlayUkagakaAnimation
(LPCSTR ukagakaID, LPCSTR anim, int animState = 0,
	float posX = 0, float posY = 0,
	float sizeX = 0, float sizeY = 0, float opaque = 1.0f);

extern "C" _declspec(dllexport) void PlayUkagakaAnimationImmediately
(LPCSTR ukagakaID, LPCSTR anim, int animState = 0,
	float posX = 0, float posY = 0,
	float sizeX = 0, float sizeY = 0, float opaque = 1.0f);

extern "C" _declspec(dllexport) void OutputText(
	LPCSTR ukagakaID, BSTR words, int style = (int)UWTextStyle::paragraph, int colorCode = (int)UWTextColor::Black
);

extern "C" _declspec(dllexport) void SetNewPhase(
	LPCSTR ukagakaID
);

extern "C" _declspec(dllexport) void WaitForTicks(
	LPCSTR ukagakaId, int ticks
);

extern "C" _declspec(dllexport) void TextOutputPause(
	LPCSTR ukagakaID, int milliseconds
);

extern "C" _declspec(dllexport) void EndSection(
	LPCSTR ukagakaID
);

extern "C" _declspec(dllexport) void HideBalloon(
	LPCSTR ukagakaID
);

extern "C" _declspec(dllexport) int CreateUserDefinedFont(
	LPCSTR ukagakaID, BSTR font, 
	float size, bool bold, bool italic
);

extern "C" _declspec(dllexport) int CreateUserDefinedColor(
	LPCSTR ukagakaID,
	int r, int g, int b, int a
);