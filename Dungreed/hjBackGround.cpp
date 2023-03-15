#include "hjBackGround.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	BackGround::BackGround()
		: mTime(0.0f)
		, mLoop(false)
	{

	}
	BackGround::~BackGround()
	{
		//delete mImage;
		//mImage = nullptr;
	}
	void BackGround::Initialize()
	{
		mLeftTop = Vector2{ 0.0f, 0.0f };
		mAnimator = AddComponent<Animator>();
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();

		tr->SetPos(Vector2{ 0.0f, 0.0f });

		Vector2 windowSize = Vector2{ (float)application.GetWidth(), (float)application.GetHeight() };
	
	
		Vector2 ImgSize = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		tr->SetSize(Vector2{ ImgSize.x, ImgSize.y });
		tr->SetPos(
			tr->GetPos()
			//+ Vector2{ windowSize.x / 2.0f, windowSize.y }
			//+ Vector2{ tr->GetSize().x / 2.0f, tr->GetSize().y }
			+ Vector2{ tr->GetSize().x / 2.0f, windowSize.y }
			+ Vector2{0.0f, 0.0f}
		);
		tr->SetSize(Vector2{ ImgSize.x / 2.0f, ImgSize.y });

		mAnimator->CreateAnimation(L"output", mImage,Vector2::Zero, 1, 1, 1, Vector2::Zero, 0.1f);
		mAnimator->Play(L"output", true);
		
	}
	void BackGround::Update()
	{
		Transform* tr = GetComponent<Transform>();

		GameObject::Update();
	}
	void BackGround::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void BackGround::Release()
	{
		GameObject::Release();
	}

	void BackGround::setPos(Vector2 pos)
	{
		tr = GetComponent<Transform>();
		Vector2 iPos = tr->GetPos();

		spr = GetComponent < SpriteRenderer>();
		UINT width = spr->GetSprite()->GetWidth();
		if((iPos.x + pos.x) < width)
			tr->SetPos(Vector2{ (float)(iPos.x + pos.x), iPos.y });
		else
			tr->SetPos(Vector2{ (float)(iPos.x + pos.x) - width, iPos.y });
	}

	void BackGround::setAnimation(const std::wstring name, const std::wstring path, float playRate, bool loop)
	{
		mImage = RscManager::Load<Img>(name, path);
		mImage->SetPlayRate(playRate);
		mImage->SetLoop(loop);
	}

	void BackGround::setScale(Vector2 scale)
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetScale(Vector2{ scale.x, scale.y });
	}
	Vector2 BackGround::GetSize()
	{
		return Vector2{(float) mImage->GetWidth(),(float) mImage->GetHeight() };
	}
	

	
}