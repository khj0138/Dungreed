#include "hjHero.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjAnimator.h"
#include "hjCollider.h"
//#include "hjSpriteRenderer.h"

namespace hj
{
	Hero::Hero()
	{
	}

	Hero::~Hero()
	{
			
	}

	void Hero::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		isJump = false;

		Image* mImage = RscManager::Load<Image>(L"Hero", L"..\\Resource\\Char\\baseChar.bmp");
		Transform* tr = GetComponent<Transform>();
		
		//tr->SetPos(Vector2{ 800.0f, 450.0f });
		tr->SetPos(Vector2{ 100.0f, 500.0f });
		Vector2 pos = tr->GetPos();

		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 8.0f;
		size.y = mImage->GetHeight() / 8.0f;
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 200.0f, 0.0f });

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

		GameObject::Initialize();
	}

	void Hero::Update()
	{
		GameObject::Update();
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		pos.y -= tr->GetVelocity().y * Time::DeltaTime();
		tr->SetPos(pos);
		POINT cursor;

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

		// **********************마우스 쓰는 방법*******************
		Vector2 size = tr->GetSize();
		GetCursorPos(&cursor);
		HWND hWnd = GetForegroundWindow();
		POINT point = { 0,0 };
		if (ClientToScreen(hWnd, &point))
		{
			if ((cursor.x - point.x ) < tr->GetPos().x)
				flip = true;
			else
				flip = false;
		}
		// **********************마우스 쓰는 방법*******************


	}
	//animator->Play(L"Die", true);
	//animator->Play(L"Idle", true);
	//animator->Play(L"FlipedRun", true);

	//animator->Play(L"Run", true);

	//animator->Play(L"Jump", true);

	void Hero::Render(HDC hdc)
	{
		GameObject::Render(hdc);

	}

	void Hero::Release()
	{
		GameObject::Release();
	}

	void Hero::idle()
	{
		if (Input::GetKeyDown(eKeyCode::W))
		{
			mState = eHeroState::Jump;
			mAnimator->Play(L"Jump", true);
			mAnimator->Reset();
		}
		else {
		if (Input::GetKey(eKeyCode::D))
		{
			mState = eHeroState::Run;
			mAnimator->Play(L"Run", true);
			mAnimator->Reset();
		}
		else if (Input::GetKey(eKeyCode::A))
		{
			mState = eHeroState::Run;
			mAnimator->Play(L"FlippedRun", true);
			mAnimator->Reset();
		}
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
		
		if (Input::GetKeyDown(eKeyCode::W))
		{
			mState = eHeroState::Jump;
			mAnimator->Play(L"Jump", true);
			mAnimator->Reset();
		}
		else {
			if (!Input::GetKeyDown(eKeyCode::A)
				&& !Input::GetKeyDown(eKeyCode::D)
				&& !Input::GetKey(eKeyCode::A)
				&& !Input::GetKey(eKeyCode::D))
			{
				mState = eHeroState::Idle;
				mAnimator->Play(L"Idle", true);
				mAnimator->Reset();
			}
			else if (Input::GetKey(eKeyCode::A))
			{
				//pos.x -= 100.0f * Time::DeltaTime();
				Vector2 velocity = tr->GetVelocity();
				pos.x -= tr->GetVelocity().x * Time::DeltaTime();
			}
			else if (Input::GetKey(eKeyCode::D))
			{
				//pos.x -= 100.0f * Time::DeltaTime();
				Vector2 velocity = tr->GetVelocity();
				pos.x += tr->GetVelocity().x * Time::DeltaTime();
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

		tr->SetPos(pos);
	}

	void Hero::die()
	{
	}

	void Hero::jump()
	{

		Transform* tr = GetComponent<Transform>();
		Vector2 velocity = tr->GetVelocity();
		Vector2 pos = tr->GetPos();
		float speed = 600.0f;
		if (isJump == false && (velocity.y <= 0.001 && velocity.y >= -0.001))
		{
			isJump = true;
			tr->SetVelocity(Vector2{ velocity.x,   speed });
		}

		else if (isJump == true && velocity.y <= (-1.f) * speed)
		{ 
			isJump = false;
			tr->SetVelocity(Vector2{ velocity.x, (float)0});
		//	tr->SetPos(Vector2{ pos.x, 449.9f });
			mState = eHeroState::Idle;
			mAnimator->Play(L"Idle", true);
		}
		else
		{
			float n = 0.65f;
			tr->SetVelocity(Vector2{ velocity.x, velocity.y - (2.0f * speed / n) * (float)Time::DeltaTime() });
			if (flip)
				mAnimator->Flip(L"FlippedJump");
			else
				mAnimator->Flip(L"Jump");

		}
		if (Input::GetKey(eKeyCode::D))
		{
			Vector2 velocity = tr->GetVelocity();
			pos.x += tr->GetVelocity().x * Time::DeltaTime();
			tr->SetPos(pos);
		}
		else if (Input::GetKey(eKeyCode::A))
		{
			Vector2 velocity = tr->GetVelocity();
			pos.x -= tr->GetVelocity().x * Time::DeltaTime();
			tr->SetPos(pos);
		}

		

	}

}