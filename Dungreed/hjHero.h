#pragma once
#include "hjGameObject.h"
#include "hjImage.h"

namespace hj
{
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
		//void StateChange(UINT prev, UINT cur);

		void idle();
		void run();
		void die();
		void jump();


	private:
		eHeroState mState;
		Animator* mAnimator;
		bool flip;
		bool isJump;
	};

}