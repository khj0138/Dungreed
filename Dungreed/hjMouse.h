#pragma once
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjCamera.h"
namespace hj
{

	class Mouse : GameObject
	{
	public:
		static void Initialize();
		static void Update();
		static void Render(HDC hdc);
		static void Release();
		
		static struct Vector2 GetPos() { return mPos; }
		static eKeyState GetLstate() { return mLstate; }
		static eKeyState GetRstate() { return mRstate; }
	private:
		static Vector2 mPos;
		static eKeyState mLstate;
		static eKeyState mRstate;
	};
}

