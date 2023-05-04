#pragma once
#include "hjWeapon.h"

//#include <ole2.h>
//#include <GdiPlus.h>
//#pragma comment(lib, "gdiplus.lib")
namespace hj
{
	class Emanager;
	class DashWeapon_mon : public Weapon
	{

		enum class DashWeapon_monState
		{
			up,
			down,
			END,
		};
	public:
		DashWeapon_mon();
		~DashWeapon_mon();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Create();

		virtual void OnCollisionEnter(class Collider* other) override;
		virtual void OnCollisionStay(class Collider* other) override;
		virtual void OnCollisionExit(class Collider* other) override;
		bool AttackCheck(class Collider* other);
		void Idle();
		void Attack();
		void Reload();
		//virtual void Reset();
	private:
		Img* mImage;
		float mRadius;
		DashWeapon_monState sState;
		eWeaponState mWstate;
		Vector2 mSpawn;
		Vector2 mSpawnDir;
		float mSpawnDegree;
		double mTime;
		bool isFlip;
		std::vector<Vector2> posCol;
		bool bRender;
		//bool bAttack;
		bool bCollision;
		Emanager* mEffects;
	};
}

