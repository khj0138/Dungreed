#include "hjOvibos.h"
#include "hjTime.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjCamera.h"
#include "hjMouse.h"
#include "hjApplication.h"
#include "hjRigidBody.h"
#include "hjEffect.h"
#include "hjTile.h"
#include "hjMath.h"
#include "hjLayer.h"
#include "hjTown.h"
#include "hjTile.h"
#include "hjWmanager_mon.h"

#include "hjSceneManager.h"
#include "hjPSceneManager.h"
#include "hjScene.h"
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjWeapon.h"
#include "hjIceBullet.h"
extern hj::Application application;

namespace hj
{
	Ovibos::Ovibos()
	{
	}
	Ovibos::~Ovibos()
	{
		delete mWeapons;
		mWeapons = nullptr;

		
	}
	void Ovibos::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 3.f;
		Img* mImage = RscManager::Load<Img>(L"Ovibos", L"..\\Resource\\Monster\\Ovibos.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 9.0f;
		size.y = mImage->GetHeight() / 6.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 9, 6, 9, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 9, 6, 9, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"AttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 9, 6, 6, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"FlippedAttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 9, 6, 6, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"Attack", mImage, size * Vector2{ 0.0f, (float)index++ }, 9, 6, 6, Vector2::Zero, 0.15);
		mAnimator->CreateAnimation(L"FlippedAttack", mImage, size * Vector2{ 0.0f, (float)index++ }, 9, 6, 6, Vector2::Zero, 0.15);
	

		SetFlip(false);
		StateChange(eOvibosState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(size);
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetGround(false);

		mWeapons = new Wmanager_mon();
		mWeapons->SetOwner(this);
		mWeapons->CreateWeapon(L"DashWeapon1", eWeaponType::DASHWEAPON_MON);
		mWeapons->EquipWeapon(L"DashWeapon1");
		mWeapons->GetDashWeapon()->SetStat(10.0f, 0.0f, 0.6f, 0.6f);

		Monster::Initialize();
		SetStat(50, 50);
		hp = 50;

		mAnimator->GetCompleteEvent(L"AttackWait") = std::bind(&Ovibos::AttackWaitCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"FlippedAttackWait") = std::bind(&Ovibos::AttackWaitCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"Attack") = std::bind(&Ovibos::AttackCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"FlippedAttack") = std::bind(&Ovibos::AttackCompleteEvent, this);
	}
	void Ovibos::Update()
	{
		if (GetStat().HP == 0)
		{
			mWeapons->GetActiveWeapon()->SetState(eState::Pause);
			mWeapons->GetDashWeapon()->SetState(eState::Pause);
			SetState(eState::Pause);
		}
		Monster::SetBAttack(true);

		if (GetHero() == nullptr)
		{
			Scene* b = SceneManager::FindScene(eSceneType::Play);
			PlayScene* c = dynamic_cast<PlayScene*>(b);
			PlayScene* a = dynamic_cast<PlayScene*>(SceneManager::FindScene(eSceneType::Play));
			if (a == nullptr)
				return;
			SetHero(a->GetHero());
			if (GetHero() == nullptr)
				return;
		}
		prevPos = GetComponent<Transform>()->GetPos();



		if (Input::GetKey(eKeyCode::R))
		{
			GetComponent<Transform>()->SetPos(Vector2{
				GetComponent<Transform>()->GetPos().x,
				200.0f
				});
		}

		if (GetHero() != nullptr)
		{
			heroPos = GetHero()->GetComponent<Transform>()->GetPos();
		}

		switch (mState)
		{

		case Ovibos::eOvibosState::Idle:
			idle();
			break;
		case Ovibos::eOvibosState::Die:
			die();
			break;
		}

		/*Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();*/

		mWeapons->Update();
		Monster::Update();
	}
	void Ovibos::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		Monster::Render(hdc);
	}
	void Ovibos::Release()
	{
		GameObject::Release();
	}

	void Ovibos::OnCollisionEnter(Collider* other)
	{
	}

	void Ovibos::OnCollisionStay(Collider* other)
	{
	}

	void Ovibos::OnCollisionExit(Collider* other)
	{

	}

	void Ovibos::StateChange(eOvibosState state, std::wstring anim, bool loop = false)
	{
		mState = state;

		if (GetFlip())
		{
			std::wstring flipAnim = L"Flipped";
			flipAnim.append(anim);
			mAnimator->Play(flipAnim, loop);
		}
		else
		{
			mAnimator->Play(anim, loop);
		}
		mAnimator->Reset();
	}
	void Ovibos::SetState(GameObject::eState type)
	{
		mWeapons->SetState(type);
		GameObject::SetState(type);
	}
	void Ovibos::idle()
	{
		if (hp > Monster::GetStat().HP)
		{
			hp = Monster::GetStat().HP;
			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			if (heroPos.x > pos.x)
				SetFlip(false);
			else
				SetFlip(true);
			StateChange(eOvibosState::AttackWait, L"AttackWait", false);
		}
	}
	
	
	void Ovibos::Flip(std::wstring Anim)
	{
		if (GetFlip())
		{
			std::wstring flipAnim = L"Flipped";
			flipAnim.append(Anim);
			mAnimator->Flip(flipAnim);
		}
		else
			mAnimator->Flip(Anim);
	}
	void Ovibos::AttackWaitCompleteEvent()
	{
		mWeapons->GetDashWeapon()->SetCAttack(true);
		

		StateChange(eOvibosState::Attack, L"Attack", false);
		if (GetFlip())
		{
			GetComponent<Rigidbody>()->SetVelocity(Vector2{ -600.0f, 0.0f });
		}
		else
		{
			GetComponent<Rigidbody>()->SetVelocity(Vector2{ 600.0f, 0.0f });
		}
	}
	void Ovibos::AttackCompleteEvent()
	{
		mWeapons->GetDashWeapon()->SetCAttack(false);
		GetComponent<Rigidbody>()->SetVelocity(Vector2{ 0.0f, 0.0f });
		StateChange(eOvibosState::Idle, L"Idle", true);
	}
	void Ovibos::die()
	{
	}
}