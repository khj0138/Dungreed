#pragma once
//#include "hjEntity.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjTransform.h"
#include "hjTime.h"
#include "hjGameObject.h"
#include "hjSpriteRenderer.h"


namespace hj
{
	class Image;
	class Wmanager;
	class Emanager;
	class Weapon : public GameObject
	{
	public:

		enum eWeaponState {
			IDLE,
			WAIT,
			ATTACK,
			RELOAD,
			End,
		};
		struct Status {
			float power;
			float dashPower;
			float reload;
			float wait;

			Status()
				: power(0.0f)
				, dashPower(0.0f)
				, reload(0.0f)
				, wait(0.0f)
			{
			}
		};
		Weapon()
		{
			//mAnimator = new Animator();
		}
		~Weapon()
		{
			//delete mAnimator;
			//mAnimator = nullptr;
		}

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Create();
		virtual void Idle();
		virtual void Attack();
		virtual void Reload();

		virtual void OnCollisionEnter(class Collider* other) override;
		virtual void OnCollisionStay(class Collider* other) override;
		virtual void OnCollisionExit(class Collider* other) override;

		//virtual void Reset();

		//bool isComplete() { return mbComplete; }
		//void SetWmanager(Wmanager* manager) { mWmanager = manager; }
		void SetDir(Vector2 direction) { mDir = direction; }
		void SetState(eWeaponState state) { mWstate = (eWeaponState)state; }
		void SetAsRatio(Vector2 asRatio) { mAsRatio = asRatio; }
		void wCheckCol(Collider* target, Collider* other);
		void colRender(HDC hdc, std::vector<Vector2> posCol, bool bCollision);
		void SetOwner(GameObject* owner) { mOwner = owner; }
		void SetBAttack(bool attack) { bAttack = attack; }
		void SetBCollision(bool collision) { bCollision = collision; }
		void SetCAttack(bool attack) { cAttack = attack; }
		void SetDamage(UINT damage) { mDamage = damage; }

		eWeaponState GetState() { return mWstate; }
		Animator* GetAnimator() { return mAnimator; }
		//Wmanager* GetManager() { return mWmanager; }
		Vector2 GetAsRatio() { return mAsRatio; }
		Emanager* GetEmanager() { return mEffects; }
		bool GetBAttack() { return bAttack; }
		bool GetCAttack() { return cAttack; }
		bool GetBCollision() { return bCollision; }
		GameObject* GetOwner() { return mOwner; }
		Vector2 GetDir() { return mDir; }
		Vector2 GetPos() { return mPos; }
		bool GetFlip() { return isFlip; }
		float GetHoldTime() { return mTime; }
		


		void SetReloadTime(float time) { stat.reload = time; }
		void SetWaitTime(float time) { stat.wait = time; }
		float GetReloadTime() { return stat.reload; }
		float GetWaitTime() { return stat.wait; }
		float GetTime() { return mTime; }
		float GetDamage() { return mDamage; }

		void SetStat(float power, float dashPower, float reload, float wait)
		{
			stat.power = power; 
			stat.dashPower = dashPower; 
			stat.reload = reload; 
			stat.wait = wait;
		}
		Status GetStat() { return stat; }
		

	private:
		//Wmanager* mWmanager;
		Emanager* mEffects;
		std::wstring mWeaponName;
		Animator* mAnimator;
		Collider* mCollider;
		Transform* mTransform;
		eWeaponState mWstate;
		double mTime;
		Vector2 mAsRatio;
		bool bAttack;
		bool bCollision;
		bool cAttack;
		UINT mDamage;

		//float reloadTime;
		//float waitTime;

		GameObject* mOwner;

		Vector2 mDir;
		Vector2 mPos;
		bool isFlip;

		Status stat;
	};

}
