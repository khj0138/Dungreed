#pragma once
//#include "hjGameObject.h"
//#include "hjImage.h"
//#include "hjWmanager_mon.h"
//#include "hjEmanager.h"
//#include "hjInput.h"
#include "hjMonster.h"

namespace hj
{

	class Rigidbody;
	class Animator;
	class Hero;
	class EliteSkelWarrior : public Monster
	{
	public:
		enum class eEliteSkelWarriorState
		{
			Idle,
			Run,
			Die,
			Jump,
			AttackWait,
			AttackReload,
			End,
		};
		EliteSkelWarrior();
		~EliteSkelWarrior();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;
		
		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eEliteSkelWarriorState state, std::wstring anim, bool loop);
		Vector2 GetHeroPos() { return heroPos; }
		virtual void SetState(GameObject::eState type);
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void run();
		void die();
		void attackWait();
		void attackReload();
		void jump();
		void dash();
		void downJump(Collider* other);

		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;
		
		Vector2 heroPos;
		eEliteSkelWarriorState mState;
		//bool mFlip;
		bool isJump;
		Wmanager_mon* mWeapons;
		Emanager* mEffects;
		Rigidbody* mRigidbody;
		int cJump;
		bool bDjump;
	};

}
