#pragma once
#include "hjEntity.h"
#include "hjMath.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjTransform.h"
#include "hjTime.h"

namespace hj
{
	class Image;
	class Wmanager;
	class Weapon : public Entity 
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
			mAnimator = new Animator();
		}
		~Weapon()
		{
			delete mAnimator;
			mAnimator = nullptr;
		}

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Create();
		virtual void Idle();
		virtual void Attack();
		virtual void Reload();
		
		//virtual void Reset();

		//bool isComplete() { return mbComplete; }
		void SetWmanager(Wmanager* manager) { mWmanager = manager; }
		void SetDir(Vector2 direction) { mDir = direction; }
		void SetState(UINT state) { mWstate = (eWeaponState)state; }

		eWeaponState GetState() { return mWstate; }
		Vector2 GetDir() { return mDir; }
		Animator* GetAnimator() { return mAnimator; }
		Wmanager* GetManager() { return mWmanager; }
		//int getIndex() { return mSpriteIndex; }
		//void setIndex(int spriteIndex) { mSpriteIndex = spriteIndex; }
		//void setLeftTop(Vector2 pos) { mSpriteSheet[0].leftTop = pos; }
		//void SetAnimationName(const std::wstring& name) { mAnimationName = name; }
		//std::wstring& GetWeaponName() { return mAnimationName; }

	private:
		Wmanager* mWmanager;
		std::wstring mWeaponName;
		Animator* mAnimator;
		Collider* mCollider;
		Transform* mTransform;
		Vector2 mDir;
		eWeaponState mWstate;
		double mTime;
	};

}
