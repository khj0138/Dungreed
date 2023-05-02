#include "hjIcePillar.h"
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
	IcePillar::IcePillar()
	{
	}
	IcePillar::~IcePillar()
	{
		/*delete mWeapons;
		mWeapons = nullptr;

		delete mEffects;
		mEffects = nullptr;*/

		for (int i = 3; i >= 0; i--)
		{
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			{
				if (!(scene->LayerEmpty(eLayerType::Bullet_Player)))
				{

					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Bullet_Player));

					auto it = std::find(temp.begin(), temp.end(), mBullets[i]);
					if (it != temp.end())
					{
						temp.erase(it);
						continue;
					}
				}
			}
			delete mBullets[i];
			mBullets[i] = nullptr;
		}
		mBullets.clear();
	}
	void IcePillar::Initialize()
	{
		Monster::SetStat(100.0f, 100.0f);
		hpBar = new MonsterHPBar(L"MonsterHPBar", L"..\\Resource\\Monster\\LifeBar", Vector2::One * 2.f);
		hpBar->Initialize();
		hpBar->SetMonster(this);



		mAnimator = AddComponent<Animator>();
		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"IcePillar", L"..\\Resource\\Monster\\IcePillar.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 20.0f;
		size.y = mImage->GetHeight() / 3.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		//tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		//tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Enter", mImage, size * Vector2{ 0.0f, (float)index++ }, 20, 3, 20, Vector2::Zero, 0.03);
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 20, 3, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Exit", mImage, size * Vector2{ 0.0f, (float)index++ }, 20, 3, 20, Vector2::Zero, 0.03);
		
		SetFlip(false);
		StateChange(eIcePillarState::Idle);

		AnimPlay(L"Enter", false);
		mAnimator->GetCompleteEvent(L"Enter") = std::bind(&IcePillar::enterCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"Exit") = std::bind(&IcePillar::exitCompleteEvent, this);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ GetComponent<Transform>()->GetSize() });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-0.5f) * colSize.y });

		/*mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);
		mRigidbody->SetGround(false);*/

		for (int i = 0; i < 180; i++)
		{

			IceBullet* newBullet = new IceBullet();
			newBullet->Create();
			newBullet->SetState(GameObject::eState::Pause);
			mBullets.push_back(newBullet);
			newBullet->SetBossRoom(true);
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
				scene->AddGameObject((GameObject*)newBullet, eLayerType::Bullet_Monster);
			
		}

		GameObject::Initialize();
		GetComponent<SpriteRenderer>()->SetRotate(true);
		Emanager* emanager = new Emanager();
		SetEmanager(emanager);
		GetEmanager()->SetOwner(this);
		GetEmanager()->RegisterEffect(L"HitEffect", L"..\\Resource\\Char\\BossHitFX.bmp", false, true, 5,
			Vector2::Zero,
			0.01f, Vector2::One * 1.f);
	}
	void IcePillar::Update()
	{
		if (GetStat().HP == 0 && mState != eIcePillarState::Pause)
		{
			Monster::SetBAttack(false);
			SetBAttack(false);
			SetState(eState::Pause);
		}
		if (mState != eIcePillarState::Pause)
			Monster::SetBAttack(true);

		switch (mState)
		{
		case eIcePillarState::Idle:
		{
			idle();
			break;
		}
		case eIcePillarState::AttackWait:
		{
			attackWait();
			break;
		}
		case eIcePillarState::Attack:
		{
			attack();
			break;
		}
		}
		GetComponent<SpriteRenderer>()->SetDir(math::Rotate(GetDir(), 0.0f));
		GetComponent<Animator>()->Update();
		GetComponent<Collider>()->Update();
		hpBar->Update();
		
		
	}
	void IcePillar::Render(HDC hdc)
	{
		/*GetComponent<Animator>()->Render(hdc);
		GetComponent<Collider>()->Render(hdc);*/
		GameObject::Render(hdc);
		hpBar->Render(hdc);
	}
	void IcePillar::Release()
	{
		GameObject::Release();
	}

	void IcePillar::OnCollisionEnter(Collider* other)
	{
	}

	void IcePillar::OnCollisionStay(Collider* other)
	{
	}

	void IcePillar::OnCollisionExit(Collider* other)
	{

	}

	void IcePillar::StateChange(eIcePillarState state)
	{
		mState = state;
	}
	void IcePillar::AnimPlay(std::wstring anim, bool loop)
	{
		mAnimator ->Play(anim, loop);

		mAnimator->Reset();
	}
	void IcePillar::SetState(GameObject::eState type)
	{
		if (type == eState::Pause)
		{
			StateChange(eIcePillarState::Pause);
			Monster::SetBAttack(false);
			SetBAttack(false);
			AnimPlay(L"Exit", false);
		}
		else if (type == eState::Active)
		{
			GameObject::SetState(type);
			StateChange(eIcePillarState::Idle);
			AnimPlay(L"Enter", false);
		}
	}
	void IcePillar::enterCompleteEvent()
	{
		AnimPlay(L"Idle", false);
	}
	void IcePillar::exitCompleteEvent()
	{
		GameObject::SetState(GameObject::eState::Pause);
	}
	void IcePillar::idle()
	{	
		mTime = 0.0f;
		index = 0;
		if (bAttack)
			StateChange(eIcePillarState::AttackWait);
	}
	void IcePillar::attackWait()
	{
		mTime += Time::DeltaTime();
		if (mTime > cmd.delayTime)
		{
			mTime = 0.0f;
			StateChange(eIcePillarState::Attack);
		}
	}
	void IcePillar::attack()
	{
		if (bAttack == false)
		{
			mTime = 0.0f;
			StateChange(eIcePillarState::Idle);
		}
		else
		{
			mTime += Time::DeltaTime();
		}
		if (cmd.zenTime > 0.03f)
		{
			if (
				mTime > ((float)index * cmd.zenTime)
				&& (index < cmd.attackLimit)
				)
			{
				mBullets[index]->SetSpeed(cmd.attackSpeed);
				mBullets[index]->SetStat(5.0f, cmd.attackDir);
				mBullets[index]->Spawn(
					GetComponent<Transform>()->GetPos()
					- Vector2{ mBullets[index]->GetComponent<Collider>()->GetSize() }
				);
				index++;
			}
		}
		else
		{
			if (
				mTime > ( (float)index * cmd.zenTime)
				&& (index < cmd.attackLimit)
				)
			{
				if (index % 3 == 0)
				{
					mBullets[index]->SetSpeed(cmd.attackSpeed);
					mBullets[index]->Spawn(
						GetComponent<Transform>()->GetPos()
						- Vector2{ mBullets[index]->GetComponent<Collider>()->GetSize() }

					);
					mBullets[index]->SetStat(5.0f, cmd.attackDir);
					index++;
				}
				else if (index % 3 == 1)
				{
					mBullets[index]->SetSpeed(cmd.attackSpeed);

					Vector2 spawnPos = GetComponent<Transform>()->GetPos()
						- Vector2{ mBullets[index]->GetComponent<Collider>()->GetSize() }
					- Vector2(GetDir().y, GetDir().x * -1.f) * GetComponent<Collider>()->GetSize().x / 4.f;

					cmd.attackDir = (GetHero()->GetComponent<Transform>()->GetPos()
						- Vector2{ 0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
					- spawnPos).Normalize();

					mBullets[index]->SetStat(5.0f
						, cmd.attackDir);
					mBullets[index]->Spawn(spawnPos);
					index++;
				}
				else if (index % 3 == 2)
				{
					mBullets[index]->SetSpeed(cmd.attackSpeed);
					Vector2 spawnPos = GetComponent<Transform>()->GetPos()
						- Vector2{ mBullets[index]->GetComponent<Collider>()->GetSize() }
					+ Vector2(GetDir().y, GetDir().x * -1.f) * GetComponent<Collider>()->GetSize().x / 4.f;

					cmd.attackDir = (GetHero()->GetComponent<Transform>()->GetPos()
						- Vector2{ 0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
					- spawnPos).Normalize();

					mBullets[index]->SetStat(5.0f
						, cmd.attackDir);
					mBullets[index]->Spawn(spawnPos);
					index++;
				}
			}
		}
		
	}
}