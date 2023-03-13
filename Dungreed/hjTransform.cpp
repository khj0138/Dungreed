#include "hjTransform.h"


namespace hj
{
	Transform::Transform()
		:Component(eComponentType::Transform)
		, mPos(Vector2::Zero)
		, mScale(Vector2::One)
		, mRotation(0, 0)
		, mSize(0, 0)
		, mVelocity(0, 0)
		, mPlayRate(0, 0)
	{
	}
	Transform::~Transform()
	{
	}
	void Transform::Initialize()
	{
	}
	void Transform::Update()
	{
	}
	void Transform::Render(HDC hdc)
	{
	}
	void Transform::Release()
	{
	}
}