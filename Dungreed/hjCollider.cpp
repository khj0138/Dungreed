#include "hjCollider.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjCamera.h"

namespace hj
{
	bool Collider::colRender;
	UINT Collider::ColliderNumber = 0;
	Collider::Collider()
		: Component(eComponentType::Collider)
		, mCenter(Vector2::Zero)
		, mScale(Vector2::One)
		, mPos(Vector2::Zero)
		, mSize(100.0f, 100.0f)
		, mID(ColliderNumber++)
		, mCollisionCount(0)
	{
	}

	Collider::~Collider()
	{
	}

	void Collider::Initialize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		mPos = tr->GetPos() + mCenter;
	}

	void Collider::Update()
	{
		collisionCheck = false;
		Transform* tr = GetOwner()->GetComponent<Transform>();
		mPos = tr->GetPos() + mCenter;
	}

	void Collider::Render(HDC hdc)
	{
		if (!colRender)
			return;
		HPEN pen = NULL;
		if (mCollisionCount <= 0)
			pen = CreatePen(BS_SOLID, 2, RGB(0, 255, 0));
		else
			pen = CreatePen(BS_SOLID, 2, RGB(255, 0, 0));

		HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

		Vector2 pos = Camera::CaluatePos(mPos, Vector2::One);
		Rectangle(hdc, pos.x, pos.y, pos.x + mSize.x, pos.y + mSize.y);

		(HPEN)SelectObject(hdc, oldPen);
		(HBRUSH)SelectObject(hdc, oldBrush);
		DeleteObject(pen);
		
		mCollisionCount = 0;
	}

	void Collider::Release()
	{
	}

	void Collider::OnCollisionEnter(Collider* other)
	{
		//lother- this
		//collisionCheck = true;
		GetOwner()->OnCollisionEnter(other);
	}

	void Collider::OnCollisionStay(Collider* other)
	{
		//collisionCheck = true;
		mCollisionCount = 1;
		GetOwner()->OnCollisionStay(other);
	}

	void Collider::OnCollisionExit(Collider* other)
	{
		GetOwner()->OnCollisionExit(other);
	}

}