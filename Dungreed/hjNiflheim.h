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
	class IcePillar;
	class BossHPBar;
	class Niflheim : public Monster
	{
	public:
		enum class eNiflheimState
		{
			Idle,
			Die,
			AttackWait1,
			AttackWait2,
			AttackWait3,
			AttackWait4,
			Attack1,
			Attack2,
			Attack3,
			Attack4,
			AttackReload,
			Groggy,
			StandUp,
			End,
		};
		
		Niflheim();
		~Niflheim();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(Collider* other) override;
		virtual void OnCollisionStay(Collider* other) override;
		virtual void OnCollisionExit(Collider* other) override;
		void StateChange(eNiflheimState state);
		void AnimPlay(std::wstring anim, bool loop = false);
		void ExitCompleteEvent();
		void EnterCompleteEvent();
		Vector2 GetHeroPos() { return heroPos; }
		virtual void SetState(GameObject::eState type);

		eNiflheimState GetNFState() { return mState; }
		//bool GetFlip() { return mFlip; }

		//Vector2 prevPos;

	private:

		void idle();
		void die();
		void moveAttackWait(UINT index);
		void teleportAttackWait(UINT index);
		void attackWait1();
		void attackWait2();
		void attackWait3();
		void attack1();
		void attack2();
		void attack3();
		void attack4();
		void attackReload();
		void groggy();
		

		void pillarMove(UINT posIndex, UINT dirIndex);
		void pillarTeleport(UINT posIndex, UINT dirIndex);
		void teleport(Vector2 pos);

		void Flip(std::wstring name);
		//.bool checkFlip() { return mFlip; }


	private:
		Animator* mAnimator;

		Vector2 heroPos;
		eNiflheimState mState;

		Wmanager_mon* mWeapons;
		Emanager* mEffects;
		Rigidbody* mRigidbody;
		
		float coolTime[5];
		bool bAttack[5];
		float mTime[6];
		Vector2 mAttackDir;
		UINT index;
		std::vector<IcePillar*> mPillars;
		std::vector<std::vector<Vector2>> pillarVector;
		std::vector<std::vector<Vector2>> pillarDir;
		UINT Degree;
		BossHPBar* hpBar;
	};

}
