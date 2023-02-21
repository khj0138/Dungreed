#pragma once
#include "CommonInclude.h"

namespace hj
{

	class Time
	{
	public:
		static void Initialize();
		static void Update();
		static void Render(HDC hdc);

		__forceinline static double Deltatime() { return mDeltaTime; }


	private:
		static double mDeltaTime;
		static double mSecond;
		static LARGE_INTEGER mCpuFrequency;
		static LARGE_INTEGER mPrevFrequency;
		static LARGE_INTEGER mCurFrequency;

	};

}
