#pragma once
#include "hjScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{

	class PlayScene : public Scene
	{
	public:
		PlayScene();
		~PlayScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		Hero* hero;
		BackGround* bg;
	};

}
