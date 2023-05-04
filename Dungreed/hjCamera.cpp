#include "hjCamera.h"
#include "hjApplication.h"
#include "hjGameObject.h"
#include "hjTransform.h"
#include "hjInput.h"
#include "hjTime.h"

extern hj::Application application;
namespace hj
{
	Vector2 Camera::mResolution = Vector2::Zero;
	Vector2 Camera::mLookPosition = Vector2::Zero;
	Vector2 Camera::mDistance = Vector2::Zero;
	Vector2 Camera::mLookRange = Vector2::Zero;
	GameObject* Camera::mTarget = nullptr;

	void Camera::Initiailize()
	{
		mResolution.x = application.GetWidth();
		mResolution.y = application.GetHeight();
		mLookPosition = (mResolution / 2.0f);
	}

	void Camera::Update()
	{

		if (Input::GetKey(eKeyCode::Z))
			mLookPosition.x -= 500.0f * Time::DeltaTime();

		if (Input::GetKey(eKeyCode::C))
			mLookPosition.x += 500.0f * Time::DeltaTime();

		if (Input::GetKey(eKeyCode::X))
			mLookPosition.y -= 500.0f * Time::DeltaTime();

		if (Input::GetKey(eKeyCode::V))
			mLookPosition.y += 500.0f * Time::DeltaTime();



		if (mTarget != nullptr)
		{
			float distance = (mTarget->GetComponent<Transform>()->GetPos() - mLookPosition).Length();
			if (distance < 1000.0f * Time::DeltaTime())
				mLookPosition = mTarget->GetComponent<Transform>()->GetPos();
			else if (distance > 4000.0f * Time::DeltaTime())
				mLookPosition += (mTarget->GetComponent<Transform>()->GetPos() - mLookPosition).Normalize() * 2000.0f * Time::DeltaTime();
			else
				mLookPosition += (mTarget->GetComponent<Transform>()->GetPos() - mLookPosition).Normalize() * 1000.0f * Time::DeltaTime();
		}

		if (mLookPosition.x <= (mResolution.x / 2.0f))
			mLookPosition.x = (mResolution.x / 2.0f);
		if (mLookPosition.x >= mLookRange.x - mResolution.x / 2.0f)
			mLookPosition.x = mLookRange.x - mResolution.x / 2.0f;
		if (mLookPosition.y >= mLookRange.y - mResolution.y / 2.0f)
			mLookPosition.y = mLookRange.y - mResolution.y / 2.0f;
		if (mLookPosition.y <= (mResolution.y / 2.0f))
			mLookPosition.y = (mResolution.y / 2.0f);

		mDistance = mLookPosition - (mResolution / 2.0f);
	}
	void Camera::Clear()
	{
		mResolution.x = application.GetWidth();
		mResolution.y = application.GetHeight();
		mLookPosition = (mResolution / 2.0f);
		mDistance = Vector2::Zero;
	}
}
