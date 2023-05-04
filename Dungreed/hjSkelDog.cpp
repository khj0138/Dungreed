#include "hjSkelDog.h"
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
	SkelDog::SkelDog()
	{
	}
	SkelDog::~SkelDog()
	{
		delete mWeapons;
		mWeapons = nullptr;


	}
	void SkelDog::Initialize()
	{
		coolTime = 1.0f;
		mAnimator = AddComponent<Animator>();
		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 3.f;
		Img* mImage = RscManager::Load<Img>(L"SkelDog", L"..\\Resource\\Monster\\SkelDog.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 7.0f;
		size.y = mImage->GetHeight() / 4.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 7, 4, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 7, 4, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Attack", mImage, size * Vector2{ 0.0f, (float)index++ }, 7, 4, 7, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedAttack", mImage, size * Vector2{ 0.0f, (float)index++ }, 7, 4, 7, Vector2::Zero, 0.1);


		SetFlip(false);
		StateChange(eSkelDogState::Idle, L"Idle", true);

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
		mWeapons->GetDashWeapon()->SetStat(5.0f, 0.0f, 0.6f, 0.6f);

		Monster::Initialize();
		SetStat(10, 10);

		mAnimator->GetCompleteEvent(L"Attack") = std::bind(&SkelDog::AttackCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"FlippedAttack") = std::bind(&SkelDog::AttackCompleteEvent, this);
	}
	void SkelDog::Update()
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

		case SkelDog::eSkelDogState::Idle:
			idle();
			break;
		case SkelDog::eSkelDogState::Die:
			die();
			break;
		}

		/*Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();*/

		mWeapons->Update();
		Monster::Update();
	}
	void SkelDog::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		Monster::Render(hdc);
	}
	void SkelDog::Release()
	{
		GameObject::Release();
	}

	void SkelDog::OnCollisionEnter(Collider* other)
	{
	}

	void SkelDog::OnCollisionStay(Collider* other)
	{
	}

	void SkelDog::OnCollisionExit(Collider* other)
	{

	}

	void SkelDog::StateChange(eSkelDogState state, std::wstring anim, bool loop = false)
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
	void SkelDog::SetState(GameObject::eState type)
	{
		mWeapons->SetState(type);
		GameObject::SetState(type);
	}
	void SkelDog::idle()
	{
		mTime += Time::DeltaTime();

		if (mTime >= coolTime)
		{
			mTime = 0.0f;
			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();

			mWeapons->GetDashWeapon()->SetCAttack(true);
			if (heroPos.x > pos.x)
			{
				SetFlip(false);
				GetComponent<Rigidbody>()->SetVelocity(Vector2{ 400.0f, 0.0f });
			}
			else
			{
				SetFlip(true);
				GetComponent<Rigidbody>()->SetVelocity(Vector2{ -400.0f, 0.0f });
			}
			StateChange(eSkelDogState::Attack, L"Attack", false);
		}
	}


	void SkelDog::Flip(std::wstring Anim)
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

	void SkelDog::AttackCompleteEvent()
	{
		mWeapons->GetDashWeapon()->SetCAttack(false);
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		GetComponent<Rigidbody>()->SetVelocity(Vector2{ 0.0f, 0.0f });

		if (heroPos.x > pos.x)
		{
			SetFlip(false);
		}
		else
		{
			SetFlip(true);
		}
		StateChange(eSkelDogState::Idle, L"Idle", true);
	}
	void SkelDog::die()
	{
	}
}