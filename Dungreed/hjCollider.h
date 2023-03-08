#pragma once
#include "hjComponent.h"


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

		Vector2 GetPos() { return mPos; }
		Vector2 GetSize() { return mSize; }

		void SetSize(Vector2 size) { mSize = size; }
		void SetCenter(Vector2 center = Vector2{ 0.0f, 0.0f }) 
		{ 
			if (center.x == 0.0f && center.y == 0.0f)
			{
				center.x = 0.0f - (mSize.x / 2.0f);
				center.y = 0.0f - (mSize.y);
			}
			else
				mCenter = center;
		};
		void SetScale(Vector2 scale) { mScale = scale; };

	private:
		Vector2 mCenter;
		Vector2 mSize;
		Vector2 mScale;
		Vector2 mPos;
	};
}



