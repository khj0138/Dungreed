#pragma once
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{

	class Dungeon1_4 : public PlayScene
	{
	public:
		Dungeon1_4();
		~Dungeon1_4();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;
	private:

	};
}
