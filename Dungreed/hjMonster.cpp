#include "hjMonster.h"
#include "hjTime.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjAnimator.h"
#include "hjCollider.h"
#include "hjCamera.h"
#include "hjMouse.h"
#include "hjApplication.h"
#include "hjRigidBody.h"
#include "hjEffect.h"
#include "hjTile.h"
#include "hjMath.h"
#include "hjLayer.h"
#include "hjTown.h"
#include "hjTile.h"

#include "hjSceneManager.h"
#include "hjScene.h"
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjWeapon.h"
#include "hjBaseBullet.h"
#include "hjWmanager_mon.h"
#include "hjMonsterHPBar.h"
#include "hjEmanager.h"

extern hj::Application application;

namespace hj
{
	Monster::Monster()
		: stat({1, 1})
	{
	}
	Monster::~Monster()
	{
		//delete mEffects;
		mEffects = nullptr;
		if (hpBar != nullptr)
		{
			delete hpBar;
			hpBar = nullptr;
		}
	}
	void Monster::Initialize()
	{
		hpBar = new MonsterHPBar(L"MonsterHPBar", L"..\\Resource\\Monster\\LifeBar", Vector2::One);
		hpBar->Initialize();
		hpBar->SetMonster(this);
		GameObject::Initialize();

		mEffects = new Emanager();
		mEffects->SetOwner(this);
		mEffects->RegisterEffect(L"DieEffect", L"..\\Resource\\Monster\\DieFX.bmp", false, false, 11,
			Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y },
			0.05f, Vector2::One * 2.f, false);
		/*
		mEffects->RegisterEffect(L"HitEffect", L"..\\Resource\\Char\\HitFX.bmp", false, true, 6,
			Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / -2.f },
			0.01f, Vector2::One * 2.f);*/
	}
	void Monster::Update()
	{
		GameObject::Update();
		hpBar->Update();
		if (GetHero() == nullptr)
		{
			Scene* b = SceneManager::FindScene(eSceneType::Play);
			PlayScene* c = dynamic_cast<PlayScene*>(b);
			PlayScene* a = dynamic_cast<PlayScene*>(SceneManager::FindScene(eSceneType::Play));
			if (a == nullptr)
				return;
			SetHero(a->GetHero());
			if (GetHero() == nullptr)
				return;
		}
		if (GetStat().HP == 0)
		{
			mEffects->CreateEffect(L"DieEffect");

			SetState(eState::Pause);
			GetHero()->monsterNum--;
		}
	}
	void Monster::Render(HDC hdc)
	{
		GameObject::Render(hdc);
		hpBar->Render(hdc);
	}
	void Monster::Release()
	{
		/*GameObject::Release();*/
	}

	void Monster::OnCollisionEnter(Collider* other)
	{
		/*Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();
		if ((GetHeroPos().y - pos.y ) >= (size.y * 2))
		{
			downJump(other);
		}*/
	}

	void Monster::OnCollisionStay(Collider* other)
	{
	}

	void Monster::OnCollisionExit(Collider* other)
	{

	}

	void Monster::StateChange(eMonsterState state, std::wstring anim, bool loop = false)
	{
		
	}
	void Monster::idle()
	{
		
	}
	void Monster::run()
	{
		
	}
	void Monster::jump()
	{

		

	}
	void Monster::Flip(std::wstring Anim)
	{
		if (GetFlip())
		{
			std::wstring flipAnim = L"Flipped";
			flipAnim.append(Anim);
			mAnimator->Flip(flipAnim);
		}
		else
			mAnimator->Flip(Anim);
	}
	void Monster::downJump(Collider* other)
	{
		Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
		if (tile != nullptr)
		{
			if ((tile->Index() == 3 || tile->Index() == 4 || tile->Index() == 5)
				&& (mState != eMonsterState::Jump))
			{
				mRigidbody->SetGround(false);
				GetComponent<Transform>()->SetPos(
					Vector2{
						GetComponent<Transform>()->GetPos().x,
						GetComponent<Transform>()->GetPos().y + 1200.0f * 0.02f
					}
				);

				prevPos = GetComponent<Transform>()->GetPos();
				mRigidbody->SetGravity(false);
				StateChange(eMonsterState::Jump, L"Jump", true);
				cJump--;

			}
		}
	}
	void Monster::Attack(Weapon* attacker)
	{
		if (bAttack == true)
		{
			bAttack = false;
			//mEffects->CreateEffect(L"HitEffect", attacker->GetDir());
			Damage(attacker->GetStat().power);
		}
	}
	void Monster::Attack(BaseBullet* attacker)
	{
		if (bAttack == true)
		{
			bAttack = false;
			Damage(attacker->GetStat().power);
		}
	}
	void Monster::SetState(GameObject::eState type)
	{
	}
}