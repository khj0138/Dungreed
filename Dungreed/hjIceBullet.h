#pragma once
#include "hjBaseBullet.h"

namespace hj
{
	class Emanager;
	class IceBullet : public BaseBullet
	{

		enum class SwordState
		{
			up,
			down,
			END,
		};
	public:
		IceBullet();
		~IceBullet();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc)override;
		virtual void Release() override;

		virtual void Create();
		virtual void Spawn(Vector2 pos);

		virtual void OnCollisionEnter(class Collider* other) override;

		void SetBossRoom(bool set) { bossRoom = set; }
		void SetSpeed(float speed) { mSpeed = speed; }
		//virtual void Reset();
	private:
		Img* mImage;
		bool bossRoom;
		float mSpeed;
		//float mRadius;
		//SwordState sState;
		//Vector2 mSpawn;
		//Vector2 mSpawnDir;
		//float mSpawnDegree;
		//double mTime;
		//bool isFlip;
		//std::vector<Vector2> posCol;
		//bool bRender;
		////bool bAttack;
		////bool bCollision;
		//Emanager* mEffects;
	};
}