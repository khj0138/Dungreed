#include "hjSword.h"
extern GraphicsPath Path;
extern const GUID ImageFormatBMP;
namespace hj
{

	Sword::Sword()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(SwordState::up)
		, isFlip(false)
		, mSpawnDegree(90)
	{
	}
	Sword::~Sword()
	{
	}
	void Sword::Initialize()
	{
	}
	void Sword::Update()
	{
		Transform* tr = this->GetManager()->GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();
		mWstate = Weapon::GetState();
		float length = mImage->GetHeight();
		Vector2 pos = this->GetManager()->GetPos();
		Vector2 dir = this->GetManager()->GetDir();
		isFlip = this->GetManager()->GetFlip();
		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;
		if (sState == SwordState::up)
		{
			mSpawnDir = math::Rotate(dir, -5.0f * flipNum + 180.0f * (float)isFlip);

			mSpawn = pos
				+ Vector2{ (xtemp) * heroSize.x * flipNum, (ytemp) * heroSize.y } // 원 중심 이동
				+ math::Rotate((dir), -90.0f * flipNum) * mRadius // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f // lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length();

		}
		else
		{
			mSpawnDir = math::Rotate(dir, -170.0f * flipNum + 180.0f * (float)isFlip);
			
			mSpawn = pos
				+ Vector2{ (xtemp) * heroSize.x * flipNum, (ytemp) * heroSize.y }  // 원 중심 이동
				+ math::Rotate((dir), -180.0f * flipNum) * mRadius//; // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f// lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length();
				//+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / 8.f * 3.f).Length();
			
		}
		mSpawnDir.Normalize();
		mSpawnDegree = atan2(mSpawnDir.y, mSpawnDir.x) / PI * 180.0f;
		switch (mWstate)
		{
		case eWeaponState::IDLE:
		{
			Idle();
			break;
		}
		case eWeaponState::ATTACK:
		{
			Attack();
			break;
		}
		case eWeaponState::RELOAD:
		{
			Reload();
			break;
		}
		}
	}
	void Sword::Render(HDC hdc)
	{

		// Gdiplus bitmap 만들기
		HBITMAP h_bitmap = (HBITMAP)GetCurrentObject(mImage->GetHdc(), OBJ_BITMAP);
		
		BITMAP bmp_info;
		GetObject(h_bitmap, sizeof(BITMAP), &bmp_info);

		int pattern_size = (bmp_info.bmWidth+1) * (bmp_info.bmHeight+1) * (bmp_info.bmBitsPixel) / 8;
		//int n = 95;
		BYTE* p_data = new BYTE[pattern_size];
		GetBitmapBits(h_bitmap, pattern_size, p_data);
		
		HBITMAP temp = NULL;
		BITMAPINFO bInfo = { 0 };
		bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bInfo.bmiHeader.biWidth = bmp_info.bmWidth;
		bInfo.bmiHeader.biHeight =  (-1) * bmp_info.bmHeight;
		bInfo.bmiHeader.biPlanes = 1;
		bInfo.bmiHeader.biBitCount = 24;
		bInfo.bmiHeader.biCompression = BI_RGB;

		void* pBits = NULL;
		temp = CreateDIBSection(NULL, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

		memcpy(pBits, p_data, pattern_size);

		Gdiplus::Bitmap* plusB = Gdiplus::Bitmap::FromHBITMAP(temp, NULL);


	
		
		if (plusB != NULL)
		{
			/////////////////////////////////////////////////////////////////////////////
			Gdiplus::Graphics G(plusB);
			
			G.ResetTransform();
			G.RotateTransform((REAL)mSpawnDegree);
			
			G.TranslateTransform(mImage->GetWidth() / 2.f, mImage->GetHeight() / 2.f, Gdiplus::MatrixOrderAppend);
			G.DrawImage(plusB, -(INT)mImage->GetWidth() / 2, -(INT)mImage->GetHeight() / 2);
			//G.DrawImage(plusB, +(INT)mImage->GetWidth() / 1, +(INT)mImage->GetHeight() / 1);
			//G.TranslateTransform( mImage->GetWidth() / -2.0f, mImage->GetHeight() / -2.0f, Gdiplus::MatrixOrderAppend);

			HBITMAP hBitmap = NULL;
			plusB->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);

			//		HBITMAP hBitmap = NULL;
				//	Gdiplus::Status status = plusB->GetHBITMAP(Color(0, 0, 0), &hBitmap);

			HDC memDC = G.GetHDC();
			HBITMAP oldMap = (HBITMAP)SelectObject(memDC, hBitmap);
			Vector2 realSpawn = Camera::CaluatePos(mSpawn, 1.f) ;
			TransparentBlt(hdc, (int)realSpawn.x, (int)realSpawn.y
				, mImage->GetWidth(), mImage->GetHeight()
				, memDC, 0, 0
				, mImage->GetWidth(), mImage->GetHeight()
				, RGB(255,0,255));
			DeleteObject((HBITMAP)SelectObject(memDC, oldMap));
			DeleteDC(memDC);
			G.ReleaseHDC(memDC);
			G.~Graphics();
			
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		delete plusB;
		//delete temp;
		DeleteObject(temp);
		DeleteObject(h_bitmap);
		delete[] p_data;
		p_data = nullptr;


		
		
	}

	void Sword::Create()
	{
		mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\BambooSword.bmp");
		//mImage = RscManager::Load<Image>(L"Sword", L"..\\Resource\\Char\\.bmp");
	}

	void Sword::Idle()
	{
		if (Mouse::GetLstate() == eKeyState::Down)
			Weapon::SetState((UINT)eWeaponState::ATTACK);
	}

	void Sword::Attack()
	{
		sState = (SwordState)(((UINT)sState + 1) % (UINT)SwordState::END);
		// collider 생성
		Weapon::SetState((UINT)eWeaponState::IDLE);
	}
	void Sword::Reload()
	{
		mTime += Time::DeltaTime();
		if (mTime > 0.5)
		{
			mTime = 0.0f;
			Weapon::SetState((UINT)eWeaponState::IDLE);
		}
	}
	
}