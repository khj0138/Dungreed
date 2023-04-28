#include "hjEmpty_mon.h"
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

extern hj::Application application;
//extern GraphicsPath Path;
//extern const GUID ImageFormatBMP;
namespace hj
{

	Empty_mon::Empty_mon()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(EmptyState::up)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
		, bCollision(false)
	{
	}
	Empty_mon::~Empty_mon()
	{
	}
	void Empty_mon::Initialize()
	{
	}
	void Empty_mon::Update()
	{
		Weapon::Update();
		Collider* tr = GetOwner()->GetComponent<Collider>();
		Vector2 ownerSize = tr->GetSize();

		mWstate = GetWState();
		Vector2 pos = GetPos();
		isFlip = GetFlip();

		float flipNum = 1.0f - 2.0f * (float)(isFlip);

		Collider* collider = GetComponent<Collider>();
		collider->SetPos(pos
			- Vector2{ collider->GetSize().x * isFlip * 1.f, collider->GetSize().y * 1.f }
			+ Vector2{ ownerSize.x / 2.f * (flipNum) ,0.0f }
		);
		GetComponent<Transform>()->SetPos(collider->GetPos());
		//mEffects->Update();


		mWstate = Weapon::GetWState();
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
	void Empty_mon::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void Empty_mon::Create()
	{
		// collider 설정

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2::One * GetOwner()->GetComponent<Collider>()->GetSize().y);
		//collider->SetCenter(Vector2{ collider->GetSize().x / -2.f, collider->GetSize().y });

		Weapon::SetReloadTime(0.3f);

	}

	void Empty_mon::OnCollisionEnter(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			SetCAttack(true);
			if (GetBAttack() == true)
			{
				hero->Attack(this);
				SetBAttack(false);
				SetBCollision(true);
			}
		}
		//if (hero != nullptr)
		//{
		//	// 공격 준비상태가 돼야함
		//	if(GetBAttack)
		//}

	}

	void Empty_mon::OnCollisionStay(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			SetCAttack(true);
			if (GetBAttack() == true)
			{
				hero->Attack(this);
				SetBAttack(false);
				SetBCollision(true);
			}
		}
	}

	void Empty_mon::OnCollisionExit(Collider* other)
	{
		//Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		//if (hero != nullptr)
		//{
		//	// 공격 준비상태가 돼야함
		//	mTime = 0.0f;
		//	SetBAttack(false);
		//	SetState(eWeaponState::IDLE);
		//}
		Hero* victim2 = dynamic_cast<Hero*>(other->GetOwner());
		if (victim2 != NULL)
		{
			SetCAttack(false);
		}
	}

	void Empty_mon::Idle()
	{
		Weapon::Idle();
		/*if (GetBAttack())
		{
			if (mTime >= 1.0f)
			{
				mTime = 0.0f;
				SetState(eWeaponState::ATTACK);
			}
			else
			{
				mTime += Time::DeltaTime();
			}
		}*/
	}

	void Empty_mon::Attack()
	{
		// 이펙트 생성 코드 필요
		Weapon::Attack();
		//mEffects->CreateEffect(L"SwingEffect", GetDir());

	}
	void Empty_mon::Reload()
	{
		Weapon::Reload();
		if (GetBCollision())
			SetBCollision(false);

	}

}