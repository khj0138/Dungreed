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
	class Niflheim;
	class MonsterHPBar;
	class SkelIceMagician : public Monster
	{
	public:
		enum class eSkelIceMagicianState
		{
			Idle,
			Attack,
			End,
		};

		SkelIceMagician();
		~SkelIceMagician();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eSkelIceMagicianState state, std::wstring anim, bool loop);
		Vector2 GetHeroPos() { return heroPos; }
		Vector2 GetDir() { return mDir; }
		void SetDir(Vector2 dir) { mDir = dir; }

		void SetBAttack(bool attack) { bAttack = attack; }
		bool GetBAttack() { return bAttack; }

		
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void attack();

		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;

		Vector2 heroPos;
		eSkelIceMagicianState mState;
		Rigidbody* mRigidbody;
		float mTime;
		std::vector<IceBullet*> mBullets;

		Vector2 mDir;
		UINT index;
		float coolTime;
		bool bAttack;

	};

}
