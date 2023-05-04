#include "hjEliteIceSkelWarrior.h"
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
extern hj::Application application;

namespace hj
{
	EliteIceSkelWarrior::EliteIceSkelWarrior()
	{
	}
	EliteIceSkelWarrior::~EliteIceSkelWarrior()
	{
		//delete mWeapons;
		mWeapons = nullptr;

		delete mEffects;
		mEffects = nullptr;

		for (int i = 3; i >= 0; i--)
		{
			/*Scene* scene = SceneManager::GetActiveScene();
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
			}*/
			//delete mBullets[i];
			mBullets[i] = nullptr;
		}
		mBullets.clear();
	}
	void EliteIceSkelWarrior::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		isJump = false;
		cJump = 1;
		Vector2 asRatio = Vector2::One * ((float)application.GetWidth() / 960.0f);
		asRatio = asRatio * 1.2f;
		//Vector2 asRatio = Vector2::One * 1.28f;
		Img* mImage = RscManager::Load<Img>(L"EliteIceSkelWarrior", L"..\\Resource\\Monster\\BigGrayIceSkel.bmp");
		mImage->MatchRatio(asRatio);
		mImage->SetMoveRate(Vector2::One);
		Vector2 size = Vector2::Zero;
		size.x = mImage->GetWidth() / 8.0f;
		size.y = mImage->GetHeight() / 14.0f;

		Transform* tr = GetComponent<Transform>();
		tr->SetSize(size);
		tr->SetVelocity(Vector2{ 300.0f, 0.0f });
		//tr->SetPos(Vector2{ 3400.0f, 1400.0f });

		Vector2 pos = tr->GetPos();

		UINT index = 0;
		mAnimator->CreateAnimation(L"Idle", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedIdle", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 8, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Run", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedRun", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"Jump", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedJump", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 1, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"AttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedAttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"AttackReload", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 7, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedAttackReload", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 7, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"SkillAttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedSkillAttackWait", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 6, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"SkillAttackReload", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 7, Vector2::Zero, 0.1);
		mAnimator->CreateAnimation(L"FlippedSkillAttackReload", mImage, size * Vector2{ 0.0f, (float)index++ }, 8, 14, 7, Vector2::Zero, 0.1);
		
		SetFlip(false);
		StateChange(eEliteIceSkelWarriorState::Idle, L"Idle", true);

		Collider* collider = AddComponent<Collider>();
		collider->SetSize(Vector2{ 80.0f, 120.0f });
		Vector2 colSize = collider->GetSize();
		collider->SetCenter(Vector2{ (-0.5f) * colSize.x, (-1.0f) * colSize.y });

		mRigidbody = AddComponent<Rigidbody>();
		mRigidbody->SetMass(1.0f);

		mWeapons = new Wmanager_mon();
		mWeapons->SetOwner(this);
		/*mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY);
		mWeapons->EquipWeapon(L"Empty", 1);*/
		mWeapons->CreateWeapon(L"Empty", eWeaponType::EMPTY_MON);
		mWeapons->EquipWeapon(L"Empty");
		mWeapons->GetActiveWeapon()->SetStat(10.0f, 0.0f, 0.6f, 0.6f);

		for (int i = 0; i < 4; i++)
		{

			IceBullet* newBullet = new IceBullet();
			newBullet->Create();
			newBullet->SetState(GameObject::eState::Pause);
			mBullets.push_back(newBullet);
			Scene* scene = SceneManager::GetActiveScene();
				scene->AddGameObject((GameObject*)newBullet, eLayerType::Bullet_Monster);
		}
		coolTime = 7.0f;
		Monster::Initialize();
		SetStat(20, 20);
		//GameObject::Initialize();
	}
	void EliteIceSkelWarrior::Update()
	{
		if (GetStat().HP == 0)
		{
			mWeapons->GetActiveWeapon()->SetState(eState::Pause);
			mWeapons->GetDashWeapon()->SetState(eState::Pause);
			SetState(eState::Pause);
		}
		SetBAttack(true);

		mTime += Time::DeltaTime();
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
		prevPos = GetComponent<Transform>()->GetPos();



		if (Input::GetKey(eKeyCode::R))
		{
			GetComponent<Transform>()->SetPos(Vector2{
				GetComponent<Transform>()->GetPos().x,
				200.0f
				});
		}

		if (GetHero() != nullptr)
		{
			heroPos = GetHero()->GetComponent<Transform>()->GetPos();

			Transform* tr = GetComponent<Transform>();
			Vector2 pos = tr->GetPos();
			if (heroPos.x > pos.x)
				SetFlip(false);
			else
				SetFlip(true);
		}
		if (isJump == false)
		{
			if (!(GetComponent<Rigidbody>()->GetGround()))
				isJump = true;
		}
		else if (!(GetComponent<Rigidbody>()->GetGround()))
		{
			if (mState != eEliteIceSkelWarriorState::Jump)
			{
				StateChange(eEliteIceSkelWarriorState::Jump, L"Jump", true);
			}
		}
		else
		{
			isJump = false;
		}

		switch (mState)
		{

		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::Idle:
			idle();
			break;
		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::Run:
			run();
			break;
		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::Jump:
			jump();
			break;
		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::AttackWait:
			attackWait();
			break;
		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::AttackReload:
			attackReload();
			break;
		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::SkillAttackWait:
			skillAttackWait();
			break;
		case hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::SkillAttackReload:
			skillAttackReload();
			break;
		}

		Transform* tr = GetComponent<Transform>();
		Vector2 size = tr->GetSize();

		if (mState != hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::SkillAttackWait &&
			mState != hj::EliteIceSkelWarrior::eEliteIceSkelWarriorState::SkillAttackReload)
			mWeapons->Update();
		//mEffects->Update();
		Monster::Update();
	}
	void EliteIceSkelWarrior::Render(HDC hdc)
	{
		//mWeapons->Render(hdc);
		Monster::Render(hdc);
	}
	void EliteIceSkelWarrior::Release()
	{
		GameObject::Release();
	}

	void EliteIceSkelWarrior::OnCollisionEnter(Collider* other)
	{
		Transform* tr = GetComponent<Transform>();
		Collider* col = GetComponent<Collider>();
		Vector2 pos = tr->GetPos();
		Vector2 size = Vector2::Zero;
		size.x = col->GetSize().x * 1.5f;
		size.y = col->GetSize().y;
		if ((GetHeroPos().y - pos.y) >= (size.y * 2))
		{
			downJump(other);
		}
	}

	void EliteIceSkelWarrior::OnCollisionStay(Collider* other)
	{
	}

	void EliteIceSkelWarrior::OnCollisionExit(Collider* other)
	{

	}

	void EliteIceSkelWarrior::StateChange(eEliteIceSkelWarriorState state, std::wstring anim, bool loop = false)
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
	void EliteIceSkelWarrior::SetState(GameObject::eState type)
	{
		mWeapons->SetState(type);
		GameObject::SetState(type);
	}
	void EliteIceSkelWarrior::idle()
	{
		if (mWeapons->GetActiveWeapon()->GetWState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eEliteIceSkelWarriorState::AttackWait, L"AttackWait", false);
			return;
		}
		else if (mTime >= coolTime)
		{
			mTime = 0;
			StateChange(eEliteIceSkelWarriorState::SkillAttackWait, L"SkillAttackWait", false);
			return;
		}
		if (GetHero() != nullptr)
		{

			Transform* tr = GetComponent<Transform>();
			Collider* col = GetComponent<Collider>();
			Vector2 pos = tr->GetPos();
			Vector2 size = Vector2::Zero;
			size.x = col->GetSize().x * 1.5f;
			size.y = col->GetSize().y;

			if (fabs(pos.x - GetHeroPos().x) >= (size.x))
				StateChange(eEliteIceSkelWarriorState::Run, L"Run", true);

			else
			{
				Flip(L"Idle");
			}

		}
	}
	void EliteIceSkelWarrior::run()
	{
		if (mWeapons->GetActiveWeapon()->GetWState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eEliteIceSkelWarriorState::AttackWait, L"AttackWait", false);
			return;
		}
		else if (mTime >= coolTime)
		{
			mTime = 0;
			StateChange(eEliteIceSkelWarriorState::SkillAttackWait, L"SkillAttackWait", false);
			return;
		}
		if (GetHero() != nullptr)
		{
			Transform* tr = GetComponent<Transform>();
			Collider* col = GetComponent<Collider>();
			Vector2 pos = tr->GetPos();
			Vector2 size = Vector2::Zero;
			size.x = col->GetSize().x * 1.5f;
			size.y = col->GetSize().y;
			//mEffects->CreateEffect(L"RunEffect");


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

				StateChange(eEliteIceSkelWarriorState::Jump, L"Jump", true);
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
				StateChange(eEliteIceSkelWarriorState::Idle, L"Idle", true);
			}

		}
	}
	void EliteIceSkelWarrior::jump()
	{
		/*if (mWeapons->GetActiveWeapon()->GetWState() == Weapon::eWeaponState::WAIT)
		{
			StateChange(eEliteIceSkelWarriorState::Attack, L"Attack", true);
		}*/
		Transform* tr = GetComponent<Transform>();
		Collider* col = GetComponent<Collider>();
		Vector2 pos = tr->GetPos();
		Vector2 size = Vector2::Zero;
		size.x = col->GetSize().x * 1.5f;
		size.y = col->GetSize().y;
		Vector2 velocity = mRigidbody->GetVelocity();
		mRigidbody->SetGravity(true);
		if (mRigidbody->GetGround())
		{
			cJump = 1;
			Vector2 velocity = mRigidbody->GetVelocity();
			velocity.y = 0.0f;
			mRigidbody->SetVelocity(velocity);
			StateChange(eEliteIceSkelWarriorState::Run, L"Run", true);
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
		}

	}
	void EliteIceSkelWarrior::Flip(std::wstring Anim)
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

	void EliteIceSkelWarrior::downJump(Collider* other)
	{
		Tile* tile = dynamic_cast<Tile*>(other->GetOwner());
		if (tile != nullptr)
		{
			if ((tile->Index() == 3 || tile->Index() == 4 || tile->Index() == 5)
				&& (mState != eEliteIceSkelWarriorState::Jump))
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
				StateChange(eEliteIceSkelWarriorState::Jump, L"Jump", true);
				cJump--;

			}
		}
	}
	void EliteIceSkelWarrior::attackWait()
	{
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity.x = 0.0f;
		mRigidbody->SetVelocity(velocity);

		if (mWeapons->GetActiveWeapon()->GetWState() == Weapon::eWeaponState::RELOAD)
		{
			StateChange(eEliteIceSkelWarriorState::AttackReload, L"AttackReload", false);
		}
	}
	void EliteIceSkelWarrior::attackReload()
	{
		if (mWeapons->GetActiveWeapon()->GetWState() != Weapon::eWeaponState::RELOAD)
		{
			if (GetHero() != nullptr)
			{

				Transform* tr = GetComponent<Transform>();
				Collider* col = GetComponent<Collider>();
				Vector2 pos = tr->GetPos();
				Vector2 size = Vector2::Zero;
				size.x = col->GetSize().x * 1.5f;
				size.y = col->GetSize().y;

				if (fabs(pos.x - GetHeroPos().x) >= (size.x))
					StateChange(eEliteIceSkelWarriorState::Run, L"Run", true);

				else
				{
					StateChange(eEliteIceSkelWarriorState::Idle, L"Idle", true);
				}

			}
		}
	}
	void EliteIceSkelWarrior::skillAttackWait()
	{
		Vector2 velocity = mRigidbody->GetVelocity();
		velocity.x = 0.0f;
		mRigidbody->SetVelocity(velocity);

		if (mTime >= 0.6f)
		{
			heroPos = GetHero()->GetComponent<Transform>()->GetPos();
			StateChange(eEliteIceSkelWarriorState::SkillAttackReload, L"SkillAttackReload", false);
			mAttackDir = (heroPos - GetComponent<Transform>()->GetPos()).Normalize();
		}

		
	}
	void EliteIceSkelWarrior::skillAttackReload()
	{
		if ((mTime - 0.69f) >= (index * 0.1f) && (mTime <= 1.0f))
		{
			mBullets[index]->SetStat(5.0f, mAttackDir);
			mBullets[index]->SetSpeed(500.0f);
			mBullets[index]->Spawn(GetComponent<Transform>()->GetPos()
				- Vector2{ 0.0f,GetComponent<Collider>()->GetSize().y / 2.f }
				- mBullets[index]->GetComponent<Transform>()->GetSize() / 2.f
			);
			index++;
		}

		if (mTime >= 1.3f)
		{
			mTime = 0.0f;
			index = 0;

			if (GetHero() != nullptr)
			{

				Transform* tr = GetComponent<Transform>();
				Collider* col = GetComponent<Collider>();
				Vector2 pos = tr->GetPos();
				Vector2 size = Vector2::Zero;
				size.x = col->GetSize().x * 1.5f;
				size.y = col->GetSize().y;

				if (fabs(pos.x - GetHeroPos().x) >= (size.x))
					StateChange(eEliteIceSkelWarriorState::Run, L"Run", true);

				else
				{
					StateChange(eEliteIceSkelWarriorState::Idle, L"Idle", true);
				}

			}
		}
		
	}
}