#include "hjPlatform.h"

#include "hjMonster.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjBaseBullet.h"
#include "hjScene.h"
#include "hjCamera.h"
#include "hjApplication.h"

extern hj::Application application;



namespace hj
{

	Platform::Platform()
	{
	}

	Platform::Platform(const std::wstring name, const std::wstring path, float moveRate, Vector2 asRatio, bool Repeat)
	{
		mImage = RscManager::Load<Img>(name, path);
		mImage->SetMoveRate(1.0f);
		mImage->MatchRatio(asRatio);
	}

	Platform::~Platform()
	{
	}

	void Platform::Initialize()
	{
		


		Vector2 mLeftTop = Vector2{ 0.0f, 0.0f };
		//mAnimator = AddComponent<Animator>();
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();
		Vector2 windowSize = Vector2{ (float)application.GetWidth(), (float)application.GetHeight() };
		Vector2 ImgSize = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };
		tr->SetPos(
			tr->GetPos()
			+ Vector2{ ImgSize.x / 2.0f, windowSize.y }
			+ Vector2{ 0.0f, 0.0f }
		);
		tr->SetScale(Vector2(1.0f, 1.0f));
		tr->SetSize(Vector2{ ImgSize.x, ImgSize.y });

		Collider* collider = AddComponent<Collider>();
		collider->SetCenter();
	}

	void Platform::Update()
	{
		GameObject::Update();
	}

	void Platform::Render(HDC hdc)
	{
		Transform* tr = GetComponent<Transform>();
		Vector2 scale = tr->GetScale();
		Vector2 pos = tr->GetPos();
		pos = Camera::CaluatePos(pos, mImage->GetMoveRate());
		pos.x -= (float)mImage->GetWidth() / 2.0f;
		pos.y -= mImage->GetHeight();


		TransparentBlt(hdc, pos.x, pos.y
			, mImage->GetWidth() * scale.x
			, mImage->GetHeight() * scale.y
			, mImage->GetHdc()
			, 0, 0
			, mImage->GetWidth(), mImage->GetHeight(),
			RGB(255, 0, 255));
		GameObject::Render(hdc);
	}

	void Platform::Release()
	{
		GameObject::Release();
	}
	void Platform::SetImage(const std::wstring name, const std::wstring path, float moveRate, Vector2 asRatio, bool Repeat)
	{
		mImage = RscManager::Load<Img>(name, path);
		mImage->SetMoveRate(1.0f);
		mImage->MatchRatio(asRatio);
	}

}