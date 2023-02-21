#include "hjHero.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"


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
		GameObject::Initialize();
	}

	void hj::Hero::Update()
	{
		GameObject::Update();
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::Pressed)
		{
			mPos.x -= 100.0f * Time::Deltatime();
		}
		if (Input::GetKeyState(eKeyCode::D) == eKeyState::Pressed)
		{
			mPos.x += 100.0f * Time::Deltatime();
		}
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::Pressed)
		{
			mPos.y -= 100.0f * Time::Deltatime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::Pressed)
		{
			mPos.y += 100.0f * Time::Deltatime();
		}
	}

	void hj::Hero::Render(HDC hdc)
	{
		GameObject::Render(hdc);
		// stock 오브젝트
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

		HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
		HPEN oldPen = (HPEN)SelectObject(hdc, pen);

		Rectangle(hdc, mPos.x, mPos.y, mPos.x + 100, mPos.y + 100);

		SelectObject(hdc, oldPen);
		DeleteObject(pen);
		SelectObject(hdc, oldBrush);
		DeleteObject(brush);
	}

	void hj::Hero::Release()
	{
		GameObject::Release();
	}

}