#include "hjBackGround.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"

namespace hj
{
	BackGround::BackGround()
		: mTime(0.0f)
	{

	}
	BackGround::~BackGround()
	{
		delete mImage;
		mImage = nullptr;
	}
	void BackGround::Initialize()
	{
		mLeftTop = Vector2{ 0.0f, 0.0f };
		mAnimator = AddComponent<Animator>();
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();
		tr->SetPos(Vector2{ 0.0f, 0.0f });
		mImage->matchResolution();

		float width = mImage->GetWidth();
		float height = mImage->GetHeight();
		tr->SetSize(Vector2{ width, height });
		tr->SetPos (tr->GetPos() + Vector2{ width / 2.0f, height });

		
		mAnimator->CreateAnimation(L"output", mImage,Vector2::Zero, 1, 1, 1, Vector2::Zero, 1000.f);
		mAnimator->Play(L"output", true);
		
	}
	void BackGround::Update()
	{
		Transform* tr = GetComponent<Transform>();
	
		if(mPlayRate != 0.0f)
		{ 
			if (mLeftTop.x >= (tr->GetSize().x))
			{
				mLeftTop = Vector2::Zero;

				mAnimator->setActAnimLeftTop(mLeftTop);
			}
			else
			{
				mTime += Time::DeltaTime();
				float temp = 1.0f * (float)mTime * (float)mPlayRate;

				if (temp > 1)
				{
					mLeftTop += Vector2{(float)((UINT)temp), 0.0f };
					mTime = 0.0f;
				}
				mAnimator->setActAnimLeftTop(mLeftTop);
			}
		}

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

	void BackGround::setAnimation(const std::wstring name, const std::wstring path, float playRate)
	{
		mImage = RscManager::Load<Image>(name, path);
		mPlayRate = playRate;
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