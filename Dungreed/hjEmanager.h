#pragma once
#include "hjGameObject.h"

namespace hj
{


	class Effect;
	class Emanager : public GameObject
	{


	public:
		Emanager();
		~Emanager();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void SetOwner(GameObject* owner) { mOwner = owner; }
		void CreateEffect(const std::wstring& name, const std::wstring& path,bool loop, bool bDir, UINT frame);
		void PlayEffect(const std::wstring& name);
		void Clear();

		Effect* FindEffect(const std::wstring& name);
		Effect* GetActiveEffect() { return mActiveEffect; }
		GameObject* GetOwner() { return mOwner; }

		Vector2 GetDir() { return mDir; }
		Vector2 GetPos() { return mPos; }
		bool GetFlip() { return isFlip; }

	private:
		Vector2 mDir;
		Effect* mActiveEffect;
		std::map<std::wstring, Effect*> mEffects;
		GameObject* mOwner;
		Vector2 mPos;
		bool isFlip;

	};
}
