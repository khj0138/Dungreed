#pragma once
#include "hjScene.h"
#include "hjBackGround.h";
#include "hjGameObject.h"

namespace hj
{
	class TitleScene : public Scene
	{
	public:
		TitleScene();
		~TitleScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;
	private:
		std::vector<BackGround*> bg;
		double mTime;
	};
}