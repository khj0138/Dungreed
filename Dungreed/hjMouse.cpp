#include "hjMouse.h"
#include "hjApplication.h"
#include "hjCamera.h"

extern hj::Application application;

namespace hj
{
	Vector2 Mouse::mPos = Vector2::Zero;
	eKeyState Mouse::mLstate = eKeyState::None;
	eKeyState Mouse::mRstate = eKeyState::None;
	void Mouse::Initialize()
	{
	}
	void Mouse::Update()
	{
		HWND hFocusedHwnd = GetFocus();
		if (hFocusedHwnd != application.GetHwnd())
		{
			return;
		}

		mLstate = (eKeyState)Input::GetKeyState(eKeyCode::MLEFT);
		mRstate = (eKeyState)Input::GetKeyState(eKeyCode::MRIGHT);

		// **********************마우스 쓰는 방법*******************
		HWND hWnd = application.GetHwnd();
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(hWnd, &cursor);
		mPos = Vector2{ (float)(cursor.x), (float)(cursor.y) };
		//mPos = Camera::CaluatePos(mPos, -1.f);

		
		// **********************마우스 쓰는 방법*******************
	}
	void Mouse::Render(HDC hdc)
	{

	}
	void Mouse::Release()
	{
	}
}