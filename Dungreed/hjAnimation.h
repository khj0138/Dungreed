#pragma once
#include "hjEntity.h"

namespace hj
{
	class Img;
	class Animator;
	class Animation : public Entity
	{
	public:
		struct Sprite
		{
			Vector2 leftTop;
			Vector2 size;
			Vector2 offset;
			float duration;

			Sprite()
				: leftTop(Vector2::Zero)
				, size(Vector2::Zero)
				, offset(Vector2::Zero)
				, duration(0.0f)
			{

			}
		};

		Animation();
		~Animation();

		void Initialize();
		void Update();
		void Render(HDC hdc);
		void Create(Img* sheet, Vector2 leftTop, UINT column, UINT row, UINT spriteLength, Vector2 offset, float duration);
		void Create(Img* sheet, UINT leftTopIdx, UINT column, UINT row, UINT spriteLength, Vector2 offset, float duration);
		void Reset();

		bool isComplete() { return mbComplete; }
		void SetAnimator(Animator* animator) { mAnimator = animator;  }
		int getIndex() { return mSpriteIndex; }
		void setIndex(int spriteIndex) { mSpriteIndex = spriteIndex; }
		void setLeftTop(Vector2 pos) {mSpriteSheet[0].leftTop = pos; }
		void SetAnimationName(const std::wstring& name) { mAnimationName = name; }
		void SetRotate(bool rotate) { bRotate = rotate; }
		void SetDirection(Vector2 direction) { mDirection = direction; }
		std::wstring& GetAnimationName() { return mAnimationName; }
	
	
	private:
		Animator* mAnimator;
		Img* mSheetImage;
		std::vector<Sprite> mSpriteSheet;
		float mTime;
		bool mbComplete;
		int mSpriteIndex;
		std::wstring mAnimationName;
		bool bRotate;
		Vector2 mDirection;
	};

}
