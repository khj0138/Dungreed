#include "hjHero.h"
#include "hjSceneManager.h"
#include "hjInput.h"
//#include "hjRscManager.h"
#include "hjTime.h"
#include "hjTransform.h"
#include "hjSpriteRenderer.h"

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

		SpriteRenderer* spr = GetComponent<SpriteRenderer>();

		spr->SetSprite(L"Idle", L"..\\Resource\\baseChar.bmp");
		spr->SetState((UINT)eStateType::Idle);
		spr->SetIndex(0);
	}

	void Hero::Update()
	{
		GameObject::Update();

		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();

		SpriteRenderer* spr = GetComponent<SpriteRenderer>();
		bool flip = spr->checkFlip();
		UINT state = spr->getState();
		UINT index = spr->getIndex();
		UINT stMax = mStates[state];
		if (
			Input::GetKeyState(eKeyCode::A) == eKeyState::None &&
			Input::GetKeyState(eKeyCode::S) == eKeyState::None &&
			Input::GetKeyState(eKeyCode::D) == eKeyState::None &&
			Input::GetKeyState(eKeyCode::W) == eKeyState::None
			)
		{
			spr->SetState((UINT)eStateType::Idle);
			if (state != (UINT)eStateType::Idle)
			{
				StateChange(state, (UINT)eStateType::Idle);
			}
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::Pressed)
		{
			tr->SetPos({ (float)(pos.x - 100.0f * Time::Deltatime()) , pos.y });
			//pos.x -= 100.0f * Time::Deltatime();
			if (!flip)
				spr->SetFlip(true);

			if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);
		}
		else if (Input::GetKeyState(eKeyCode::D) == eKeyState::Pressed)
		{
			tr->SetPos({ (float)(pos.x + 100.0f * Time::Deltatime()) , pos.y });
			//pos.x += 100.0f * Time::Deltatime();
			if (flip)
				spr->SetFlip(false);

			if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);
		}
		else if (Input::GetKeyState(eKeyCode::W) == eKeyState::Pressed)
		{
			tr->SetPos({ pos.x  , (float)(pos.y - 100.0f * Time::Deltatime()) });

			if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);
		}
		else if (Input::GetKeyState(eKeyCode::S) == eKeyState::Pressed)
		{
			tr->SetPos({ pos.x , (float)(pos.y + 100.0f * Time::Deltatime()) });

			if (state == (UINT)eStateType::Idle)
				StateChange(state, (UINT)eStateType::Run);
			else if (Time::GetTime())
				spr->SetIndex((index + 1) % stMax);
		}
	}

	void Hero::Render(HDC hdc)
	{
		GameObject::Render(hdc);

		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();

		SpriteRenderer* spr = GetComponent<SpriteRenderer>();
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
		UINT sprNum = spr->getIndex();


		// GdiTransparentBlt(
		// hdc, 복사할 위치(x), 복사할 위치(y), 복사할 가로, 복사할 세로,
		// 복사되는 hdc, 복사되는 x, 복사되는 y, 복사되는 가로, 복사되는 세로, 제외할 색);

		GdiTransparentBlt(
			hdc, pos.x, pos.y, width, height,
			iHdc, width * sprNum, height * ((sprType * 2) + flip), width, height, RGB(255, 0, 255));

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