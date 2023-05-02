#include "hjAnimation.h"
#include "hjTime.h"
#include "hjImage.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjAnimator.h"
#include "hjCamera.h"

#include "hjSpriteRenderer.h"
#include "hjComponent.h"
namespace hj
{
	Animation::Animation()
		: mAnimator(nullptr)
		, mSheetImage(nullptr)
		, mTime(0.0f)
		, mbComplete(false)
		, mSpriteIndex(0)
		, mAnimationName(L"")
		, bRotate(false)
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
		
		if (mAnimator->GetOwner()->GetComponent<SpriteRenderer>()->getRotate())
		{
			//Vector2 dir = mAnimator->GetOwner()->GetComponent<SpriteRenderer>()->getDir() - Vector2{ 0.f, -90.f };
			//Vector2 dir = Vector2{ -1.f,-1.f };
			Vector2 dir = math::Rotate(mAnimator->GetOwner()->GetComponent<SpriteRenderer>()->getDir(), 90.0f);
			HBITMAP ImgBmp = CreateCompatibleBitmap(mSheetImage->GetHdc()
				, mSpriteSheet[mSpriteIndex].size.x
				, mSpriteSheet[mSpriteIndex].size.y);
			HDC ImgHdc = CreateCompatibleDC(mSheetImage->GetHdc());
			DeleteObject(SelectObject(ImgHdc, ImgBmp));

			BitBlt(ImgHdc, 0, 0
				, mSpriteSheet[mSpriteIndex].size.x
				, mSpriteSheet[mSpriteIndex].size.y
				, mSheetImage->GetHdc()
				, mSpriteSheet[mSpriteIndex].leftTop.x
				, mSpriteSheet[mSpriteIndex].leftTop.y
				, SRCCOPY);


			/*pos = Camera::CaluatePos(pos, mSheetImage->GetMoveRate());
			pos += mSpriteSheet[mSpriteIndex].offset;
			pos.x -= mSpriteSheet[mSpriteIndex].size.x / 2.0f;
			pos.y -= mSpriteSheet[mSpriteIndex].size.y;
			TransparentBlt(hdc, pos.x, pos.y
				, mSpriteSheet[mSpriteIndex].size.x * scale.x
				, mSpriteSheet[mSpriteIndex].size.y * scale.y
				, ImgHdc
				, 0,0
				, mSpriteSheet[mSpriteIndex].size.x, mSpriteSheet[mSpriteIndex].size.y,
				RGB(255, 0, 255));*/
			float a = atan2(dir.y, dir.x) / PI * 180.f;
			int b = 0;

			pos.x -= mSpriteSheet[mSpriteIndex].size.x / 2.0f;
			pos.y -= mSpriteSheet[mSpriteIndex].size.y / 2.0f;

			mAnimator->GetOwner()->GetComponent<SpriteRenderer>()->rotateRender(hdc, ImgHdc,
				Vector2{ mSpriteSheet[mSpriteIndex].size.x 
				, mSpriteSheet[mSpriteIndex].size.y } 
			, atan2(dir.y, dir.x)/PI * 180.f, pos + mSpriteSheet[mSpriteIndex].offset);
			DeleteObject(ImgBmp);
			DeleteDC(ImgHdc);
			return;
		}
		
		
		
		pos = Camera::CaluatePos(pos, mSheetImage->GetMoveRate());
		pos += mSpriteSheet[mSpriteIndex].offset;
		pos.x -= mSpriteSheet[mSpriteIndex].size.x / 2.0f;
		pos.y -= mSpriteSheet[mSpriteIndex].size.y;

			//if (mAnimator->GetOwner()->GetType() == eLayerType::BackBG)
			//{
		if(mSheetImage->GetRepeat() == true)
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
	void Animation::Create(Img* sheet, UINT leftTopIdx
		, UINT column, UINT row, UINT spriteLength
		, Vector2 offset, float duration)
	{
		mSheetImage = sheet;

		Vector2 size = Vector2::One;
		size.x = mSheetImage->GetWidth() / (float)column;
		size.y = mSheetImage->GetHeight() / (float)row;
		Vector2 leftTop = Vector2{ 0.0f, (float)leftTopIdx * size.y };
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