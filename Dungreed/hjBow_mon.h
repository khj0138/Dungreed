#pragma once
#include "hjWeapon.h"

//#include <ole2.h>
//#include <GdiPlus.h>
//#pragma comment(lib, "gdiplus.lib")
namespace hj
{
	class Emanager;
	class Arrow_mon;
	class Bow_mon : public Weapon
	{

	public:
		Bow_mon();
		~Bow_mon();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Create();

		virtual void OnCollisionEnter(class Collider* other) override;
		virtual void OnCollisionStay(class Collider* other) override;
		virtual void OnCollisionExit(class Collider* other) override;
		bool AttackCheck(class Collider* other);
		void Idle();
		void Wait();
		void Attack();
		void Reload();
		void shootCompleteEvent();
		void Play(std::wstring anim, bool loop);
		void Flip();


		//virtual void Reset();
	private:
		Img* mImage;
		float mRadius;
		eWeaponState mWstate;
		Vector2 mSpawn;
		Vector2 mSpawnDir;
		Vector2 mAttackDir;
		float mSpawnDegree;
		double mTime;
		bool isFlip;
		std::vector<Vector2> posCol;
		bool bRender;
		//bool bAttack;
		//bool bCollision;
		Emanager* mEffects;
		float Damage;
		Arrow_mon* arrow;
	};
}

