#include "hjAnimation.h"
#include "hjTime.h"
#include "hjImage.h"
#include "hjTransform.h"
#include "hjGameObject.h"

#include "hjAnimator.h"
#include "hjCamera.h"

namespace hj
{
	Animation::Animation()
		: mAnimator(nullptr)
		, mSheetImage(nullptr)
		, mTime(0.0f)
		, mbComplete(false)
		, mSpriteIndex(0)
		, mAnimationName(L"")
	{

	}
	
	Animation::~Animation()
	{
		//delete mSheetImage;
	}

	void Animation::Initialize()
	{
	}
	
	void Animation::Update()
	{
		if (mbComplete == true)
			return;

		mTime += Time::DeltaTime();
		if (mSpriteSheet[mSpriteIndex].duration < mTime)
		{
			mTime = 0.0f;

			if (mSpriteSheet.size() <= mSpriteIndex + 1)
			{
				mbComplete = true;

			}
			else
			{
				mSpriteIndex++;
			}
		}
	}
	void Animation::Render(HDC hdc)
	{
		Transform* tr
			= mAnimator->GetOwner()->GetComponent<Transform>();
		Vector2 scale = tr->GetScale();

		Vector2 pos = tr->GetPos();
		pos = Camera::CaluatePos(pos, mSheetImage->GetPlayRate());
		pos += mSpriteSheet[mSpriteIndex].offset;
		pos.x -= mSpriteSheet[mSpriteIndex].size.x / 2.0f;
		pos.y -= mSpriteSheet[mSpriteIndex].size.y;

			//if (mAnimator->GetOwner()->GetType() == eLayerType::BackBG)
			//{
		if(mSheetImage->GetLoop() == true)
			if (pos.x <= (-tr->GetSize().x))
				pos.x += ((UINT)(fabs(pos.x) - tr->GetSize().x) / (UINT)(mSpriteSheet[mSpriteIndex].size.x / 2.0f) + 1)
				* (mSpriteSheet[mSpriteIndex].size.x / 2.0f);
			//}

		
		TransparentBlt(hdc, pos.x, pos.y
			, mSpriteSheet[mSpriteIndex].size.x * scale.x
			, mSpriteSheet[mSpriteIndex].size.y * scale.y
			, mSheetImage->GetHdc()
			, mSpriteSheet[mSpriteIndex].leftTop.x, mSpriteSheet[mSpriteIndex].leftTop.y
			, mSpriteSheet[mSpriteIndex].size.x, mSpriteSheet[mSpriteIndex].size.y,
			RGB(255, 0, 255));
 	}

	void Animation::Create(Img* sheet, Vector2 leftTop
		, UINT column, UINT row, UINT spriteLength
		, Vector2 offset, float duration)
	{
		mSheetImage = sheet;

		Vector2 size = Vector2::One;
		size.x = mSheetImage->GetWidth() / (float)column;
		size.y = mSheetImage->GetHeight() / (float)row;

		for (size_t i = 0; i < spriteLength; i++)
		{
			Sprite spriteInfo;

			spriteInfo.leftTop.x = leftTop.x + (size.x * i);
			spriteInfo.leftTop.y = leftTop.y;
			spriteInfo.size = size;
			spriteInfo.offset = offset;
			spriteInfo.duration = duration;

			mSpriteSheet.push_back(spriteInfo);
		}
	}
	void Animation::Reset()
	{
		mSpriteIndex = 0;
		mTime = 0.0f;
		mbComplete = false;
	}
}