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
		+ GetStat().direction.Normalize() * mSpeed * Time::DeltaTime()
	);

	if (bossRoom)
	{
		Vector2 pos = tr->GetPos();
		if (pos.x >= 2480 ||
			pos.x <= 80 ||
			pos.y >= 1120 ||
			pos.y <= 160)
		{
			mEffects->CreateEffect(L"IceBulletFX");
			SetState(GameObject::eState::Pause);
		}
	}
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

	mEffects = new Emanager();
	mEffects->SetOwner(this);
	//mEffects->RegisterEffect(L"IceBulletFX", L"..\\Resource\\Ice\\IceBulletEnter.bmp", false, false, 3, Vector2{ 0.0f, 0.0f }, 0.02f, Vector2::One * 4.0f, false);
	mEffects->RegisterEffect(L"IceBulletFX", L"..\\Resource\\Ice\\IceBulletExit.bmp", false, false, 3, Vector2{ 0.0f, 0.0f }, 0.02f, Vector2::One * 4.0f, false);
	
}

void hj::IceBullet::Spawn(Vector2 pos)
{
	SetState(GameObject::eState::Active);
	GetComponent<Transform>()->SetPos(pos);
	mEffects->CreateEffect(L"IceBulletFX");
}

void hj::IceBullet::OnCollisionEnter(Collider* other)
{
	Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
	if (tile != nullptr)
	{
		if (tile->Index() <= 2)
		{
			SetState(GameObject::eState::Pause);
			mEffects->CreateEffect(L"IceBulletFX");
		}
		return;
	}
	Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
	if (hero != nullptr)
	{
		hero->Attack(this);
		SetState(GameObject::eState::Pause);
		mEffects->CreateEffect(L"IceBulletFX");
		return;
	}
	Weapon* weapon = dynamic_cast<Weapon*>(other->GetOwner());
	if (weapon != nullptr)
	{
		if (weapon->GetBCollision())
		{
			SetState(GameObject::eState::Pause);
			mEffects->CreateEffect(L"IceBulletFX");
		}
	}
}

