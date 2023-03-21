#include "hjHero.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjCamera.h"
#include "hjMouse.h"
#include "hjApplication.h"
#include "hjRigidBody.h"

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
	}
	
	void Hero::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		isJump = false;
		mDash = 0;
		bDash = true;
		Vector2 asRatio = Vector2::One * ( (float)application.GetWidth()/ 960.0f);
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
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{0.0f, (float)index++}, 8, 8, 5, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Run", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedRun", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Die", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedDie", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Jump", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedJump", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 8, 1, Vector2::Zero, 0.1);

		mState = eHeroState::Idle;
		mAnimator->Play(L"Idle", true);
		flip = false;
		
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
		mEffects->CreateEffect(L"RunEffect", L"..\\Resource\\Char\\RunEffect.bmp", true, false, 5);

		GameObject::Initialize();

	}

	void Hero::Update()
	{
		

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

		switch(mState)
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
			flip = true;
		else
			flip = false;
		//mWeapons->Update();
		GameObject::Update();
	}

	void Hero::Render(HDC hdc)
	{
		mWeapons->Render(hdc);
		GameObject::Render(hdc);
		//mWeapons->Render(hdc);
		mEffects->Render(hdc);
	}

	void Hero::Release()
	{
		GameObject::Release();
	}

	void Hero::StateChange(eHeroState state, std::wstring anim, bool loop = false)
	{
		mState = state;
		mAnimator->Play(anim, loop);
		mAnimator->Reset();
	}

	void Hero::OnCollisionEnter(Collider* other)
	{
		int a = 0;
	}

	void Hero::OnCollisionStay(Collider* other)
	{

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
			velocity.y -= 2000.0f;
			mRigidbody->SetVelocity(velocity);
			mRigidbody->SetGround(false);
			if (flip)
				StateChange(eHeroState::Jump, L"FlippedJump", true);
			else
				StateChange(eHeroState::Jump, L"Jump", true);
		}
		else if (!leftRight.empty())
		{
			if (flip)
				StateChange(eHeroState::Run, L"FlippedRun", true);
			else
				StateChange(eHeroState::Run, L"Run", true);
		}

		else
		{
			if (flip)
				mAnimator->Flip(L"FlippedIdle");
			else
				mAnimator->Flip(L"Idle");
		}
	}

	void Hero::run()
	{
		

		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		if (Mouse::GetRstate() == eKeyState::Down && bDash)
		{
			dash();
		}
		else if (Input::GetKeyDown(eKeyCode::W))
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y -= 2000.0f;
			mRigidbody->SetVelocity(velocity);
			mRigidbody->SetGround(false);
			if (flip)
				StateChange(eHeroState::Jump, L"FlippedJump", true);
			else
				StateChange(eHeroState::Jump, L"Jump", true);
		}
		else {
			if (leftRight.empty())
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 0.0f;
				mRigidbody->SetVelocity(velocity);
				if (flip)
					StateChange(eHeroState::Idle, L"FlippedIdle", true);
				else
					StateChange(eHeroState::Idle, L"Idle", true);
			}
			else if (leftRight.back() == eKeyCode::A)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = -200.0f;
				mRigidbody->SetVelocity(velocity);
				//Vector2 velocity = tr->GetVelocity();

				//pos.x -= tr->GetVelocity().x * Time::DeltaTime();
			}
			else if (leftRight.back() == eKeyCode::D)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 200.0f;
				mRigidbody->SetVelocity(velocity);
				/*Vector2 velocity = tr->GetVelocity();
				pos.x += tr->GetVelocity().x * Time::DeltaTime();*/
			}
		

		if (flip)
			mAnimator->Flip(L"FlippedRun");
		else
			mAnimator->Flip(L"Run");
		}

		//else if (Input::GetKeyDown(eKeyCode::A))
		//{
		//	//pos.x -= 1.0f * Time::DeltaTime();
		//	mAnimator->Flip(L"FlipedRun");
		//}
		//else if (Input::GetKeyDown(eKeyCode::D))
		//{
		//	//pos.x -= 1.0f * Time::DeltaTime();
		//	mAnimator->Flip(L"Run");
		//}

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
			mDash--;
			if (mDash == 0)
			{
				velocity.y = 0.0f;
			}
		}
		
		else
		{
			mRigidbody->SetGravity(true);
			//if (velocity.y >= 500.0f)
			if (mRigidbody->GetGround())
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.y = 0.0f;
				mRigidbody->SetVelocity(velocity);
				//mRigidbody->SetGround(true);
				bDash = true;
				if (flip)
					StateChange(eHeroState::Idle, L"FlippedIdle", true);
				else
					StateChange(eHeroState::Idle, L"Idle", true);
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
					velocity.x = 200.0f;
					mRigidbody->SetVelocity(velocity);
					//Vector2 velocity = tr->GetVelocity();
					//pos.x += tr->GetVelocity().x * Time::DeltaTime();
					//tr->SetPos(pos);
				}
				else if (leftRight.back() == eKeyCode::A)
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = -200.0f;
					mRigidbody->SetVelocity(velocity);
					//Vector2 velocity = tr->GetVelocity();
					//pos.x -= tr->GetVelocity().x * Time::DeltaTime();
					//tr->SetPos(pos);
				}
			}
		}
		if (flip)
			mAnimator->Flip(L"FlippedJump");
		else
			mAnimator->Flip(L"Jump");

	}

	void Hero::dash()
	{
		bDash = false;
		Transform* tr = GetComponent<Transform>();
		Vector2 dir = (Mouse::GetPos() - Camera::CaluatePos(tr->GetPos(), 1.f)).Normalize();
		dir = dir * 16.0f;
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity = dir;
		mRigidbody->SetVelocity(velocity);
		mDash = 16;
		mRigidbody->SetGravity(false);
		mRigidbody->SetGround(false);
		if (flip)
			StateChange(eHeroState::Jump, L"FlippedJump", true);
		else
			StateChange(eHeroState::Jump, L"Jump", true);
	}

}