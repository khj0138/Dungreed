#include "hjHero.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTime.h"
#include "hjTransform.h"
#include "hjSpriteRenderer.h"
#include "hjAnimator.h"

namespace hj
{
	Hero::Hero()
	{
		mStates.resize((UINT)eStateType::End);
		mStates[(UINT)eStateType::Idle] = 5;
		mStates[(UINT)eStateType::Run] = 8;
	}

	Hero::~Hero()
	{
		
	}

	void Hero::Initialize()
	{
		GameObject::Initialize();

		Image* mImage = RscManager::Load<Image>(L"Hero", L"..\\Resource\\Char\\baseChar.bmp");

		Animator* animator = AddComponent<Animator>();
		Vector2 leftTopPos = { 0.0f, 0.0f };
		leftTopPos.y = mImage->GetHeight() / 8.0f;

		animator->CreateAnimation(L"Idle", mImage, leftTopPos * Vector2::Zero, 8, 8, 5, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"FlipedIdle", mImage, leftTopPos * Vector2{0.0f, 1.0f}, 8, 8, 5, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"Run", mImage, leftTopPos * Vector2{ 0.0f, 2.0f }, 8, 8, 8, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"FlipedRun", mImage, leftTopPos * Vector2{ 0.0f, 3.0f }, 8, 8, 8, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"Die", mImage, leftTopPos * Vector2{ 0.0f, 4.0f }, 8, 8, 1, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"FlipedDie", mImage, leftTopPos * Vector2{ 0.0f, 5.0f }, 8, 8, 1, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"Jump", mImage, leftTopPos * Vector2{ 0.0f, 6.0f }, 8, 8, 1, Vector2::Zero, 0.1);
		animator->CreateAnimation(L"FlipedJump", mImage, leftTopPos * Vector2{ 0.0f, 7.0f }, 8, 8, 1, Vector2::Zero, 0.1);

		animator->Play(L"Idle", true);

		//SpriteRenderer* spr = GetComponent<SpriteRenderer>();

		//spr->SetSprite(L"Idle", L"..\\Resource\\baseChar.bmp");
		//spr->SetState((UINT)eStateType::Idle);
		//spr->SetIndex(0);
	}

	void Hero::Update()
	{
		GameObject::Update();

		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();

		/*SpriteRenderer* spr = GetComponent<SpriteRenderer>();
		bool flip = spr->checkFlip();
		UINT state = spr->getState();
		UINT index = spr->getIndex();
		UINT stMax = mStates[state];
		*/
		Animator* animator = GetComponent<Animator>();
		if (
			Input::GetKeyState(eKeyCode::A) == eKeyState::None &&
			Input::GetKeyState(eKeyCode::S) == eKeyState::None &&
			Input::GetKeyState(eKeyCode::D) == eKeyState::None &&
			Input::GetKeyState(eKeyCode::W) == eKeyState::None
			)
		{
			animator->Play(L"Idle", true);
			/*spr->SetState((UINT)eStateType::Idle);
			if (state != (UINT)eStateType::Idle)
			{
				StateChange(state, (UINT)eStateType::Idle);
			}
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);*/
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::Pressed)
		{
			tr->SetPos({ (float)(pos.x - 100.0f * Time::Deltatime()) , pos.y });
			animator->Play(L"FlipedRun", true);
			//pos.x -= 100.0f * Time::Deltatime();
			/*if (!flip)
				spr->SetFlip(true);

			if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);*/
		}
		else if (Input::GetKeyState(eKeyCode::D) == eKeyState::Pressed)
		{
			tr->SetPos({ (float)(pos.x + 100.0f * Time::Deltatime()) , pos.y });
			animator->Play(L"Run", true);
			//pos.x += 100.0f * Time::Deltatime();
			/*if (flip)
				spr->SetFlip(false);

			if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);*/
		}
		else if (Input::GetKeyState(eKeyCode::W) == eKeyState::Pressed)
		{
			tr->SetPos({ pos.x  , (float)(pos.y - 100.0f * Time::Deltatime()) });
			animator->Play(L"Jump", true);
	/*		if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);*/
		}
		else if (Input::GetKeyState(eKeyCode::S) == eKeyState::Pressed)
		{
			tr->SetPos({ pos.x , (float)(pos.y + 100.0f * Time::Deltatime()) });
			animator->Play(L"Die", true);
			/*if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);*/
		}
	}

	void Hero::Render(HDC hdc)
	{
		GameObject::Render(hdc);

		//Transform* tr = GetComponent<Transform>();
		//Vector2 pos = tr->GetPos();
		//Vector2 size = tr->GetSize();

		/*SpriteRenderer* spr = GetComponent<SpriteRenderer>();
		Image* img = spr->GetSprite();
		bool flip = spr->checkFlip();

		UINT width = img->GetWidth() / 8;
		UINT height = img->GetHeight() / 4;
		HDC iHdc = img->GetHdc();

		UINT iWidth = img->GetWidth() / 8;
		UINT iHeight = img->GetHeight() / 4;
		UINT iPosX = pos.x;
		UINT iPosY = pos.y;
	
		UINT sprType = spr->getState();
		UINT sprNum = spr->getIndex();*/


		// GdiTransparentBlt(
		// hdc, 복사할 위치(x), 복사할 위치(y), 복사할 가로, 복사할 세로,
		// 복사되는 hdc, 복사되는 x, 복사되는 y, 복사되는 가로, 복사되는 세로, 제외할 색);

		//GdiTransparentBlt(
		//	hdc, pos.x, pos.y, width, height,
		//	iHdc, width * sprNum, height * ((sprType * 2) + flip), width, height, RGB(255, 0, 255));
		////GdiTransparentBlt(
			//hdc, -300,-600, width, height,
			//iHdc, width * sprNum, height * ((sprType * 2) + flip), width, height, RGB(255, 0, 255));
//		GdiTransparentBlt(hdc, -300, -600, 5000, 6000, iHdc, 2500, 0, width, height, (UINT)RGB(255, 0, 255));
		//BitBlt(hdc, pos.x, pos.y, width, height,iHdc, width * sprNum, height * ((sprType * 2) + flip), SRCCOPY);
	}

	void Hero::Release()
	{
		GameObject::Release();
	}

	void Hero::StateChange(UINT prev, UINT cur)
	{
		SpriteRenderer* spr = GetComponent<SpriteRenderer>();

		spr->SetState(cur);
		spr->SetIndex(0);
		Time::SetTime();
	}

}