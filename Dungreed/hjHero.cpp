#include "hjHero.h"
#include "hjTime.h"
#include "hjSceneManager.h"
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
#include "hjScene.h"
#include "hjLayer.h"
#include "hjTown.h"

extern hj::Application application;
namespace hj
{
	Hero::Hero()
	{
	}

	Hero::~Hero()
	{
		//delete mWeapons;
		//mWeapons = nullptr;
		delete mEffects;
		mEffects = nullptr;
	}

	void Hero::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		isJump = false;
		mDash = 0;
		bDash = true;
		bDjump = true;
		if (bDjump == true)
			cJump = 2;
		else
			cJump = 1;

		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		Img* mImage = RscManager::Load<Img>(L"Hero", L"..\\Resource\\Char\\baseChar.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(1.0f);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 8.0f;
		size.y = mImage->GetHeight() / 8.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		tr->SetPos(Vector2{ 3200.0f, 450.0f });

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

		mFlip = false;
		StateChange(eHeroState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 36.0f, 56.0f });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);

		mWeapons = new Wmanager();
		mWeapons->SetOwner(this);
		SceneManager::FindScene(eSceneType::Play)->AddGameObject(mWeapons, eLayerType::Bullet);
		mWeapons->CreateWeapon(L"Sword", eWeaponType::SWORD);
		mWeapons->EquipWeapon(L"Sword");

		mEffects = new Emanager();
		mEffects->SetOwner(this);

		mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\RunEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.04f, Vector2::One * 1.5f);
		mEffects->RegisterEffect(L"DashEffect", L"..\\Resource\\Char\\DashEffect.bmp", true, false, 1, Vector2{ 0.0f, 0.0f }, 0.08f, Vector2::One * ((float)application.GetWidth() / 960.0f));
		mEffects->RegisterEffect(L"JumpEffect", L"..\\Resource\\Char\\JumpFX.bmp", false, false, 5, Vector2{ (-0.5f) * colSize.x, 0.0f }, 0.03f);
		mEffects->RegisterEffect(L"DJumpEffect", L"..\\Resource\\Char\\DoubleJumpFX.bmp", false, false, 6, Vector2{ (-0.5f) * colSize.x, 0.0f }, 0.03f);

		GameObject::Initialize();

	}

	void Hero::Update()
	{
		prevPos = GetComponent<Transform>()->GetPos();
		if (Input::GetKey(eKeyCode::A))
		{
			if (find(leftRight.begin(), leftRight.end(), eKeyCode::A) == leftRight.end())
				leftRight.push_back(eKeyCode::A);
		}
		else if (Input::GetKeyUp(eKeyCode::A))
		{
			leftRight.erase(find(leftRight.begin(), leftRight.end(), eKeyCode::A));
		}
		if (Input::GetKey(eKeyCode::D))
		{
			if (find(leftRight.begin(), leftRight.end(), eKeyCode::D) == leftRight.end())
				leftRight.push_back(eKeyCode::D);
		}
		else if (Input::GetKeyUp(eKeyCode::D))
		{
			leftRight.erase(find(leftRight.begin(), leftRight.end(), eKeyCode::D));
		}

		if (Input::GetKey(eKeyCode::R))
		{
			GetComponent<Transform>()->SetPos(Vector2{
				GetComponent<Transform>()->GetPos().x,
				200.0f
				});
		}

		if (isJump == false)
		{
			if (!(GetComponent<Rigidbody>()->GetGround()))
				isJump = true;
		}
		else if (!(GetComponent<Rigidbody>()->GetGround()))
		{
			if (mState != eHeroState::Jump)
			{
				StateChange(eHeroState::Jump, L"Jump", true);
			}
		}
		else
		{
			isJump = false;
		}
		switch (mState)
		{

		case hj::Hero::eHeroState::Idle:
			idle();
			break;
		case hj::Hero::eHeroState::Run:
			run();
			break;
		case hj::Hero::eHeroState::Die:
			die();
			break;
		case hj::Hero::eHeroState::Jump:
			jump();
			break;
		}

		Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();

		if (Mouse::GetPos().x < (tr->GetPos().x - (Camera::GetPos().x - application.GetWidth() / 2.0f)))
			mFlip = true;
		else
			mFlip = false;
		//mWeapons->Update();
		mEffects->Update();
		GameObject::Update();
	}

	void Hero::Render(HDC hdc)
	{
		mWeapons->Render(hdc);
		GameObject::Render(hdc);
		//mWeapons->Render(hdc);
		//mEffects->Render(hdc);
	}

	void Hero::Release()
	{
		GameObject::Release();
	}

	void Hero::StateChange(eHeroState state, std::wstring anim, bool loop = false)
	{
		mState = state;
		switch (state)
		{
		case eHeroState::Jump:
		{
			if (bDash)
			{
				if (!isJump)
				{
					mEffects->CreateEffect(L"JumpEffect");
					cJump--;
				}
			}
			break;
		}
		case eHeroState::Run:
		{
			mEffects->CreateEffect(L"RunEffect");
			break;
		}
		}
		if (mFlip)
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

	void Hero::OnCollisionEnter(Collider* other)
	{
		if (Input::GetKeyDown(eKeyCode::S))
		{
			downJump(other);
		}
	}

	void Hero::OnCollisionStay(Collider* other)
	{
		if (Input::GetKeyDown(eKeyCode::S))
		{
			downJump(other);
		}
	}

	void Hero::OnCollisionExit(Collider* other)
	{

	}
	void Hero::idle()
	{
		if ((Mouse::GetRstate() == eKeyState::Down) && bDash)
		{

			dash();
		}
		else if (Input::GetKeyDown(eKeyCode::W))
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = -1200.0f;
			mRigidbody->SetVelocity(velocity);
			mRigidbody->SetGround(false);

			StateChange(eHeroState::Jump, L"Jump", true);

		}
		/*else if (Input::GetKeyDown(eKeyCode::S))
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = 1100.0f;
			mRigidbody->SetVelocity(velocity);
			mRigidbody->SetGround(false);
			StateChange(eHeroState::Jump, L"Jump", true);

		}*/
		else if (!leftRight.empty())
		{
			StateChange(eHeroState::Run, L"Run", true);
		}

		else
		{
			Flip(L"Idle");
		}
	}

	void Hero::run()
	{
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		mEffects->CreateEffect(L"RunEffect");
		if (Mouse::GetRstate() == eKeyState::Down && bDash)
		{
			dash();
		}
		else if (Input::GetKeyDown(eKeyCode::W))
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = -1200.0f;
			mRigidbody->SetVelocity(velocity);
			mRigidbody->SetGround(false);

			StateChange(eHeroState::Jump, L"Jump", true);
		}
		/*else if (Input::GetKeyDown(eKeyCode::S))
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = 1100.0f;
			mRigidbody->SetVelocity(velocity);
			mRigidbody->SetGround(false);
			mRigidbody->SetGravity(false);
			StateChange(eHeroState::Jump, L"Jump", true);

		}*/
		else {
			if (leftRight.empty())
			{
				//mEffects->Clear();
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 0.0f;
				mRigidbody->SetVelocity(velocity);
				StateChange(eHeroState::Idle, L"Idle", true);
			}
			else if (leftRight.back() == eKeyCode::A)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = -300.0f;
				mRigidbody->SetVelocity(velocity);
			}
			else if (leftRight.back() == eKeyCode::D)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 300.0f;
				mRigidbody->SetVelocity(velocity);
			}
			Flip(L"Run");
		}


	}

	void Hero::die()
	{
	}

	void Hero::jump()
	{

		Transform* tr = GetComponent<Transform>();
		Vector2 velocity = mRigidbody->GetVelocity();
		if (Mouse::GetRstate() == eKeyState::Down && bDash)
		{
			dash();
		}

		if (mDash > 0)
		{
			if (((UINT)mDash % 2 == 0) && ((UINT)mDash / 2 > 4))
				mEffects->CreateEffect(L"DashEffect");
			mDash--;
			if (mDash == 0)
			{
				velocity.y = 0.0f;
			}
		}
		else if (
			!(bDash == false && mDash > 0)
			)
		{
			if (cJump == 1)
				int a = 0;
			if (Input::GetKeyDown(eKeyCode::W) && (cJump > 0))
			{
				//if (--cJump)
					//mEffects->CreateEffect(L"JumpEffect");
				if (!(--cJump))
					mEffects->CreateEffect(L"DJumpEffect");
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.y = -1200.0f;
				mRigidbody->SetVelocity(velocity);
				mRigidbody->SetGround(false);
			}
			else if (Input::GetKey(eKeyCode::W) && isJump)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				if (velocity.y < 0.0f && !(mRigidbody->GetGround()))
					velocity.y = velocity.y - 1400.0f * Time::DeltaTime();
				mRigidbody->SetVelocity(velocity);
			}
			mRigidbody->SetGravity(true);
			if (mRigidbody->GetGround())
			{
				if (bDjump)
					cJump = 2;
				else
					cJump = 1;
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.y = 0.0f;
				mRigidbody->SetVelocity(velocity);
				bDash = true;
				StateChange(eHeroState::Run, L"Run", true);
			}
			if (leftRight.empty())
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 0.0f;
				mRigidbody->SetVelocity(velocity);
			}
			else
			{
				if (leftRight.back() == eKeyCode::D)
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = 300.0f;
					mRigidbody->SetVelocity(velocity);
				}
				else if (leftRight.back() == eKeyCode::A)
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = -300.0f;
					mRigidbody->SetVelocity(velocity);
				}
			}
		}
		Flip(L"Jump");
	}

	void Hero::dash()
	{
		bDash = false;
		Transform* tr = GetComponent<Transform>();
		Vector2 dir = (Mouse::GetPos() - Camera::CaluatePos(tr->GetPos(), 1.f));
		float n = 256.f;
		if (dir.Length() > n)
		{
			dir.Normalize();
			dir = dir * n;
		}
		dir = dir / (n / 16.f);
		//Vector2 velocity = mRigidbody->GetVelocity();
		Vector2 velocity = dir;
		mRigidbody->SetVelocity(velocity);
		mDash = 16;
		mRigidbody->SetGravity(false);
		mRigidbody->SetGround(false);
		StateChange(eHeroState::Jump, L"Jump", true);
	}
	void Hero::downJump(Collider* other)
	{
		Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
		if (tile != nullptr)
		{
			if ((tile->Index() == 3 || tile->Index() == 4 || tile->Index() == 5)
				&& (mState != eHeroState::Jump))
			{
				/*Vector2 velocity = mRigidbody->GetVelocity();
				velocity.y = 1200.0f;
				mRigidbody->SetVelocity(velocity);*/
				mRigidbody->SetGround(false);
				GetComponent<Transform>()->SetPos(
					Vector2{
						GetComponent<Transform>()->GetPos().x,
						GetComponent<Transform>()->GetPos().y + 1200.0f * 0.02f
					}
				);

				prevPos = GetComponent<Transform>()->GetPos();
				mRigidbody->SetGravity(false);
				StateChange(eHeroState::Jump, L"Jump", true);
				cJump--;

			}
		}
	}

	void Hero::Flip(std::wstring Anim)
	{
		if (mFlip)
		{
			std::wstring flipAnim = L"Flipped";
			flipAnim.append(Anim);
			mAnimator->Flip(flipAnim);
		}
		else
			mAnimator->Flip(Anim);
	}

}