#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjSpriteRenderer.h"
#include "hjTransform.h"
#include "hjAnimator.h"


namespace hj
{
	class Hero;
	class HPBar : public GameObject
	{
	public:
		HPBar();
		HPBar(const std::wstring name, const std::wstring path, Vector2 asRatio, Vector2 offset = Vector2::Zero);
		~HPBar();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void SetHero(Hero* hero) { mHero = hero; }
		Hero* GetHero() { return mHero; }

	private:
		SpriteRenderer* spr;
		Transform* tr;
		double mTime;
		Img* image_full;
		Img* image_empty;
		//Vector2 mLeftTop;
		Vector2 mPos;
		Hero* mHero;

	};

}