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
	}
	void Monster::Initialize()
	{
		hpBar = new MonsterHPBar(L"MonsterHPBar", L"..\\Resource\\Monster\\LifeBar", Vector2::One);
		hpBar->Initialize();
		hpBar->SetMonster(this);
		GameObject::Initialize();

		mEffects = new Emanager();
		mEffects->SetOwner(this);
		mEffects->RegisterEffect(L"HitEffect", L"..\\Resource\\Char\\HitFX.bmp", false, true, 6,
			Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / -2.f },
			0.01f, Vector2::One * 2.f);
	}
	void Monster::Update()
	{
		GameObject::Update();
		hpBar->Update();
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
		/*mState = state;
		if (GetFlip())
		{
			std::wstring flipAnim = L"Flipped";
			flipAnim.append(anim);
			mAnimator->Play(flipAnim, loop);
		}
		else
		{
			mAnimator->Play(anim, loop);
		}
		mAnimator->Reset();*/
	}
	void Monster::idle()
	{
		/*if (hero != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();

			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
				StateChange(eMonsterState::Run, L"Run", true);

			else
			{
				Flip(L"Idle");
			}

		}*/
	}
	void Monster::run()
	{
		/*if (hero != nullptr)
		{
			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			Vector2 size = tr->GetSize();


			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
			{

				if (GetFlip())
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = -300.0f;
					mRigidbody->SetVelocity(velocity);
				}
				else
				{
					Vector2 velocity = mRigidbody->GetVelocity();
					velocity.x = 300.0f;
					mRigidbody->SetVelocity(velocity);
				}
				Flip(L"Run");
			}

			if ((pos.y - GetHeroPos().y) >= (size.y * 2))
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.y = -1200.0f;
				mRigidbody->SetVelocity(velocity);
				mRigidbody->SetGround(false);

				StateChange(eMonsterState::Jump, L"Jump", true);
			}
			else if ((pos.y - GetHeroPos().y) <= (size.y))
			{

			}

			if (
				fabs(pos.x - GetHeroPos().x) <= (size.x) &&
				fabs(pos.y - GetHeroPos().y) <= (size.y)
				)
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 0.0f;
				mRigidbody->SetVelocity(velocity);
				StateChange(eMonsterState::Idle, L"Idle", true);
			}

		}*/
	}
	void Monster::jump()
	{

		/*Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();
		Vector2 velocity = mRigidbody->GetVelocity();
		mRigidbody->SetGravity(true);
		if (mRigidbody->GetGround())
		{
			cJump = 1;
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = 0.0f;
			mRigidbody->SetVelocity(velocity);
			StateChange(eMonsterState::Run, L"Run", true);
		}
		if (fabs(pos.x - GetHeroPos().x) >= (size.x))
		{

			if (GetFlip())
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = -300.0f;
				mRigidbody->SetVelocity(velocity);
			}
			else
			{
				Vector2 velocity = mRigidbody->GetVelocity();
				velocity.x = 300.0f;
				mRigidbody->SetVelocity(velocity);
			}
			Flip(L"Jump");
		}
		else
		{
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.x = 0.0f;
			mRigidbody->SetVelocity(velocity);
		}*/

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
			mEffects->CreateEffect(L"HitEffect", attacker->GetDir());
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