#include "hjEmpty.h"
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

	Empty::Empty()
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
	Empty::~Empty()
	{
	}
	void Empty::Initialize()
	{
	}
	void Empty::Update()
	{
		Weapon::Update();
		Collider* tr = GetOwner()->GetComponent<Collider>();
		Vector2 ownerSize = tr->GetSize();

		mWstate = GetState();
		Vector2 pos = GetPos();
		isFlip = GetFlip();

		float flipNum = 1.0f - 2.0f * (float)(isFlip);

		Collider* collider = GetComponent<Collider>();
		collider->SetPos(pos
			- Vector2{collider->GetSize().x * isFlip * 1.f, collider->GetSize().y * 1.f}
			+ Vector2{ ownerSize.x / 2.f * (flipNum) ,0.0f }
		);
		GetComponent<Transform>()->SetPos(collider->GetPos());
		//mEffects->Update();

		
		mWstate = Weapon::GetState();
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
	void Empty::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void Empty::Create()
	{
		// collider 설정
		
		Collider* collider = AddComponent<Collider>();
		collider->SetSize(GetOwner()->GetComponent<Transform>()->GetSize());
		//collider->SetCenter(Vector2{ collider->GetSize().x / -2.f, collider->GetSize().y });
		
		Weapon::SetReloadTime(0.3f);
		Weapon::AddComponent<SpriteRenderer>();

	}

	void Empty::OnCollisionEnter(Collider* other)
	{

		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != nullptr)
		{
			// 공격 준비상태가 돼야함
			SetBAttack(true);
		}
		
	}

	void Empty::OnCollisionStay(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != nullptr)
		{
			// 공격 준비상태가 돼야함
			SetBAttack(true);
		}
	}

	void Empty::OnCollisionExit(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero != nullptr)
		{
			// 공격 준비상태가 돼야함
			mTime = 0.0f;
			SetBAttack(false);
			SetState(eWeaponState::IDLE);
		}
	}

	void Empty::Idle()
	{
		Weapon::Idle();
		if (GetBAttack())
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
		}
	}

	void Empty::Attack()
	{
		// 이펙트 생성 코드 필요
		Weapon::Attack();
		//mEffects->CreateEffect(L"SwingEffect", GetDir());
		SetState(eWeaponState::RELOAD);
		bCollision = true;
		sState = (EmptyState)(((UINT)sState + 1) % (UINT)EmptyState::END);
	}
	void Empty::Reload()
	{
		Weapon::Reload();
		mTime += Time::DeltaTime();
		if (mTime > GetReloadTime())
		{
			bCollision = false;
			mTime = 0.0f;
			Weapon::SetState((UINT)eWeaponState::IDLE);
		}
	}

}