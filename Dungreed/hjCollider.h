#pragma once
#include "hjComponent.h"
#include "hjTransform.h"


namespace hj
{

	class Collider : public Component
	{
	public:
		Collider();
		~Collider();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;
	private:
		Vector2 mCenter;
		Vector2 mScale;
		Vector2 mPos;
	};
}

