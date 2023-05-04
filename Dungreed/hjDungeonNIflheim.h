#pragma once
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{
	class PlaneObject;
	class DungeonNiflheim : public PlayScene
	{
	public:
		DungeonNiflheim();
		~DungeonNiflheim();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;
	private:
		float mTime;
		GameObject* niflheim;
		std::vector<PlaneObject*> DungeonGate;
	};
}
