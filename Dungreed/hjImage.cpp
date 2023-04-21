#include "hjImage.h"
#include "hjRscManager.h"
#include "hjApplication.h"

extern hj::Application application;


namespace hj {
	Img* Img::Create(const std::wstring& name, UINT width, UINT height)
	{
		if (width == 0 || height == 0)
			return nullptr;

		Img* image = RscManager::Find<Img>(name);
		if (image != nullptr)
		{
			return image;
		}
		image = new Img();
		HDC mainHdc = application.GetHdc();

		image->mBitmap = CreateCompatibleBitmap(mainHdc, width, height);

		image->mHdc = CreateCompatibleDC(mainHdc);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
		DeleteObject(oldBitmap);

		image->mWidth = width;
		image->mHeight = height;

		image->SetKey(name);
		RscManager::Insert<Img>(name, image);

		Rectangle(image->GetHdc(), -1, -1, image->mWidth + 1, image->mHeight + 1);

		return image;
	}
	Img::Img()
		: mBitmap(NULL)
		, mHdc(NULL)
		, mWidth(0)
		, mHeight(0)
		, mRepeat(false)
		, mMoveRate(Vector2::Zero)
		, mask(nullptr)
	{
	}

	Img::~Img()
	{
		DeleteObject(mBitmap);
		DeleteDC(mHdc);
	}

	HRESULT Img::Load(const std::wstring& path)
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
		
		DeleteObject((HBITMAP)SelectObject(mHdc, mBitmap));
		
		//makeMask(path);
		//HDC memDC = CreateCompatibleDC(mHdc);
		//HBITMAP memBitmap = CreateCompatibleBitmap(mHdc, mWidth, mHeight);

		//DeleteObject((HBITMAP)SelectObject(memDC, memBitmap));

		//BLENDFUNCTION func = {};
		//func.BlendOp = AC_SRC_OVER;
		//func.BlendFlags = 0;
		//func.AlphaFormat = AC_SRC_ALPHA;
		//func.SourceConstantAlpha = 0; // 0(투명) ~ 255(불투명) 알파값
		///*COLORREF a = RGB(0, 0,0);
		//SetBkColor(memDC, a);*/
		///*AlphaBlend(memDC, 0, 0
		//	, mWidth, mHeight
		//	, mHdc
		//	, 0, 0
		//	, mWidth, mHeight
		//    , func);*/
		//TransparentBlt(memDC
		//	, 0, 0
		//	, mWidth, mHeight
		//	, mHdc
		//	, 0, 0
		//	, mWidth, mHeight,
		//	RGB(255, 0, 254));
		//

		//DeleteDC(mHdc);
		//mHdc = memDC;
		//mBitmap = memBitmap;
		////DeleteObject((HBITMAP)SelectObject(mHdc, memBitmap));

		return S_OK;
	}
	void Img::makeMask(const std::wstring& path)
	{	
		std::wstring temp = L"..\Resource\mask_IceBackBG.bmp";
		temp.append(path);

		HBITMAP a =  (HBITMAP)LoadImageW(nullptr
			, temp.c_str(), IMAGE_BITMAP
			, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (a == nullptr)
			return;
		mBitmap = a;
		BITMAP bitInfo = {};
		GetObject(mBitmap, sizeof(BITMAP), &bitInfo);

		mWidth = bitInfo.bmWidth;
		mHeight = bitInfo.bmHeight;

		HDC mainDC = application.GetHdc();
		mHdc = CreateCompatibleDC(mainDC);

		DeleteObject((HBITMAP)SelectObject(mHdc, mBitmap));
	}
	void Img::SetOutputSize(Vector2 size)
	{
		if (mWidth > size.x)
		{
			mWidth = size.x;
		}
		if (mHeight > size.y)
		{
			mHeight = size.y;
		}
	}

	void Img::MatchRatio(Vector2 ratio)
	{
		if (!(ratio.x && ratio.y))
			return;
		
		float height = (float)mHeight * ratio.y;
		float width = (float)mWidth * ratio.x;
		
		BITMAP oldBitInfo = {};
		GetObject(mBitmap, sizeof(BITMAP), &oldBitInfo);

		HDC memDC = CreateCompatibleDC(mHdc);
		HBITMAP memBitmap = CreateCompatibleBitmap(mHdc, width, height);

		DeleteObject((HBITMAP)SelectObject(memDC, memBitmap));
		
		/*COLORREF a = RGB(0,0,0);
		SetBkColor(memDC, a);*/
		StretchBlt(memDC, 0, 0, width, height, mHdc, 0, 0, oldBitInfo.bmWidth, oldBitInfo.bmHeight, SRCCOPY);

		DeleteDC(mHdc);
		
		mHdc = memDC;
		mBitmap = memBitmap;

		BITMAP changedBitInfo = {};
		GetObject(memBitmap, sizeof(BITMAP), &changedBitInfo);

		mWidth = changedBitInfo.bmWidth;
		mHeight = changedBitInfo.bmHeight;
	}

}