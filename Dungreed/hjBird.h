#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjAnimator.h"

#include "hjTransform.h"

namespace hj
{
	class Bird : public GameObject
	{
	public:
		Bird(double time, Vector2 pos);
		~Bird();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;
		void setAnimation(const std::wstring name, const std::wstring path);
		double getSpawnTime() { return spawnTime; }
		Vector2 getPos() { return spawnPos; }
		void spawn() { isSpawn = true; }
		bool getSpawn() { return isSpawn; }
	private:
		Animator* mAnimator;
		Vector2 spawnPos;
		bool isSpawn;
		double spawnTime;
	};

}
