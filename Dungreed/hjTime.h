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
		static UINT GetTime();
		static void SetTime();

		__forceinline static double DeltaTime() { return mDeltaTime; }


	private:
		static double mDeltaTime;
		static double mSecond;
		static double temp;
		static LARGE_INTEGER mCpuFrequency;
		static LARGE_INTEGER mPrevFrequency;
		static LARGE_INTEGER mCurFrequency;

	};

}
