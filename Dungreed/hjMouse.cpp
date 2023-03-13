#include "hjMouse.h"
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
		mLstate = (eKeyState)Input::GetKeyState(eKeyCode::MLEFT);
		mRstate = (eKeyState)Input::GetKeyState(eKeyCode::MRIGHT);

		// **********************마우스 쓰는 방법*******************
		POINT cursor;
		GetCursorPos(&cursor);
		HWND hWnd = GetForegroundWindow();
		POINT point = { 0,0 };
		ClientToScreen(hWnd, &point);

		mPos = Vector2{ (float)(cursor.x - point.x), (float)(cursor.y - point.y) };
		
		// **********************마우스 쓰는 방법*******************
	}
	void Mouse::Render(HDC hdc)
	{

	}
	void Mouse::Release()
	{
	}
}