#include "hjRigidbody.h"
#include "hjTime.h"
#include "hjGameObject.h"
#include "hjTransform.h"

namespace hj
{
	Rigidbody::Rigidbody()
		: Component(eComponentType::Rigidbody)
		, mMass(0.0f)
		, mForce(Vector2::Zero)
		, mAccelation(Vector2::Zero)
		, mVelocity(Vector2::Zero)
	{
		mLimitedVelocity.x = 200.0f;
		mLimitedVelocity.y = 1400.0f;
		//mbGround = false;
		mbGround = false;
		mGravity = Vector2(0.0f, 4800.0f);
		mbGravity = true;
		mFriction = 100.0f;
	}

	Rigidbody::~Rigidbody()
	{

	}

	void Rigidbody::Initialize()
	{

	}

	void Rigidbody::Update()
	{
		// F = M * A
		// A = M / F
		//mAccelation = mForce / mMass;

		// 속도에 가속도를 더해준다.
		//mVelocity += mAccelation * Time::DeltaTime();

		if (mbGround)
		{
			// 땅위에 있을때
			Vector2 gravity = mGravity;
			gravity.Normalize();

			float dot = math::Dot(mVelocity, gravity);
			mVelocity -= gravity * dot;
		}
		else
		{
			if (mbGravity)
				mVelocity += mGravity * Time::DeltaTime();
		}

		// 중력가속도 최대 속도 제한
		Vector2 gravity = mGravity;
		gravity.Normalize();
		float dot = math::Dot(mVelocity, gravity);
		gravity = gravity * dot;
		//Vector2 sideVelocity = mVelocity - gravity;

		if (mLimitedVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitedVelocity.y;
			mVelocity.y = gravity.y;
		}

		/*if (mLimitedVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitedVelocity.x;
		}*/

		// 마찰력 조건 ( 적용된 힘이 없고, 속도가 0이 아님)
		//if (!(mVelocity == Vector2::Zero))
		//{
		//	//속도에 반대방향으로 마찰력이 적용된다.
		//	Vector2 friction = mVelocity * (-1.f);
		//	friction = friction.Normalize() * mFriction * mMass * Time::DeltaTime();

		//	//마찰력으로 인한 속도 감소는 현재 속도보다 큰 경우

		//	if (mVelocity.Length() < friction.Length())
		//	{
		//		mVelocity = Vector2::Zero;
		//	}
		//	else
		//	{
		//		mVelocity += friction;
		//	}
		//}

		// 속도에 맞게끔 물체를 이동시킨다.
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		if (!mbGround && !mbGravity)
			pos = pos + mVelocity;
		else
		{

			if (Time::DeltaTime() > 0.08f)
				pos = pos + mVelocity * 0.08f;
			else if (Time::DeltaTime() < 0.015f)
				pos = pos + mVelocity * 0.015f;
			else
				pos = pos + mVelocity * Time::DeltaTime();
		}
			//pos = pos + mVelocity * (Time::DeltaTime() > 1.0f ? 1.0f : Time::DeltaTime());
		tr->SetPos(pos);
		mForce.Clear();
	}

	void Rigidbody::Render(HDC hdc)
	{
	}
	void Rigidbody::Release()
	{
	}
	void Rigidbody::AddForce(Vector2 force)
	{
		mForce += force;
	}
}