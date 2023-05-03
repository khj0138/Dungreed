#include "hjCosmosSword.h"
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
#include "hjBaseBullet.h"

extern hj::Application application;
//extern GraphicsPath Path;
//extern const GUID ImageFormatBMP;
namespace hj
{

	CosmosSword::CosmosSword()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(CosmosSwordState::up)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
	{
	}
	CosmosSword::~CosmosSword()
	{
		delete mEffects;
		mEffects = nullptr;
	}
	void CosmosSword::Initialize()
	{
	}
	void CosmosSword::Update()
	{
		Weapon::Update();
		GetComponent<Animator>()->Update();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		mWstate = Weapon::GetWState();

		float length = mSize;
		Vector2 pos = GetPos();
		Vector2 dir = GetDir();
		isFlip = GetFlip();

		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ mSize, mSize };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;



		if (sState == CosmosSwordState::up)
		{
			mSpawnDir = math::Rotate(dir, -5.0f * flipNum + 180.0f * (float)isFlip); // 무기 비틀기

			mSpawn = pos
				+ Vector2{ (xtemp)*heroSize.x * flipNum,0.0f }// (ytemp)*heroSize.y} // 원 중심 이동
				+ math::Rotate((dir), -90.0f * flipNum) * mRadius // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f // lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length(); // 무기를 각도에 맞게 밀어내기
		}
		else
		{
			mSpawnDir = math::Rotate(dir, -175.0f * flipNum + 180.0f * (float)isFlip); // 무기 비틀기

			mSpawn = pos
				+ Vector2{ (xtemp)*heroSize.x * flipNum,0.0f }// (ytemp)*heroSize.y} // 원 중심 이동
				+ math::Rotate((dir), -180.0f * flipNum) * mRadius//; // 원 중심을 기준으로 위치 회전
				- imgVect / 2.f// lefttop -> middle
				+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length(); // 무기를 각도에 맞게 밀어내기
			//+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / 8.f * 3.f).Length();
		}

		GetComponent<Transform>()->SetPos(pos
			+ Vector2{ (xtemp)*heroSize.x * flipNum ,0.0f } // 무기 실제 위치 원 중심으로 이동
			+ (dir * ((float)mImage->GetHeight() - fabs(posCol[0].x) * 0.8f)) // 각도에 따라 무기 위치 이동
		);
		//mPos = GetComponent<Transform>()->GetPos();
		Collider* collider = GetComponent<Collider>();
		collider->SetPos(
			GetComponent<Transform>()->GetPos()
			- collider->GetSize() / 2.f
		);
		//collider->Update();

		GetComponent<SpriteRenderer>()->SetDir(math::Rotate(mSpawnDir, -90.0f));
		mEffects->Update();

		mPos = mSpawn
			+ Vector2{
				GetComponent<Transform>()->GetSize().x / 2.0f,
				GetComponent<Transform>()->GetSize().y / 2.0f
		};
		//mPos = mSpawn;
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
		if (sState == CosmosSwordState::down)
		{
			bRender = true;
			return;
		}
		else
		{
			bRender = false;
		}
	}
	void CosmosSword::Render(HDC hdc)
	{
		if (bRender == true)
		{
			bRender = false;
			return;
		}

		Weapon::colRender(hdc, posCol, GetBCollision());
		//Weapon::GetComponent<SpriteRenderer>()->rotateRender(hdc, mImage->GetHdc(), Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() }, mSpawnDegree, mSpawn);
		GetComponent<Transform>()->SetPos(mPos);
		GetComponent<Animator>()->Render(hdc);
		bRender = true;
		//GameObject::Render(hdc);
	}

	void CosmosSword::Create()
	{
		//Weapon::SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		SetStat(10.0f, 5.0f, 0.05f, 0.0f);
		mEffects = GetEmanager();
		mEffects = new Emanager();
		mEffects->SetOwner(this);
		//mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\SwingEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.07f);
		mEffects->RegisterEffect(L"SwingEffect", L"..\\Resource\\Char\\CosmosSwingFX.bmp", true, true, 8, Vector2{ 0.0f, 0.0f }, 0.03f, Vector2::One * 4.f);
		mEffects->RegisterEffect(L"SlashEffect", L"..\\Resource\\Char\\CosmosSlashFX.bmp", true, true, 7, Vector2{ 0.0f, 0.0f }, 0.03f, Vector2::One * 2.f);
		//mEffects->RegisterEffect(L"HitEffect", L"..\\Resource\\Char\\HitFX.bmp", false, true, 6, Vector2{ 0.0f, 0.0f }, 0.01f, Vector2::One * 4.f);

		Weapon::SetAsRatio(Vector2::One * 2.f);

		mImage = RscManager::Load<Img>(L"CosmosSword", L"..\\Resource\\Char\\CosmosSword.bmp");
		//mImage = RscManager::Load<Img>(L"CosmosSword", L"..\\Resource\\Char\\BambooCosmosSword.bmp");
		//mImage = RscManager::Load<Img>(L"CosmosSword", L"..\\Resource\\Char\\BambooCosmosSword3.bmp");
		mImage->MatchRatio(Weapon::GetAsRatio());


		mImage->SetMoveRate(Vector2::One * 1.f);
		Vector2 sizeImg = Vector2::Zero;
		sizeImg.x = mImage->GetWidth() / 12.0f;
		sizeImg.y = mImage->GetHeight() / 1.0f;
		mSize = sizeImg.x;

		GetComponent<Transform>()->SetSize(Vector2{ sizeImg.x, sizeImg.y });
		GetComponent<SpriteRenderer>()->SetRotate(true);

		Animator* mAnimator = AddComponent<Animator>();
		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, sizeImg * Vector2{ 0.0f, (float)index++ }, 12, 1, 12, Vector2::Zero, 0.05);
		Play(L"Idle", true);
		// collider 설정
		Collider* collider = AddComponent<Collider>();

		float n = 3.2f / 1.6f;
		collider->SetSize(Vector2::One * Vector2{ (float)(sizeImg.x) / n, (float)(sizeImg.y) / 1.f }.Length() * 2.f);
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

	void CosmosSword::OnCollisionEnter(Collider* other)
	{
		if (GetBAttack() == true)
		{

			Monster* victim = dynamic_cast<Monster*>(other->GetOwner());
			if (victim != NULL)
			{
				if (AttackCheck(other))
				{
					// other에게 알려줘야함
					if(victim->GetBAttack())
						mEffects->CreateEffect(L"SlashEffect", GetDir(),
							other->GetPos()
							+ Vector2{ other->GetSize() / 2.f });
					victim->Attack(this);
					SetBCollision(true);
				}
				return;
			}
			BaseBullet* bullet = dynamic_cast<BaseBullet*>(other->GetOwner());
			if (bullet != NULL)
			{
				if (AttackCheck(other))
				{
					// other에게 알려줘야함
					bullet->Attack();
					SetBCollision(true);
				}
				return;
			}
		}

	}

	void CosmosSword::OnCollisionStay(Collider* other)
	{
		if (GetBAttack() == true)
		{
			Monster* victim = dynamic_cast<Monster*>(other->GetOwner());
			if (victim != NULL)
			{
				if (AttackCheck(other))
				{
					// other에게 알려줘야함
					if (victim->GetBAttack())
						mEffects->CreateEffect(L"SlashEffect", GetDir(),
							other->GetPos()
							+ Vector2{ other->GetSize() / 2.f });
					victim->Attack(this);
					SetBCollision(true);
					//mEffects->CreateEffect(L"HitEffect", GetDir());
				}
				return;
			}
			BaseBullet* bullet = dynamic_cast<BaseBullet*>(other->GetOwner());
			if (bullet != NULL)
			{
				if (AttackCheck(other))
				{
					// other에게 알려줘야함
					bullet->Attack();
					SetBCollision(true);
				}
				return;
			}
		}
	}

	void CosmosSword::OnCollisionExit(Collider* other)
	{
	}
	bool CosmosSword::AttackCheck(class Collider* other)
	{
		// 충돌된 collider 네 점 계산
		Vector2 otherRect[4] =
		{
			other->GetPos(),// + Vector2{other->GetSize().x * (-1.f), other->GetSize().y * (-1.f)},
			other->GetPos() + Vector2{other->GetSize().x, 0.0f},
			other->GetPos() + Vector2{other->GetSize().x, other->GetSize().y},
			other->GetPos() + Vector2{0.0f , other->GetSize().y},
		};

		// CosmosSword collider 네 점 계산
		Collider* collider = GetComponent<Collider>();
		Vector2 colPos = collider->GetPos() + collider->GetSize() / 2.f;
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{
			rect[i] = colPos + math::Rotate(posCol[i], atan2(GetDir().y, GetDir().x) / PI * 180);
		}

		// CosmosSword collider 네 점이 충돌했는지 확인
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

	void CosmosSword::Idle()
	{
		Weapon::Idle();
	}

	void CosmosSword::Attack()
	{
		// 이펙트 생성 코드 필요
		Weapon::Attack();
		mEffects->CreateEffect(L"SwingEffect", GetDir());
		sState = (CosmosSwordState)(((UINT)sState + 1) % (UINT)CosmosSwordState::END);
	}
	void CosmosSword::Reload()
	{
		Weapon::Reload();
		if (GetBCollision())
			SetBCollision(false);
	}

	void CosmosSword::Play(std::wstring anim, bool loop)
	{
		GetComponent<Animator>()->Play(anim, loop);
	}

}