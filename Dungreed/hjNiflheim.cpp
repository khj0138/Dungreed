#include "hjNiflheim.h"
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
#include "hjIcePillar.h"
#include "hjBossHPBar.h"
extern hj::Application application;

namespace hj
{
	Niflheim::Niflheim()
		: Degree(0)
	{
	}
	Niflheim::~Niflheim()
	{
		//delete mWeapons;
		mWeapons = nullptr;

		delete mEffects;
		mEffects = nullptr;
		delete hpBar;
		hpBar = nullptr;

		for (int i = 3; i >= 0; i--)
		{
			Scene* scene = SceneManager::GetActiveScene();
			if (!(scene->LayerEmpty(eLayerType::Bullet_Player)))
			{

				std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Bullet_Player));

				auto it = std::find(temp.begin(), temp.end(), mPillars[i]);
				if (it != temp.end())
				{
					temp.erase(it);
					continue;
				}
			}
			
			
			//delete mPillars[i];
			mPillars[i] = nullptr;
		}
		mPillars.clear();
	}
	void Niflheim::Initialize()
	{
		SetStat(400, 400);
		bAttack[0] = true;
		bAttack[1] = true;
		bAttack[2] = false;
		bAttack[3] = false;
		bAttack[4] = true;

		mAnimator = AddComponent<Animator>();

		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 3.f;
		Img* mImage = RscManager::Load<Img>(L"Niflheim", L"..\\Resource\\Monster\\NIflheim.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 30.0f;
		size.y = mImage->GetHeight() / 5.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		tr->SetPos(Vector2{ 1280.0f, 560.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Attack", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 11, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Die", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 30, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Enter", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 16, Vector2::Zero, 0.03);
		mAnimator->CreateAnimation(L"Exit", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 16, Vector2::Zero, 0.03);

		SetFlip(false);
		StateChange(eNiflheimState::Idle);
		AnimPlay(L"Idle", true);

		mAnimator->GetCompleteEvent(L"Exit") = std::bind(&Niflheim::ExitCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"Enter") = std::bind(&Niflheim::EnterCompleteEvent, this);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 80.0f, 120.0f });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);
		mRigidbody->SetGravity(false);

		//struct Vector2 temp[4];
		pillarVector.resize(5);
		pillarDir.resize(5);
		for (int i = 0; i < 4; i++)
		{
			pillarVector[0].push_back(Vector2::Zero);
			pillarVector[3].push_back(Vector2::Zero);
			pillarVector[4].push_back(Vector2::Zero);
			pillarDir[0].push_back(Vector2::Zero);
			pillarDir[1].push_back(Vector2::Up);
			pillarDir[2].push_back(Vector2::Up);
			pillarDir[3].push_back(Vector2::Zero);
			pillarDir[4].push_back(Vector2::Zero);
		}
		pillarVector[1].push_back(Vector2{ 760.0f, 520.f });
		pillarVector[1].push_back(Vector2{ 760.0f, 760.f });
		pillarVector[1].push_back(Vector2{ 1800.0f, 520.f });
		pillarVector[1].push_back(Vector2{ 1800.0f, 760.f });
		pillarVector[2].push_back(Vector2{ 770.0f, 520.f });
		pillarVector[2].push_back(Vector2{ 1110.0f, 520.f });
		pillarVector[2].push_back(Vector2{ 1450.0f, 520.f });
		pillarVector[2].push_back(Vector2{ 1790.0f, 520.f });
		/*pillarVector.push_back(temp);
		pillarDir.push_back(temp);*/
		for (int i = 0; i < 4; i++)
		{
			IcePillar* newPillar = new IcePillar();
			mPillars.push_back(newPillar);
			Scene* scene = SceneManager::GetActiveScene();
			scene->AddGameObject((GameObject*)newPillar, eLayerType::Monster);
			mPillars[i]->Initialize();

			pillarDir[0][i] = math::Rotate(Vector2::Right, i * (-90.0f)).Normalize();
			pillarDir[3][i] = math::Rotate(Vector2::Right, i * (-90.0f)).Normalize();
			mPillars[i]->SetDir(pillarDir[0][i]);

			pillarVector[0][i] =
				GetComponent<Transform>()->GetPos()
				+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
			+ pillarDir[0][i] * 200.0;
			//+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f };
			mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[0][i]);
			mPillars[i]->SetNiflheim(this);

		}
		/*pillarVector[0] = Vector2{ 760.0f, 520.f };
		pillarVector[1] = Vector2{ 760.0f, 760.f };
		pillarVector[2] = Vector2{ 1800.0f, 520.f };
		pillarVector[3] = Vector2{ 1800.0f, 760.f };*/
		//pillarVector[0] = (Vector2{ 520.0f, -120.f });
		//pillarVector[1] = Vector2{ -520.0f, -120.f };
		//pillarVector[2] = Vector2{ -520.0f, 120.f };
		//pillarVector[3] = Vector2{ 520.0f, 120.f };
		//

		//Vector2 temp1[4] = {Vector2{ 520.0f, -120.f },Vector2{ -520.0f, -120.f },Vector2{ -520.0f, 120.f },Vector2{ 520.0f, 120.f }};
		//Vector2 temp2[4] = { Vector2{ -510.0f, -120.f },Vector2{ -170.0f, -120.f },Vector2{ 170.0f, -120.f },Vector2{ 510.0f, -120.f } };

		/*temp[0] = Vector2(520.0f, -120.f );
		temp[1] = Vector2(-520.0f, -120.f);
		temp[2] = Vector2(-520.0f, 120.f);
		temp[3] = Vector2(520.0f, 120.f);
		pillarVector.push_back(temp);

		temp[0] = Vector2( - 510.0f, -120.f);
		temp[1] = Vector2(-170.0f, -120.f);
		temp[2] = Vector2(170.0f, -120.f);
		temp[3] = Vector2(510.0f, -120.f);
		pillarVector.push_back(temp);*/

		coolTime[0] = 3.0f;
		coolTime[1] = 5.0f;
		coolTime[2] = 2.0f;
		coolTime[3] = 2.0f;
		coolTime[4] = 10.0f;

		GameObject::Initialize();
		hpBar = new BossHPBar(L"BossHPBar", L"..\\Resource\\Monster\\Boss_LifeBar", Vector2::One, Vector2{ 800.0f, 800.0f });
		hpBar->Initialize();
		hpBar->SetMonster((Monster*)this);

		/*Emanager* emanager = new Emanager();
		SetEmanager(emanager);
		GetEmanager()->SetOwner(this);
		GetEmanager()->RegisterEffect(L"HitEffect", L"..\\Resource\\Char\\BossHitFX.bmp", false, true, 5,
			Vector2::Zero,
			0.01f, Vector2::One * 1.f);*/
	}
	void Niflheim::Update()
	{
		//SetBAttack(true);
		if (GetStat().HP == 0 && mState != eNiflheimState::Die)
		{
			mRigidbody->SetGround(false);
			mRigidbody->SetGravity(false);
			GetComponent<Transform>()->SetPos(Vector2{ 1280.0f, 560.0f });
			StateChange(eNiflheimState::Die);
			Camera::SetTarget(this);
			Camera::SetBVelocity(true);
			for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetState(eState::Pause);
			}
		}
		if (mState == eNiflheimState::Die && Camera::GetPos() == GetComponent<Transform>()->GetPos()
			&& GetComponent<Animator>()->GetActiveAnimation()->GetAnimationName() != L"Die")
		{
			mRigidbody->SetGravity(true);
			AnimPlay(L"Die", false);
		}
		if (GetHero() == nullptr)
		{
			/*Scene* b = SceneManager::FindScene(eSceneType::Play);
			PlayScene* c = dynamic_cast<PlayScene*>(b);*/
			PlayScene* a = dynamic_cast<PlayScene*>(SceneManager::FindScene(eSceneType::Play));
			if (a == nullptr)
				return;
			SetHero(a->GetHero());
			if (GetHero() == nullptr)
				return;
			for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetHero(GetHero());
			}
		}
		prevPos = GetComponent<Transform>()->GetPos();

		/*if (Input::GetKey(eKeyCode::R))
		{
			GetComponent<Transform>()->SetPos(Vector2{
				GetComponent<Transform>()->GetPos().x,
				200.0f
				});
		}*/

		if (GetHero() != nullptr)
		{
			heroPos = GetHero()->GetComponent<Transform>()->GetPos();
		}

		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP != 0)
			{
				break;
			}
			else if (i == 3 &&
				mState != Niflheim::eNiflheimState::Groggy &&
				mState != Niflheim::eNiflheimState::StandUp &&
				mState != Niflheim::eNiflheimState::Die)
			{
				mTime[0] = 0.0f;
				mRigidbody->SetGravity(true);
				StateChange(Niflheim::eNiflheimState::Groggy);
			}
		}

		if (mState == Niflheim::eNiflheimState::Attack1 ||
			mState == Niflheim::eNiflheimState::AttackWait1 ||
			mState == Niflheim::eNiflheimState::Attack2 ||
			mState == Niflheim::eNiflheimState::AttackWait2 ||
			mState == Niflheim::eNiflheimState::Idle)
		{
			mTime[5] += Time::DeltaTime();
			if (mTime[5] >= coolTime[4])
			{
				mTime[5] = 0.0f;
				AnimPlay(L"Exit", false);
			}
		}


		switch (mState)
		{

		case hj::Niflheim::eNiflheimState::Idle:
			idle();
			break;

		case hj::Niflheim::eNiflheimState::AttackWait1:
		{
			moveAttackWait(1);
			break;
		}
		case hj::Niflheim::eNiflheimState::AttackWait2:
		{
			moveAttackWait(2);
			break;
		}
		case hj::Niflheim::eNiflheimState::AttackWait3:
		{
teleportAttackWait(3);
break;
		}
		case hj::Niflheim::eNiflheimState::AttackWait4:
		{
			teleportAttackWait(4);
			break;
		}
		case hj::Niflheim::eNiflheimState::Attack1:
		{
			attack1();
			break;
		}
		case hj::Niflheim::eNiflheimState::Attack2:
		{
			attack2();
			break;
		}
		case hj::Niflheim::eNiflheimState::Attack3:
		{
			attack3();
			break;
		}
		case hj::Niflheim::eNiflheimState::Attack4:
		{
			attack4();
			break;
		}
		case hj::Niflheim::eNiflheimState::AttackReload:
		{
			attackReload();
			break;
		}
		case hj::Niflheim::eNiflheimState::Groggy:
		{
			groggy();
			break;
		}
		}




		//mEffects->Update();
		GameObject::Update();
	}
	void Niflheim::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		GameObject::Render(hdc);
		hpBar->Render(hdc);
	}
	void Niflheim::Release()
	{
		GameObject::Release();
	}

	void Niflheim::OnCollisionEnter(Collider* other)
	{
		Transform* tr = GetComponent<Transform>();
		Collider* col = GetComponent<Collider>();
		Vector2 pos = tr->GetPos();
		Vector2 size = Vector2::Zero;
		size.x = col->GetSize().x * 1.5f;
		size.y = col->GetSize().y;

	}

	void Niflheim::OnCollisionStay(Collider* other)
	{
	}

	void Niflheim::OnCollisionExit(Collider* other)
	{

	}

	void Niflheim::StateChange(eNiflheimState state)
	{
		mState = state;
	}
	void Niflheim::AnimPlay(std::wstring anim, bool loop)
	{
		mAnimator->Play(anim, loop);

		mAnimator->Reset();
	}
	void Niflheim::ExitCompleteEvent()
	{
		if (mState == eNiflheimState::StandUp ||
			mState == eNiflheimState::AttackWait3 ||
			mState == eNiflheimState::AttackWait4)
		{
			GetComponent<Transform>()->SetPos(Vector2{ 1280.0f, 600.0f });
		}
		else
		{
			UINT temp = ((int)(mTime[5] / 0.01f) % 4);
			switch (temp)
			{
			case 0:
			{
				GetComponent<Transform>()->SetPos(Vector2{ 480.0f, 320.0f });
				break;
			}
			case 1:
			{
				GetComponent<Transform>()->SetPos(Vector2{ 480.0f, 960.0f });
				break;
			}
			case 2:
			{
				GetComponent<Transform>()->SetPos(Vector2{ 2080.0f, 960.0f });
				break;
			}
			case 3:
			{
				GetComponent<Transform>()->SetPos(Vector2{ 2080.0f, 320.0f });
				break;
			}
			}
		}
		
		AnimPlay(L"Enter", false);
	}
	void Niflheim::EnterCompleteEvent()
	{
		for (int i = 0; i < 4; i++)
		{
			pillarVector[0][i] =
				GetComponent<Transform>()->GetPos()
				+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
			+ pillarDir[0][i] * 200.0;
		}
		if (mState == eNiflheimState::StandUp)
		{
			for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetStat(mPillars[i]->GetStat().maxHP, mPillars[i]->GetStat().maxHP);
				mPillars[i]->SetState(eState::Active);
				mPillars[i]->SetDir(pillarDir[0][i]);
				mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[0][i]);
			}

			StateChange(eNiflheimState::Idle);
		}
		if (mTime[5] >= coolTime[4])
		{
			mTime[5] = 0.0f;
		}
		AnimPlay(L"Idle", true);
	}
	void Niflheim::SetState(GameObject::eState type)
	{
		//mWeapons->SetState(type);
		GameObject::SetState(type);
	}
	void Niflheim::idle()
	{
		//mTime[0] += Time::DeltaTime();
		mTime[1] += Time::DeltaTime();
		mTime[2] += Time::DeltaTime();
		mTime[3] += Time::DeltaTime();
		mTime[4] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			Vector2 dir = mPillars[i]->GetDir();
			mPillars[i]->SetDir(math::Rotate((dir), -1.f));
			mPillars[i]->GetComponent<Transform>()->SetPos(GetComponent<Transform>()->GetPos()
				+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
				+ mPillars[i]->GetDir() * 200.0f // 원 중심을 기준으로 그림 위치 회전
				//+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f }
			);
		}
		for (int i = 4; i >= 1; i--)
		{
			if (mTime[i] >= coolTime[i - 1] && bAttack[i-1])
			{
				if (i == 3 || i == 4)
					AnimPlay(L"Exit", false);
				
				mTime[i] = 0.0f;
				StateChange(eNiflheimState(i+1));
				return;
			}
		}
		
		if (GetHero() != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Collider* col = GetComponent<Collider>();
			Vector2 pos = tr->GetPos();
			Vector2 size = Vector2::Zero;
			size.x = col->GetSize().x * 1.5f;
			size.y = col->GetSize().y;
		}
	}
	void Niflheim::moveAttackWait(UINT index)
	{
		if (mTime[index] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				mPillars[i]->SetTPPos(mPillars[i]->GetComponent<Transform>()->GetPos());
				if(index == 0)
					pillarVector[0][i] =
						GetComponent<Transform>()->GetPos()
						+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
				+ pillarDir[0][i] * 200.0;
						//+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f };
			}

		}
		else if (mTime[index] < 0.5f)
		{
			pillarMove(index, index);
		}
		else
		{
			mTime[index] = 0.0f - Time::DeltaTime();
			StateChange(eNiflheimState( 5 + index));
		}
		mTime[index] += Time::DeltaTime();
	}
	void Niflheim::teleportAttackWait(UINT index)
	{
		if (mTime[index] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				
				mPillars[i]->SetState(eState::Pause);
			}

		}
		else if (mTime[index] > 1.f)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				if (index == 3)
					pillarVector[index][i] =
					GetComponent<Transform>()->GetPos()
					+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
				+ pillarDir[index][i] * 124.0;
			}
			pillarTeleport(index, index);
			mTime[index] = 0.0f - Time::DeltaTime();
			StateChange(eNiflheimState(5 + index));
		}
		mTime[index] += Time::DeltaTime();
	}
	
	void Niflheim::attackWait3()
	{
	}
	void Niflheim::attack1()
	{
		if (mTime[1] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				mPillars[i]->SetBAttack(true);
				mPillars[i]->SetCommand(
					0.8f * (float)i
					, (heroPos
						- Vector2{0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f}
						- (mPillars[i]->GetComponent<Transform>()->GetPos())
						//+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f }
					  ).Normalize()
					, 500.0f
					, 10
					, 0.0625f
				);
				mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[1][i]);
				mPillars[i]->SetDir(pillarDir[1][i]);
			}
			index = 0;
		}
		mTime[1] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			Vector2 dir = mPillars[i]->GetDir();
			mPillars[i]->SetDir(math::Rotate((dir), -4.8f));
			if(mTime[1] >= (0.625f * (float)(index)) && index == i)
			{ 
				mPillars[i]->SetCommand(
					0.625f * (float)i
					, (heroPos
						- Vector2{ 0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (mPillars[i]->GetComponent<Transform>()->GetPos())
						//+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f }
						).Normalize()
					, 500.0f
					, 10
					, 0.0625f

				);
				index++;
			}
		}
		if (mTime[1] >= 3.0f)
		{
			mTime[1] = 0.0f;
			StateChange(eNiflheimState::AttackReload);
		}
	}
	void Niflheim::attack2()
	{
		if (mTime[2] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				mPillars[i]->SetBAttack(true);
				
				mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[2][i]);
				mPillars[i]->SetDir(pillarDir[2][i]);
			}
		}
		mTime[2] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			Vector2 dir = mPillars[i]->GetDir();
			
			//Degree = (Degree + 1) % 360;
			mPillars[i]->SetDir(math::Rotate((dir), -4.8f));
			mPillars[i]->SetCommand(
				0.0f
				, mPillars[i]->GetDir()
				, 500.0f
				, 40
				, 0.0625f
			);
		}
		if (mTime[2] >= 3.0f)
		{
			mTime[2] = 0.0f;
			StateChange(eNiflheimState::AttackReload);
		}
	}
	void Niflheim::attack3()
	{
		if (mTime[3] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				mPillars[i]->SetBAttack(true);
				/*mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[2][i]);
				mPillars[i]->SetDir(pillarDir[[i]);*/
			}
		}
		mTime[3] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			Vector2 dir = mPillars[i]->GetDir();

			//Degree = (Degree + 1) % 360;
			mPillars[i]->SetDir(math::Rotate((dir), -9.0f));
			mPillars[i]->SetCommand(
				0.0f
				, mPillars[i]->GetDir()
				, 500.0f
				, 60
				, 0.033333f
			);
			mPillars[i]->GetComponent<Transform>()->SetPos(
				GetComponent<Transform>()->GetPos()
				+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
				+ mPillars[i]->GetDir() * 124.f
			);
		}
		if(mTime[3] >= 2.5f)
		{
			mTime[3] = 0.0f;
			StateChange(eNiflheimState::AttackReload);
		}
	}
	void Niflheim::attack4()
	{
		if (mTime[4] <= 0.0000001f)
		{
			/*for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetCommand(
					0.0f
					, (heroPos
						- Vector2{ 0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (mPillars[i]->GetComponent<Transform>()->GetPos())
						).Normalize()
					, 1000.0f
					, 80
					, 0.033333f
				);
			}*/
		}
		mTime[4] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			
			mPillars[i]->SetCommand(
				0.0f
				, (heroPos
					- Vector2{ 0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
					- (mPillars[i]->GetComponent<Transform>()->GetPos())
					//- (Vector2::One * ((float)i - 2.f) * (float)((UINT)(mTime[4] * 1000.f) % 100) )

					).Normalize()
				, 1500.0f
				, 60
				, 0.03f
			);
			switch (i)
			{
			case 0:
			{
				mPillars[i]->SetDir(
					math::Rotate(
						heroPos - Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (GetComponent<Transform>()->GetPos() + Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f })
						, 0.f).Normalize());
				mPillars[i]->GetComponent<Transform>()->SetPos(
					GetComponent<Transform>()->GetPos()
					- Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / 2.f }
					+ mPillars[i]->GetDir() * 124.f
				);
				break;
			}
			case 1:
			{
				mPillars[i]->SetDir(
					math::Rotate(
						heroPos - Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (GetComponent<Transform>()->GetPos() + Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f })
						, 45.f).Normalize());
				mPillars[i]->GetComponent<Transform>()->SetPos(
					GetComponent<Transform>()->GetPos()
					- Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / 2.f }
					+ mPillars[i]->GetDir() * 124.f
					+ Vector2{ mPillars[i]->GetDir().y * -1.f, mPillars[i]->GetDir().x }*mPillars[0]->GetComponent<Collider>()->GetSize() / 2.f
				);
				break;
			}
			case 2:
			{
				mPillars[i]->SetDir(
					math::Rotate(
						heroPos - Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (GetComponent<Transform>()->GetPos() + Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f })
						, -45.f).Normalize());
				mPillars[i]->GetComponent<Transform>()->SetPos(
					GetComponent<Transform>()->GetPos()
					- Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / 2.f }
					+ mPillars[i]->GetDir() * 124.f
					+ Vector2{ mPillars[i]->GetDir().y, mPillars[i]->GetDir().x * -1.f } * mPillars[0]->GetComponent<Collider>()->GetSize() / 2.f
				);
				break;
			}
			case 3:
			{
				mPillars[i]->SetDir(
					math::Rotate(
						heroPos - Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (GetComponent<Transform>()->GetPos() + Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f })
						, 0.f).Normalize());
				mPillars[i]->GetComponent<Transform>()->SetPos(
					GetComponent<Transform>()->GetPos()
					- Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / 2.f }
					+ mPillars[i]->GetDir() * 0.f
				);
				break;
			}
			}
		
				
			
			if (mTime[4] > 1.0f)
				mPillars[i]->SetBAttack(true);
		}
		if (mTime[4] >= 3.5f)
		{
			mTime[4] = 0.0f;
			StateChange(eNiflheimState::AttackReload);
		}
	}
	void Niflheim::attackReload()
	{
		if (mTime[0] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				mPillars[i]->SetTPPos(mPillars[i]->GetComponent<Transform>()->GetPos());
				pillarVector[0][i] =
					GetComponent<Transform>()->GetPos()
					+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
				+ pillarDir[0][i] * 200.0;
					//+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f };
				mPillars[i]->SetBAttack(false);
			}
			
		}
		else if (mTime[0] < 0.501f)
		{
			pillarMove(0, 0);
		}
		else
		{
			mTime[0] = 0.0f - Time::DeltaTime();
			StateChange(eNiflheimState::Idle);
			for (int i = 0; i < 4; i++)
			{
				if (mPillars[i]->GetStat().HP == 0)
					continue;
				mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[0][i]);
				mPillars[i]->SetDir(pillarDir[0][i]);
			}
		}
		mTime[0] += Time::DeltaTime();
	}

	void Niflheim::groggy()
	{
		if (mTime[0] > 5.0f && mState != eNiflheimState::StandUp)
		{
			mTime[0] = 0.0f;
			mRigidbody->SetGravity(false);
			mRigidbody->SetGround(false);
			Monster::SetBAttack(false);
			AnimPlay(L"Exit", false);
			StateChange(eNiflheimState::StandUp);
			return;
		}
		else if ((float)(GetStat().HP) / (float)(GetStat().maxHP) <= 0.5f && 
			bAttack[2] == false &&
			bAttack[3] == false)
		{
			mTime[0] = 0.0f;
			mRigidbody->SetGravity(false);
			mRigidbody->SetGround(false);
			Monster::SetBAttack(false);
			AnimPlay(L"Exit", false);
			StateChange(eNiflheimState::StandUp);
			bAttack[2] = true;
			bAttack[3] = true;
			return;
		}
		mTime[0] += Time::DeltaTime();
		Monster::SetBAttack(true);
	}

	void Niflheim::pillarMove(UINT posIndex, UINT dirIndex)
	{

		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			Vector2 dir = mPillars[i]->GetDir();
			if (!(fabs(dir.x - pillarDir[dirIndex][i].x) < 0.1f &&fabs(dir.y- pillarDir[dirIndex][i].y) < 0.1f))
				mPillars[i]->SetDir(math::Rotate((dir), -12.f));

			if (mTime[posIndex] > 0.5f)
				mTime[posIndex] = 0.5f;
			mPillars[i]->GetComponent<Transform>()->SetPos(
				mPillars[i]->GetTPPos()
				+ (pillarVector[posIndex][i] - mPillars[i]->GetTPPos()) * (mTime[posIndex] / 0.5f)
			);

		}
	}
	void Niflheim::pillarTeleport(UINT posIndex, UINT dirIndex)
	{

		for (int i = 0; i < 4; i++)
		{
			if (mPillars[i]->GetStat().HP == 0)
				continue;
			mPillars[i]->SetDir(
				pillarDir[posIndex][i]
			);
			mPillars[i]->GetComponent<Transform>()->SetPos(
				pillarVector[posIndex][i]
			);
			mPillars[i]->SetState(eState::Active);

		}
	}
	
}/*
mon2->GetComponent<Transform>()->SetPos(Vector2{ 480.0f, 320.0f });
mon2->GetComponent<Transform>()->SetPos(Vector2{ 480.0f, 960.0f });
mon2->GetComponent<Transform>()->SetPos(Vector2{ 2080.0f, 960.0f });
mon2->GetComponent<Transform>()->SetPos(Vector2{ 2080.0f, 320.0f });*/