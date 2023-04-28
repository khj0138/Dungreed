#include "hjAnimObject.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjCamera.h"
#include "hjTilePalatte.h"

extern hj::Application application;

namespace hj
{
	AnimObject::AnimObject(const std::wstring name, const std::wstring path, Vector2 asRatio, Vector2 colRow, Vector2 offset, Vector2 moveRate, float PlayRate)
		: mTime(0.0f)
		, mPos(offset)
	{
		Img* mImage = RscManager::Load<Img>(name, path);
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(moveRate);
		mImage->SetRepeat(false);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / colRow.x;
		size.y = mImage->GetHeight() / colRow.y;
		mAnimator = AddComponent<Animator>();
		for (int i = 0; i < (UINT)(colRow.y); i++)
		{
			wchar_t temp[10];
			_itow_s(i, temp, 10);
			std::wstring index(temp);
			std::wstring anim = L"anim";
			//anim.append(index);
			mAnimator->CreateAnimation(anim, mImage, Vector2::Zero, (UINT)(colRow.x), (UINT)(colRow.y), (UINT)(colRow.x), Vector2::Zero, PlayRate);
		}
	}
	AnimObject::~AnimObject()
	{
	}
	void AnimObject::Initialize()
	{
		GameObject::Initialize();
		mAnimator->Play(L"anim", false);
		GetComponent<Transform>()->SetPos(mPos);

	}
	void AnimObject::Update()
	{
		GameObject::Update();
	}
	void AnimObject::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
	void AnimObject::Release()
	{
		GameObject::Release();
	}
}