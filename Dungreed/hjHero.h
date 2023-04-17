#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjWmanager.h"
#include "hjEmanager.h"

#include "hjInput.h"
//#include "hjMath.h"


namespace hj
{
	class Rigidbody;
	class Animator;
	class Hero : public GameObject
	{
	public:
		struct status
		{
			UINT maxHP;
			UINT HP;
		};
		enum class eHeroState
		{
			Idle,
			Run,
			Die,
			Jump,
			End,
		};
		Hero();
		~Hero();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(class Collider* other) override;
		virtual void OnCollisionStay(class Collider* other) override;
		virtual void OnCollisionExit(class Collider* other) override;
		void StateChange(eHeroState state, std::wstring anim, bool loop);
		//Vector2 prevPos;

	private:

		void idle();
		void run();
		void die();
		void jump();
		void dash();
		void downJump(Collider* other);

		void Flip(std::wstring name);
		//bool checkFlip() { return mFlip; }


	private:
		eHeroState mState;
		Animator* mAnimator;
		//bool mFlip;
		bool isJump;
		Wmanager* mWeapons;
		Emanager* mEffects;
		std::vector<eKeyCode> leftRight;
		Rigidbody* mRigidbody;
		UINT mDash;
		bool bDash;
		int cJump;
		bool bDjump;
		status stat;
	};

}