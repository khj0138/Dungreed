#include "hjTime.h"
#include "hjApplication.h"
#include "hjMouse.h"
#include "hjCamera.h"

extern hj::Application application;

namespace hj
{
	double Time::mDeltaTime = 0.0l;
	double Time::mSecond = 0.0f;
	double Time::temp = 0.0f;
	LARGE_INTEGER Time::mCpuFrequency = {};
	LARGE_INTEGER Time::mPrevFrequency = {};
	LARGE_INTEGER Time::mCurFrequency = {};

	void Time::Initialize()
	{
		// CPU 고유 진동수 할당
		QueryPerformanceFrequency(&mCpuFrequency);

		// 프로그램이 처음 시작할 때, 진동수
		QueryPerformanceCounter(&mPrevFrequency);
	}

	void Time::Update()
	{
		QueryPerformanceCounter(&mCurFrequency);

		double differenceFrequency = mCurFrequency.QuadPart - mPrevFrequency.QuadPart;

		mDeltaTime = differenceFrequency / mCpuFrequency.QuadPart;
		//if (mDeltaTime >= (1.0f / 60.0f))
		while (mDeltaTime <= (1.0f / 60.0f))
		{
			QueryPerformanceCounter(&mCurFrequency);
			differenceFrequency = mCurFrequency.QuadPart - mPrevFrequency.QuadPart;
			mDeltaTime = differenceFrequency / mCpuFrequency.QuadPart;
		}
		
		mDeltaTime = 1.0f / 60.0f;

		mPrevFrequency.QuadPart = mCurFrequency.QuadPart;
	}

	void Time::Render(HDC hdc)
	{
		mSecond += mDeltaTime;
		temp += mDeltaTime;
		if (mSecond > 1.0f)
		{
			HWND hWnd = application.GetHwnd();

			/*wchar_t szFloat[50] = {};
			float FPS = 1.0f / (float)mDeltaTime;
			swprintf_s(szFloat, 50, L"FPS : %d", (UINT)FPS);*/

			wchar_t szFloat2[100] = {};
			float FPS = 1.0f / (float)mDeltaTime;
			Vector2 pos = Mouse::GetPos(); 
			Vector2 realPos = Camera::CaluatePos(pos, Vector2::One * -1.0f);
			swprintf_s(
				szFloat2, 100,
				L"FPS : %d  REALPOS : {%d, %d}  POS : {%d, %d}",
				(UINT)FPS, (UINT)realPos.x, (UINT)realPos.y, (UINT)pos.x, (UINT)pos.y
			);
			//SetWindowText(hWnd, szFloat);
			SetWindowText(hWnd, szFloat2);

			mSecond = 0.0f;
		}
	}

	UINT Time::GetTime()
	{
		if (temp > 0.15)
		{ 
			temp = 0.0f;
			return 1;
		}
		return 0;
	}
	void Time::SetTime()
	{
		temp = 0;
	}

}