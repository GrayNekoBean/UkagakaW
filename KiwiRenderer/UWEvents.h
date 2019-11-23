
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

#define MSE_LEFT_CLICK 0
#define MSE_LEFT_DOWN 1
#define MSE_LEFT_UP 2
#define MSE_LEFT_HOLD 3
#define MSE_LEFT_DRAG_IN_GLOBAL 4
#define MSE_LEFT_DRAG_IN_WINDOW 5
#define MSE_RIGHT_CLICK 6
#define MSE_RIGHT_DOWN 7
#define MSE_RIGHT_UP 8
#define MSE_LEFT_DOUBLE_CLICK 9
#define MSE_RIGHT_DOUBLE_CLICK 10


typedef void (*MouseEvent)(POINT, HWND);

static vector<MouseEvent> mLeftDoubleClickEvents = vector<MouseEvent>();
static vector<MouseEvent> mLeftClickEvents = vector<MouseEvent>();
static vector<MouseEvent> mLeftDownEvents = vector<MouseEvent>();
static vector<MouseEvent> mLeftUpEvents = vector<MouseEvent>();
static vector<MouseEvent> mLeftHoldEvents = vector<MouseEvent>();
static vector<MouseEvent> mLeftDragGlobalEvents = vector<MouseEvent>();
static vector<MouseEvent> mLeftDragInWindowEvents = vector<MouseEvent>();
static vector<MouseEvent> mRightDoubleClickEvents = vector<MouseEvent>();
static vector<MouseEvent> mRightClickEvents = vector<MouseEvent>();
static vector<MouseEvent> mRightDownEvents = vector<MouseEvent>();
static vector<MouseEvent> mRightUpEvents = vector<MouseEvent>();

const int MaxClickInterval = 200;

Milliseconds LeftHoldingTime;

bool LeftHolding;
bool IsMouseMoved;

void RegisterMouseEvent(MouseEvent evnt, INT MOUSE_EVENT_TYPE) {
	if (evnt != NULL) {
		switch (MOUSE_EVENT_TYPE) {
		case MSE_LEFT_DOUBLE_CLICK:
			mLeftDoubleClickEvents.push_back(evnt);
			break;
		case MSE_LEFT_CLICK:
			mLeftClickEvents.push_back(evnt);
			break;
		case MSE_LEFT_DOWN:
			mLeftDownEvents.push_back(evnt);
			break;
		case MSE_LEFT_HOLD:
			mLeftHoldEvents.push_back(evnt);
			break;
		case MSE_LEFT_UP:
			mLeftUpEvents.push_back(evnt);
			break;
		case MSE_LEFT_DRAG_IN_GLOBAL:
			mLeftDragGlobalEvents.push_back(evnt);
			break;
		case MSE_LEFT_DRAG_IN_WINDOW:
			mLeftDragInWindowEvents.push_back(evnt);
			break;
		case MSE_RIGHT_CLICK:
			mRightClickEvents.push_back(evnt);
		case MSE_RIGHT_DOUBLE_CLICK:
			mRightDoubleClickEvents.push_back(evnt);
			break;
		case MSE_RIGHT_DOWN:
			mRightDownEvents.push_back(evnt);
			break;
		case MSE_RIGHT_UP:
			mRightUpEvents.push_back(evnt);
			break;
		}
	}
}

void OnLeftClick(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mLeftClickEvents) {
		e(LocalPos, hWnd);
	}
}

void OnLeftDoubleClick(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mLeftDoubleClickEvents) {
		e(LocalPos, hWnd);
	}
}

void OnLeftDown(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mLeftDownEvents) {
		e(LocalPos, hWnd);
	}
}

void OnLeftUp(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mLeftUpEvents) {
		e(LocalPos, hWnd);
	}
}

void OnLeftHold(POINT GlobalPos, HWND hWnd) {
	for (MouseEvent e : mLeftHoldEvents) {
		e(GlobalPos, hWnd);
	}
}

void OnLeftDragInGlobal(POINT GlobalPos, HWND hWnd) {
	for (MouseEvent e : mLeftDragGlobalEvents) {
		e(GlobalPos, hWnd);
	}
}

void OnLeftDragInWindow(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mLeftDragInWindowEvents) {
		e(LocalPos, hWnd);
	}
}

void OnRightClick(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mRightClickEvents) {
		e(LocalPos, hWnd);
	}
}

void OnRightDoubleClick(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mRightDoubleClickEvents) {
		e(LocalPos, hWnd);
	}
}

void OnRightDown(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mRightDownEvents) {
		e(LocalPos, hWnd);
	}
}

void OnRightUp(POINT LocalPos, HWND hWnd) {
	for (MouseEvent e : mRightUpEvents) {
		e(LocalPos, hWnd);
	}
}

