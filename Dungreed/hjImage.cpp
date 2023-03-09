#include "hjImage.h"
#include "hjApplication.h"
#include "hjRscManager.h"

extern hj::Application application;

namespace hj {
	Image* Image::Create(const std::wstring& name, UINT width, UINT height)
	{
		if (width == 0 || height == 0)
			return nullptr;

		Image* image = RscManager::Find<Image>(name);
		if (image != nullptr)
			return image;

		image = new Image();
		HDC mainHdc = application.GetHdc();

		image->mBitmap = CreateCompatibleBitmap(mainHdc, width, height);

		image->mHdc = CreateCompatibleDC(mainHdc);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
		DeleteObject(oldBitmap);

		image->mWidth = width;
		image->mHeight = height;

		image->SetKey(name);
		RscManager::Insert<Image>(name, image);

		Rectangle(image->GetHdc(), -1, -1, image->mWidth + 1, image->mHeight + 1);

		return image;
	}
	Image::Image()
		: mBitmap(NULL)
		, mHdc(NULL)
		, mWidth(0)
		, mHeight(0)
	{
	}

	Image::~Image()
	{
	}

	HRESULT Image::Load(const std::wstring& path)
	{
		mBitmap = (HBITMAP)LoadImageW(nullptr
			, path.c_str(), IMAGE_BITMAP
			, 0, 0, LR_LOADFROMFILE| LR_CREATEDIBSECTION);
		if (mBitmap == nullptr)
			return E_FAIL;

		BITMAP bitInfo = {};
		GetObject(mBitmap, sizeof(BITMAP), &bitInfo);

		mWidth = bitInfo.bmWidth;
		mHeight = bitInfo.bmHeight;

		HDC mainDC = application.GetHdc();
		mHdc = CreateCompatibleDC(mainDC);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
		if (oldBitmap == mBitmap)
		{
			int a = 0;
		}
		DeleteObject(oldBitmap);

		return S_OK;
	}

	void Image::SetOutputRatio(Vector2 ratio)
	{
		if ((float)mWidth * ratio.y > (float)mHeight * ratio.x)
		{
			mWidth = mHeight / ratio.y * ratio.x;
		}
		else
		{
			mHeight = mWidth / ratio.x * ratio.y;
		}
	}

	void Image::matchResolution(Vector2 Resolution)
	{
		if (Resolution.x == 0.0f && Resolution.y == 0.0f)
			Resolution = Vector2{ (float)application.GetmWidth(), (float)application.GetmHeight() };

		float width = 1600.0f;
		float height = 900.0f;

		if ((float)mWidth * Resolution.y > (float)mHeight * Resolution.x)
		{
			height = (float)Resolution.y;
			width = (float)mWidth * ((float)Resolution.y / (float)mHeight);
		}
		else
		{
			width = (float)Resolution.x;
			height = (float)mHeight * ((float)Resolution.x / (float)mWidth);
		}

		BITMAP oldBitInfo = {};
		GetObject(mBitmap, sizeof(BITMAP), &oldBitInfo);

		HDC bufferDC = CreateCompatibleDC(mHdc);
		HBITMAP changedMap = CreateCompatibleBitmap(mHdc, width, height);

		DeleteObject((HBITMAP)SelectObject(bufferDC, changedMap));
		
		StretchBlt(bufferDC, 0, 0, width, height, mHdc, 0, 0, oldBitInfo.bmWidth, oldBitInfo.bmHeight, SRCCOPY);

		DeleteDC(mHdc);
		
		mHdc = bufferDC;
		mBitmap = changedMap;

		BITMAP changedBitInfo = {};
		GetObject(changedMap, sizeof(BITMAP), &changedBitInfo);

		mWidth = changedBitInfo.bmWidth;
		mHeight = changedBitInfo.bmHeight;

		SetOutputRatio(Resolution);
		//HDC mainHdc = application.GetHdc();
		//TransparentBlt(mainHdc, 0, 0
		//	, mWidth
		//	, mHeight
		//	, mHdc
		//	, 0,0
		//	//, 200, 180,
		//	, width, height,
		//	RGB(255, 255, 255));
	}

}