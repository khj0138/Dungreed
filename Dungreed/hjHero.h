#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjWmanager.h"


namespace hj
{
	class Rigidbody;
	class Animator;
	class Hero : public GameObject
	{
	public:
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

	private:
		void StateChange(eHeroState state, std::wstring anim, bool loop);
		
		void idle();
		void run();
		void die();
		void jump();
		void dash();

		bool checkFlip() { return flip; }


	private:
		eHeroState mState;
		Animator* mAnimator;
		bool flip;
		bool isJump;
		Wmanager* mWeapons;
		std::vector<eKeyCode> leftRight;
		Rigidbody* mRigidbody;
		UINT mDash;
		bool bDash;
	};

}