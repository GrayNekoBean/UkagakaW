
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

#include "UWRenderer.h"

typedef class Ukagaka {

public:
	string id;
	wstring name;
	wstring givenName;

	HWND handle;
	UUID UID;

	SPUkagakaRenderer renderer;

	UINT 
		height,
		width;

	Ukagaka(string id);

	Ukagaka(string id, HWND hWnd);

	~Ukagaka();

	void Initiate(HWND hWnd);

	void SetConfig(string key, string value);

	void SetProperty(wstring prpty, wstring value);

private:
	map<string, string> configs;
	map<wstring, wstring> properties;

}
UkagakaInstance,
*PUkagaka,
*LPUkagaka
;
typedef unique_ptr<Ukagaka> UPUkagaka;
typedef shared_ptr<Ukagaka> SPUkagaka;