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
	class IcePillar : public Monster
	{
	public:
		enum class eIcePillarState
		{
			Idle,
			AttackWait,
			Attack,
			Pause,
			End,
		};
		struct command
		{
			float delayTime;
			Vector2 attackDir;
			float attackSpeed;
			UINT attackLimit;
			float zenTime;
		};
		IcePillar();
		~IcePillar();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eIcePillarState state);
		void AnimPlay(std::wstring anim, bool loop = false);
		Vector2 GetHeroPos() { return heroPos; }
		virtual void SetState(GameObject::eState type);
		Vector2 GetDir() { return mDir; }
		void SetDir(Vector2 dir) { mDir = dir; }
		void SetNiflheim(Niflheim* niflheim) { mOwner = niflheim; }
		Niflheim* GetNiflheim() { return mOwner; }
		void SetTPPos(Vector2 pos) { mTempPos = pos; }
		void SetTPDir(Vector2 dir) { mTempDir = dir; }
		Vector2 GetTPPos() { return mTempPos; }
		Vector2 GetTPDir() { return mTempDir; }
		void SetBAttack(bool attack) { bAttack = attack; }
		bool GetBAttack() { return bAttack; }

		void enterCompleteEvent();
		void exitCompleteEvent();

		void SetCommand(float delayTime,
			Vector2 attackDir,
			float attackSpeed,
			UINT attackLimit,
			float zenTime)
		{
			cmd = { delayTime, attackDir, attackSpeed, attackLimit, zenTime };
		}
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void attackWait();
		void attack();
	
		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;

		Vector2 heroPos;
		Wmanager_mon* mWeapons;
		Emanager* mEffects;
		eIcePillarState mState;
		Rigidbody* mRigidbody;
		float mTime;
		std::vector<IceBullet*> mBullets;

		Niflheim* mOwner;

		Vector2 mDir;
		Vector2 mTempPos;
		Vector2 mTempDir;
		command cmd;
		UINT index;
		bool bAttack;
		MonsterHPBar* hpBar;
	};

}
