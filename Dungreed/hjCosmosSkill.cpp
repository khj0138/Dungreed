#include "hjCosmosSkill.h"
#include "hjTransform.h"
#include "hjTime.h"
#include "hjEmpty.h"
#include "hjTile.h"
#include "hjMonster.h"
#include "hjWeapon.h"
#include "hjEmanager.h"


hj::CosmosSkill::CosmosSkill()
{
}

hj::CosmosSkill::~CosmosSkill()
{
}

void hj::CosmosSkill::Initialize()
{
}

void hj::CosmosSkill::Update()
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
			mEffects->CreateEffect(L"CosmosSkillFX");
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

void hj::CosmosSkill::Render(HDC hdc)
{
	Vector2 dir = math::Rotate(GetStat().direction, 90.0f);
	float mSpawnDegree = atan2(dir.y, dir.x) / PI * 180.0f;
	GetComponent<SpriteRenderer>()->rotateRender(hdc, mImage->GetHdc(), Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() }, mSpawnDegree, GetComponent<Transform>()->GetPos());
	GetComponent<Collider>()->Render(hdc);
}

void hj::CosmosSkill::Release()
{
}

void hj::CosmosSkill::Create()
{
	mImage = RscManager::Load<Img>(L"CosmosSkill", L"..\\Resource\\Char\\Ice\\CosmosSkill.bmp");
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
	//mEffects->RegisterEffect(L"CosmosSkillFX", L"..\\Resource\\Ice\\CosmosSkillEnter.bmp", false, false, 3, Vector2{ 0.0f, 0.0f }, 0.02f, Vector2::One * 4.0f, false);
	mEffects->RegisterEffect(L"CosmosSkillFX", L"..\\Resource\\Ice\\CosmosSkillExit.bmp", false, false, 3, Vector2{ 0.0f, 0.0f }, 0.02f, Vector2::One * 4.0f, false);

}

void hj::CosmosSkill::Spawn(Vector2 pos)
{
	SetState(GameObject::eState::Active);
	GetComponent<Transform>()->SetPos(pos);
	mEffects->CreateEffect(L"CosmosSkillFX");
}

void hj::CosmosSkill::OnCollisionEnter(Collider* other)
{
	Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
	if (tile != nullptr)
	{
		if (tile->Index() <= 2)
		{
			SetState(GameObject::eState::Pause);
			mEffects->CreateEffect(L"CosmosSkillFX");
		}
		return;
	}
	Monster* monster = dynamic_cast<Monster*>(other->GetOwner());
	if (monster != nullptr)
	{
		monster->Attack(this);
		//SetState(GameObject::eState::Pause);
		mEffects->CreateEffect(L"CosmosSkillFX");
		return;
	}
	Weapon* weapon = dynamic_cast<Weapon*>(other->GetOwner());
	if (weapon != nullptr)
	{
		if (weapon->GetBCollision())
		{
			//SetState(GameObject::eState::Pause);
			mEffects->CreateEffect(L"CosmosSkillFX");
		}
	}
}

