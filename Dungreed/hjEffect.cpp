#include "hjEffect.h"
#include "hjEmanager.h"
#include "hjAnimator.h"
#include "hjComponent.h"
#include "hjImage.h"
#include "hjMath.h"
#include "hjRscmanager.h"
#include "hjTransform.h"
#include "hjApplication.h"

//#include "hjCamera.h"
//#include "hjMouse.h"
//#include "hjTime.h"
//#include "hjCollider.h"

extern hj::Application application;

namespace hj
{
	Effect::Effect()
	{
		mAnimator = new Animator();
		//mAnimator = GameObject::AddComponent<Animator>();
	}
	Effect::~Effect()
	{
	}

	void Effect::Initialize()
	{
	}
	void Effect::Update()
	{
	}
	void Effect::Render(HDC hdc)
	{
		//// Gdiplus bitmap ¸¸µé±â
		//HBITMAP h_bitmap = (HBITMAP)GetCurrentObject(mImage->GetHdc(), OBJ_BITMAP);

		//BITMAP bmp_info;
		//GetObject(h_bitmap, sizeof(BITMAP), &bmp_info);

		//int pattern_size = (bmp_info.bmWidth + 1) * (bmp_info.bmHeight + 1) * (bmp_info.bmBitsPixel) / 8;
		////int n = 95;
		//BYTE* p_data = new BYTE[pattern_size];
		//GetBitmapBits(h_bitmap, pattern_size, p_data);

		//HBITMAP temp = NULL;
		//BITMAPINFO bInfo = { 0 };
		//bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//bInfo.bmiHeader.biWidth = bmp_info.bmWidth;
		//bInfo.bmiHeader.biHeight = (-1) * bmp_info.bmHeight;
		//bInfo.bmiHeader.biPlanes = 1;
		//bInfo.bmiHeader.biBitCount = 24;
		//bInfo.bmiHeader.biCompression = BI_RGB;

		//void* pBits = NULL;
		//temp = CreateDIBSection(NULL, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

		//memcpy(pBits, p_data, pattern_size);

		//Gdiplus::Bitmap* plusB = Gdiplus::Bitmap::FromHBITMAP(temp, NULL);




		//if (plusB != NULL)
		//{
		//	/////////////////////////////////////////////////////////////////////////////
		//	Gdiplus::Graphics G(plusB);

		//	G.ResetTransform();
		//	G.RotateTransform((REAL)mSpawnDegree);

		//	G.TranslateTransform(mImage->GetWidth() / 2.f, mImage->GetHeight() / 2.f, Gdiplus::MatrixOrderAppend);
		//	G.DrawImage(plusB, -(INT)mImage->GetWidth() / 2, -(INT)mImage->GetHeight() / 2);
		//	//G.DrawImage(plusB, +(INT)mImage->GetWidth() / 1, +(INT)mImage->GetHeight() / 1);
		//	//G.TranslateTransform( mImage->GetWidth() / -2.0f, mImage->GetHeight() / -2.0f, Gdiplus::MatrixOrderAppend);

		//	HBITMAP hBitmap = NULL;
		//	plusB->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);

		//	//		HBITMAP hBitmap = NULL;
		//		//	Gdiplus::Status status = plusB->GetHBITMAP(Color(0, 0, 0), &hBitmap);

		//	HDC memDC = G.GetHDC();
		//	HBITMAP oldMap = (HBITMAP)SelectObject(memDC, hBitmap);
		//	Vector2 realSpawn = Camera::CaluatePos(mSpawn, 1.f);
		//	TransparentBlt(hdc, (int)realSpawn.x, (int)realSpawn.y
		//		, mImage->GetWidth(), mImage->GetHeight()
		//		, memDC, 0, 0
		//		, mImage->GetWidth(), mImage->GetHeight()
		//		, RGB(255, 0, 255));
		//	DeleteObject((HBITMAP)SelectObject(memDC, oldMap));
		//	DeleteDC(memDC);
		//	G.ReleaseHDC(memDC);
		//	G.~Graphics();

		//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//}

		//delete plusB;
		////delete temp;
		//DeleteObject(temp);
		//DeleteObject(h_bitmap);
		//delete[] p_data;
		//p_data = nullptr;
		return;
		if (mEmanager != nullptr)
			Transform* tr = mEmanager->GetOwner()->GetComponent<Transform>();
		
		//mAnimator->GetActiveAnimation()->

	}
	void Effect::Create(const std::wstring& name, const std::wstring& path, bool loop, bool bDir, UINT frame)
	{
		SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		mImage = RscManager::Load<Img>(name, path);
		mImage->MatchRatio(Effect::GetAsRatio());
		mBdir = bDir;
		std::wstring flipName = (L"Flipped");
		flipName.append(name);
		mAnimator->CreateAnimation(name, mImage, Vector2::Zero, frame, 2, frame, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(flipName, mImage, Vector2{ 0.0f,(float)(mImage->GetHeight() / 2) }, frame, 2, frame, Vector2::Zero, 0.1);

	}
	void Effect::Idle()
	{
	}
	void Effect::Attack()
	{
	}
	void Effect::Reload()
	{
	}
}