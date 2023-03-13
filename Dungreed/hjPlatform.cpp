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
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{

	Platform::Platform()
	{
	}

	Platform::~Platform()
	{
	}

	void Platform::Initialize()
	{
		Vector2 mLeftTop = Vector2{ 0.0f, 0.0f };
		mAnimator = AddComponent<Animator>();
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();

		tr->SetPos(Vector2(tr->GetSize().x/2.0f + 800.0f, tr->GetSize().y + 900.0f));
		tr->SetScale(Vector2(1.0f, 1.0f));

		Vector2 windowSize = Vector2{ (float)application.GetWidth(), (float)application.GetHeight() };
		Vector2 ImgSize = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };
		tr->SetSize(Vector2{ ImgSize.x, ImgSize.y });

		mAnimator->CreateAnimation(L"output", mImage, Vector2::Zero, 1, 1, 1, Vector2::Zero, 0.1f);
		mAnimator->Play(L"output", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetCenter();
	}

	void Platform::Update()
	{
		GameObject::Update();
	}

	void Platform::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void Platform::Release()
	{
		GameObject::Release();
	}
	void Platform::setAnimation(const std::wstring name, const std::wstring path)
	{
		mImage = RscManager::Load<Image>(name, path);
		mImage->SetPlayRate(1.0f);
	}

}