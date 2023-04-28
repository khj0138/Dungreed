#pragma once
#include "hjGameObject.h"
#include "hjImage.h"

#include "hjEmanager.h"
#include "hjInput.h"

namespace hj
{
	class Wmanager_mon;
	class BaseBullet;
	class Rigidbody;
	class Animator;
	class Hero;
	class Weapon;
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
		struct status
		{
			UINT HP;
			UINT maxHP;
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
		Hero* GetHero() { return mHero; }

		void SetBAttack(bool attack) { bAttack = attack; }
		void Attack(Weapon* attacker);
		void Attack(BaseBullet* attacker);
		void Damage(float damage)
		{
			if (stat.HP < (UINT)damage)
				stat.HP = 0.0f;
			else
				stat.HP -= (UINT)damage;
		}
		void SetStat(UINT hp, UINT maxHp)
		{
			stat.HP = hp;
			stat.maxHP = maxHp;
		}
		status GetStat() { return stat; }
		virtual void SetState(GameObject::eState type);
		void SetHero(Hero* hero) {
			mHero = hero;
		}
		
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
		Hero* mHero;
		Vector2 heroPos;
		eMonsterState mState;
		//bool mFlip;
		bool isJump;
		Wmanager_mon* mWeapons;
		Emanager* mEffects;
		Rigidbody* mRigidbody;
		int cJump;
		bool bDjump;

		status stat;
		bool bAttack;
	};

}
