#include "hjWmanager.h"
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
		//for (std::pair<std::wstring, Weapon*> weapon : mWeapons)
		for (auto weapon : mWeapons)
		{
			delete weapon.second;
			weapon.second = nullptr;
		}
	}

	void Wmanager::Initialize()
	{
	}

	void Wmanager::Update()
	{
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
		case eWeaponType::EMPTY:
			newWeapon = new Empty();
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

	void Wmanager::EquipWeapon(const std::wstring& name)
	{
		if (mActiveWeapon != nullptr)
			int a = 0;

		mActiveWeapon = FindWeapon(name);
		for (PlayScene* scene : SceneManager::GetPManager()->GetPlayScenes())
			scene->AddGameObject(mActiveWeapon, eLayerType::Weapon);
		if (mActiveWeapon == nullptr)
			return;
	}

	//	return wfuncs->Wrender.mWfunc;
	//}
}