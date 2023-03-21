#pragma once
//#include "hjEntity.h"
#include "hjMath.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjTransform.h"
#include "hjTime.h"
#include "hjGameObject.h"


namespace hj
{
	class Image;
	class Wmanager;
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
		void SetWmanager(Wmanager* manager) { mWmanager = manager; }
		void SetDir(Vector2 direction) { mDir = direction; }
		void SetState(UINT state) { mWstate = (eWeaponState)state; }
		void SetAsRatio(Vector2 asRatio) { mAsRatio = asRatio; }

		eWeaponState GetState() { return mWstate; }
		Vector2 GetDir() { return mDir; }
		Animator* GetAnimator() { return mAnimator; }
		Wmanager* GetManager() { return mWmanager; }
		Vector2 GetAsRatio() { return mAsRatio; }

	private:
		Wmanager* mWmanager;
		std::wstring mWeaponName;
		Animator* mAnimator;
		Collider* mCollider;
		Transform* mTransform;
		Vector2 mDir;
		eWeaponState mWstate;
		double mTime;
		Vector2 mAsRatio;
	};

}
