#include "hjDashWeapon.h"
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

	DashWeapon::DashWeapon()
		: mRadius(20.f)
		, mWstate(eWeaponState::IDLE)
		, sState(DashWeaponState::up)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
		, bCollision(false)
	{
	}
	DashWeapon::~DashWeapon()
	{
	}
	void DashWeapon::Initialize()
	{
	}
	void DashWeapon::Update()
	{
		//Weapon::Update();
		if (GetBAttack() == false)
		{
			SetBCollision(false);
		}
		Transform* HeroTr = GetOwner()->GetComponent<Transform>();

		GetComponent<Transform>()->SetPos(HeroTr->GetPos());
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
	void DashWeapon::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void DashWeapon::Create()
	{
		Collider* collider = AddComponent<Collider>();
		collider->SetSize(GetOwner()->GetComponent<Collider>()->GetSize());
		collider->SetCenter(
			Vector2{
				GetComponent<Collider>()->GetSize().x / -2.f
			, GetComponent<Collider>()->GetSize().y * -1.f }
		);
	}

	void DashWeapon::OnCollisionEnter(Collider* other)
	{
		Monster* mon = dynamic_cast<Monster*>(other->GetOwner());
		if (mon != NULL)
		{
			if (GetBAttack() == true)
			{
				mon->Attack(this);
				SetBCollision(true);
			}
		}
		//if (hero != nullptr)
		//{
		//	// 공격 준비상태가 돼야함
		//	if(GetBAttack)
		//}

	}

	void DashWeapon::OnCollisionStay(Collider* other)
	{
		/*Monster* mon = dynamic_cast<Monster*>(other->GetOwner());
		if (mon != NULL)
		{
			if (GetBAttack() == true)
			{
				mon->Attack(this);
				SetBCollision(true);
			}
		}*/
	}

	void DashWeapon::OnCollisionExit(Collider* other)
	{

	}

	void DashWeapon::Idle()
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

	void DashWeapon::Attack()
	{
		// 이펙트 생성 코드 필요
		Weapon::Attack();
		//mEffects->CreateEffect(L"SwingEffect", GetDir());

	}
	void DashWeapon::Reload()
	{
		Weapon::Reload();
		if (GetBCollision())
			SetBCollision(false);

	}

}