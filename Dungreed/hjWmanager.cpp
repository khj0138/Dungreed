#include "hjWmanager.h"
#include "hjMouse.h"
#include "hjMath.h"
#include "hjMouse.h"
#include "hjWeapon.h"
#include "hjComponent.h"
#include "hjTransform.h"
#include "hjCollider.h"
#include "hjSceneManager.h"
#include "hjPSceneManager.h"
#include "hjSword.h"
#include "hjEmpty.h"
#include "hjBow.h"
#include "hjRigidBody.h"
#include "hjCosmosSword.h"
namespace hj
{


	Wmanager::Wmanager()
		: GameObject()
		, mOwner(nullptr)
		, mDir(Vector2::Zero)
		, mActiveWeapon(nullptr)
		, mPos(Vector2::Zero)
		, isFlip(false)
	{

	}

	Wmanager::~Wmanager()
	{
		//for (std::pair<std::wstring, Weapon*> weapon : mWeapons).0000000
		ReleaseDashWeapon();
		ReleaseWeapon();
		for (auto weapon : mWeapons)
		{
			/*if (weapon.second == nullptr)
			{
				continue;
			}*/
			delete weapon.second;
			weapon.second = nullptr;
		}
		delete mDashWeapon;
		mDashWeapon = nullptr;
	}

	void Wmanager::Initialize()
	{
	}

	void Wmanager::Update()
	{
		/*if (GetOwner()->GetState() == GameObject::eState::Pause ||
			GetOwner()->GetState() == GameObject::eState::Wait
			)
		{
			mActiveWeapon->GameObject::SetState(GetOwner()->GetState());
			mDashWeapon->GameObject::SetState(GetOwner()->GetState());
		}*/
		if (!(GetOwner()->GetComponent<Rigidbody>()->GetGravity()))
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
		
		if (Mouse::GetLstate() == eKeyState::Down)
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
			else if(mTime < mActiveWeapon->GetWaitTime())
			{
				mTime += Time::DeltaTime();
			}

			if (mTime > mActiveWeapon->GetWaitTime())
			{
				mTime = mActiveWeapon->GetWaitTime();
			}
			if (Mouse::GetLstate() == eKeyState::Up)
			{
				if (mTime == mActiveWeapon->GetWaitTime())
				{
					mActiveWeapon->SetDamage((UINT)(mActiveWeapon->GetStat().power * (mTime / mActiveWeapon->GetStat().wait)));
					mActiveWeapon->SetBAttack(true);
					mActiveWeapon->SetWState(Weapon::eWeaponState::ATTACK);
					mTime = 0.0f;
				}
				else
				{
					mTime = 0.0f;
					mActiveWeapon->SetWState(Weapon::eWeaponState::IDLE);
				}
				
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

	void Wmanager::Render(HDC hdc)
	{
		if (mActiveWeapon != nullptr)
		{
			mActiveWeapon->Render(hdc);
		}
	}

	void Wmanager::Release()
	{
	}

	void Wmanager::OnCollisionEnter(Collider* other)
	{
		mActiveWeapon->OnCollisionEnter(other);
	}

	void Wmanager::OnCollisionStay(Collider* other)
	{
		mActiveWeapon->OnCollisionEnter(other);
	}

	void Wmanager::OnCollisionExit(Collider* other)
	{
		mActiveWeapon->OnCollisionExit(other);
	}

	void Wmanager::CreateWeapon(const std::wstring& name, eWeaponType wType)
	{
		Weapon* newWeapon = nullptr;
		switch (wType)
		{
		case eWeaponType::SWORD:
			newWeapon = new Sword();
			break;
		case eWeaponType::COSMOSSWORD:
			newWeapon = new CosmosSword();
			break;
		case eWeaponType::EMPTY:
			newWeapon = new Empty();
			break;
		case eWeaponType::BOW:
			newWeapon = new Bow();
			break;
		}
		
		if (newWeapon != nullptr)
		{
			//newWeapon->SetWmanager(this);
			newWeapon->SetOwner(mOwner);
			newWeapon->Create();
			mWeapons.insert(std::make_pair(name, newWeapon));
			//newWeapon->Initialize();
		}
		else
			return;

	}

	Weapon* Wmanager::FindWeapon(const std::wstring& name)
	{
		std::map<std::wstring, Weapon*>::iterator iter
			= mWeapons.find(name);

		if (iter == mWeapons.end())
			return nullptr;
		return iter->second;
	}

	void Wmanager::EquipWeapon(const std::wstring& name/*, UINT index*/)
	{
		if (mActiveWeapon == nullptr)
		{
			CreateWeapon(L"Empty", eWeaponType::EMPTY);

			Empty* newWeapon = new Empty();
			if (newWeapon != nullptr)
			{
				newWeapon->SetOwner(mOwner);
				newWeapon->Create();
				mDashWeapon = newWeapon;
				newWeapon->GetComponent<Collider>()->SetSize(
					GetOwner()->GetComponent<Collider>()->GetSize());
				for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
					scene->AddGameObject(mDashWeapon, eLayerType::Weapon_Player);
			}
		}
		ReleaseWeapon();
		mActiveWeapon = FindWeapon(name);
		for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			scene->AddGameObject(mActiveWeapon, eLayerType::Weapon_Player);
		mDashWeapon->SetStat(mActiveWeapon->GetStat().dashPower, 0.0f, 0.0f, 0.0f);

		if (mActiveWeapon == nullptr)
			return;
	}

	void Wmanager::ReleaseWeapon()
	{
		if (mActiveWeapon != nullptr)
		{
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			{
				if (!(scene->LayerEmpty(eLayerType::Weapon_Player)))
				{

					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Weapon_Player));

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
	void Wmanager::ReleaseDashWeapon()
	{
		if (mDashWeapon != nullptr)
		{
			for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			{
				if (!(scene->LayerEmpty(eLayerType::Weapon_Player)))
				{
					std::vector<GameObject*>& temp = (scene->GetGameObjects(eLayerType::Weapon_Player));

					auto it = std::find(temp.begin(), temp.end(), mDashWeapon);
					if (it != temp.end())
					{
						temp.erase(it);
						continue;
					}
				}
			}
		}
	}
	void Wmanager::SetState(GameObject::eState type)
	{
		mActiveWeapon->GameObject::SetState(type);
		mDashWeapon->GameObject::SetState(type);
	}
	//	return wfuncs->Wrender.mWfunc;
	//}
}