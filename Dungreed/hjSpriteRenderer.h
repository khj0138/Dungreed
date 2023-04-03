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
			mSprite = RscManager::Load<Img>(key, path);
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

		Img* GetSprite() { return mSprite; }
		bool checkFlip() { return mFlip; }
		int getState() { return mState; }
		int getIndex() { return mIndex; }
		bool getRotate() { return bRotate; }
		Vector2 getDir() { return mDir; }

		void rotateRender(HDC hdc, HDC ImgHdc, Vector2 size, float degree, Vector2 pos);
		void SetRotate(bool rotate) { bRotate = rotate; }
		void SetDir(Vector2 direction) { mDir = direction;	}

	private:
		Img* mSprite;
		int mState;
		int mIndex;
		bool mFlip;
		bool bRotate;
		Vector2 mDir;
	};

}
