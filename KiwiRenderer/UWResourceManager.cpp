
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
#include "UWResourceManager.h"

HRESULT InstantiateDirect2DRenderer() {
	HRESULT hr = S_OK;

	return hr;
}

Animation::Animation() {}

Animation::Animation(int singleBitmap) {
	FramePerSecond = 25;

	frames = new AnimFrame[1];

	for (int i = 0; i < 8; i++) {
		frames[0][i] = 0;
	}

	this->frames[0][0] = singleBitmap;

	realFrameCount = 1;
}

Animation::Animation(vector<int> animFrames, unsigned int FPS) {
	FramePerSecond = FPS;
	frames = new AnimFrame[animFrames.size()];

	for (int i = 0; i < animFrames.size(); i++) {
		for (int j = 0; j < 8; j++) {
			frames[i][j] = 0;
		}
	}

	if (FPS == 1000 / currentRenderFrameRate) {

		for (int i = 0; i < animFrames.size(); i++) {
			frames[i][0] = animFrames[i];
		}

		realFrameCount = animFrames.size();
	}
}

Animation::Animation(map<string, int> bitmapMapping, vector<string> frames_str, unsigned int FPS) {
	FramePerSecond = FPS;
	frames = new AnimFrame[frames_str.size()];

	for (int i = 0; i < frames_str.size(); i++) {
		for (int j = 0; j < 8; j++) {
			frames[i][j] = 0;
		}
	}

	if (FPS == 1000 / currentRenderFrameRate) {

		int i = 0;
		for (string s : frames_str) {
			frames[i][0] = bitmapMapping[s];
			i++;
		}

		realFrameCount = i + 1;
	}
}

Animation::Animation(map<string, int> bitmapMapping, TIMELINE_FRM timeline_byFrame, unsigned int FPS) {
	FramePerSecond = FPS;

	int sumFrames = 0;
	for (pair<int, string> pr : timeline_byFrame) {
		sumFrames += pr.first;
	}
	frames = new AnimFrame[sumFrames];

	for (int i = 0; i < sumFrames; i++) {
		for (int j = 0; j < 8; j++) {
			frames[i][j] = 0;
		}
	}

	if (FPS == 1000/currentRenderFrameRate) {
		int i = 0;
		for (pair<int, string> pr : timeline_byFrame) {
			for (int j = 0; j < pr.first; j++) {
				frames[i + j][0] = bitmapMapping[pr.second];
			}
			i += pr.first;
		}

		realFrameCount = i + 1;
	}
}

Animation::Animation(map<string, int> bitmapMapping, TIMELINE_SEC timeline_bySecond, unsigned int FPS) {
	FramePerSecond = FPS;

	int sumFrames = 0;
	for (pair<float, string> pr : timeline_bySecond) {
		sumFrames += (int)std::ceil(pr.first / (1.0 * FPS));
	}
	frames = new AnimFrame[sumFrames];

	for (int i = 0; i < sumFrames; i++) {
		for (int j = 0; j < 8; j++) {
			frames[i][j] = 0;
		}
	}

	int i = 0;
	for (pair<int, string> pr : timeline_bySecond) {
		double a = (1.0 / FPS);
		double b = (pr.first / ((double)(1.0 * (int)FPS)));
		int f = (int)std::ceil((float)(pr.first / ((float)FPS * 1.0f)));
		for (int j = 0; j < f; j++) {
			frames[i + j][0] = bitmapMapping[pr.second];
		}
		i += f;
	}

	realFrameCount = i - 1;
}

HRESULT Animation::FetchToQueue(queue<AnimFrame>& animQueue) {
	if (frames != NULL) {
		if (sizeof(frames) > 0) {
			for (int i = 0; i < realFrameCount; i++) {
				animQueue.push(frames[i]);
			}
			return S_OK;
		}
	}
	return E_FAIL;
}