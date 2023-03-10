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

namespace hj
{
	Monster::Monster()
	{
	}
	Monster::~Monster()
	{
	}
	void Monster::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetPos(Vector2(700.0f, 400.0f));
		tr->SetScale(Vector2(1.0f, 1.0f));

		mAnimator = AddComponent<Animator>();
		mAnimator->CreateAnimations(L"", Vector2::Zero, 0.1f);

		mAnimator->Play(L"", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetCenter();
		GameObject::Initialize();
	}
	void Monster::Update()
	{
		GameObject::Update();
	}
	void Monster::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
	void Monster::Release()
	{
		GameObject::Release();
	}
}