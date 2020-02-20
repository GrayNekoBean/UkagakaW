#pragma once

#include "UWRenderer.h"

class UWInteraction
{
private:
	vector<int> tops, bottoms;
	map<UINT, int> Order2IDTable;  //order based on heights

public:
	vector<RECT> rectangles;

	UWInteraction();
	~UWInteraction();

	HRESULT AddRectangle(RECT rectanle);

	int GetRectByPoint(POINT point);

	bool IsPointInRectangle(POINT point, RECT rect);
};

void OnWndRightDown(POINT pos, HWND hWnd);

void OnWndLeftClick(POINT pos, HWND hWnd);

void OnWndDoubleLeftClick(POINT pos, HWND hWnd);

void OnWndLeftDown(POINT pos, HWND hWnd);