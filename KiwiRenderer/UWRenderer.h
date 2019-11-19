
#pragma region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018 Bio-Electric Studio
* All Right Reserved
* This C++ header file is for the Kiwi Renderer, which is part of the Project Ukagaka_W.
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

#include "UWResourceManager.h"

struct UWRenderElement {

	RectangleF rect;
	float opaque;

	UWRenderElement() {}

	UWRenderElement(RectangleF rectangle, float alpha) {
		rect = rectangle;
		opaque = alpha;
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
			if (WndPosition.x + (element->rect.right - element->rect.left) < WndPosition.x + WndSize.cx) {
				if (WndPosition.y + (element->rect.bottom - element->rect.top) < WndPosition.y + WndSize.cy) {
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

class UWD2DRenderer {

public:

	UINT animationFrameRate = FrameRate::_40MS;

	CPDCRenderTarget mDCRT;
	CPWICImageFactory mWicImgFactory;

	CPGdiItrpRenderTarget mGdiRT;

	string UkagakaID;

	map<tag, int> BitmapMappingTable = map<tag, int>();

	vector<CPBitmap> UWBitmapResrouces = vector<CPBitmap>();

	map<tag, SPAnimation> UWAnimationResources = map<tag, SPAnimation>();

private:
	
	HWND MainHwnd;

	CPD2DFactory mFactory;
	CPDWriteFactory mDwFactory;

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

enum AnimationState {
	InfinityLoop = 0,
	EndWithLastFrame = 1
};

class UkagakaRenderer {
	
public:

	SPD2DRenderer pDirect2DRenderer;

	SPAnimation currentAnimation;

	queue<int> bitmapQueue = queue<int>();

	AnimationState currentAnimState;

private:
	AnimationState nextAnimState;

	int LastFrame;
	bool changing;

public:
	UkagakaRenderer(SPD2DRenderer D2dRenderer) :
		pDirect2DRenderer(D2dRenderer) {

	}

	HRESULT PlayAnimation(string id, AnimationState state);

	HRESULT PlayAnimationImmediately(string id, AnimationState state);

	HRESULT MainLogicUpdate();
};

typedef UkagakaRenderer* PUkagakaRenderer;
typedef unique_ptr<UkagakaRenderer> UPUkagakaRenderer;
typedef shared_ptr<UkagakaRenderer> SPUkagakaRenderer;