#include "hjEliteSkelWarrior.h"
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
	EliteSkelWarrior::EliteSkelWarrior()
	{
	}
	EliteSkelWarrior::~EliteSkelWarrior()
	{
		//delete mEffects;
		mEffects = nullptr;
	}
	void EliteSkelWarrior::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		isJump = false;
		cJump = 1;

		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"EliteSkelWarrior", L"..\\Resource\\Monster\\bigWhiteSkel.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 6.0f;
		size.y = mImage->GetHeight() / 10.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Run", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedRun", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Jump", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedJump", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"AttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedAttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"AttackReload", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedAttackReload", mImage, size * Vector2{ 0.0f, (float)index++ }, 6, 10, 6, Vector2::Zero, 0.1);

		SetFlip(false);
		StateChange(eEliteSkelWarriorState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 80.0f, 120.0f });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);

		mWeapons = new Wmanager_mon();
		mWeapons->SetOwner(this);
		/*mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY);
		mWeapons->EquipWeapon(L"Empty", 1);*/
		mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY);
		mWeapons->EquipWeapon(L"Empty", 1);
		mWeapons->GetActiveWeapon()->SetStat(5.0f, 0.0f, 0.6f, 0.6f);
		/*mWeapons->GetActiveWeapon()->SetWaitTime(0.6f);
		mWeapons->GetActiveWeapon()->SetReloadTime(0.6f);*/
		//SceneManager::FindScene(eSceneType::Play)->AddGameObject(mWeapons, eLayerType::Bullet);
		//mWeapons->CreateWeapon(L"Sword", eWeaponType::SWORD);
		//mWeapons->EquipWeapon(L"Sword");

	/*	mEffects = new Emanager();
		mEffects->SetOwner(this);

		mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\RunEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.04f, Vector2::One * 1.5f);
		mEffects->RegisterEffect(L"DashEffect", L"..\\Resource\\Char\\DashEffect.bmp", true, false, 1, Vector2{ 0.0f, 0.0f }, 0.08f, Vector2::One * ((float)application.GetWidth() / 960.0f));
		mEffects->RegisterEffect(L"JumpEffect", L"..\\Resource\\Char\\JumpFX.bmp", false, false, 5, Vector2{ (-0.5f) * colSize.x, 0.0f }, 0.03f);
		mEffects->RegisterEffect(L"DJumpEffect", L"..\\Resource\\Char\\DoubleJumpFX.bmp", false, false, 6, Vector2{ (-0.5f) * colSize.x, 0.0f }, 0.03f);*/

		GameObject::Initialize();
	}
	void EliteSkelWarrior::Update()
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
			if (mState != eEliteSkelWarriorState::Jump)
			{
				StateChange(eEliteSkelWarriorState::Jump, L"Jump", true);
			}
		}
		else
		{
			isJump = false;
		}

		switch (mState)
		{

		case hj::EliteSkelWarrior::eEliteSkelWarriorState::Idle:
			idle();
			break;
		case hj::EliteSkelWarrior::eEliteSkelWarriorState::Run:
			run();
			break;
		case hj::EliteSkelWarrior::eEliteSkelWarriorState::Jump:
			jump();
			break;
		case hj::EliteSkelWarrior::eEliteSkelWarriorState::AttackWait:
			attackWait();
			break;
		case hj::EliteSkelWarrior::eEliteSkelWarriorState::AttackReload:
			attackReload();
			break;
		}

		Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();


		mWeapons->Update();
		//mEffects->Update();
		GameObject::Update();
	}
	void EliteSkelWarrior::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		GameObject::Render(hdc);
	}
	void EliteSkelWarrior::Release()
	{
		GameObject::Release();
	}

	void EliteSkelWarrior::OnCollisionEnter(Collider* other)
	{
		Transform* tr = GetComponent<Transform>();
		Collider* col = GetComponent<Collider>();
		Vector2 pos = tr->GetPos();
		Vector2 size = Vector2::Zero;
		size.x = col->GetSize().x * 1.5f;
		size.y = col->GetSize().y;
		if ((GetHeroPos().y - pos.y) >= (size.y * 2))
		{
			downJump(other);
		}
	}

	void EliteSkelWarrior::OnCollisionStay(Collider* other)
	{
	}

	void EliteSkelWarrior::OnCollisionExit(Collider* other)
	{

	}

	void EliteSkelWarrior::StateChange(eEliteSkelWarriorState state, std::wstring anim, bool loop = false)
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
	void EliteSkelWarrior::idle()
	{
		if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eEliteSkelWarriorState::AttackWait, L"AttackWait", false);
			return;
		}
		if (hero != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Collider* col = GetComponent<Collider>();
			Vector2 pos = tr->GetPos();
			Vector2 size = Vector2::Zero;
			size.x = col->GetSize().x * 1.5f;
			size.y = col->GetSize().y;

			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
				StateChange(eEliteSkelWarriorState::Run, L"Run", true);

			else
			{
				Flip(L"Idle");
			}

		}
	}
	void EliteSkelWarrior::run()
	{
		if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eEliteSkelWarriorState::AttackWait, L"AttackWait", false);
			return;
		}
		if (hero != nullptr)
		{
			Transform* tr = GetComponent<Transform>();
			Collider* col = GetComponent<Collider>();
			Vector2 pos = tr->GetPos();
			Vector2 size = Vector2::Zero;
			size.x = col->GetSize().x * 1.5f;
			size.y = col->GetSize().y;
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

				StateChange(eEliteSkelWarriorState::Jump, L"Jump", true);
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
				StateChange(eEliteSkelWarriorState::Idle, L"Idle", true);
			}

		}
	}
	void EliteSkelWarrior::jump()
	{
		/*if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eEliteSkelWarriorState::Attack, L"Attack", true);
		}*/
		Transform* tr = GetComponent<Transform>();
		Collider* col = GetComponent<Collider>();
		Vector2 pos = tr->GetPos();
		Vector2 size = Vector2::Zero;
		size.x = col->GetSize().x * 1.5f;
		size.y = col->GetSize().y;
		Vector2 velocity = mRigidbody->GetVelocity();
		mRigidbody->SetGravity(true);
		if (mRigidbody->GetGround())
		{
			cJump = 1;
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = 0.0f;
			mRigidbody->SetVelocity(velocity);
			StateChange(eEliteSkelWarriorState::Run, L"Run", true);
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
	void EliteSkelWarrior::Flip(std::wstring Anim)
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

	void EliteSkelWarrior::downJump(Collider* other)
	{
		Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
		if (tile != nullptr)
		{
			if ((tile->Index() == 3 || tile->Index() == 4 || tile->Index() == 5)
				&& (mState != eEliteSkelWarriorState::Jump))
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
				StateChange(eEliteSkelWarriorState::Jump, L"Jump", true);
				cJump--;

			}
		}
	}
	void EliteSkelWarrior::attackWait()
	{
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity.x = 0.0f;
		mRigidbody->SetVelocity(velocity);
		if (mWeapons->GetActiveWeapon()->GetState() == Weapon::eWeaponState::RELOAD)
		{
			StateChange(eEliteSkelWarriorState::AttackReload, L"AttackReload", false);
		}
	}
	void EliteSkelWarrior::attackReload()
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
					StateChange(eEliteSkelWarriorState::Run, L"Run", true);

				else
				{
					StateChange(eEliteSkelWarriorState::Idle, L"Idle", true);
				}

			}
		}
	}
}