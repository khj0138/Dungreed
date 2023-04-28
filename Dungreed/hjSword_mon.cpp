#include "hjSword_mon.h"
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
#include "hjHero.h"

#include "hjMonster.h"

extern hj::Application application;
//extern GraphicsPath Path;
//extern const GUID ImageFormatBMP;
namespace hj
{

	Sword_mon::Sword_mon()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(Sword_monState::up)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
	{
	}
	Sword_mon::~Sword_mon()
	{
		delete mEffects;
		mEffects = nullptr;
	}
	void Sword_mon::Initialize()
	{
	}
	void Sword_mon::Update()
	{
		Weapon::Update();
		if (GetOwner()->GetFlip())
			SetFlip(true);
		else
			SetFlip(false);
		switch (mWstate)
		{
		case eWeaponState::IDLE:
		{
			Idle();
			break;
		}
		case eWeaponState::WAIT:
		{
			Wait();
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
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Collider* col = GetOwner()->GetComponent<Collider>();
		Vector2 heroSize = col->GetSize();

		mWstate = Weapon::GetWState();

		float length = mImage->GetHeight();
		Vector2 pos = GetPos();
		Vector2 dir = GetDir();
		isFlip = GetFlip();

		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;



		if (sState == Sword_monState::up)
		{
			mSpawnDir = math::Rotate(dir, -5.0f * flipNum + 180.0f * (float)isFlip);

			mSpawn = pos
				+ Vector2{ (xtemp)*heroSize.x * flipNum,0.0f } // 원 중심 이동
				+ math::Rotate((dir), -90.0f * flipNum) * mRadius // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f // lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length();
		}
		else
		{
			mSpawnDir = math::Rotate(dir, -175.0f * flipNum + 180.0f * (float)isFlip);

			mSpawn = pos
				+ Vector2{ (xtemp)*heroSize.x * flipNum, 0.0f } // 원 중심 이동
				+ math::Rotate((dir), -180.0f * flipNum) * mRadius//; // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f// lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length();
			//+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / 8.f * 3.f).Length();
		}

		GetComponent<Transform>()->SetPos(pos
			+ Vector2{ (xtemp)*heroSize.x * flipNum, 0.0f }
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
		
		/*if (sState == Sword_monState::down)
		{
			bRender = true;
			return;
		}
		else
		{
			bRender = false;
		}*/
	}
	void Sword_mon::Render(HDC hdc)
	{
		/*if (bRender == true)
		{
			bRender = false;
			return;
		}*/

		Weapon::colRender(hdc, posCol, GetBCollision());
		Weapon::GetComponent<SpriteRenderer>()->rotateRender(hdc, mImage->GetHdc(), Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() }, mSpawnDegree, mSpawn);

		/*bRender = true;*/
		//GameObject::Render(hdc);
	}

	void Sword_mon::Create()
	{
		//Weapon::SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		mEffects = GetEmanager();
		mEffects = new Emanager();
		mEffects->SetOwner(this);
		//mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\SwingEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.07f);
		mEffects->RegisterEffect(L"SwingEffect", L"..\\Resource\\Char\\SwingFX.bmp", false, true, 3, Vector2{ 0.0f, 0.0f }, 0.03f, Vector2::One * 3.f);

		Weapon::SetAsRatio(Vector2::One * 4.f);

		mImage = RscManager::Load<Img>(L"Sword_mon", L"..\\Resource\\Char\\ShortSword_mon.bmp");
		//mImage = RscManager::Load<Img>(L"Sword_mon", L"..\\Resource\\Char\\BambooSword_mon.bmp");
		//mImage = RscManager::Load<Img>(L"Sword_mon", L"..\\Resource\\Char\\BambooSword_mon3.bmp");
		mImage->MatchRatio(Weapon::GetAsRatio());

		// collider 설정
		Collider* collider = AddComponent<Collider>();

		float n = 3.2f / 1.6f;
		collider->SetSize(Vector2::One * Vector2{ (float)(mImage->GetWidth()) / n, (float)(mImage->GetHeight()) / 1.f }.Length() * 2.f);
		collider->SetCenter(Vector2{ collider->GetSize().x / 2.f, collider->GetSize().y / 2.f });
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


	}

	void Sword_mon::OnCollisionEnter(Collider* other)
	{
		
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			SetCAttack(true);
			if (GetBAttack() == true)
			{
				/*if (AttackCheck(other))
				{*/
					// other에게 알려줘야함
					hero->Attack(this);
					SetBCollision(true);
				//}
			}
		}
	}

	void Sword_mon::OnCollisionStay(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			SetCAttack(true);
			if (GetBAttack() == true)
			{
				/*if (AttackCheck(other))
				{*/
					// other에게 알려줘야함
					hero->Attack(this);
					SetBCollision(true);
				//}
			}
		}
	}

	void Sword_mon::OnCollisionExit(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			SetCAttack(false);
		}
	}
	bool Sword_mon::AttackCheck(class Collider* other)
	{
		// 충돌된 collider 네 점 계산
		Vector2 otherRect[4] =
		{
			other->GetPos(),// + Vector2{other->GetSize().x * (-1.f), other->GetSize().y * (-1.f)},
			other->GetPos() + Vector2{other->GetSize().x, 0.0f},
			other->GetPos() + Vector2{other->GetSize().x, other->GetSize().y},
			other->GetPos() + Vector2{0.0f , other->GetSize().y},
		};

		// Sword_mon collider 네 점 계산
		Collider* collider = GetComponent<Collider>();
		Vector2 colPos = collider->GetPos() + collider->GetSize() / 2.f;
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{
			rect[i] = colPos + math::Rotate(posCol[i], atan2(GetDir().y, GetDir().x) / PI * 180);
		}

		// Sword_mon collider 네 점이 충돌했는지 확인
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

	void Sword_mon::Idle()
	{

		sState = (Sword_monState)(0);
		if (GetFlip())
			SetDir(Vector2{ -1.0f, 0.0f });
		else
			SetDir(Vector2{ 1.0f, 0.0f });
	}

	void Sword_mon::Wait()
	{
		mTime += Time::DeltaTime();
		Vector2 temp = Vector2{ 1.0f, 0.0f };
		
		if (GetFlip())
		{
			temp *= -1.f;
			int rot = (int)(mTime * 30);
			temp = math::Rotate(temp, rot);
			SetDir(temp);
		}
		else
		{
			int rot = (int)(mTime * 30);
			temp = math::Rotate(temp, rot * -1.f);
			SetDir(temp);
		}
	}

	void Sword_mon::Attack()
	{
		mTime = 0;
		Weapon::Attack();
		// 이펙트 생성 코드 필요
		if (GetFlip())
			SetDir(Vector2{ -1.0f, 0.0f });
		else
			SetDir(Vector2{ 1.0f, 0.0f });
		mEffects->CreateEffect(L"SwingEffect", GetDir());
		sState = (Sword_monState)(1);
	}
	void Sword_mon::Reload()
	{
		Weapon::Reload();
		if (GetFlip())
			SetDir(Vector2{ -1.0f, 0.0f });
		else
			SetDir(Vector2{ 1.0f, 0.0f });
		if (GetBCollision())
			SetBCollision(false);
	}

}