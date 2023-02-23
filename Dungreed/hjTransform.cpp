#include "hjTransform.h"


namespace hj
{
	Transform::Transform()
		:Component(eComponentType::Transform)
		, mPos(0,0)
		, mSize(0,0)
		, mRotation(0,0)
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