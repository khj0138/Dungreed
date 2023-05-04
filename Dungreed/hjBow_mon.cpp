#include "hjBow_mon.h"
#include "hjApplication.h"
#include "hjSpriteRenderer.h"
#include "hjEmanager.h"

#include "hjMath.h"

#include "hjImage.h"
#include "hjRscmanager.h"
#include "hjWmanager.h"
#include "hjTransform.h"
#include "hjComponent.h"
#include "hjMouse.h"
#include "hjTime.h"
#include "hjCamera.h"
#include "hjHero.h"

#include "hjMonster.h"
#include "hjArrow_mon.h"
#include "hjBaseBullet.h"

#include "hjPSceneManager.h"
#include "hjSceneManager.h"

extern hj::Application application;
namespace hj
{

	Bow_mon::Bow_mon()
		: mRadius(5.f)
		, mWstate(eWeaponState::IDLE)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
	{
	}
	Bow_mon::~Bow_mon()
	{
		delete mEffects;
		mEffects = nullptr;
		Scene* scene = SceneManager::GetActiveScene();
		{
			if (!(scene->LayerEmpty(eLayerType::Bullet_Player)))
			{

				std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Bullet_Player));

				auto it = std::find(temp.begin(), temp.end(), arrow);
				if (it != temp.end())
				{
					temp.erase(it);

				}
			}
		}
	}
	void Bow_mon::Initialize()
	{
	}
	void Bow_mon::Update()
	{
		Weapon::Update();
		switch (mWstate)
		{
		case eWeaponState::IDLE:
		{
			Idle();
			break;
		}
		case eWeaponState::WAIT:
		{
			Wait();
			break;
		}
		case eWeaponState::ATTACK:
		{
			Attack();
			break;
		}
		case eWeaponState::RELOAD:
		{
			Reload();
			break;
		}
		}
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		mWstate = Weapon::GetWState();

		float length = mImage->GetHeight();
		Vector2 pos = GetPos();
		Vector2 dir = mAttackDir;
		isFlip = GetFlip();

		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;


		GetComponent<Transform>()->SetPos(pos
			+ Vector2{ (xtemp)*heroSize.x * flipNum ,0.0f }// (ytemp)*heroSize.y} // 원 중심 이동
			+ math::Rotate((dir), -90.0f * flipNum) * mRadius // 원 중심을 기준으로 그림 위치 회전
			//+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length() * 5.f
		);

		GetComponent<SpriteRenderer>()->SetDir(math::Rotate(dir, -90.0f + 180.0f * (float)isFlip));

		mEffects->Update();


		mSpawnDir.Normalize();
		mSpawnDegree = atan2(mSpawnDir.y, mSpawnDir.x) / PI * 180.0f;

		bRender = true;
		GetComponent<Animator>()->Update();
	}
	void Bow_mon::Render(HDC hdc)
	{
		

		Flip();
		GetComponent<Animator>()->Render(hdc);
		bRender = true;
	}

	void Bow_mon::Create()
	{
		//Weapon::SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		Weapon::SetAsRatio(Vector2::One * 4.f);
		mImage = RscManager::Load<Img>(L"Bow_mon", L"..\\Resource\\Char\\OakBow.bmp");
		mImage->MatchRatio(Weapon::GetAsRatio());
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 3.0f;
		size.y = mImage->GetHeight() / 6.0f;

		GetComponent<SpriteRenderer>()->SetRotate(true);


		Animator* mAnimator = AddComponent<Animator>();
		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 3, 6, 1, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 3, 6, 1, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"Wait", mImage, size * Vector2{ 0.0f, (float)index++ }, 3, 6, 3, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"FlippedWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 3, 6, 3, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"Shoot", mImage, size * Vector2{ 0.0f, (float)index++ }, 3, 6, 2, Vector2::Zero, 0.05);
		mAnimator->CreateAnimation(L"FlippedShoot", mImage, size * Vector2{ 0.0f, (float)index++ }, 3, 6, 2, Vector2::Zero, 0.05);

		Play(L"Idle", false);

		SetStat(5.0f, 5.0f, 3.0f, 2.0f);
		mEffects = GetEmanager();
		mEffects = new Emanager();
		mEffects->SetOwner(this);
		//mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\SwingEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.07f);
		mEffects->RegisterEffect(L"SwingEffect", L"..\\Resource\\Char\\SwingFX.bmp", false, true, 3, Vector2{ 0.0f, 0.0f }, 0.03f, Vector2::One * 3.f);

		mAnimator->GetCompleteEvent(L"Shoot") = std::bind(&Bow_mon::shootCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"FlippedShoot") = std::bind(&Bow_mon::shootCompleteEvent, this);

		arrow = new Arrow_mon();
		arrow->Create();
		arrow->SetState(GameObject::eState::Pause);
		Scene* scene = SceneManager::GetActiveScene();
			scene->AddGameObject((GameObject*)arrow, eLayerType::Bullet_Monster);

	}

	void Bow_mon::OnCollisionEnter(Collider* other)
	{
	}

	void Bow_mon::OnCollisionStay(Collider* other)
	{

	}

	void Bow_mon::OnCollisionExit(Collider* other)
	{
	}


	void Bow_mon::Idle()
	{
		Weapon::Idle();
		/*if (Mouse::GetLstate() == eKeyState::Up)
		{
			Play(L"Idle", false);
		}*/
		
		
		Monster* owner = dynamic_cast<Monster*>(GetOwner());
			
		if (owner != nullptr)
		{
			Vector2 heroPos = owner->GetHero()->GetComponent<Transform>()->GetPos();
			SetDir((heroPos - GetComponent<Transform>()->GetPos()).Normalize());
			mAttackDir = GetDir();
			Play(L"Wait", false);
			SetWState(eWeaponState::WAIT);
		}
		
	}
	void Bow_mon::Wait()
	{
	}
	void Bow_mon::Attack()
	{
		// 이펙트 생성 코드 필요
		Play(L"Shoot", false);
		Weapon::Attack();
		
		arrow->SetStat(GetStat().power, mAttackDir);
		arrow->Spawn(GetComponent<Transform>()->GetPos()
			- arrow->GetComponent<Transform>()->GetSize() / 2.f
		);
		//sState = (Bow_monState)(((UINT)sState + 1) % (UINT)Bow_monState::END);
	}
	void Bow_mon::Reload()
	{
		Weapon::Reload();
		Monster* owner = dynamic_cast<Monster*>(GetOwner());

		if (owner != nullptr)
		{
			Vector2 heroPos = owner->GetHero()->GetComponent<Transform>()->GetPos();
			SetDir((heroPos - GetComponent<Transform>()->GetPos()).Normalize());
			mAttackDir = GetDir();
		}
	}
	void Bow_mon::shootCompleteEvent()
	{
		Play(L"Idle", false);
	}

	void Bow_mon::Play(std::wstring anim, bool loop)
	{
		if (GetFlip())
		{
			std::wstring flipAnim = L"Flipped";
			flipAnim.append(anim);
			GetComponent<Animator>()->Play(flipAnim, loop);
		}
		else
		{
			GetComponent<Animator>()->Play(anim, loop);
		}
	}
	void Bow_mon::Flip()
	{
		std::wstring Anim = GetComponent<Animator>()->GetActiveAnimation()->GetAnimationName();
		if (Anim != L"")
		{
			if (GetFlip())
			{
				int temp = Anim.find(L"Flipped");
				std::wstring flipAnim = L"Flipped";
				if (temp == std::wstring::npos)
				{
					flipAnim.append(Anim);
					GetComponent<Animator>()->Flip(flipAnim);
				}
				else
				{
					GetComponent<Animator>()->Flip(Anim);
				}
			}
			else
			{
				int temp = Anim.find(L"Flipped");
				if (temp == std::wstring::npos)
				{
					GetComponent<Animator>()->Flip(Anim);
				}
				else
				{
					Anim.erase(Anim.begin(), Anim.begin() + 7);
					GetComponent<Animator>()->Flip(Anim);
				}
				GetComponent<Animator>()->Flip(Anim);

			}
		}

	}
}