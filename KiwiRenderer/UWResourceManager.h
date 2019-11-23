
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

#include "Utils.h"

#include <gdiplus.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>

#if WINVER >= 0x0601
#include <d2d1.h>
#include <d2d1helper.h>
#endif
#if WINVER >= 0x0602
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#endif
#if WINVER >= 0x0603
#include <d2d1_2.h>
#include <d2d1_2helper.h>
#endif
#if WINVER == 0x0A00
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#endif   //Get Windows Version

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "D2D1.lib")

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "DWrite.lib")

using namespace std;
using namespace D2D1;

typedef ID2D1Bitmap D2DBitmap, *PBitmap, *LPBitmap;
typedef CComPtr<D2DBitmap> CPBitmap;
typedef ID2D1Brush Brush, *PBrush, *LPBrush;
typedef CComPtr<Brush> CPBrush;
typedef ID2D1SolidColorBrush SolidBrush, *PSolidBrush, *LPSolidBrush;
typedef CComPtr<SolidBrush> CPSolidBrush;
typedef ID2D1Effect RenderEffect, *PRenderEffect, *LPRenderEffect;
typedef CComPtr<RenderEffect> CPRenderEffect;

typedef ID2D1DCRenderTarget DCRenderTarget, *PDCRenderTarget, *LPDCRenderTarget;
typedef ID2D1GdiInteropRenderTarget GdiItrpRenderTarget, *PGdiItrpRenderTarget, *LPGdiItrpRenderTarget;
typedef ID2D1HwndRenderTarget HWNDRenderTarget, *PHWNDRenderTarget, *LPHWNDRenderTarget;
typedef ID2D1RenderTarget RenderTarget, *PRenderTarget, *LPRenderTarget;
typedef ID2D1Factory D2DFactory, *PD2DFactory, *LPD2DFactory;
typedef IWICImagingFactory WICImageFactory, *PWICImageFactory, *LPWICImageFactory;
typedef IDWriteTextFormat DWriteTextFormat, *PDWriteTextFormat, *LPDWdriteTextFormat;
typedef IDWriteFactory DWriteFactory, *PDWriteFactory, *LPDWriteFactory;
typedef ID2D1DrawingStateBlock DrawingStateBlock, *PDrawingStateBlock;

typedef CComPtr<ID2D1DCRenderTarget> CPDCRenderTarget;
typedef CComPtr<ID2D1HwndRenderTarget> CPHWNDRenderTarget;
typedef CComPtr<ID2D1GdiInteropRenderTarget> CPGdiItrpRenderTarget;
typedef CComPtr<ID2D1RenderTarget> CPRenderTarget;
typedef CComPtr<ID2D1Factory> CPD2DFactory;
typedef CComPtr<IWICImagingFactory> CPWICImageFactory;
typedef CComPtr<IDWriteTextFormat> CPDWriteTextFormat;
typedef CComPtr<IDWriteFactory> CPDWriteFactory;
typedef CComPtr<ID2D1Layer> CPLayer;

typedef D2D1_RECT_F RectangleF;
typedef D2D1_RECT_L RectangleL;
typedef D2D1_RECT_U RectangleU;
typedef D2D1_SIZE_F SizeF;
typedef D2D1_SIZE_U SizeU;
typedef D2D1_POINT_2F PointF;
typedef D2D1_POINT_2L PointL;
typedef D2D1_POINT_2U PointU;

typedef array<int, 8> AnimFrame;

//Max time for an Animation is 40 seconds
typedef struct Animation {

	int currentRenderFrameRate = 40;

	UINT FramePerSecond = 25;
	AnimFrame (*frames);
	int realFrameCount = 0;

	Animation();

	Animation(int singleBitmap);

	Animation(vector<int> animFrames, unsigned int FPS = 25);

	Animation(map<string, int> bitmapMapping, 
		vector<string> frames_str, unsigned int FPS = 25);

	Animation(map<string, int> bitmapMapping, 
		TIMELINE_FRM timeline_byFrame, unsigned int FPS = 25);

	Animation(map<string, int> bitmapMapping, 
		TIMELINE_SEC timeline_bySecond, unsigned int FPS = 25);

	//~Animation() {
	//	delete frames;
	//}
	HRESULT FetchToQueue(queue<AnimFrame> &animQueue);

} Anim, *PAnimation, *PAnim;

typedef shared_ptr<Animation> SPAnimation;
typedef unique_ptr<Animation> UPAnimation;
