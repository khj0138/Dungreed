#include "hjMonsterP.h"
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
	MonsterP::MonsterP()
	{
	}
	MonsterP::~MonsterP()
	{
		//delete mEffects;
		mEffects = nullptr;
	}
	void MonsterP::Initialize()
	{





		mAnimator = AddComponent<Animator>();
		isJump = false;
		cJump = 1;

		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"MonsterP", L"..\\Resource\\Char\\baseChar.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 8.0f;
		size.y = mImage->GetHeight() / 8.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Run", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedRun", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Die", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedDie", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Jump", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedJump", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);

		SetFlip(false);
		StateChange(eMonsterPState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 36.0f, 56.0f });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);

		mWeapons = new Wmanager_mon();
		mWeapons->SetOwner(this);
		/*mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY);
		mWeapons->EquipWeapon(L"Empty", 1);*/
		mWeapons->CreateWeapon(L"Sword", eWeaponType::SWORD_MON);
		mWeapons->EquipWeapon(L"Sword", 1);
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
	void MonsterP::Update()
	{
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
			if (mState != eMonsterPState::Jump)
			{
				StateChange(eMonsterPState::Jump, L"Jump", true);
			}
		}
		else
		{
			isJump = false;
		}
		switch (mState)
		{

		case hj::MonsterP::eMonsterPState::Idle:
			idle();
			break;
		case hj::MonsterP::eMonsterPState::Run:
			run();
			break;
		case hj::MonsterP::eMonsterPState::Jump:
			jump();
			break;
		}

		Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();


		mWeapons->Update();
		//mEffects->Update();
		GameObject::Update();
	}
	void MonsterP::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
	void MonsterP::Release()
	{
		GameObject::Release();
	}

	void MonsterP::OnCollisionEnter(Collider* other)
	{
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();
		if ((GetHeroPos().y - pos.y) >= (size.y * 2))
		{
			downJump(other);
		}
	}

	void MonsterP::OnCollisionStay(Collider* other)
	{
	}

	void MonsterP::OnCollisionExit(Collider* other)
	{

	}

	void MonsterP::StateChange(eMonsterPState state, std::wstring anim, bool loop = false)
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
	void MonsterP::idle()
	{
		if (hero != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();

			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
				StateChange(eMonsterPState::Run, L"Run", true);

			else
			{
				Flip(L"Idle");
			}

		}
	}
	void MonsterP::run()
	{
		if (hero != nullptr)
		{
			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();


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

				StateChange(eMonsterPState::Jump, L"Jump", true);
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
				StateChange(eMonsterPState::Idle, L"Idle", true);
			}

		}
	}
	void MonsterP::jump()
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
			StateChange(eMonsterPState::Run, L"Run", true);
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
	void MonsterP::Flip(std::wstring Anim)
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
	void MonsterP::downJump(Collider* other)
	{
		Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
		if (tile != nullptr)
		{
			if ((tile->Index() == 3 || tile->Index() == 4 || tile->Index() == 5)
				&& (mState != eMonsterPState::Jump))
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
				StateChange(eMonsterPState::Jump, L"Jump", true);
				cJump--;

			}
		}
	}
}