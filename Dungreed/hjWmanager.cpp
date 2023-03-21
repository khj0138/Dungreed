#include "hjWmanager.h"


namespace hj
{

	Wmanager::Wmanager()
		: GameObject()
		, mOwner(nullptr)
		, mDir(Vector2::Zero)
		, mActiveWeapon(nullptr)
		, mPos(Vector2::Zero)
	{
		AddComponent<Collider>();
	}

	Wmanager::~Wmanager()
	{
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
		mPos = GetOwner()->GetComponent<Transform>()->GetPos();
		mDir = (Mouse::GetPos() - Camera::CaluatePos(mPos,1.f)).Normalize();
		isFlip = Mouse::GetPos().x < Camera::CaluatePos(mPos,1.f).x;
		if (mActiveWeapon != nullptr)
		{
			mActiveWeapon->Update();
		}
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
		}
		if (newWeapon != nullptr)
		{
			newWeapon->SetWmanager(this);
			newWeapon->Create();
			mWeapons.insert(std::make_pair(name, newWeapon));
		}
		
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
		mActiveWeapon = FindWeapon(name);
	}

	//	return wfuncs->Wrender.mWfunc;
	//}
}