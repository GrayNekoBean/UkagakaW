
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
	this->handle = hwnd;
	this->renderer = SPUkagakaRenderer(new UkagakaRenderer(
		SPD2DRenderer(new D2DRenderer(hwnd))));
	this->renderer->pDirect2DRenderer->UkagakaID = this->id;
}

void Ukagaka::SetConfig(string key, string value)
{
}

void Ukagaka::SetProperty(wstring prpty, wstring value)
{
}
