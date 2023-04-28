#include "hjArrow_mon.h"
#include "hjTransform.h"
#include "hjTime.h"
#include "hjEmpty.h"
#include "hjTile.h"
#include "hjHero.h"


hj::Arrow_mon::Arrow_mon()
{
}

hj::Arrow_mon::~Arrow_mon()
{
}

void hj::Arrow_mon::Initialize()
{
}

void hj::Arrow_mon::Update()
{
	Transform* tr = GetComponent<Transform>();
	tr->SetPos(
		tr->GetPos()
		+ GetStat().direction.Normalize() * 1500.0f * Time::DeltaTime()
	);
	Collider* col = GetComponent<Collider>();
	col->SetPos(
		tr->GetPos()
		+ Vector2{ (float)(mImage->GetWidth()) / 2.f, (float)(mImage->GetHeight()) / 2.f } // È­»ì Áß¾ÓÀ¸·Î collider ÀÌµ¿
		+ col->GetCenter() // colliderÀÇ Áß¾Ó ¸ÂÃã
		+ GetStat().direction.Normalize() * (float)(mImage->GetWidth()) / 2.f
		//+ GetStat().direction.Normalize() * mImage->GetWidth()
	);
	//GetEmpty()->GetComponent<Collider>()->SetPos(col->GetPos());
}

void hj::Arrow_mon::Render(HDC hdc)
{
	Vector2 dir = math::Rotate(GetStat().direction, 90.0f);
	float mSpawnDegree = atan2(dir.y, dir.x) / PI * 180.0f;
	GetComponent<SpriteRenderer>()->rotateRender(hdc, mImage->GetHdc(), Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() }, mSpawnDegree, GetComponent<Transform>()->GetPos());
	GetComponent<Collider>()->Render(hdc);
}

void hj::Arrow_mon::Release()
{
}

void hj::Arrow_mon::Create()
{
	mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\Bow\\Arrow.bmp");
	mImage->MatchRatio(Vector2::One * 4.f);
	GetComponent<Transform>()->SetSize(Vector2::One * (float)(mImage->GetWidth()));
	GetComponent<SpriteRenderer>()->SetRotate(true);
	Collider* col = GetComponent<Collider>();
	col->SetCenter(Vector2{ col->GetSize().x / -2.f, col->GetSize().y / -2.f });
}

void hj::Arrow_mon::Spawn(Vector2 pos)
{
	SetState(GameObject::eState::Active);
	GetComponent<Transform>()->SetPos(pos);
}

void hj::Arrow_mon::OnCollisionEnter(Collider* other)
{
	Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
	if (tile != nullptr)
	{
		if (tile->Index() <= 2)
		{
			SetState(GameObject::eState::Pause);
		}
		return;
	}
	
	Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
	if (hero != nullptr)
	{
		hero->Attack(this);
		SetState(GameObject::eState::Pause);
	}
}

