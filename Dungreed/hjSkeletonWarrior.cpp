#include "hjSkeletonWarrior.h"
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
extern hj::Application application;

namespace hj
{
	SkeletonWarrior::SkeletonWarrior()
	{
	}
	SkeletonWarrior::~SkeletonWarrior()
	{
		delete mWeapons;
		mWeapons = nullptr;
		//delete mEffects;
		mEffects = nullptr;
	}
	void SkeletonWarrior::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		isJump = false;
		cJump = 1;

		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"SkeletonWarrior", L"..\\Resource\\Monster\\skel.bmp");
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
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ },5, 6, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Run", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedRun", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Jump", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedJump", mImage, size * Vector2{ 0.0f, (float)index++ }, 5, 6, 1, Vector2::Zero, 0.1);

		SetFlip(false);
		StateChange(eSkeletonWarriorState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 40.0f, 68.0f });
		Vector2 colSize = collider->GetSize();
		//collider->SetCenter(Vector2{ -1.f * (128.f - colSize.x) / 2.f , -1.f * (128.f - colSize.y) });
		collider->SetCenter(Vector2{ -1.f * (colSize.x) / 2.f , -1.f * (colSize.y) });
		
		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);

		mWeapons = new Wmanager_mon();
		mWeapons->SetOwner(this);
		/*mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY);
		mWeapons->EquipWeapon(L"Empty", 1);*/
		mWeapons->CreateWeapon(L"Sword", eWeaponType::SWORD_MON);
		mWeapons->EquipWeapon(L"Sword");

		mWeapons->GetActiveWeapon()->SetStat(2.0f, 0.0f, 0.3f, 0.6f);

		GameObject::Initialize();
	}
	void SkeletonWarrior::Update()
	{
		SetBAttack(true);
		if (hero == nullptr)
		{
			Scene* b = SceneManager::FindScene(eSceneType::Play);
			PlayScene* c = dynamic_cast<PlayScene*>(b);
			PlayScene* a = dynamic_cast<PlayScene*>(SceneManager::FindScene(eSceneType::Play));
			if (a == nullptr)
				return;
			hero = a->GetHero();
			if (hero == nullptr)
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

		if (hero != nullptr)
		{
			heroPos = hero->GetComponent<Transform>()->GetPos();

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			if (heroPos.x > pos.x)
				SetFlip(false);
			else
				SetFlip(true);
		}
		if (isJump == false)
		{
			if (!(GetComponent<Rigidbody>()->GetGround()))
				isJump = true;
		}
		else if (!(GetComponent<Rigidbody>()->GetGround()))
		{
			if (mState != eSkeletonWarriorState::Jump)
			{
				StateChange(eSkeletonWarriorState::Jump, L"Jump", true);
			}
		}
		else
		{
			isJump = false;
		}
		switch (mState)
		{

		case hj::SkeletonWarrior::eSkeletonWarriorState::Idle:
			idle();
			break;
		case hj::SkeletonWarrior::eSkeletonWarriorState::Run:
			run();
			break;
		case hj::SkeletonWarrior::eSkeletonWarriorState::Jump:
			jump();
			break;
		case hj::SkeletonWarrior::eSkeletonWarriorState::AttackWait:
			attackWait();
			break;
		case hj::SkeletonWarrior::eSkeletonWarriorState::AttackReload:
			attackReload();
			break;
		}

		Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();


		mWeapons->Update();
		//mEffects->Update();
		GameObject::Update();
	}
	void SkeletonWarrior::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		GameObject::Render(hdc);
	}
	void SkeletonWarrior::Release()
	{
		GameObject::Release();
	}

	void SkeletonWarrior::OnCollisionEnter(Collider* other)
	{
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();
		if ((GetHeroPos().y - pos.y) >= (size.y * 2))
		{
			downJump(other);
		}
	}

	void SkeletonWarrior::OnCollisionStay(Collider* other)
	{
	}

	void SkeletonWarrior::OnCollisionExit(Collider* other)
	{

	}

	void SkeletonWarrior::StateChange(eSkeletonWarriorState state, std::wstring anim, bool loop = false)
	{
		mState = state;
		if (
			(state == eSkeletonWarriorState::AttackWait) ||
			(state == eSkeletonWarriorState::AttackReload)
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
	void SkeletonWarrior::idle()
	{
		if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eSkeletonWarriorState::AttackWait, L"AttackWait", false);
			return;
		}
		if (hero != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();

			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
				StateChange(eSkeletonWarriorState::Run, L"Run", true);

			else
			{
				Flip(L"Idle");
			}

		}
	}
	void SkeletonWarrior::run()
	{
		if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eSkeletonWarriorState::AttackWait, L"AttackWait", false);
			return;
		}
		if (hero != nullptr)
		{
			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();
			//mEffects->CreateEffect(L"RunEffect");


			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
			{

				if (GetFlip())
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = -300.0f;
					mRigidbody->SetVelocity(velocity);
				}
				else
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = 300.0f;
					mRigidbody->SetVelocity(velocity);
				}
				Flip(L"Run");
			}

			if ((pos.y - GetHeroPos().y) >= (size.y * 2))
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.y = -1200.0f;
				mRigidbody->SetVelocity(velocity);
				mRigidbody->SetGround(false);

				StateChange(eSkeletonWarriorState::Jump, L"Jump", true);
			}
			else if ((pos.y - GetHeroPos().y) <= (size.y))
			{

			}

			if (
				fabs(pos.x - GetHeroPos().x) <= (size.x) &&
				fabs(pos.y - GetHeroPos().y) <= (size.y)
				)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 0.0f;
				mRigidbody->SetVelocity(velocity);
				StateChange(eSkeletonWarriorState::Idle, L"Idle", true);
			}

		}
	}
	void SkeletonWarrior::jump()
	{

		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();
		Vector2 velocity = mRigidbody->GetVelocity();
		mRigidbody->SetGravity(true);
		if (mRigidbody->GetGround())
		{
			cJump = 1;
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = 0.0f;
			mRigidbody->SetVelocity(velocity);
			StateChange(eSkeletonWarriorState::Run, L"Run", true);
		}
		if (fabs(pos.x - GetHeroPos().x) >= (size.x))
		{

			if (GetFlip())
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = -300.0f;
				mRigidbody->SetVelocity(velocity);
			}
			else
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 300.0f;
				mRigidbody->SetVelocity(velocity);
			}
			Flip(L"Jump");
		}
		else
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.x = 0.0f;
			mRigidbody->SetVelocity(velocity);
		}

	}
	void SkeletonWarrior::attackWait()
	{
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity.x = 0.0f;
		mRigidbody->SetVelocity(velocity);
		if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::RELOAD)
		{
			StateChange(eSkeletonWarriorState::AttackReload, L"AttackReload", false);
		}
	}
	void SkeletonWarrior::attackReload()
	{
		if (mWeapons->GetActiveWeapon()->GetState() != Weapon::eWeaponState::RELOAD)
		{
			if (hero != nullptr)
			{

				Transform* tr = GetComponent<Transform>();
				Collider* col = GetComponent<Collider>();
				Vector2 pos = tr->GetPos();
				Vector2 size = Vector2::Zero;
				size.x = col->GetSize().x * 1.5f;
				size.y = col->GetSize().y;

				if (fabs(pos.x - GetHeroPos().x) >= (size.x))
					StateChange(eSkeletonWarriorState::Run, L"Run", true);

				else
				{
					StateChange(eSkeletonWarriorState::Idle, L"Idle", true);
				}

			}
		}
	}
	void SkeletonWarrior::Flip(std::wstring Anim)
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
	void SkeletonWarrior::downJump(Collider* other)
	{
		Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
		if (tile != nullptr)
		{
			if ((tile->Index() == 3 || tile->Index() == 4 || tile->Index() == 5)
				&& (mState != eSkeletonWarriorState::Jump))
			{
				mRigidbody->SetGround(false);
				GetComponent<Transform>()->SetPos(
					Vector2{
						GetComponent<Transform>()->GetPos().x,
						GetComponent<Transform>()->GetPos().y + 1200.0f * 0.02f
					}
				);

				prevPos = GetComponent<Transform>()->GetPos();
				mRigidbody->SetGravity(false);
				StateChange(eSkeletonWarriorState::Jump, L"Jump", true);
				cJump--;

			}
		}
	}
}