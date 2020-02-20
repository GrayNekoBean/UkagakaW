
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
#include "Ukagaka.h"

Ukagaka::Ukagaka(string id) {
	this->id = id;
}

Ukagaka::~Ukagaka()
{
}

void Ukagaka::Initiate(HWND hwnd) {

	if (hwnd == NULL) {
		Error(L"Window handle null! HWND may create failed.");
	}

	this->handle = hwnd;
	this->renderer = SPUkagakaRenderer(new UkagakaRenderer(
		SPD2DRenderer(new D2DRenderer(hwnd))));
	this->interaction = UWInteraction();
	this->renderer->pDirect2DRenderer->UkagakaID = this->id;
}

void Ukagaka::SetConfig(string key, string value)
{
}

void Ukagaka::SetProperty(wstring prpty, wstring value)
{
}

extern LPCSTR LastUkagakaTag;
extern SPUkagaka LastUkagakaPtr;

extern map<string, SPUkagaka> UkagakaInstances_ID;

SPUkagaka GetUkagakaByTag(LPCSTR id) {
	if (id == LastUkagakaTag) {
		return LastUkagakaPtr;
	}
	else {
		return UkagakaInstances_ID[string(id)];
	}
}