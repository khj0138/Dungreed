#include "hjSkelIceMagician.h"
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
#include "hjWmanager_mon.h"

#include "hjSceneManager.h"
#include "hjPSceneManager.h"
#include "hjScene.h"
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjWeapon.h"
#include "hjIceBullet.h"
#include "hjNiflheim.h"
#include "hjMonsterHPBar.h"

extern hj::Application application;

namespace hj
{
	SkelIceMagician::SkelIceMagician()
	{
	}
	SkelIceMagician::~SkelIceMagician()
	{
		/*delete mWeapons;
		mWeapons = nullptr;

		delete mEffects;
		mEffects = nullptr;*/

		for (int i = 5; i >= 0; i--)
		{
			Scene* scene = SceneManager::GetActiveScene();
			{
				if (!(scene->LayerEmpty(eLayerType::Bullet_Player)))
				{

					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Bullet_Player));

					auto it = std::find(temp.begin(), temp.end(), mBullets[i]);
					if (it != temp.end())
					{
						temp.erase(it);
					}
				}
			}
			//delete mBullets[i];
			mBullets[i] = nullptr;
		}
		mBullets.clear();
	}
	void SkelIceMagician::Initialize()
	{
		Monster::SetStat(50.0f, 50.0f);
		
		coolTime = 5.0f;
		mAnimator = AddComponent<Animator>();
		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		Img* mImage = RscManager::Load<Img>(L"SkelIceMagician", L"..\\Resource\\Monster\\SkelIceMagician.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 12.0f;
		size.y = mImage->GetHeight() / 4.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		//tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		//tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 12, 4, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 12, 4, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Attack", mImage, size * Vector2{ 0.0f, (float)index++ }, 12, 4, 12, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedAttack", mImage, size * Vector2{ 0.0f, (float)index++ }, 12, 4, 12, Vector2::Zero, 0.1);

		SetFlip(false);
		StateChange(eSkelIceMagicianState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ GetComponent<Transform>()->GetSize() });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);
		mRigidbody->SetGravity(false);
		mRigidbody->SetGround(false);

		for (int i = 0; i < 6; i++)
		{
			IceBullet* newBullet = new IceBullet();
			newBullet->Create();
			newBullet->SetState(GameObject::eState::Pause);
			mBullets.push_back(newBullet);
			Scene* scene = SceneManager::GetActiveScene();
				scene->AddGameObject((GameObject*)newBullet, eLayerType::Bullet_Monster);
		}

		GameObject::Initialize();

		Monster::Initialize();
	}
	void SkelIceMagician::Update()
	{
		if (GetStat().HP == 0)
		{
			Monster::SetBAttack(false);
			
			GameObject::SetState(eState::Pause);
		}
		else
		{
			Monster::SetBAttack(true);
		}
		switch (mState)
		{
		case eSkelIceMagicianState::Idle:
		{
			idle();
			break;
		}

		case eSkelIceMagicianState::Attack:
		{
			attack();
			break;
		}
		}

		Monster::Update();
	}
	void SkelIceMagician::Render(HDC hdc)
	{
		Monster::Render(hdc);
	}
	void SkelIceMagician::Release()
	{
		Monster::Release();
	}

	void SkelIceMagician::OnCollisionEnter(Collider* other)
	{
	}

	void SkelIceMagician::OnCollisionStay(Collider* other)
	{
	}

	void SkelIceMagician::OnCollisionExit(Collider* other)
	{

	}

	void SkelIceMagician::StateChange(eSkelIceMagicianState state, std::wstring anim, bool loop)
	{
		mState = state;

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
		mAnimator->Reset();
	}
	void SkelIceMagician::idle()
	{
		mTime += Time::DeltaTime();
		if (GetHero() != nullptr)
		{
			heroPos = GetHero()->GetComponent<Transform>()->GetPos()
				+ Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y * -0.5f };
		}
		if (mTime >= coolTime)
		{
			if (heroPos.x >= GetComponent<Transform>()->GetPos().x)
				SetFlip(false);
			else
				SetFlip(true);
			mTime = 0.0f;
			StateChange(eSkelIceMagicianState::Attack, L"Attack", false);
			index = 0;
		}
	}
	void SkelIceMagician::attack()
	{
		mTime += Time::DeltaTime();

		if(mTime >= ((float)(index) * 0.3f) && index < 6)
		{
			mBullets[index]->SetStat(5.0f, math::Rotate(Vector2::Right, -60.f * (float)index).Normalize());
			Vector2 spawnPos = heroPos
				- mBullets[index]->GetComponent<Collider>()->GetSize() / 2.f
				- Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y * 1.5f}
			+ mBullets[index]->GetStat().direction * mBullets[index]->GetComponent<Collider>()->GetSize();

			//mBullets[index]->GetComponent<Collider>()->SetPos(spawnPos + mBullets[index]->GetComponent<Collider>()->GetSize() / 2.f);
			mBullets[index]->GetComponent<Transform>()->SetPos(spawnPos);
			mBullets[index]->Spawn(spawnPos // 2.f
			);
			mBullets[index]->SetSpeed(0.0f);
			//mBullets[index]->GameObject::SetState(eState::Wait);
			index++;
		}
		else if (index == 6)
		{
			for (int i = 0; i < 6; i++)
			{
				mBullets[i]->SetSpeed(750.0f);
				//mBullets[i]->GameObject::SetState(eState::Active);
			}
			mTime = 0.0f;
			StateChange(eSkelIceMagicianState::Idle, L"Idle", true);
		}
	}
}