#pragma once
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{
	class PlaneObject;
	class Dungeon1_2 : public PlayScene
	{
	public:
		Dungeon1_2();
		~Dungeon1_2();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;
	private:
		std::vector<PlaneObject*> DungeonGate;
	};
}
