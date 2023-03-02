#pragma once
#include "hjComponent.h"

namespace hj
{

	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void SetPos(Vector2 pos) { mPos = pos; }
		void SetScale(Vector2 scale) { mScale = scale; }
		void SetSize(Vector2 size) { mSize = size; }
		
		// ******rigidbody velocity ***********
		void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
		Vector2 GetVelocity() { return mVelocity; }
		Vector2 mVelocity;
		// ******rigidbody velocity ***********
		Vector2 GetPos() { return mPos; }
		Vector2 GetScale() { return mScale; }
		Vector2 GetSize() { return mSize; }
	private:
		Vector2 mPos;
		Vector2 mScale; 
		Vector2 mRotation;
		Vector2 mSize;
	};

}