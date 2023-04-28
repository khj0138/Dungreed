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
	class SkeletonArcher : public Monster
	{
	public:
		enum class eSkeletonArcherState
		{
			Idle,
			Die,
			Jump,
			AttackWait,
			AttackReload,
			End,
		};
		SkeletonArcher();
		~SkeletonArcher();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eSkeletonArcherState state, std::wstring anim, bool loop);
		Vector2 GetHeroPos() { return heroPos; }
		virtual void SetState(GameObject::eState type);
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void run();
		void die();
		void jump();
		void dash();
		void attackWait();
		void attackReload();
		void downJump(Collider* other);

		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;
		Vector2 heroPos;
		eSkeletonArcherState mState;
		//bool mFlip;
		bool isJump;
		Wmanager_mon* mWeapons;
		Emanager* mEffects;
		Rigidbody* mRigidbody;
		int cJump;
		bool bDjump;
	};

}
