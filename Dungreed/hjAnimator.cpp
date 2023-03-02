#include "hjAnimator.h"


namespace hj
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mActiveAnimation(nullptr)
		, mSpriteSheet(nullptr)
		, mbLoop(false)
	{

	}
	Animator::~Animator()
	{
		for (auto animation : mAnimations)
		{
			delete animation.second;
			animation.second = nullptr;
		}
	}
	void Animator::Initialize()
	{

	}
	void Animator::Update()
	{
		if (mActiveAnimation)
		{
			mActiveAnimation->Update();

			if (mbLoop && mActiveAnimation->isComplete())
			{
				mActiveAnimation->Reset();
			}
		}
	}
	void Animator::Render(HDC hdc)
	{
		if (mActiveAnimation)
		{
			mActiveAnimation->Render(hdc);
		}
	}
	void Animator::Release()
	{

	}
	void Animator::CreateAnimation(const std::wstring& name
		, Image* sheet, Vector2 leftTop
		, UINT column, UINT row, UINT spriteLength
		, Vector2 offset, float duration)
	{
		Animation* animation = FindAnimation(name);

		if (animation != nullptr)
			return;

		animation = new Animation();
		animation->Create(sheet, leftTop, column, row, spriteLength, offset, duration);
		animation->SetName(name);
		animation->SetAnimator(this);

		mAnimations.insert(std::make_pair(name, animation));
	}
	void Animator::CreateAnimations()
	{
	}
	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter
			= mAnimations.find(name);

		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;
	}
	void Animator::Play(const std::wstring& name, bool loop)
	{
		mActiveAnimation = FindAnimation(name);
		mbLoop = loop;
	}
	void Animator::Reset()
	{
		mActiveAnimation->Reset();
	}
	void Animator::Flip(const std::wstring& name)
	{
		UINT spriteIndex = mActiveAnimation->getIndex();
		mActiveAnimation = FindAnimation(name);
		mActiveAnimation->setIndex(spriteIndex);
	}
	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		return nullptr;
	}
}