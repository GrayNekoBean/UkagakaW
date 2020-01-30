
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

extern "C" _declspec(dllexport) void SpeakSentence(
	LPCSTR ukagakaID, BSTR words
);

extern "C" _declspec(dllexport) void ContinueSpeak(
	LPCSTR ukagakaID, LPCWSTR words
);

extern "C" _declspec(dllexport) void StartNewPhase(
	LPCSTR ukagakaID, BSTR words
);

extern "C" _declspec(dllexport) void PauseSpeak(
	LPCSTR ukagakaID, int milliseconds
);

extern "C" _declspec(dllexport) void EndSpeak(
	LPCSTR ukagakaID
);

extern "C" _declspec(dllexport) void InterruptWithNewPhase(
	LPCSTR ukagakaID, LPCWSTR words
);

extern "C" _declspec(dllexport) void HideBalloon(
	LPCSTR ukagakaID
);

extern "C" _declspec(dllexport) void WaitForTick(
	LPCSTR ukagakaId, int ticks
);