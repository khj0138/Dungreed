#pragma once
#include "hjComponent.h"
#include "hjImage.h"
#include "hjRscManager.h"

namespace hj
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();

		void SetSprite(const std::wstring& key, const std::wstring& path)
		{
			mSprite = RscManager::Load<Image>(key, path);
		}
		void SetFlip(bool flip)
		{
			mFlip = flip;
		}
		void SetState(int state)
		{
			mState = state;
		}
		void SetIndex(int index)
		{
			mIndex = index;
		}

		Image* GetSprite() { return mSprite; }
		bool checkFlip() { return mFlip; }
		int getState() { return mState; }
		int getIndex() { return mIndex; }

	private:
		Image* mSprite;
		int mState;
		int mIndex;
		bool mFlip;
	};

}
