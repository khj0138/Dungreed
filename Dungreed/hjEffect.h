#pragma once
//#include "hjEntity.h"

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

		Effect(Vector2 pos);
		~Effect();

		void Initialize();
		void Update();
		void Render(HDC hdc);
		//void Create(const std::wstring& name, const std::wstring& path, bool loop, bool bDir, UINT frame, Vector2 offset, float playRate = 1.f);
		void Create(Img* image, std::wstring name, UINT frame, UINT leftTopIdx, Vector2 offset, float playRate, bool cFlip = true);
		void Register(const std::wstring& name, const std::wstring& path, UINT frame, Vector2 offset, float playRate = 0.1f);
		void Idle();
		void Attack();
		void Reload();

		//virtual void Reset();

		//bool isComplete() { return mbComplete; }
		void SetEmanager(Emanager* manager) { mEmanager = manager; }
		//void SetDir(Vector2 direction) { mDir = direction; }
		void SetAsRatio(Vector2 asRatio) { mAsRatio = asRatio; }
		//void SetOffset(Vector2 offset) { mOffset = offset; }
		void Play();
		void effectCompleteEvent();


		//Vector2 GetDir() { return mDir; }
		Animator* GetAnimator() { return mAnimator; }
		Emanager* GetManager() { return mEmanager; }
		Vector2 GetAsRatio() { return mAsRatio; }

	private:
		Emanager* mEmanager;
		std::wstring mEffectName;
		Animator* mAnimator;
		std::wstring mName;
		//Collider* mCollider;
		// Transform* mTransform;
		Img* mImage;
		double mTime;
		//bool bRotate;
		//Vector2 mDir;
		Vector2 mAsRatio;
		Vector2 mOffset;
	};

}
