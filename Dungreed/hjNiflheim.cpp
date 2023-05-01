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
extern hj::Application application;

namespace hj
{
	Niflheim::Niflheim()
		: Degree(0)
	{
	}
	Niflheim::~Niflheim()
	{
		delete mWeapons;
		mWeapons = nullptr;

		delete mEffects;
		mEffects = nullptr;

		for (int i = 3; i >= 0; i--)
		{
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			{
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
			}
			delete mPillars[i];
			mPillars[i] = nullptr;
		}
		mPillars.clear();
	}
	void Niflheim::Initialize()
	{
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
		mAnimator->CreateAnimation(L"Enter", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 16, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Exit", mImage, size * Vector2{ 0.0f, (float)index++ }, 30, 5, 16, Vector2::Zero, 0.1);

		SetFlip(false);
		StateChange(eNiflheimState::Idle);
		AnimPlay(L"Idle", true);
		

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
		pillarVector[1].push_back( Vector2{760.0f, 520.f});
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
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
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

		coolTime[0] = 100.0f;
		coolTime[1] = 200.0f;
		coolTime[2] = 300.0f;
		coolTime[3] = 4.0f;

		GameObject::Initialize();
	}
	void Niflheim::Update()
	{
		//SetBAttack(true);
		
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
			attackReload();
			break;
		}

		

		
		//mEffects->Update();
		GameObject::Update();
	}
	void Niflheim::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		GameObject::Render(hdc);
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
	void Niflheim::SetState(GameObject::eState type)
	{
		mWeapons->SetState(type);
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
			if (mTime[i] >= coolTime[i - 1])
			{
				mTime[i] = 0.0f;
				StateChange(eNiflheimState(i+1));
				return;
			}
		}
		
		//for (int i = 0; i < 4; i++)
		//{
		//	Vector2 dir = mPillars[i]->GetDir();
		//	Vector2 mSpawnDir = math::Rotate(dir, -5.0f); // 무기 비틀기
		//	//Degree = (Degree + 1) % 360;
		//	mPillars[i]->SetDir(math::Rotate((dir), -6.f));
		//	mPillars[i]->GetComponent<Transform>()->SetPos(GetComponent<Transform>()->GetPos()
		//		+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
		//		+ mPillars[i]->GetDir() * 124.0f // 원 중심을 기준으로 그림 위치 회전
		//	);
		//}
		//for (int i = 0; i < 4; i++)
		//{
		//	Vector2 dir = mPillars[i]->GetDir();
		//	Vector2 mSpawnDir = math::Rotate(dir, -5.0f); // 무기 비틀기
		//	//Degree = (Degree + 1) % 360;
		//	mPillars[i]->SetDir(math::Rotate((dir), -12.f));
		//	mPillars[i]->GetComponent<Transform>()->SetPos(
		//		GetComponent<Transform>()->GetPos()
		//		+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
		//		+ pillarVector[i]
		//	);
		//}
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
				if(index == 4)
					pillarDir[index][i] = 
					math::Rotate(
						heroPos - Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
						- (GetComponent<Transform>()->GetPos() + Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f })
					, (float)(i - 2) * -90.f).Normalize();
				pillarVector[index][i] =
					GetComponent<Transform>()->GetPos()
					+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
					+ pillarDir[index][i] * 124.0;
				mPillars[i]->SetState(eState::Pause);
			}

		}
		else if (mTime[index] > 1.f)
		{
			pillarTeleport(index, index);
			mTime[index] = 0.0f - Time::DeltaTime();
			StateChange(eNiflheimState(5 + index));
		}
		mTime[index] += Time::DeltaTime();
	}
	/*void Niflheim::attackWait1()
	{
		if (mTime[0] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetTPPos(mPillars[i]->GetComponent<Transform>()->GetPos());
				pillarVector[0][i] =
					GetComponent<Transform>()->GetPos()
					+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
					+ pillarDir[0][i] * 200.0
					+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f };
			}

		}
		else if (mTime[0] < 0.4f)
		{
			pillarMove(0, 0);
		}
		else
		{
			mTime[0] = 0.0f - Time::DeltaTime();
			StateChange(eNiflheimState::Idle);
		}
	}*/
	/*void Niflheim::attackWait2()
	{
		if (mTime[1] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetTPPos(mPillars[i]->GetComponent<Transform>()->GetPos());
			}
			mTime[1] += Time::DeltaTime();
			return;
		}
		mTime[1] += Time::DeltaTime();
		if (mTime[1] < 0.4f)
		{
			for (int i = 0; i < 4; i++)
			{
				Vector2 SpawnPos = GetComponent<Transform>()->GetPos()
					+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
					+ pillarVector[i]
					+ Vector2{ 0.0f, mPillars[i]->GetComponent<Collider>()->GetSize().y * 0.5f };
				mPillars[i]->GetComponent<Transform>()->SetPos(
					mPillars[i]->GetTPPos()
					+ (SpawnPos - mPillars[i]->GetTPPos()) * (mTime[1] / 0.4f)
				);
			}
			return;
		}
		else
		{
			mTime[1] = 0.0f;
			for (int i = 0; i < 4; i++)
			{
				mPillars[i]->SetCommand(
					0.8f * (float)i
					, heroPos
					- (mPillars[i]->GetComponent<Transform>()->GetPos()
						- Vector2{ 0.0f,mPillars[i]->GetComponent<Collider>()->GetSize().y / 2.f })
					, 500.0f
					, 8
				);
				mPillars[i]->SetDir(Vector2::Up);
				mPillars[i]->SetBAttack(true);
			}

			StateChange(eNiflheimState::Attack2);
		}
	}*/
	void Niflheim::attackWait3()
	{
	}
	void Niflheim::attack1()
	{
		if (mTime[1] <= 0.0000001f)
		{
			for (int i = 0; i < 4; i++)
			{
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
		if (mTime[1] >= 2.6f)
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
				mPillars[i]->SetBAttack(true);
				
				mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[2][i]);
				mPillars[i]->SetDir(pillarDir[2][i]);
			}
		}
		mTime[2] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
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
		if (mTime[2] >= 2.5f)
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
				mPillars[i]->SetBAttack(true);
				/*mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[2][i]);
				mPillars[i]->SetDir(pillarDir[[i]);*/
			}
		}
		mTime[3] += Time::DeltaTime();
		for (int i = 0; i < 4; i++)
		{
			Vector2 dir = mPillars[i]->GetDir();

			//Degree = (Degree + 1) % 360;
			mPillars[i]->SetDir(math::Rotate((dir), -9.f));
			mPillars[i]->SetCommand(
				0.0f
				, mPillars[i]->GetDir()
				, 500.0f
				, 60
				, 0.0333f
			);
			mPillars[i]->GetComponent<Transform>()->SetPos(
				GetComponent<Transform>()->GetPos()
				+ Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f }
				+ mPillars[i]->GetDir() * 124.f
			);
		}
		if (mTime[3] >= 2.f)
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
			
			mPillars[i]->SetCommand(
				0.0f
				, (heroPos
					- Vector2{ 0.0f,GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
					- (mPillars[i]->GetComponent<Transform>()->GetPos())
					//- (Vector2::One * ((float)i - 2.f) * (float)((UINT)(mTime[4] * 1000.f) % 100) )

					).Normalize()
				, 1500.0f
				, 90
				, 0.022222f
			);
			mPillars[i]->SetDir(
				math::Rotate(
					heroPos - Vector2{ 0.0f, GetHero()->GetComponent<Collider>()->GetSize().y / 2.f }
					- (GetComponent<Transform>()->GetPos() + Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y * -0.5f })
					, (float)(i - 2) * -90.f).Normalize());
			mPillars[i]->GetComponent<Transform>()->SetPos(
				GetComponent<Transform>()->GetPos()
				- Vector2{ 0.0f, GetComponent<Collider>()->GetSize().y / 2.f }
				+ mPillars[i]->GetDir() * 124.f
			);
			if (mTime[4] > 1.0f)
				mPillars[i]->SetBAttack(true);
		}
		if (mTime[4] >= 3.f)
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
				mPillars[i]->GetComponent<Transform>()->SetPos(pillarVector[0][i]);
				mPillars[i]->SetDir(pillarDir[0][i]);
			}
		}
		mTime[0] += Time::DeltaTime();
	}

	void Niflheim::pillarMove(UINT posIndex, UINT dirIndex)
	{

		for (int i = 0; i < 4; i++)
		{
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
			mPillars[i]->SetDir(
				pillarDir[posIndex][i]
			);
			mPillars[i]->GetComponent<Transform>()->SetPos(
				pillarVector[posIndex][i]
			);
			mPillars[i]->SetState(eState::Active);

		}
	}
}