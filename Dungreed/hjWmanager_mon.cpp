#include "hjWmanager_mon.h"
#include "hjMouse.h"
#include "hjMath.h"
#include "hjMouse.h"
#include "hjWeapon.h"
#include "hjSword.h"
#include "hjComponent.h"
#include "hjTransform.h"
#include "hjCollider.h"
#include "hjSceneManager.h"
#include "hjEmpty.h"
#include "hjPSceneManager.h"
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
		ReleaseWeapon();
		for (auto weapon : mWeapons)
		{
			delete weapon.second;
			weapon.second = nullptr;
		}
		
	}

	void Wmanager_mon::Initialize()
	{
	}

	void Wmanager_mon::Update()
	{
		if (mActiveWeapon->GetCAttack())
		{
			if (mActiveWeapon->GetState() == Weapon::eWeaponState::IDLE)
			{
				mActiveWeapon->SetState(Weapon::eWeaponState::WAIT);
				return;
			}
		}
		if (mActiveWeapon->GetState() == Weapon::eWeaponState::WAIT)
		{
			if (mActiveWeapon->GetWaitTime() == 0.0f)
			{
				mActiveWeapon->SetBAttack(true);
				mActiveWeapon->SetState(Weapon::eWeaponState::ATTACK);
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
				mActiveWeapon->SetState(Weapon::eWeaponState::ATTACK);
				return;
			}
		}


		if (mActiveWeapon->GetState() == Weapon::eWeaponState::ATTACK)
		{
			mActiveWeapon->SetBAttack(false);
			mActiveWeapon->SetState(Weapon::eWeaponState::RELOAD);
			return;
		}

		if (mActiveWeapon->GetState() == Weapon::eWeaponState::RELOAD)
		{
			mTime += Time::DeltaTime();
			if (mTime > mActiveWeapon->GetReloadTime())
			{
				mTime = 0.0f;
				mActiveWeapon->SetState(Weapon::eWeaponState::IDLE);
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
		case eWeaponType::EMPTY:
			newWeapon = new Empty();
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
		if (mActiveWeapon != nullptr)
			int a = 0;

		mActiveWeapon = FindWeapon(name);
		PlayScene* scene = SceneManager::GetPManager()->GetPlayScene();
		scene->AddGameObject(mActiveWeapon, eLayerType::Weapon_Monster);
		mActiveWeapon->Initialize();
		/*if (index == 0)
		{

			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
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
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			{
				/*if (!(scene->LayerEmpty(eLayerType::Weapon_Player)))
				{

					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Weapon_Player));

					auto it = std::find(temp.begin(), temp.end(), mActiveWeapon);
					if (it != temp.end())
					{
						temp.erase(it);
						continue;
					}
				}*/
				if (!(scene->LayerEmpty(eLayerType::Weapon_Monster)))
				{
					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Weapon_Monster));
					auto it = std::find(temp.begin(), temp.end(), mActiveWeapon);
					if (it != temp.end())
					{
						temp.erase(it);
						continue;
					}
				}
			}
		}
	}
	//	return wfuncs->Wrender.mWfunc;
	//}
}