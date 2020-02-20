
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

#include "UWAPI.h"

extern GeneralEvent MT_PostInitialize;

extern InteractEvent MT_OnUkagakaInteract;

extern RenderEvent MT_OnGeneralRender;
extern RenderEvent MT_OnAnimFinishPlay;

extern InteractEvent MT_OnUkagakaTouch;
extern InteractEvent MT_OnUkagakaInteract;
extern InteractEvent MT_OnElementClick;

extern map<string, SPUkagaka> UkagakaInstances_ID;
extern map<HWND, SPUkagaka> UkagakaInstances_HWND;

void PassPostInitializeEvent(GeneralEvent _event) {
	MT_PostInitialize = _event;
}

void PassGeneralRenderEvent(RenderEvent _event) {
	if (MT_OnGeneralRender == nullptr && _event != nullptr) {
		MT_OnGeneralRender = _event;
	}
	else {
		Error(L"There is already an OnGeneralRender Event exist, do not support process multi event currently");
	}
}

void PassAnimPlayFinishEvent(RenderEvent _event) {
	if (MT_OnAnimFinishPlay == nullptr && _event != nullptr) {
		MT_OnAnimFinishPlay = _event;
	}
	else {
		Error(L"There is already an OnAnimFinishPlay Event exist, do not support process multi event currently");
	}
}

void PassUkagakaTouchEvent(InteractEvent _event) {
	if (MT_OnUkagakaTouch == nullptr && _event != nullptr) {
		MT_OnUkagakaTouch = _event;
	}
	else {
		Error(L"There is already an OnUkagakaTouch Event exist, do not support process multi event currently");
	}
}

void PassElementClickEvent(InteractEvent _event) {
	if (MT_OnElementClick == nullptr && _event != nullptr) {
		MT_OnElementClick = _event;
	}
	else {
		Error(L"There is already an OnElementClick Event exist, do not support process multi event currently");
	}
}

void PassUkagakaInteractEvent(InteractEvent _event) {
	if (MT_OnUkagakaInteract == NULL) {
		MT_OnUkagakaInteract = _event;
		if (_event == NULL) {
			Error(L"Fuck");
		}
		if (MT_OnUkagakaInteract == NULL) {
			Error(L"WTF?");
		}
	}
	else {
		Error(L"There is already an OnUkagakaInteract Event exist, do not support process multi event currently");
	}
}

void PlayUkagakaAnimation
(LPCSTR ukagakaID, LPCSTR anim, int animState,
	float posX, float posY,
	float sizeX, float sizeY, float opaque) {

	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	HRESULT hr = ukagaka->renderer->PlayAnimationTB(anim, (UWAnimationState)animState, false);
	return;
}

void PlayUkagakaAnimationImmediately
(LPCSTR ukagakaID, LPCSTR anim, int animState,
	float posX, float posY,
	float sizeX, float sizeY, float opaque) {

	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	HRESULT hr = ukagaka->renderer->PlayAnimationTB(anim, (UWAnimationState)animState, true);

}

void OutputText(LPCSTR ukagakaID, BSTR words, int style, int colorCode)
{
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	HRESULT hr = ukagaka->renderer->OutputTextTB(wstring(words, SysStringLen(words)), style, colorCode);
}

void SetNewPhase(LPCSTR ukagakaID)
{
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	HRESULT hr = ukagaka->renderer->NewPhaseTB();
}

void WaitForTicks(LPCSTR ukagakaID, int ticks) 
{
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	ukagaka->renderer->WaitTicksTB(ticks);
}

void EndSection(LPCSTR ukagakaID) {
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	ukagaka->renderer->EndSectionTB();
}

void HideBalloon(LPCSTR ukagakaID) {
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	ukagaka->renderer->HideBalloon();
}

int CreateUserDefinedFont(
	LPCSTR ukagakaID,
	BSTR font, float size, bool bold, bool italic
) {
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	return ukagaka->renderer->pDirect2DRenderer->CreateFontStyle((LPCWSTR)font, size, bold, italic);
}

int CreateUserDefinedColor(
	LPCSTR ukagakaID,
	int r, int g, int b, int a
) {
	SPUkagaka ukagaka = GET_UKAGAKA(ukagakaID);
	return ukagaka->renderer->pDirect2DRenderer->CreateColorBrush(r, g, b, a);
}