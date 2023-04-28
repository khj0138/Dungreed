#include "hjIceBullet.h"
#include "hjTransform.h"
#include "hjTime.h"
#include "hjEmpty.h"
#include "hjTile.h"
#include "hjHero.h"
#include "hjWeapon.h"


hj::IceBullet::IceBullet()
{
}

hj::IceBullet::~IceBullet()
{
}

void hj::IceBullet::Initialize()
{
}

void hj::IceBullet::Update()
{
	Transform* tr = GetComponent<Transform>();
	tr->SetPos(
		tr->GetPos()
		+ GetStat().direction.Normalize() * 500.0f * Time::DeltaTime()
	);
	Collider* col = GetComponent<Collider>();
	col->SetPos(
		tr->GetPos()
		+ Vector2{ (float)(mImage->GetWidth()) / 2.f, (float)(mImage->GetHeight()) / 2.f } // »≠ªÏ ¡ﬂæ”¿∏∑Œ collider ¿Ãµø
		+ col->GetCenter() // collider¿« ¡ﬂæ” ∏¬√„
	
	);
}

void hj::IceBullet::Render(HDC hdc)
{
	Vector2 dir = math::Rotate(GetStat().direction, 90.0f);
	float mSpawnDegree = atan2(dir.y, dir.x) / PI * 180.0f;
	GetComponent<SpriteRenderer>()->rotateRender(hdc, mImage->GetHdc(), Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() }, mSpawnDegree, GetComponent<Transform>()->GetPos());
	GetComponent<Collider>()->Render(hdc);
}

void hj::IceBullet::Release()
{
}

void hj::IceBullet::Create()
{
	mImage = RscManager::Load<Img>(L"IceBullet", L"..\\Resource\\Char\\Ice\\IceBullet.bmp");
	mImage->MatchRatio(Vector2::One * 4.f);
	GetComponent<Transform>()->SetSize(Vector2::One * (float)(mImage->GetWidth()));
	GetComponent<SpriteRenderer>()->SetRotate(true);
	Collider* col = GetComponent<Collider>();
	col->SetSize(Vector2{ (float)(mImage->GetWidth()), (float)(mImage->GetHeight()) } / 2.f);
	col->SetCenter(
		Vector2{ col->GetSize().x / -2.f, col->GetSize().y / -2.f }
	);
}

void hj::IceBullet::Spawn(Vector2 pos)
{
	SetState(GameObject::eState::Active);
	GetComponent<Transform>()->SetPos(pos
	);
}

void hj::IceBullet::OnCollisionEnter(Collider* other)
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
		return;
	}
	Weapon* weapon = dynamic_cast<Weapon*>(other->GetOwner());
	if (weapon != nullptr)
	{
		if (weapon->GetBCollision())
		{
			SetState(GameObject::eState::Pause);
		}
	}
}

