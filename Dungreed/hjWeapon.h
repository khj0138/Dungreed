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
			ATTACK,
			RELOAD,
			End,
		};
		struct Status {
			float power;
			float dashPower;
			float reload;

			Status()
				: power(0.0f)
				, dashPower(0.0f)
				, reload(0.0f)
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
		void SetState(UINT state) { mWstate = (eWeaponState)state; }
		void SetAsRatio(Vector2 asRatio) { mAsRatio = asRatio; }
		void wCheckCol(Collider* target, Collider* other);
		void colRender(HDC hdc, std::vector<Vector2> posCol, bool bCollision);
		void SetReloadTime(float time) { reloadTime = time; }
		void SetOwner(GameObject* owner) { mOwner = owner; }
		void SetBAttack(bool attack) { bAttack = attack; }

		eWeaponState GetState() { return mWstate; }
		Animator* GetAnimator() { return mAnimator; }
		//Wmanager* GetManager() { return mWmanager; }
		Vector2 GetAsRatio() { return mAsRatio; }
		Emanager* GetEmanager() { return mEffects; }
		bool GetBAttack() { return bAttack; }
		bool GetBCollision() { return bCollision; }
		GameObject* GetOwner() { return mOwner; }
		Vector2 GetDir() { return mDir; }
		Vector2 GetPos() { return mPos; }
		bool GetFlip() { return isFlip; }
		float GetReloadTime() { return reloadTime; }
		

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
		float reloadTime;
		GameObject* mOwner;

		Vector2 mDir;
		Vector2 mPos;
		bool isFlip;
	};

}
