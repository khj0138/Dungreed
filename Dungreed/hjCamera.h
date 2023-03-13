#pragma once
#include "hjhjEngine.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	class GameObject;
	class Camera
	{
	public:
		static void Initiailize();
		static void Update();
		static void Clear();
		static Vector2 GetPos() { return mLookPosition; }
		static Vector2 GetLookRange() { return mLookRange; }

		static void SetTarget(GameObject* target) { mTarget = target; }
		static void SetLookRange(Vector2 range) { mLookRange = range; }
		static Vector2 CaluatePos(Vector2 pos, float playRate)
		{
			//if (playRate == 0.0f)
				//return pos - Vector2{ (float)application.GetWidth()/2.0f, (float)application.GetHeight() / 2.0f };
			return pos - mDistance * playRate; 
		}
		static void SetPos(Vector2 pos) { mLookPosition = pos; }


	private:
		static Vector2 mResolution;
		static Vector2 mLookPosition;
		static Vector2 mDistance;
		static GameObject* mTarget;
		static Vector2 mLookRange;
	};
}
