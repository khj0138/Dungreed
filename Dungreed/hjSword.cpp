#include "hjSword.h"
#include "hjApplication.h"

extern hj::Application application;
//extern GraphicsPath Path;
//extern const GUID ImageFormatBMP;
namespace hj
{

	Sword::Sword()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(SwordState::up)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		, bAttack(false)
		, bCollision(false)
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
		Transform* tr = GetManager()->GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();
		mWstate = Weapon::GetState();
		float length = mImage->GetHeight();
		Vector2 pos = GetManager()->GetPos();
		Vector2 dir = GetManager()->GetDir();
		isFlip = GetManager()->GetFlip();
		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;

		Collider* collider = GetManager()->GetComponent<Collider>();
		collider->SetPos(
			pos
			+ Vector2{ (xtemp)*heroSize.x * flipNum, (ytemp)*heroSize.y } // 원 중심 이동
			+ (dir * ((float)mImage->GetHeight() - fabs(posCol[0].x) * 2.f))
		);
		

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
			mSpawnDir = math::Rotate(dir, -175.0f * flipNum + 180.0f * (float)isFlip);
			
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
		if (sState == SwordState::down)
		{
			bRender = true;
			return;
		}
		else
		{
			bRender = false;
		}
	}
	void Sword::Render(HDC hdc)
	{
		if (bRender == true)
		{
			bRender = false;
			return;
		}
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

			HBITMAP hBitmap = NULL;
			plusB->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);

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
		DeleteObject(temp);
		DeleteObject(h_bitmap);
		delete[] p_data;
		p_data = nullptr;
		

		Collider* collider = GetManager()->GetComponent<Collider>();
		collider->SetPos(collider->GetPos() - collider->GetCenter());
		collider->Render(hdc);
		collider->SetPos(collider->GetPos() + collider->GetCenter());

		Vector2 pos = collider->GetPos();
		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;
		Transform* tr = GetManager()->GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		// collider render 부분
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{ 
			rect[i] = math::Rotate( posCol[i], atan2(GetManager()->GetDir().y, GetManager()->GetDir().x) / PI * 180);
			rect[i] = Camera::CaluatePos(collider->GetPos() + rect[i], 1.f);
		}
		HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		if (bCollision)
		{
			HPEN redpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			DeleteObject(SelectObject(hdc, redpen));
		}
		for (int i = 0; i < 4; i++)
		{
			MoveToEx(hdc, (int)(rect[i].x) , (int)(rect[i].y) , nullptr);
			LineTo(hdc, (int)(rect[(i + 1) % 4].x) , (int)(rect[(i + 1) % 4].y ));
		}
		DeleteObject((HPEN)SelectObject(hdc, oldPen));
		// collider render 부분

		// 무기 render 타이밍 조율
		bRender = true;
	}

	void Sword::Create()
	{
		Weapon::SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\BambooSword.bmp");
		mImage->MatchRatio(Weapon::GetAsRatio());

		// collider 설정
		Collider* collider = GetManager()->GetComponent<Collider>();
		collider->SetSize(Vector2::One * Vector2{ (float)(mImage->GetWidth()) /4.f, (float)(mImage->GetHeight()) / 1.f }.Length() * 2.f);
		collider->SetCenter(Vector2{ collider->GetSize().x / 2.f, collider->GetSize().y / 2.f });
		Vector2 rect[4] =
		{
			Vector2{ (float)(mImage->GetWidth()) / -4.f, (float)(mImage->GetHeight()) / -1.f },
			Vector2{ (float)(mImage->GetWidth()) / 4.f, (float)(mImage->GetHeight()) / -1.f },
			Vector2{ (float)(mImage->GetWidth()) / 4.f, (float)(mImage->GetHeight()) / 1.f },
			Vector2{ (float)(mImage->GetWidth()) / -4.f, (float)(mImage->GetHeight()) / 1.f },
		};
		for (int i = 0; i < 4; i++)
		{
			posCol.push_back(rect[i]);
		}
	}

	void Sword::OnCollisionEnter(Collider* other)
	{
		if (bAttack == true)
		{
			if (AttackCheck(other))
			{
				// other에게 알려줘야함
				bCollision = true;
			}
		}
	}

	void Sword::OnCollisionStay(Collider* other)
	{
	}

	void Sword::OnCollisionExit(Collider* other)
	{
	}
	bool Sword::AttackCheck(class Collider* other)
	{
		// 충돌된 collider 네 점 계산
		Vector2 otherRect[4] =
		{
			other->GetPos(),// + Vector2{other->GetSize().x * (-1.f), other->GetSize().y * (-1.f)},
			other->GetPos() + Vector2{other->GetSize().x, 0.0f},
			other->GetPos() + Vector2{0.0f , other->GetSize().y},
			other->GetPos() + Vector2{other->GetSize().x, other->GetSize().y},
		};

		// sword collider 네 점 계산
		Collider* collider = GetManager()->GetComponent<Collider>();
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{
			rect[i] = collider->GetPos() + math::Rotate(posCol[i], atan2(GetManager()->GetDir().y, GetManager()->GetDir().x) / PI * 180);
		}

		// sword collider 네 점이 충돌했는지 확인
		Vector2 oRectCenter = (otherRect[0] + otherRect[2]) / 2.0f;
		for (int i = 0; i < 4; i++)
		{
			if (fabs(rect[i].x - oRectCenter.x) < (other->GetSize().x / 2.0f)
				&& fabs(rect[i].y - oRectCenter.y) < (other->GetSize().y / 2.0f))
			{
				return true;
			}
		}

		// other collider 네 점이 충돌했는지 확인
		Vector2 rectCenter = (rect[0] + rect[2]) / 2.0f;
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 2; i++)
			{
				Vector2 dir = rect[i] - rect[i + 1];
				float length = dir.Length() / 2.f;
				Vector2 dir2 = otherRect[j] - rectCenter;
				if (fabs(math::Dot(dir.Normalize(), dir2)) < length)
					return true;
			}
		}

		// 미충돌시 false
		return false;
	}

	void Sword::Idle()
	{
		if (Mouse::GetLstate() == eKeyState::Down)
			Weapon::SetState((UINT)eWeaponState::ATTACK);
	}

	void Sword::Attack()
	{
		// 이펙트 생성 코드 필요
		bAttack = true;
		sState = (SwordState)(((UINT)sState + 1) % (UINT)SwordState::END);
		Weapon::SetState((UINT)eWeaponState::RELOAD);
	}
	void Sword::Reload()
	{
		bAttack = false;
		mTime += Time::DeltaTime();
		if (mTime > 0.5)
		{
			bCollision = false;
			mTime = 0.0f;
			Weapon::SetState((UINT)eWeaponState::IDLE);
		}
	}
	
}