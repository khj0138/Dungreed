#include "hjHero.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"


namespace hj
{

	hj::Hero::Hero()
	{
	}

	hj::Hero::~Hero()
	{
		
	}

	void hj::Hero::Initialize()
	{
		
		mImage = RscManager::Load<Image>(L"Idle", L"..\\Resource\\baseCharIdle.bmp");
		flip = false;
		GameObject::Initialize();
	}

	void hj::Hero::Update()
	{
		GameObject::Update();
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::Pressed)
		{
			mPos.x -= 100.0f * Time::Deltatime();
			flip = true;
		}
		if (Input::GetKeyState(eKeyCode::D) == eKeyState::Pressed)
		{
			mPos.x += 100.0f * Time::Deltatime();
			flip = false;
		}
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::Pressed)
		{
			mPos.y -= 100.0f * Time::Deltatime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::Pressed)
		{
			mPos.y += 100.0f * Time::Deltatime();
		}
		if(Time::GetTime())
			index = (index+1) % 5;
	}

	void hj::Hero::Render(HDC hdc)
	{
		GameObject::Render(hdc);
		UINT width = mImage->GetWidth() / 5;
		UINT height = mImage->GetHeight() / 2;
		
		GdiTransparentBlt(hdc, mPos.x, mPos.y, width, height, mImage->GetHdc(), width * index, flip * height, width, height, RGB(255, 0, 255));
		//BitBlt(hdc, mPos.x, mPos.y, mImage->GetWidth(), mImage->GetHeight(), mImage->GetHdc(), 0, 0, SRCCOPY);
	}

	void hj::Hero::Release()
	{
		GameObject::Release();
	}

}