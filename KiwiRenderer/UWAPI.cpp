
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

#include "UWAPI.h"

extern InteractEvent MT_OnUkagakaInteract;

extern RenderEvent MT_OnGeneralRender;
extern RenderEvent MT_OnAnimFinishPlay;

extern InteractEvent MT_OnUkagakaTouch;
extern InteractEvent MT_OnUkagakaInteract;
extern InteractEvent MT_OnElementClick;

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

	SPUkagaka ukagaka = UkagakaInstances_ID[ukagakaID];
	HRESULT hr = ukagaka->renderer->PlayAnimation(anim, (AnimationState)animState);

}

void PlayUkagakaAnimationImmediately
(LPCSTR ukagakaID, LPCSTR anim, int animState,
	float posX, float posY,
	float sizeX, float sizeY, float opaque) {

	SPUkagaka ukagaka = UkagakaInstances_ID[ukagakaID];
	HRESULT hr = ukagaka->renderer->PlayAnimationImmediately(anim, (AnimationState)animState);

}