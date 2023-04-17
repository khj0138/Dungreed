#include "hjSword.h"
#include "hjApplication.h"
#include "hjSpriteRenderer.h"
#include "hjEmanager.h"

#include "hjMath.h"

#include "hjImage.h"
#include "hjRscmanager.h"
#include "hjWmanager.h"
#include "hjTransform.h"
#include "hjComponent.h"
#include "hjMouse.h"
#include "hjTime.h"
#include "hjCamera.h"

#include "hjMonster.h"

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
		//, bAttack(false)
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
		Weapon::Update();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		mWstate = Weapon::GetState();

		float length = mImage->GetHeight();
		Vector2 pos = GetPos();
		Vector2 dir = GetDir();
		isFlip = GetFlip();
		
		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;



		if (sState == SwordState::up)
		{
			mSpawnDir = math::Rotate(dir, -5.0f * flipNum + 180.0f * (float)isFlip);

			mSpawn = pos
				+ Vector2{ (xtemp)*heroSize.x * flipNum, (ytemp)*heroSize.y } // 원 중심 이동
				+ math::Rotate((dir), -90.0f * flipNum) * mRadius // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f // lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length();
		}
		else
		{
			mSpawnDir = math::Rotate(dir, -175.0f * flipNum + 180.0f * (float)isFlip);

			mSpawn = pos
				+ Vector2{ (xtemp)*heroSize.x * flipNum, (ytemp)*heroSize.y } // 원 중심 이동
				+ math::Rotate((dir), -180.0f * flipNum) * mRadius//; // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f// lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length();
			//+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / 8.f * 3.f).Length();
		}

		GetComponent<Transform>()->SetPos(pos
			+ Vector2{ (xtemp)*heroSize.x * flipNum, (ytemp)*heroSize.y }
			+ (dir * ((float)mImage->GetHeight() - fabs(posCol[0].x) * 0.8f))
		);
		Collider* collider = GetComponent<Collider>();
		collider->SetPos(
			GetComponent<Transform>()->GetPos()
			- collider->GetSize() / 2.f
		);
		//collider->Update();
		mEffects->Update();


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

		Weapon::colRender(hdc, posCol, bCollision);
		Weapon::GetComponent<SpriteRenderer>()->rotateRender(hdc, mImage->GetHdc(), Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() }, mSpawnDegree, mSpawn);

		bRender = true;
		//GameObject::Render(hdc);
	}

	void Sword::Create()
	{
		//Weapon::SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		mEffects = GetEmanager();
		mEffects = new Emanager();
		mEffects->SetOwner(this);
		//mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\SwingEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.07f);
		mEffects->RegisterEffect(L"SwingEffect", L"..\\Resource\\Char\\SwingFX.bmp", false, true, 3, Vector2{ 0.0f, 0.0f }, 0.03f, Vector2::One * 3.f);

		Weapon::SetAsRatio(Vector2::One * 4.f);

		mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\ShortSword.bmp");
		//mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\BambooSword.bmp");
		//mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\BambooSword3.bmp");
		mImage->MatchRatio(Weapon::GetAsRatio());

		// collider 설정
		Collider* collider = AddComponent<Collider>();
		
		float n = 3.2f;
		collider->SetSize(Vector2::One * Vector2{ (float)(mImage->GetWidth()) / n, (float)(mImage->GetHeight()) / 1.f }.Length() * 2.f);
		collider->SetCenter(Vector2{ collider->GetSize().x / 2.f, collider->GetSize().y /  2.f});
		Vector2 size = collider->GetSize() / 2.f;
		Vector2 rect[4] =
		{
			Vector2{ size.x / n * (-1.f), size.y / -1.f},
			Vector2{ size.x / n, size.y / -1.f },
			Vector2{ size.x / n, size.y / 1.f },
			Vector2{ size.x / n * (-1.f), size.y / 1.f },
		};
		for (int i = 0; i < 4; i++)
		{
			posCol.push_back(rect[i]);
		}

		Weapon::SetReloadTime(0.3f);
		Weapon::AddComponent<SpriteRenderer>();

	}

	void Sword::OnCollisionEnter(Collider* other)
	{
		if (GetBAttack() == true)
		{
			Monster* mon = dynamic_cast<Monster*>(other->GetOwner());
			if (mon != NULL)
			{

				if (AttackCheck(other))
				{
					// other에게 알려줘야함
					bCollision = true;
				}
			}
		}
	}

	void Sword::OnCollisionStay(Collider* other)
	{
		if (GetBAttack() == true)
		{
			Monster* mon = dynamic_cast<Monster*>(other->GetOwner());
			if (mon != NULL)
			{

				if (AttackCheck(other))
				{
					// other에게 알려줘야함
					bCollision = true;
				}
			}
		}
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
			other->GetPos() + Vector2{other->GetSize().x, other->GetSize().y},
			other->GetPos() + Vector2{0.0f , other->GetSize().y},
		};

		// sword collider 네 점 계산
		Collider* collider = GetComponent<Collider>();
		Vector2 colPos = collider->GetPos() + collider->GetSize()/ 2.f;
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{
			rect[i] = colPos + math::Rotate(posCol[i], atan2(GetDir().y, GetDir().x) / PI * 180);
		}

		// sword collider 네 점이 충돌했는지 확인
		Vector2 oRectCenter = (otherRect[0] + otherRect[2]) / 2.f;
		Vector2 oRectVector[2]
			=
		{
			(otherRect[0] - otherRect[1]) / 2.0f,
			(otherRect[1] - otherRect[2]) / 2.0f,
		};

		Vector2 rectCenter = (rect[0] + rect[2]) / 2.f;
		Vector2 rectVector[2]
			=
		{
			(rect[0] - rect[1]) / 2.0f,
			(rect[1] - rect[2]) / 2.0f,
		};

		Vector2 centerVect = oRectCenter - rectCenter;

		for (int i = 0; i < 2; i++)
		{
			if (
				fabs(math::Dot(rectVector[i], rectVector[0]))
				+ fabs(math::Dot(rectVector[i], rectVector[1]))
				+ fabs(math::Dot(rectVector[i], oRectVector[0]))
				+ fabs(math::Dot(rectVector[i], oRectVector[1]))

				< fabs(math::Dot(rectVector[i], centerVect))
				)
				return false;
			if (
				fabs(math::Dot(oRectVector[i], rectVector[0]))
				+ fabs(math::Dot(oRectVector[i], rectVector[1]))
				+ fabs(math::Dot(oRectVector[i], oRectVector[0]))
				+ fabs(math::Dot(oRectVector[i], oRectVector[1]))

				< fabs(math::Dot(oRectVector[i], centerVect))
				)
				return false;
		}

		// 충돌시 false
		return true;
	}

	void Sword::Idle()
	{
		Weapon::Idle();
		if (Mouse::GetLstate() == eKeyState::Down)
		{
			SetBAttack(true);
			SetState((UINT)eWeaponState::ATTACK);
		}
	}

	void Sword::Attack()
	{
		// 이펙트 생성 코드 필요
		Weapon::Attack();
		mEffects->CreateEffect(L"SwingEffect", GetDir());
		SetBAttack(false);
		SetState((UINT)eWeaponState::RELOAD);
		sState = (SwordState)(((UINT)sState + 1) % (UINT)SwordState::END);
	}
	void Sword::Reload()
	{
		Weapon::Reload();
		mTime += Time::DeltaTime();
		bCollision = false;
		if (mTime > GetReloadTime())
		{
			mTime = 0.0f;
			Weapon::SetState((UINT)eWeaponState::IDLE);
		}
	}

}