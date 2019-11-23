
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

#include "UWRenderer.h"


UWD2DRenderer::UWD2DRenderer(HWND hWnd):
	mFactory(NULL),
	mDCRT(NULL),
	MainHwnd(hWnd),
	mGdiRT(NULL),
	mWicImgFactory(NULL),
	mDwFactory(NULL)
{
	InitRenderer(hWnd);
}

HRESULT UWD2DRenderer::InitRenderer(HWND hwnd){
	HRESULT hr;

	MainHwnd = hwnd;
	RECT WndRect;
	GetWindowRect(hwnd, &WndRect);

	WndPos = {
		WndRect.top,
		WndRect.left
	};

	WndSize = {
		WndRect.right - WndRect.left,
		WndRect.bottom - WndRect.top
	};

	hr = this->CreateDIR();
	hr = this->CreateDDR();

	return hr;
}

HRESULT UWD2DRenderer::CreateDIR() {
	HRESULT hr;

	hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->mFactory);

	if (this->mDwFactory == NULL && SUCCEEDED(hr)) {
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(this->mDwFactory),
			reinterpret_cast<IUnknown **>(&this->mDwFactory)
		);
	}

	if (!this->mWicImgFactory&&SUCCEEDED(hr)) {
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&this->mWicImgFactory)
		);
	}

	return hr;
}

HRESULT UWD2DRenderer::CreateDDR() {
	HRESULT hr = S_OK;

	if (mFactory != NULL && this->mDCRT == NULL) {
		D2D1_RENDER_TARGET_PROPERTIES rtp = RenderTargetProperties();
		rtp.usage = D2D1_RENDER_TARGET_USAGE::D2D1_RENDER_TARGET_USAGE_NONE;
		rtp.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE;
		rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, 
			D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED);
		rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

		hr = mFactory->CreateDCRenderTarget(&rtp, &this->mDCRT);

		this->LoadResources(L".\\Resources");
	}

	return hr;
}

HRESULT UWD2DRenderer::DiscardDIR() {
	this->mDwFactory.Release();
	this->mFactory.Release();
	this->mWicImgFactory.Release();
	//IDWriteFactory* idwf = this->mDwFactory.Detach();
	//ID2D1Factory* idf = this->mFactory.Detach();
	//IWICImagingFactory* iwicif = this->mWicImgFactory.Detach();
	//SafeRelease(&idwf);
	//SafeRelease(&idf);
	//SafeRelease(&iwicif);

	return S_OK;
}

HRESULT UWD2DRenderer::DiscardDDR() {
	HRESULT hr;

	this->mDCRT.Release();
	for (CPBitmap b : UWBitmapResrouces) {
		b.Release();
	}

	return S_OK;
}

HRESULT UWD2DRenderer::CleanUp() {
	this->mDwFactory.Release();
	this->mFactory.Release();
	this->mWicImgFactory.Release();

	this->mDCRT.Release();

	if(mGdiRT != NULL)
		this->mGdiRT.Release();

	for (CPBitmap b : UWBitmapResrouces) {
		if (b != NULL) {
			b.Release();
		}
	}

	return S_OK;
}

HRESULT UWD2DRenderer::LoadResources(wstring path) {
	
	HRESULT hr = S_OK;
	wstring BitmapPath = path + L"\\Bitmap";
	wstring AnimationPath = path + L"\\Animation";

	vector<wstring> bitmapFiles = GetAllFileNamesWithExtName(BitmapPath, L"png");
	for (wstring bp : bitmapFiles) {
		if (SUCCEEDED(hr)) {
			hr = LoadD2DBitmap(BitmapPath+(L"\\")+(bp), WString2String(bp).substr(0, bp.size() - 4));
		}
		else {
			return hr;
		}
	}

	vector<wstring> animFiles = GetAllFileNamesWithExtName(AnimationPath, L"gif");
	for (wstring bp : animFiles) {
		if (SUCCEEDED(hr)) {
			hr = LoadD2DAnimation(AnimationPath+(L"\\")+(bp), WString2String(bp).substr(0, bp.size()-4));
		}
		else {
			return hr;
		}
	}

	return hr;
}

void UWD2DRenderer::OnLeftDown(POINT localPos) {
	POINT gp = GetMouseGlobalPos();

	this->TempPos = WndPos;
	this->MouseStartPos = gp;
}

void UWD2DRenderer::OnLeftDrag_Global(POINT globalPos) {
	WndPos =
	{
		TempPos.x + (globalPos.x - MouseStartPos.x),
		TempPos.y + (globalPos.y - MouseStartPos.y)
	};
}

HRESULT UWD2DRenderer::BindDC2RenderTarget(HDC hDC) {

	HRESULT hr = S_OK;

	RECT rect;
	GetClientRect(MainHwnd, &rect);

	if (SUCCEEDED(hr)) {
		if (mDCRT != NULL) {
			hr = mDCRT->BindDC(hDC, &rect);
		}
		else {
			hr = E_FAIL;
		}
	}

	return hr;
}

int UWD2DRenderer::AddNewBitmap(string tag) {
	int ID = UWBitmapResrouces.size();
	UWBitmapResrouces.push_back(NULL);
	BitmapMappingTable[tag] = ID;

	return ID;
}

CPBitmap UWD2DRenderer::GetBitmapByTag(string tag) {
	return UWBitmapResrouces[BitmapMappingTable[tag]];
}

HRESULT UWD2DRenderer::LoadD2DBitmap(wstring path, string tag, UINT height, UINT width) {
	this->mWicImgFactory;

	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr;

	hr = mWicImgFactory->CreateDecoderFromFilename(
		path.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder
	);


	if (SUCCEEDED(hr))
		pDecoder->GetFrame(0, &pSource);
	if (SUCCEEDED(hr))
		hr = mWicImgFactory->CreateFormatConverter(&pConverter);

	if (SUCCEEDED(hr)) {
		if (width != 0 || height != 0) {
			UINT originWidth, originHeight;
			hr = pSource->GetSize(&originWidth, &originHeight);

			if (SUCCEEDED(hr)) {
				if (width == 0 && height != 0) {
					//FLOAT scale = static_cast<FLOAT>(height) / static_cast<FLOAT>(originHeight);
					//width = static_cast<UINT>(static_cast<float>(originWidth) * scale);
					float scale = (float)(height / originHeight);
					width = (UINT)((float)originWidth * scale);
				}
				else if (height == 0 && width != 0) {
					float scale = float(width / originWidth);
					height = (UINT)((float)originHeight * scale);
				}

				hr = mWicImgFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr)) {
					hr = pScaler->Initialize(pSource,
						width,
						height,
						WICBitmapInterpolationMode::WICBitmapInterpolationModeCubic);
				}
				if (SUCCEEDED(hr)) {
					hr = pConverter->Initialize(pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherType::WICBitmapDitherTypeNone,
						NULL,
						0.0F,
						WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
				}
			}
		}
		else {
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherType::WICBitmapDitherTypeNone,
				NULL,
				0.0F,
				WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
		}

		if (SUCCEEDED(hr)) {
			int bmid = AddNewBitmap(tag);
			hr = this->mDCRT->CreateBitmapFromWicBitmap(pConverter, NULL, &(UWBitmapResrouces[bmid]));
		}
	}
	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	return hr;
}

HRESULT UWD2DRenderer::LoadD2DAnimation(wstring path, string tag, UINT height, UINT width) {

	IWICImagingFactory* pWicFac = this->mWicImgFactory;

	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICMetadataQueryReader* pMetaReader = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr;

	wstring extName = path.substr(path.length() - 4, 4);

	TIMELINE_SEC tl = TIMELINE_SEC();
	vector<int> anim = vector<int>();

	if (extName._Equal(L".gif")) {

		hr = pWicFac->CreateDecoderFromFilename(
			path.c_str(), NULL, GENERIC_READ, WICDecodeOptions::WICDecodeMetadataCacheOnLoad, &pDecoder
		);

		UINT frameCount;
		hr = pDecoder->GetFrameCount(&frameCount);

		PROPVARIANT meta;
		PropVariantInit(&meta);

		for (int f = 0; f < frameCount; f++) {

			int frameDelay = 40;

			if (SUCCEEDED(hr)) {
				hr = pDecoder->GetFrame(f, &pSource);
			}

			if (SUCCEEDED(hr)) {
				hr = pSource->GetMetadataQueryReader(&pMetaReader);
				if (SUCCEEDED(hr)) {
					hr = pMetaReader->GetMetadataByName(L"/grctlext/Delay", &meta);
					hr = (meta.vt == VT_UI2 ? S_OK : E_FAIL);
					if (SUCCEEDED(hr)) {
						frameDelay = meta.uiVal * 10;
					}
				}
			}

			if (SUCCEEDED(hr)) {
				hr = pWicFac->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr)) {
				if (width != 0 || height != 0) {
					UINT originWidth, originHeight;
					hr = pSource->GetSize(&originWidth, &originHeight);

					if (SUCCEEDED(hr)) {
						if (width == 0) {
							//FLOAT scale = static_cast<FLOAT>(height) / static_cast<FLOAT>(originHeight);
							//width = static_cast<UINT>(static_cast<float>(originWidth) * scale);
							float scale = (float)(width / originWidth);
							width = (UINT)((float)originWidth * scale);
						}
						if (height == 0) {
							float scale = float(width / originWidth);
							height = (UINT)((float)originHeight * scale);
						}

						hr = pWicFac->CreateBitmapScaler(&pScaler);
						if (SUCCEEDED(hr)) {
							hr = pScaler->Initialize(pSource,
								width,
								height,
								WICBitmapInterpolationMode::WICBitmapInterpolationModeCubic);
						}
						if (SUCCEEDED(hr)) {
							hr = pConverter->Initialize(pScaler,
								GUID_WICPixelFormat32bppPBGRA,
								WICBitmapDitherType::WICBitmapDitherTypeNone,
								NULL,
								0.0F,
								WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
						}
					}
				}
				else {
					hr = pConverter->Initialize(
						pSource,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherType::WICBitmapDitherTypeNone,
						NULL,
						0.0F,
						WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
				}
			}

			if (SUCCEEDED(hr)) {
				string tg = tag + "_" + std::to_string(f);
				tl.push_back(pair<Milliseconds, string>(frameDelay, tg));
				int bmp = AddNewBitmap(tg);
				anim.push_back(bmp);
				hr = this->mDCRT->CreateBitmapFromWicBitmap(pConverter, NULL, &UWBitmapResrouces[bmp]);
			}
		}
		UWAnimationResources[tag] = SPAnimation(new Animation(BitmapMappingTable, tl, (int)(1000 / this->animationFrameRate)));
		UWAnimationResources[tag]->currentRenderFrameRate = this->animationFrameRate;
	}

	return hr;
}

HRESULT UWD2DRenderer::CreateDCRTEnv(POINT position, SIZE size) {
	
	int ix, iy;

	ix = size.cx;
	iy = size.cy;

	DCRTENV_SCREENDC = GetDC(NULL);
	DCRTENV_MEMDC = CreateCompatibleDC(DCRTENV_SCREENDC);

	BYTE * pBits;
	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = ix;
	bmih.biHeight = iy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32; //这里一定要是32 16位色没有alpha通道
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	DCRTENV_CANVAS = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);

	SelectObject(DCRTENV_MEMDC, DCRTENV_CANVAS);

	HRESULT hr = BindDC2RenderTarget(DCRTENV_MEMDC);

	return hr;
}

void UWD2DRenderer::ReleaseDCRTEnv() {
	
	DeleteDC(DCRTENV_MEMDC);
	DeleteDC(DCRTENV_SCREENDC);
	DeleteObject(DCRTENV_CANVAS);

	ReleaseDC(MainHwnd, DCRTENV_MEMDC);
	ReleaseDC(NULL, DCRTENV_SCREENDC);

}

HRESULT UWD2DRenderer::OnDrawFrame(Milliseconds &deltaTime) {

	Timer timer = Timer();
	timer.Start();

	HRESULT hr;

	POINT position = WndPos;
	SIZE size = WndSize;

	hr = CreateDCRTEnv(position, size);

	HDC screenDC = DCRTENV_SCREENDC, 
		memDC = DCRTENV_MEMDC;
	HBITMAP canvas = DCRTENV_CANVAS;

	if (SUCCEEDED(hr)) {

		try {
			mDCRT->BeginDraw();
		}
		catch (_com_error &e) {
			IErrorInfo* info = e.ErrorInfo();
			BSTR errDesc;
			BSTR errSource;
			hr = info->GetDescription(&errDesc);
			hr = info->GetSource(&errSource);

			_bstr_t err = e.Description();
			Error(LPCWSTR(err));

			info->Release();
		}
		Milliseconds dt;
		hr = OnFrameRender(this->PopBuffer(), position, size, dt);
		//CPBitmap bm = GetBitmapByTag("BM0_Test");
		//mDCRT->DrawBitmap(bm, { 0, 0, 100, 100 }, 1.0f);

		mDCRT->EndDraw();

		//Sleep(40);
	}
	deltaTime = timer.GetMilliseconds();
	timer.End();

	if (hr == D2DERR_RECREATE_TARGET) {
		hr = S_OK;
		DiscardDDR();
		CreateDDR();
	}
	InvalidateRect(MainHwnd, NULL, FALSE);

	return hr;
}

void UWD2DRenderer::RenderOnScreen() {

	HRESULT hr = S_OK;

	POINT	pPos = WndPos;
	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = WndSize;

	BLENDFUNCTION blendFunc;
	blendFunc.AlphaFormat = AC_SRC_ALPHA;
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 255;
	//SetLayeredWindowAttributes(hwnd, 0, 255, ULW_ALPHA);
	bool re = UpdateLayeredWindow(MainHwnd, DCRTENV_SCREENDC, &pPos, &sizeWnd, DCRTENV_MEMDC, &pSrc, NULL, &blendFunc, ULW_ALPHA);
	DWORD err = GetLastError();

	ReleaseDCRTEnv();
}

HRESULT UWD2DRenderer::OnFrameRender(RenderTask task, POINT position, SIZE canvasSize, _Out_ Milliseconds& deltaTime) {
	Timer timer = Timer();
	timer.Start();
	if (!task.IsEmpty()) {
		task.Render(mDCRT, position, canvasSize);
		mLastBuffer = task;
	}
	else {
		mLastBuffer.Render(mDCRT, position, canvasSize);
	}
		
	deltaTime = timer.GetMilliseconds();
	timer.End();

	return S_OK;
}

RenderTask UWD2DRenderer::PopBuffer() {
	if (mBuffer.size() > 0) {
		RenderTask task = mBuffer.front();
		mBuffer.pop();
		return task;
	}
	else {
		Warning(L"Buffer pool empty, frame render stroked, please check any time-cost codes in Main Thread");
		return RenderTask();
	}
}

BOOL UWD2DRenderer::FetchBuffer(RenderTask renderTask) {
	if (mBuffer.size() < 1) {
		this->mBuffer.push(renderTask);
		return true;
	}
	else {
		return false;
	}
}

BOOL UWD2DRenderer::IsBufferEmpty() {
	return mBuffer.empty();
}


HRESULT UWRenderElement_Bitmap::Render(CPDCRenderTarget &DCRT) {
	DCRT->DrawBitmap(bitmap, rect, opaque);
	return S_OK;
}

HRESULT	UkagakaRenderer::PlayAnimation(string id, UWAnimationState state){
	changing = true;

	currentAnimation = pDirect2DRenderer->UWAnimationResources[id];
	nextAnimState = state;

	return S_OK;
}

HRESULT UkagakaRenderer::PlayAnimationImmediately(string id, UWAnimationState state) {
	bitmapQueue = queue<int>();
	return PlayAnimation(id, state);
}

HRESULT UkagakaRenderer::MainLogicUpdate() {
	
	if (bitmapQueue.empty()) {

		if (changing) {
			currentAnimation->FetchToQueue(bitmapQueue);
			currentAnimState = nextAnimState;
			changing = false;
		}else if(currentAnimState == UWAnimationState::EndWithLastFrame) {
			bitmapQueue.push(LastFrame);
		}else if (currentAnimState == UWAnimationState::InfinityLoop) {
			currentAnimation->FetchToQueue(bitmapQueue);
		}
	}

	if (this->pDirect2DRenderer->IsBufferEmpty()) {
		int frame = bitmapQueue.front();
		CPBitmap bm = pDirect2DRenderer->UWBitmapResrouces[frame];
		LastFrame = frame;
		bitmapQueue.pop();
		RenderTask task = RenderTask();
		
		UWRenderElement_Bitmap* element1 = new UWRenderElement_Bitmap({ 0, 0 , 100, 100 }, 1.0f, bm);
		task.AddElement(element1);
		this->pDirect2DRenderer->FetchBuffer(task);
	}

	return S_OK;

}