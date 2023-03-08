#include "hjBird.h"
//#include "hjAnimator.h"
#include "hjRscManager.h"
#include "hjTime.h"

namespace hj
{
	Bird::Bird()
	{
	}
	Bird::~Bird()
	{
	}
	void Bird::Initialize()
	{
		
		//tr->SetPos(Vector2{ 0.0f, 0.0f });
		//Vector2 size = Vector2::Zero;
		//float width = mImage->GetWidth();
		//float height = mImage->GetHeight();
		//if (width * 1600.f > height * 900.f)
		//{
		//	size.x = (float)height / 900.f * 1600.f;
		//	size.y = height;
		//}
		//else
		//{
		//	size.x = width;
		//	size.y = (float)width / 1600.f * 900.f;
		//}
		//tr->SetSize(Vector2{ width - size.x, height });
		//tr->SetPos(tr->GetPos() + Vector2{ size.x / 2.0f, size.y });

		//mImage->SetSize(size);

	}
	void Bird::Update()
	{
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		pos.x += 100.0f * Time::DeltaTime();
		tr->SetPos(pos);
		GameObject::Update();
	}
	void Bird::Render(HDC hdc)
	{
		mAnimator;
		GameObject::Render(hdc);
	}
	void Bird::Release()
	{
	}
	void Bird::setAnimation(const std::wstring name, const std::wstring path)
	{
		mAnimator = AddComponent<Animator>();
		//GameObject::Initialize();
		//Transform* tr = GetComponent<Transform>();

		mImage = RscManager::Load<Image>(name, path);
		//mAnimator->CreateAnimation(L"bird", mImage, Vector2::Zero, 8, 1, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(name, mImage, Vector2::Zero, 8, 1, 8, Vector2::Zero, 0.1);
		mAnimator->Play(name, true);
	}
}

