#include "hjBird.h"
#include "hjRscManager.h"
#include "hjTime.h"
#include "hjApplication.h"
#include "hjObject.h"

extern hj::Application application;

namespace hj
{
	Bird::Bird(double time, Vector2 pos, const std::wstring name, const std::wstring path, Vector2 asRatio)
		: spawnPos(0.0f, 0.0f)
		, spawnTime(0.0f)
		, isSpawn(false)
	{
		spawnPos = pos;
		spawnTime = time;

		mAnimator = AddComponent<Animator>();
		Img* mImage = RscManager::Load<Img>(name, path);
		if (mImage->GetHeight() < 40)
			mImage->MatchRatio(asRatio);

		mAnimator->CreateAnimation(name, mImage, Vector2::Zero, 8, 1, 8, Vector2::Zero, 0.075);
		mAnimator->Play(name, true);
	}
	Bird::~Bird()
	{

	}
	void Bird::Initialize()
	{
	}
	void Bird::Update()
	{
		if (isSpawn == false)
			return;
		
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		pos.x += 100.0f * Time::DeltaTime();
		tr->SetPos(pos);
		GameObject::Update();

		if (this == nullptr)
			return;
		if (pos.x >= application.GetWidth() + 36.0f)
		{
			object::Destroy(this);
		}
	}
	void Bird::Render(HDC hdc)
	{

		if (isSpawn == false)
			return;
		GameObject::Render(hdc);
	}
	void Bird::Release()
	{
	}
	
	void Bird::setAnimation(const std::wstring name, const std::wstring path, Vector2 asRatio)
	{
		/*mAnimator = AddComponent<Animator>();
		Img* mImage = RscManager::Load<Img>(name, path);
		if (mImage->GetHeight() < 40)
			mImage->MatchRatio(asRatio);
		
		mAnimator->CreateAnimation(name, mImage, Vector2::Zero, 8, 1, 8, Vector2::Zero, 0.075);
		mAnimator->Play(name, true);*/
	}
}

