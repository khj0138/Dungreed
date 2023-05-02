#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjSpriteRenderer.h"
#include "hjTransform.h"
#include "hjAnimator.h"


namespace hj
{
	class Monster;
	class BossHPBar : public GameObject
	{
	public:
		BossHPBar();
		BossHPBar(const std::wstring name, const std::wstring path, Vector2 asRatio, Vector2 offset = Vector2::Zero);
		~BossHPBar();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void SetMonster(Monster* Monster) { mMonster = Monster; }
		Monster* GetMonster() { return mMonster; }

	private:
		SpriteRenderer* spr;
		Transform* tr;
		double mTime;
		Img* image_full;
		Img* image_empty;
		//Vector2 mLeftTop;
		Vector2 mPos;
		Monster* mMonster;

	};

}