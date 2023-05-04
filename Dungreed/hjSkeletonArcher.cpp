#include "hjSkeletonArcher.h"
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

#include "hjSceneManager.h"
#include "hjScene.h"
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjWeapon.h"
#include "hjWmanager_mon.h"
extern hj::Application application;

namespace hj
{
	SkeletonArcher::SkeletonArcher()
	{
	}
	SkeletonArcher::~SkeletonArcher()
	{
		//delete mWeapons;
		//mWeapons = nullptr;
		//delete mEffects;
		mEffects = nullptr;
	}
	void SkeletonArcher::Initialize()
	{
		if (GetStat().HP == 0)
		{
			mWeapons->GetActiveWeapon()->SetState(eState::Pause);
			SetState(eState::Pause);
		}
		mAnimator = AddComponent<Animator>();
		isJump = false;
		cJump = 1;

		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"SkeletonArcher", L"..\\Resource\\Monster\\skel.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 5.0f;
		size.y = mImage->GetHeight() / 6.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 1, Vector2::Zero, 0.1);

		SetFlip(false);
		StateChange(eSkeletonArcherState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 40.0f, 68.0f });
		Vector2 colSize = collider->GetSize();
		//collider->SetCenter(Vector2{ -1.f * (128.f - colSize.x) / 2.f , -1.f * (128.f - colSize.y) });
		collider->SetCenter(Vector2{ -1.f * (colSize.x) / 2.f , -1.f * (colSize.y) });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);
		mRigidbody->SetGround(true);

		mWeapons = new Wmanager_mon();
		mWeapons->SetOwner(this);
		/*mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY);
		mWeapons->EquipWeapon(L"Empty", 1);*/
		mWeapons->CreateWeapon(L"Bow", eWeaponType::BOW_MON);
		mWeapons->EquipWeapon(L"Bow");

		mWeapons->GetActiveWeapon()->SetStat(5.0f, 5.0f, 2.0f, 1.0f);

		Monster::Initialize();
		SetStat(10, 10);
	}
	void SkeletonArcher::Update()
	{
		if (GetStat().HP == 0)
		{
			mWeapons->GetActiveWeapon()->SetState(eState::Pause);
			mWeapons->GetDashWeapon()->SetState(eState::Pause);
			SetState(eState::Pause);
		}
		SetBAttack(true);
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

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			if (heroPos.x > pos.x)
				SetFlip(false);
			else
				SetFlip(true);
		}
		
		else
		{
			isJump = false;
		}
		switch (mState)
		{

		case hj::SkeletonArcher::eSkeletonArcherState::Idle:
			idle();
			break;
		}

		Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();


		mWeapons->Update();
		//mEffects->Update();
		Monster::Update();
	}
	void SkeletonArcher::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		Monster::Render(hdc);
	}
	void SkeletonArcher::Release()
	{
		GameObject::Release();
	}

	void SkeletonArcher::OnCollisionEnter(Collider* other)
	{
	}

	void SkeletonArcher::OnCollisionStay(Collider* other)
	{
	}

	void SkeletonArcher::OnCollisionExit(Collider* other)
	{

	}

	void SkeletonArcher::StateChange(eSkeletonArcherState state, std::wstring anim, bool loop = false)
	{
		mState = state;
		if (
			(state == eSkeletonArcherState::AttackWait) ||
			(state == eSkeletonArcherState::AttackReload)
			)
			anim = L"Idle";
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
	void SkeletonArcher::SetState(GameObject::eState type)
	{
		mWeapons->SetState(type);
		GameObject::SetState(type);
	}
	void SkeletonArcher::idle()
	{
		
		if (GetHero() != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();

			Flip(L"Idle");
		}
	}
	
	
	void SkeletonArcher::attackWait()
	{
		
	}
	void SkeletonArcher::attackReload()
	{
		
	}
	void SkeletonArcher::Flip(std::wstring Anim)
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
	void SkeletonArcher::downJump(Collider* other)
	{
		
	}
}