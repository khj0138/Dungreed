#include "hjCollider.h"

namespace hj
{

	Collider::Collider()
		: Component(eComponentType::Collider)
		, mCenter(Vector2::Zero)
		, mScale(Vector2::One)
		, mPos(Vector2::Zero)
	{
	}

	Collider::~Collider()
	{
	}

	void Collider::Initialize()
	{
	}

	void Collider::Update()
	{
//		Transform* tr = GetOwner()->GetComponent<Transform>();
	}

	void Collider::Render(HDC hdc)
	{
	}

	void Collider::Release()
	{
	}

}