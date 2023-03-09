#include "hjBird.h"
//#include "hjAnimator.h"
#include "hjRscManager.h"
#include "hjTime.h"

namespace hj
{
	Bird::Bird(double time, Vector2 pos)
		: spawnPos(0.0f, 0.0f)
		, spawnTime(0.0f)
		, isSpawn(false)
	{
		spawnPos = pos;
		spawnTime = time;
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
	}
	void Bird::Render(HDC hdc)
	{
		if (isSpawn == false)
			return;
		mAnimator;
		GameObject::Render(hdc);
	}
	void Bird::Release()
	{
	}
	void Bird::setAnimation(const std::wstring name, const std::wstring path)
	{
		mAnimator = AddComponent<Animator>();
		Image* mImage = RscManager::Load<Image>(name, path);
		mAnimator->CreateAnimation(name, mImage, Vector2::Zero, 8, 1, 8, Vector2::Zero, 0.1);
		mAnimator->Play(name, true);
	}
}

