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
		mAnimator = AddComponent<Animator>();
		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"IcePillar", L"..\\Resource\\Monster\\IcePillar.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 20.0f;
		size.y = mImage->GetHeight() / 2.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		//tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		//tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Enter", mImage, size * Vector2{ 0.0f, (float)index++ }, 20, 2, 20, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 20, 2, 1, Vector2::Zero, 0.1);
		
		SetFlip(false);
		StateChange(eIcePillarState::Idle);

		AnimPlay(L"Enter", false);
		mAnimator->GetCompleteEvent(L"Enter") = std::bind(&IcePillar::enterCompleteEvent, this);

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
	}
	void IcePillar::Update()
	{
		GetComponent<SpriteRenderer>()->SetDir(math::Rotate(GetDir(), 0.0f));
		GetComponent<Animator>()->Update();
		GetComponent<Collider>()->Update();
		if (bAttack && cmd.zenTime > 0.03f)
		{
			mTime += Time::DeltaTime();
			if (
				mTime > (cmd.delayTime + (float)index * cmd.zenTime)
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
		else if (bAttack)
		{
			mTime += Time::DeltaTime();
			if (
				mTime > (cmd.delayTime + (float)index * cmd.zenTime)
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
		else
		{
			mTime = 0.0f;
			index = 0;
		}
		/*switch (GetNiflheim()->GetNFState())
		{
		case (Niflheim::eNiflheimState::Idle):
		{
			idle();
			break;
		}
		}*/
		//SetBAttack(true);m

		//mTime += Time::DeltaTime();
		//if (GetHero() == nullptr)
		//{
		//	Scene* b = SceneManager::FindScene(eSceneType::Play);
		//	PlayScene* c = dynamic_cast<PlayScene*>(b);
		//	PlayScene* a = dynamic_cast<PlayScene*>(SceneManager::FindScene(eSceneType::Play));
		//	if (a == nullptr)
		//		return;
		//	SetHero(a->GetHero());
		//	if (GetHero() == nullptr)
		//		return;
		//}
		//prevPos = GetComponent<Transform>()->GetPos();



		//if (Input::GetKey(eKeyCode::R))
		//{
		//	GetComponent<Transform>()->SetPos(Vector2{
		//		GetComponent<Transform>()->GetPos().x,
		//		200.0f
		//		});
		//}

		//if (GetHero() != nullptr)
		//{
		//	heroPos = GetHero()->GetComponent<Transform>()->GetPos();

		//	Transform* tr = GetComponent<Transform>();
		//	Vector2 pos = tr->GetPos();
		//	if (heroPos.x > pos.x)
		//		SetFlip(false);
		//	else
		//		SetFlip(true);
		//}
		//if (isJump == false)
		//{
		//	if (!(GetComponent<Rigidbody>()->GetGround()))
		//		isJump = true;
		//}
		//else if (!(GetComponent<Rigidbody>()->GetGround()))
		//{
		//	if (mState != eIcePillarState::Jump)
		//	{
		//		StateChange(eIcePillarState::Jump, L"Jump", true);
		//	}
		//}
		//else
		//{
		//	isJump = false;
		//}

		//switch (mState)
		//{

		//case hj::IcePillar::eIcePillarState::Idle:
		//	idle();
		//	break;
		//case hj::IcePillar::eIcePillarState::Run:
		//	run();
		//	break;
		//case hj::IcePillar::eIcePillarState::Jump:
		//	jump();
		//	break;
		//case hj::IcePillar::eIcePillarState::AttackWait:
		//	attackWait();
		//	break;
		//case hj::IcePillar::eIcePillarState::AttackReload:
		//	attackReload();
		//	break;
		//case hj::IcePillar::eIcePillarState::SkillAttackWait:
		//	skillAttackWait();
		//	break;
		//case hj::IcePillar::eIcePillarState::SkillAttackReload:
		//	skillAttackReload();
		//	break;
		//}

		//Transform* tr = GetComponent<Transform>();
		//Vector2 size = tr->GetSize();

		//if (mState != hj::IcePillar::eIcePillarState::SkillAttackWait &&
		//	mState != hj::IcePillar::eIcePillarState::SkillAttackReload)
		//	mWeapons->Update();
		////mEffects->Update();
		//GameObject::Update();
	}
	void IcePillar::Render(HDC hdc)
	{
		GetComponent<Animator>()->Render(hdc);
		GetComponent<Collider>()->Render(hdc);
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
		GameObject::SetState(type);
	}
	void IcePillar::enterCompleteEvent()
	{
		AnimPlay(L"Idle", false);
	}
	void IcePillar::idle()
	{	
		GetComponent<Transform>()->SetPos(mOwner->GetComponent<Transform>()->GetPos()
			+ Vector2{ 0.0f, mOwner->GetComponent<Collider>()->GetSize().y * -0.5f }
			+ GetDir() * 200.0f // 원 중심을 기준으로 그림 위치 회전
		);
	}
	void IcePillar::attackWait()
	{
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity.x = 0.0f;
		mRigidbody->SetVelocity(velocity);

		
	}
	void IcePillar::attack()
	{
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity.x = 0.0f;
		mRigidbody->SetVelocity(velocity);

		
	}
}