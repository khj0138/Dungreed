#pragma once
#include "hjGameObject.h"
#include "hjWeapon.h"

namespace hj
{


	//class Weapon;

	class Wmanager_mon : public GameObject
	{

	public:
		Wmanager_mon();
		~Wmanager_mon();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;


		virtual void OnCollisionEnter(class Collider* other) override;
		virtual void OnCollisionStay(class Collider* other) override;
		virtual void OnCollisionExit(class Collider* other) override;

		void SetOwner(GameObject* owner) { mOwner = owner; }
		void CreateWeapon(const std::wstring& name, eWeaponType wType);
		void EquipWeapon(const std::wstring& name/*, UINT index*/);
		void SetDir(Vector2 dir) { mActiveWeapon->SetDir(dir); }
		void ReleaseWeapon();

		Weapon* FindWeapon(const std::wstring& name);
		Weapon* GetActiveWeapon() { return mActiveWeapon; }
		GameObject* GetOwner() { return mOwner; }

		Vector2 GetDir() { return mDir; }
		Vector2 GetPos() { return mPos; }
		bool GetFlip() { return isFlip; }
		/*Weapon::Wfuncs* FindWeapon(const std::wstring& name);

		std::function<void()>& Wupdate(const std::wstring& name);
		std::function<void()>& Wrender(const std::wstring& name);*/

	private:
		//std::map<std::wstring, Wfuncs*> mWeapons;
		Vector2 mDir;
		Weapon* mActiveWeapon;
		std::map<std::wstring, Weapon*> mWeapons;
		GameObject* mOwner;
		Vector2 mPos;
		bool isFlip;
		double mTime;
		bool CAttack;

	};
}
