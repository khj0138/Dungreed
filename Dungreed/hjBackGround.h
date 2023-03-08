#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjSpriteRenderer.h"
#include "hjTransform.h"
#include "hjAnimator.h"


namespace hj
{

	class BackGround : public GameObject
	{
	public:
		BackGround();
		~BackGround();
		
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void setPos(Vector2 pos);
		//void setV(double v) { mV = v; }
		void setAnimation(const std::wstring name, const std::wstring path, float playRate);
		void setScale(Vector2 scale);
		void setPlayRate(float playRate) { mPlayRate = playRate; }

		Vector2 GetSize();

	private:
		SpriteRenderer* spr;
		Transform* tr;
		//double mV;
		double mTime;
		Animator* mAnimator;
		Image* mImage;
		float mPlayRate;
		Vector2 mLeftTop;
	};

}