#pragma once
#include "hjEntity.h"

#include "hjGameObject.h"

namespace hj
{
	class Img;
	class Emanager;
	class Animator;
	//class GameObject;
	class Effect : public GameObject
	{
	public:
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
		Effect();
		~Effect();

		void Initialize();
		void Update();
		void Render(HDC hdc);
		void Create(const std::wstring& name, const std::wstring& path, bool loop, bool bDir, UINT frame);
		void Idle();
		void Attack();
		void Reload();

		//virtual void Reset();

		//bool isComplete() { return mbComplete; }
		void SetEmanager(Emanager* manager) { mEmanager = manager; }
		void SetDir(Vector2 direction) { mDir = direction; }
		void SetAsRatio(Vector2 asRatio) { mAsRatio = asRatio; }

		Vector2 GetDir() { return mDir; }
		Animator* GetAnimator() { return mAnimator; }
		Emanager* GetManager() { return mEmanager; }
		Vector2 GetAsRatio() { return mAsRatio; }

	private:
		Emanager* mEmanager;
		std::wstring mEffectName;
		Animator* mAnimator;
		//Collider* mCollider;
		// Transform* mTransform;
		Img* mImage;
		double mTime;
		bool mBdir;
		Vector2 mDir;
		Vector2 mAsRatio;
	};

}
