#pragma once
#include "hjGameObject.h"

namespace hj
{


	class Effect;
	class Img;
	class Emanager : public GameObject
	{
		struct effectInfo {
			Img* mImage;
			std::wstring mName;
			UINT mFrame;
			Vector2 mOffset;
			Vector2 mLeftTop;
			float mPlayRate;
			bool bCreate;
			bool bDuplicate;
			bool bRotate;
		};
		//void Effect::Register(const std::wstring& name, const std::wstring& path, UINT frame, Vector2 offset, float playRate)
		//{
		//	SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		//	mImage = RscManager::Load<Img>(name, path);
		//	mImage->MatchRatio(Effect::GetAsRatio());
		//	mName = name;
		//	//std::wstring flipName = (L"Flipped");
		//	//flipName.append(name);
		//}

	public:
		Emanager();
		~Emanager();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void SetOwner(GameObject* owner) { mOwner = owner; }
		void CreateEffect(const std::wstring& name, Vector2 direction = Vector2::Zero);
		void RegisterEffect(const std::wstring& name, const std::wstring& path,bool loop, bool rotate, UINT frame, Vector2 offset, float playRate, Vector2 size = Vector2::One * 2.f);
		
		//void PlayEffect(const std::wstring& name);
		void Clear();
		void SetAsRatio(Vector2 asRatio) { mAsRatio = asRatio; }

		effectInfo* FindEffect(const std::wstring& name);
		Effect* GetActiveEffect() { return mActiveEffect; }
		GameObject* GetOwner() { return mOwner; }

		Vector2 GetDir() { return mDir; }
		Vector2 GetPos() { return mPos; }
		bool GetFlip() { return isFlip; }

	private:
		Vector2 mDir;
		Effect* mActiveEffect;
		std::map<std::wstring, effectInfo*> mEffects;
		std::map<effectInfo*, bool> bEffects;
		GameObject* mOwner;
		Vector2 mPos;
		bool isFlip;
		Vector2 mAsRatio;

	};
}
