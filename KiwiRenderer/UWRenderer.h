
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

#include "UWResourceManager.h"

struct UWRenderElement {

	RectangleF rect;
	float scale;
	float opaque;

	UWRenderElement() {}

	UWRenderElement(RectangleF rectangle, float alpha) {
		rect = rectangle;
		opaque = alpha;

		scale = 0;
	}

	UWRenderElement(D2D1_POINT_2F pos, float scale_, float alpha) {
		scale = scale_;
		opaque = alpha;

		rect = { pos.x, pos.y, pos.x, pos.y };
	}

	virtual HRESULT Render(CPDCRenderTarget &DCRT) = 0;
};
typedef UWRenderElement RenderElement;
typedef UWRenderElement *PRenderElement, *LPRenderElement;
typedef unique_ptr<UWRenderElement> UPRenderElement;
typedef shared_ptr<UWRenderElement> SPRenderElement;

struct UWRenderElement_Bitmap : public UWRenderElement {

public:

	CPBitmap bitmap;

	UWRenderElement_Bitmap(RectangleF rectangle, float alpha, CPBitmap bm) :UWRenderElement(rectangle, alpha) {
		bitmap = bm;
	}

	UWRenderElement_Bitmap(D2D1_POINT_2F pos, float scale_, float alpha, CPBitmap bm) {
		bitmap = bm;
		scale = scale_;
		opaque = alpha;

		float cx, cy;
		D2D1_SIZE_F orgSize= bm->GetSize();
		cx = orgSize.width * scale;
		cy = orgSize.height * scale;

		rect = { pos.x, pos.y, pos.x + cx, pos.y + cy };
	}

	HRESULT Render(CPDCRenderTarget &DCRT) override;
};
typedef UWRenderElement_Bitmap RenderElement_Bitmap;
typedef UWRenderElement_Bitmap *PRenderElement_Bitmap, *LPRenderElement_Bitmap;
typedef unique_ptr<UWRenderElement_Bitmap> UPRenderElement_Bitmap;
typedef shared_ptr<UWRenderElement_Bitmap> SPRenderElement_Bitmap;

struct UWRenderElement_Text : public UWRenderElement {

public:
	wstring text;
	CPDWriteTextFormat textFormat;
	CPBrush textBrush;

	UWRenderElement_Text(RectangleF rectangle, float alpha, wstring txt) : UWRenderElement(rectangle, alpha) {
		this->text = txt;
	}

	UWRenderElement_Text(D2D1_POINT_2F pos, D2D1_POINT_2F size, float alpha, wstring txt, CPDWriteTextFormat format, CPBrush brush) {
		this->text = txt;
		this->textFormat = format;
		this->textBrush = brush;
		
		this->opaque = alpha;

		this->rect = {
			pos.x, pos.y,
			pos.x + size.x, pos.y + size.y
		};

	}

	HRESULT Render(CPDCRenderTarget &DCRT) override;
};

class UWRenderTask {

	vector<RenderElement*> elements;

public:

	UWRenderTask() :
		elements(vector<RenderElement*>()) {

	}

	void AddElement(RenderElement* element) {
		elements.push_back(element);
	}

	BOOL IsEmpty() {
		return elements.empty();
	}

	HRESULT Render(CPDCRenderTarget &DCRT, POINT WndPosition, SIZE WndSize) {
		HRESULT hr = S_OK;
		for (RenderElement* element : elements)
		{
			if (WndPosition.x + (element->rect.right - element->rect.left) <= WndPosition.x + WndSize.cx) {
				if (WndPosition.y + (element->rect.bottom - element->rect.top) <= WndPosition.y + WndSize.cy) {
					if (SUCCEEDED(hr))
						element->Render(DCRT);
					else
						return hr;
				}
			}
		}
		return hr;
	}
};
typedef UWRenderTask RenderTask;
typedef UWRenderTask *PRenderTask, *LPRenderTask;
typedef unique_ptr<UWRenderTask> UPRenderTask;
typedef shared_ptr<UWRenderTask> SPRenderTask;

typedef void (*RenderEvent)(LPCSTR); // Par1: Ukagaka ID

typedef void (*InteractEvent)(LPCSTR, int); // Par1: Ukagaka ID, Par2: Element/Ukagaka body part ID

enum UkagakaBodyPart {
	Head = 0,
	Face = 1,
	Opai = 2,
	Arm = 3,
	Hand = 4,
	Stomach = 5,
	Leg = 6,
	Feet = 7
};

enum FrameRate {
	_20MS = 20, //50FPS
	_40MS = 40, //25FPS
	_50MS = 50, //20FPS
	_100MS = 1000 //10FPS
};


enum UWTextStyle {
	paragraph = 0,
	strong = 1,
	italic = 2,
	underline = 3,
	deleteLine = 4,
	large = 5
};

enum UWTextColor {
	Black = 0, White = 1, DarkGray = 2, LightGray = 3,
	Red = 10, Orange = 11, Yellow = 12, Green = 13, Blue = 14, PureBlue = 15, Purple = 16, Violet = 17,
	Pink = 20, Brown = 21
};


class UWD2DRenderer {

public:

	CPDCRenderTarget mDCRT;
	CPWICImageFactory mWicImgFactory;

	CPD2DFactory mFactory;
	CPDWriteFactory mDwFactory;

	CPGdiItrpRenderTarget mGdiRT;

	string UkagakaID;

	map<tag, int> BitmapMappingTable = map<tag, int>();

	vector<CPBitmap> UWBitmapResrouces = vector<CPBitmap>();

	map<tag, SPAnimation> UWAnimationResources = map<tag, SPAnimation>();

	unordered_map<int, CPSolidBrush> UWBrushResources = unordered_map<int, CPSolidBrush>();

	unordered_map<int, CPDWriteTextFormat> TextStyleResources = unordered_map<int, CPDWriteTextFormat>();

private:
	
	HWND MainHwnd;

	queue<RenderTask> mBuffer = queue<RenderTask>();
	RenderTask mLastBuffer;

	HDC DCRTENV_SCREENDC;
	HDC DCRTENV_MEMDC;
	HBITMAP DCRTENV_CANVAS;
	
	POINT WndPos, TempPos, MouseStartPos;

	SIZE WndSize, TempSize;

public:

	UWD2DRenderer(HWND hWnd);

	HRESULT InitRenderer(HWND hWnd);

	BOOL FetchBuffer(RenderTask renderTask);

	BOOL IsBufferEmpty();
	/*
	<summary>
	Crate an GDI & D2D mixed render enviroment context
	for D2D to draw on the DCRenderTarget and update it on the window
	</summary>
	*/
	HRESULT OnDrawFrame(Milliseconds &deltaTime);

	void RenderOnScreen();

	void OnLeftDown(POINT pos);

	void OnLeftDrag_Global(POINT pos);

private:

	/*
	Create Device Dependent Resources
	*/
	HRESULT CreateDDR();

	/*
	Create Device Independent Resources
	*/
	HRESULT CreateDIR();

	HRESULT DiscardDIR();

	HRESULT DiscardDDR();

	HRESULT CleanUp();

	/*
	Load all bitmap(PNG) file and Animation(GIF) file,
	May also load Animation Description file 
	which is a unique format only for this program
	*/
	HRESULT LoadResources(wstring DirectoryPath);

	HRESULT LoadD2DBitmap(wstring path, string tag, UINT height = 0, UINT width = 0);

	HRESULT LoadD2DAnimation(wstring path, string tag, UINT height = 0, UINT width = 0);

	int AddNewBitmap(string tag);

	CPBitmap GetBitmapByTag(string tag);

	/*
	<summary>
	bind a memory DC to the RenderTarget
	each time when create a new buffer DC
	this new DC should be bind by this function.
	</summary>
	*/
	HRESULT BindDC2RenderTarget(HDC memDC);

	HRESULT CreateDCRTEnv(POINT position, SIZE size);

	void ReleaseDCRTEnv();

	/*
	<summary>
	this function will be always called in main loop,
	but useless when JIT_RENDER mode is on,
	Otherwise this should done all the background render tasks.
	</summary>
	*/
	HRESULT OnGeneralRender(
		_Out_ Milliseconds &deltaTime
	);



	/*
	<summary>
	this will be called every frame,
	in default there will be 25 frame in a second,
	which means it allows 40ms for render process.

	This Function is only called in Main Thread
	</summary>
	*/
	HRESULT OnFrameRender(
		RenderTask task,
		POINT position,
		SIZE size,
		/*_Out_ */Milliseconds &deltaTime
	);

	RenderTask PopBuffer();

	CPSolidBrush RequestBrush(UINT Color);
};
typedef UWD2DRenderer D2DRenderer;
typedef UWD2DRenderer *PD2DRenderer, *LPD2DRenderer;
typedef unique_ptr<UWD2DRenderer> UPD2DRenderer;
typedef shared_ptr<UWD2DRenderer> SPD2DRenderer;

class UWRenderBuffer {
private:
	queue<UWRenderTask> buffer;

public:
	UWRenderBuffer();

	~UWRenderBuffer();

	/* FIXED FRAME THREAD FUNCTION */
	void FetchTaskBuffer(UWRenderTask renderTask);

	HDC RenderBufferToDC();

	void ClearBuffer();

private:
	UWRenderTask PopFromBuffer();
};
typedef UWRenderBuffer RenderBuffer;
typedef UWRenderBuffer *PRenderBuffer, *LPRenderBuffer;
typedef unique_ptr<UWRenderBuffer> UPRenderBuffer;
typedef shared_ptr<UWRenderBuffer> SPRenderBuffer;

enum UWAnimationState {
	InfinityLoop = 0,
	EndWithLastFrame = 1
};

class UkagakaRenderer {
	
public:

	SPD2DRenderer pDirect2DRenderer;

	int CharPerFrame = 1;

	SPAnimation currentAnimation;

	queue<AnimFrame> bitmapQueue = queue<AnimFrame>();

	int currentBalloonTextIndex = 0;

	wstring currentBalloonText = L"";

	wstring cacheBalloonText = L"";

	UWAnimationState currentAnimState;

private:
	UWAnimationState nextAnimState;

	array<int, 8> LastFrame;
	bool changing;

public:
	UkagakaRenderer(SPD2DRenderer D2dRenderer) :
		pDirect2DRenderer(D2dRenderer) {

	}

	HRESULT PlayAnimation(string id, UWAnimationState state);

	HRESULT PlayAnimationImmediately(string id, UWAnimationState state);

	HRESULT PrintText(wstring text, UWTextStyle = UWTextStyle::paragraph, UWTextColor = UWTextColor::Black);

	HRESULT ClearText();

	HRESULT MainLogicUpdate();
};

typedef UkagakaRenderer* PUkagakaRenderer;
typedef unique_ptr<UkagakaRenderer> UPUkagakaRenderer;
typedef shared_ptr<UkagakaRenderer> SPUkagakaRenderer;