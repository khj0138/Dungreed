#include "hjDashWeapon_mon.h"
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
#include "hjHero.h"

extern hj::Application application;
//extern GraphicsPath Path;
//extern const GUID ImageFormatBMP;
namespace hj
{

	DashWeapon_mon::DashWeapon_mon()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(DashWeapon_monState::up)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
		, bCollision(false)
	{
	}
	DashWeapon_mon::~DashWeapon_mon()
	{
	}
	void DashWeapon_mon::Initialize()
	{
	}
	void DashWeapon_mon::Update()
	{
		//Weapon::Update();
		if (GetBAttack() == false)
		{
			SetBCollision(false);
		}
		Transform* MonsterTr = GetOwner()->GetComponent<Transform>();

		GetComponent<Transform>()->SetPos(MonsterTr->GetPos());
		GetComponent<Collider>()->Update();

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
	void DashWeapon_mon::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void DashWeapon_mon::Create()
	{
		Collider* collider = AddComponent<Collider>();
		collider->SetSize(GetOwner()->GetComponent<Collider>()->GetSize());
		collider->SetCenter(
			Vector2{
				GetComponent<Collider>()->GetSize().x / -2.f
			, GetComponent<Collider>()->GetSize().y * -1.f }
		);
	}

	void DashWeapon_mon::OnCollisionEnter(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			if (Weapon::GetBAttack() == true)
			{
				hero->Attack(this);
				SetBCollision(true);
				SetCAttack(false);
			}
		}
		//if (hero != nullptr)
		//{
		//	// 공격 준비상태가 돼야함
		//	if(GetBAttack)
		//}

	}

	void DashWeapon_mon::OnCollisionStay(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != NULL)
		{
			if (Weapon::GetBAttack() == true)
			{
				hero->Attack(this);
				SetBCollision(true);
				SetCAttack(false);
			}
		}
	}

	void DashWeapon_mon::OnCollisionExit(Collider* other)
	{

	}

	void DashWeapon_mon::Idle()
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

	void DashWeapon_mon::Attack()
	{
		// 이펙트 생성 코드 필요
		Weapon::Attack();
		//mEffects->CreateEffect(L"SwingEffect", GetDir());

	}
	void DashWeapon_mon::Reload()
	{
		Weapon::Reload();
		if (GetBCollision())
			SetBCollision(false);

	}

}