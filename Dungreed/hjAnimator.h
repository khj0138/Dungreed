#pragma once
#include "hjComponent.h"
#include "hjAnimation.h"
#include "hjImage.h"

namespace hj
{

	class Animator : public Component
	{
	public:
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				mEvent = std::move(func);
			}
			void operator()()
			{
				if (mEvent)
					mEvent();
			}

			std::function<void()> mEvent;
		};

		struct Events
		{
			Event mStartEvent;
			Event mCompleteEvent;
			Event mEndEvent;
		};

		Animator();
		~Animator();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void CreateAnimation(const std::wstring& name
			, Img* sheet
			, Vector2 leftTop
			, UINT column, UINT row, UINT spriteLength
			, Vector2 offset, float duration);
		void CreateAnimation(const std::wstring& name
			, Img* sheet
			, UINT leftTopIdx
			, UINT column, UINT row, UINT spriteLength
			, Vector2 offset, float duration
			, bool rotate = false, Vector2 direction = Vector2{ 0.0f, 0.0f });
		void CreateAnimations(const std::wstring&path, Vector2 offset, float duration);
		
		Animation* FindAnimation(const std::wstring& name);
		void Play(const std::wstring& name, bool loop);


		void Reset();
		void Flip(const std::wstring& name);
		void setActAnimLeftTop(Vector2 leftTop) { mActiveAnimation->setLeftTop(leftTop); }
		Animation* GetActiveAnimation() { return mActiveAnimation; }

		Events* FindEvents(const std::wstring& name);
		std::function<void()>& GetStartEvent(const std::wstring& name);
		std::function<void()>& GetCompleteEvent(const std::wstring& name);
		std::function<void()>& GetEndEvent(const std::wstring& name);

	private:
		std::map<std::wstring, Animation*> mAnimations;
		std::map<std::wstring, Events*> mEvents;
		Animation* mActiveAnimation;
		Img* mSpriteSheet;
		bool mbLoop;
	};

}
