#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjSpriteRenderer.h"
#include "hjTransform.h"
#include "hjAnimator.h"


namespace hj
{

	class PlaneObject : public GameObject
	{
	public:
		PlaneObject();
		PlaneObject(const std::wstring name, const std::wstring path, Vector2 asRatio, Vector2 offset = Vector2::Zero, Vector2 moveRate = Vector2::One);
		~PlaneObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:
		SpriteRenderer* spr;
		Transform* tr;
		double mTime;
		Img* mImage;
		Vector2 mLeftTop;
		Vector2 mPos;
	};

}