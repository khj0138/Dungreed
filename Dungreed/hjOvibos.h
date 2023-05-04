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
	class IceBullet;
	class Ovibos : public Monster
	{
	public:
		enum class eOvibosState
		{
			Idle,
			AttackWait,
			Attack,
			Die,
			End,
		};
		Ovibos();
		~Ovibos();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eOvibosState state, std::wstring anim, bool loop);
		Vector2 GetHeroPos() { return heroPos; }
		virtual void SetState(GameObject::eState type);
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void AttackWaitCompleteEvent();
		void AttackCompleteEvent();
		void die();

		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;

		Vector2 heroPos;
		eOvibosState mState;
		Wmanager_mon* mWeapons;
		Emanager* mEffects;
		Rigidbody* mRigidbody;

		float coolTime;
		float mTime;
		Vector2 mAttackDir;
		UINT index;
		UINT hp;
	};

}
