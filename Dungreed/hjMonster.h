#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjWmanager.h"
#include "hjEmanager.h"
#include "hjInput.h"

namespace hj
{

	class Rigidbody;
	class Animator;
	class Hero;
	class Monster : public GameObject
	{
	public:
		enum class eMonsterState
		{
			Idle,
			Run,
			Die,
			Jump,
			End,
		};
		Monster();
		~Monster();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eMonsterState state, std::wstring anim, bool loop);
		Vector2 GetHeroPos() { return heroPos; }
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void run();
		void die();
		void jump();
		void dash();
		void downJump(Collider* other);

		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;
		Hero* hero;
		Vector2 heroPos;
		eMonsterState mState;
		//bool mFlip;
		bool isJump;
		Wmanager* mWeapons;
		Emanager* mEffects;
		Rigidbody* mRigidbody;
		int cJump;
		bool bDjump;
	};

}
