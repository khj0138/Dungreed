#include "hjWmanager_mon.h"
#include "hjMouse.h"
#include "hjMath.h"
#include "hjMouse.h"
#include "hjWeapon.h"
#include "hjSword_mon.h"
#include "hjComponent.h"
#include "hjTransform.h"
#include "hjCollider.h"
#include "hjSceneManager.h"
#include "hjPSceneManager.h"
#include "hjBow_mon.h"
#include "hjEmpty_mon.h"
#include "hjDashWeapon_mon.h"
namespace hj
{


	Wmanager_mon::Wmanager_mon()
		: GameObject()
		, mOwner(nullptr)
		, mDir(Vector2::Zero)
		, mActiveWeapon(nullptr)
		, mPos(Vector2::Zero)
		, isFlip(false)
	{
	}

	Wmanager_mon::~Wmanager_mon()
	{
		//for (std::pair<std::wstring, Weapon*> weapon : mWeapons)
		ReleaseDashWeapon();
		ReleaseWeapon();
		for (auto& weapon : mWeapons)
		{
			delete weapon.second;
			weapon.second = nullptr;
		}
		delete mDashWeapon;
		mDashWeapon = nullptr;
	}

	void Wmanager_mon::Initialize()
	{
	}

	void Wmanager_mon::Update()
	{
		/*if (GetOwner()->GetState() == GameObject::eState::Pause ||
			GetOwner()->GetState() == GameObject::eState::Wait
			)
		{
			mActiveWeapon->GameObject::SetState(GetOwner()->GetState());
			mDashWeapon->GameObject::SetState(GetOwner()->GetState());
		}*/
		/*if (GetOwner()->GetState() == eState::Pause)
		{
			mActiveWeapon->SetState(eState::Pause);
		}*/
		if (mDashWeapon->GetCAttack())
		{
			if (mDashWeapon->GetWState() == Weapon::eWeaponState::IDLE)
			{
				mDashWeapon->SetBAttack(true);
				mDashWeapon->SetWState(Weapon::eWeaponState::ATTACK);
			}
		}
		else
		{
			if (mDashWeapon->GetWState() == Weapon::eWeaponState::ATTACK)
			{
				mDashWeapon->SetBAttack(false);
				mDashWeapon->SetWState(Weapon::eWeaponState::IDLE);
			}
		}

		if (mActiveWeapon->GetCAttack())
		{
			if (mActiveWeapon->GetWState() == Weapon::eWeaponState::IDLE)
			{
				mActiveWeapon->SetWState(Weapon::eWeaponState::WAIT);
				return;
			}
		}
		if (mActiveWeapon->GetWState() == Weapon::eWeaponState::WAIT)
		{
			if (mActiveWeapon->GetWaitTime() == 0.0f)
			{
				mActiveWeapon->SetBAttack(true);
				mActiveWeapon->SetWState(Weapon::eWeaponState::ATTACK);
				return;
			}
			else if (mTime < mActiveWeapon->GetWaitTime())
			{
				mTime += Time::DeltaTime();
			}

			if (mTime > mActiveWeapon->GetWaitTime())
			{
				mTime = 0.0f;
				mActiveWeapon->SetBAttack(true);
				mActiveWeapon->SetWState(Weapon::eWeaponState::ATTACK);
				return;
			}
		}


		if (mActiveWeapon->GetWState() == Weapon::eWeaponState::ATTACK)
		{
			mActiveWeapon->SetBAttack(false);
			mActiveWeapon->SetWState(Weapon::eWeaponState::RELOAD);
			return;
		}

		if (mActiveWeapon->GetWState() == Weapon::eWeaponState::RELOAD)
		{
			mTime += Time::DeltaTime();
			if (mTime > mActiveWeapon->GetReloadTime())
			{
				mTime = 0.0f;
				mActiveWeapon->SetWState(Weapon::eWeaponState::IDLE);
			}
			return;
		}

		/*mPos = GetOwner()->GetComponent<Transform>()->GetPos();
		mDir = (Mouse::GetPos() - Camera::CaluatePos(mPos, 1.f)).Normalize();
		isFlip = Mouse::GetPos().x < Camera::CaluatePos(mPos, 1.f).x;
		if (mActiveWeapon != nullptr)
		{
			mActiveWeapon->Update();
		}*/
	}

	void Wmanager_mon::Render(HDC hdc)
	{
		/*if (mActiveWeapon != nullptr)
		{
			mActiveWeapon->Render(hdc);
		}*/
	}

	void Wmanager_mon::Release()
	{
	}

	void Wmanager_mon::OnCollisionEnter(Collider* other)
	{
		mActiveWeapon->OnCollisionEnter(other);
	}

	void Wmanager_mon::OnCollisionStay(Collider* other)
	{
		mActiveWeapon->OnCollisionEnter(other);
	}

	void Wmanager_mon::OnCollisionExit(Collider* other)
	{
		mActiveWeapon->OnCollisionExit(other);
	}

	void Wmanager_mon::CreateWeapon(const std::wstring& name, eWeaponType wType)
	{
		Weapon* newWeapon = nullptr;
		switch (wType)
		{
		case eWeaponType::SWORD_MON:
			newWeapon = new Sword_mon();
			break;
		case eWeaponType::BOW_MON:
			newWeapon = new Bow_mon();
			break;
		case eWeaponType::EMPTY_MON:
			newWeapon = new Empty_mon();
			break;
		case eWeaponType::DASHWEAPON_MON:
			newWeapon = new DashWeapon_mon();
			break;
		}
		if (newWeapon != nullptr)
		{
			//newWeapon->SetWmanager_mon(this);
			newWeapon->SetOwner(mOwner);
			newWeapon->Create();
			mWeapons.insert(std::make_pair(name, newWeapon));
			//newWeapon->Initialize();
		}
		else
			return;

	}

	Weapon* Wmanager_mon::FindWeapon(const std::wstring& name)
	{
		std::map<std::wstring, Weapon*>::iterator iter
			= mWeapons.find(name);

		if (iter == mWeapons.end())
			return nullptr;
		return iter->second;
	}

	void Wmanager_mon::EquipWeapon(const std::wstring& name/*, UINT index*/)
	{
		if (mActiveWeapon == nullptr)
		{
			CreateWeapon(L"DashWeapon", eWeaponType::DASHWEAPON_MON);

			DashWeapon_mon* newWeapon = new DashWeapon_mon();
			if (newWeapon != nullptr)
			{
				newWeapon->SetOwner(mOwner);
				newWeapon->Create();
				mDashWeapon = newWeapon;
				newWeapon->GetComponent<Collider>()->SetSize(
					GetOwner()->GetComponent<Collider>()->GetSize());
				PlayScene* scene = SceneManager::GetPManager()->GetPlayScene();
				scene->AddGameObject(mDashWeapon, eLayerType::Weapon_Monster);
				newWeapon->Initialize();
			}
		}
		ReleaseWeapon();
		mActiveWeapon = FindWeapon(name);
		PlayScene* scene = SceneManager::GetPManager()->GetPlayScene();
		scene->AddGameObject(mActiveWeapon, eLayerType::Weapon_Monster);
		mActiveWeapon->Initialize();
		/*if (index == 0)
		{

			Scene* scene = SceneManager::GetActiveScene();
				scene->AddGameObject(mActiveWeapon, eLayerType::Weapon_Player);

		}
		else if (index == 1)
		{
			PlayScene* scene = SceneManager::GetPManager()->GetPlayScene();
			scene->AddGameObject(mActiveWeapon, eLayerType::Weapon_Monster);
			mActiveWeapon->Initialize();
		}*/
		if (mActiveWeapon == nullptr)
			return;

	}
	void Wmanager_mon::ReleaseWeapon()
	{
		if (mActiveWeapon != nullptr)
		{
			Scene* scene = SceneManager::GetActiveScene();
			{
				
				if (!(scene->LayerEmpty(eLayerType::Weapon_Monster)))
				{
					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Weapon_Monster));
					auto it = std::find(temp.begin(), temp.end(), mActiveWeapon);
					if (it != temp.end())
					{
						temp.erase(it);
					}
				}
			}
		}
	}
	void Wmanager_mon::ReleaseDashWeapon()
	{
		if (mDashWeapon != nullptr)
		{
			Scene* scene = SceneManager::GetActiveScene();
			{
				if (!(scene->LayerEmpty(eLayerType::Weapon_Monster)))
				{
					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Weapon_Monster));

					auto it = std::find(temp.begin(), temp.end(), mDashWeapon);
					if (it != temp.end())
					{
						temp.erase(it);
					}
				}
			}
		}
	}
}