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
		static Vector2 CaluatePos(Vector2 pos, Vector2 moveRate = Vector2::One)
		{
			return pos - mDistance * moveRate; 
		}
		static void SetPos(Vector2 pos) { mLookPosition = pos; }
		static void SetBVelocity(bool vel) { bVelocity = vel; }


	private:
		static Vector2 mResolution;
		static Vector2 mLookPosition;
		static Vector2 mDistance;
		static GameObject* mTarget;
		static Vector2 mLookRange;
		static bool bVelocity;
	};
}
