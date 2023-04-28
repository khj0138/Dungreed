#include "hjBow.h"
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
#include "hjArrow.h"
#include "hjBaseBullet.h"

#include "hjPSceneManager.h"
#include "hjSceneManager.h"

extern hj::Application application;
namespace hj
{

	Bow::Bow()
		: mRadius(5.f)
		, mWstate(eWeaponState::IDLE)
		, isFlip(false)
		, mSpawnDegree(90)
		, bRender(false)
		//, bAttack(false)
	{
	}
	Bow::~Bow()
	{
		delete mEffects;
		mEffects = nullptr;
		for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
		{
			if (!(scene->LayerEmpty(eLayerType::Bullet_Player)))
			{

				std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Bullet_Player));

				auto it = std::find(temp.begin(), temp.end(), arrow);
				if (it != temp.end())
				{
					temp.erase(it);
					continue;
				}
			}
		}
		delete arrow;
		arrow = nullptr;
	}
	void Bow::Initialize()
	{
	}
	void Bow::Update()
	{
		Weapon::Update();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		mWstate = Weapon::GetWState();

		float length = mImage->GetHeight();
		Vector2 pos = GetPos();
		Vector2 dir = GetDir();
		isFlip = GetFlip();

		float flipNum = 1.0f - 2.0f * (float)(isFlip);
		Vector2 imgVect = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };

		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;

		
		GetComponent<Transform>()->SetPos(pos
			+ Vector2{ (xtemp)*heroSize.x * flipNum ,0.0f }// (ytemp)*heroSize.y} // �� �߽� �̵�
			+ math::Rotate((dir), -90.0f * flipNum) * mRadius // �� �߽��� �������� �׸� ��ġ ȸ��
			//+ math::Rotate(mSpawnDir, -90.0f) * (imgVect / hands).Length() * 5.f
		);

		GetComponent<SpriteRenderer>()->SetDir(math::Rotate(dir, -90.0f + 180.0f * (float)isFlip));

		mEffects->Update();


		mSpawnDir.Normalize();
		mSpawnDegree = atan2(mSpawnDir.y, mSpawnDir.x) / PI * 180.0f;
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
		
		bRender = true;
		GetComponent<Animator>()->Update();
	}
	void Bow::Render(HDC hdc)
	{
		if (bRender == true)
		{
			bRender = false;
			return;
		}

		Flip();
		GetComponent<Animator>()->Render(hdc);
		bRender = true;
	}

	void Bow::Create()
	{
		//Weapon::SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		Weapon::SetAsRatio(Vector2::One * 4.f);
		mImage = RscManager::Load<Img>(L"Bow", L"..\\Resource\\Char\\OakBow.bmp");
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

		SetStat(10.0f, 5.0f, 0.0f, 0.15f);
		mEffects = GetEmanager();
		mEffects = new Emanager();
		mEffects->SetOwner(this);
		//mEffects->RegisterEffect(L"RunEffect", L"..\\Resource\\Char\\SwingEffect.bmp", false, false, 5, Vector2{ (-1.5f) * colSize.x, 0.0f }, 0.07f);
		mEffects->RegisterEffect(L"SwingEffect", L"..\\Resource\\Char\\SwingFX.bmp", false, true, 3, Vector2{ 0.0f, 0.0f }, 0.03f, Vector2::One * 3.f);

		mAnimator->GetCompleteEvent(L"Shoot") = std::bind(&Bow::shootCompleteEvent, this);
		mAnimator->GetCompleteEvent(L"FlippedShoot") = std::bind(&Bow::shootCompleteEvent, this);
		
		arrow = new Arrow();
		arrow->Create();
		arrow->SetState(GameObject::eState::Pause);
		for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			scene->AddGameObject((GameObject*)arrow, eLayerType::Bullet_Player);

	}

	void Bow::OnCollisionEnter(Collider* other)
	{
	}

	void Bow::OnCollisionStay(Collider* other)
	{
		
	}

	void Bow::OnCollisionExit(Collider* other)
	{
	}
	

	void Bow::Idle()
	{
		Weapon::Idle();
		if (Mouse::GetLstate() == eKeyState::Up)
		{
			Play(L"Idle", false);
		}
	}
	void Bow::Wait()
	{
		if (Mouse::GetLstate() == eKeyState::Down)
		{
			Play(L"Wait", false);
		}
	}
	void Bow::Attack()
	{
		// ����Ʈ ���� �ڵ� �ʿ�
		Play(L"Shoot", true);
		Weapon::Attack();
		Damage = GetDamage();
		arrow->SetStat(Damage, GetDir());
		arrow->Spawn(GetComponent<Transform>()->GetPos()
		-arrow->GetComponent<Transform>()->GetSize()/2.f
		);
		//sState = (BowState)(((UINT)sState + 1) % (UINT)BowState::END);
	}
	void Bow::Reload()
	{
		Weapon::Reload();
		/*if (GetBCollision())
			SetBCollision(false);*/
	}
	void Bow::shootCompleteEvent()
	{
		Play(L"Idle", false);
	}
	
	void Bow::Play(std::wstring anim, bool loop)
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
	void Bow::Flip()
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