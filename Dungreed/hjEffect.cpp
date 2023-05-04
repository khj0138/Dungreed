#include "hjEffect.h"
#include "hjEmanager.h"
#include "hjAnimator.h"
#include "hjComponent.h"
#include "hjImage.h"
#include "hjMath.h"
#include "hjRscmanager.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjSceneManager.h"
#include "hjSpriteRenderer.h"

#include "hjCamera.h"
//#include "hjMouse.h"
//#include "hjTime.h"
//#include "hjCollider.h"

extern hj::Application application;

namespace hj
{
	Effect::Effect(Vector2 pos)
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetPos(pos);
		mAnimator = new Animator();
	}
	Effect::~Effect()
	{
		delete mAnimator;
		mAnimator = nullptr;
	}

	void Effect::Initialize()
	{
	}
	void Effect::Update()
	{
		mAnimator->Update();
	}
	void Effect::Render(HDC hdc)
	{
		
		if (GetState() == eState::Active)
			mAnimator->Render(hdc);

	}
	
	void Effect::Register(const std::wstring& name, const std::wstring& path, UINT frame, Vector2 offset, float playRate)
	{
		SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		mImage = RscManager::Load<Img>(name, path);
		mImage->MatchRatio(Effect::GetAsRatio());
		mName = name;
		//std::wstring flipName = (L"Flipped");
		//flipName.append(name);
	}
	void Effect::Idle()
	{
	}
	void Effect::Attack()
	{
	}
	void Effect::Reload()
	{
	}
	void Effect::effectCompleteEvent()
	{
		if (GetManager()->FindEffect(mName) != nullptr)
			GetManager()->FindEffect(mName)->bCreate = true;
		SetState(eState::Death);
	}
	void Effect::Create(Img* image, std::wstring name, UINT frame, UINT leftTopIdx, Vector2 offset, float playRate, bool cFlip)
	{
		UINT row = 2;
		if (!cFlip)
			row = 1;
		mName = name;
		if (offset == Vector2::Zero
			&& cFlip == false
			&& GetComponent<SpriteRenderer>()->getRotate() == false)
		{
			float temp = (float)image->GetWidth() / (float)frame;
			offset = Vector2{ temp / 2.f, temp };
		}
		mAnimator->CreateAnimation(name, image, leftTopIdx, frame, row, frame, offset, playRate);
		mAnimator->SetOwner(this);
		mAnimator->GetCompleteEvent(name) = std::bind(&Effect::effectCompleteEvent, this);
		mAnimator->Play(name, false);
	}
}