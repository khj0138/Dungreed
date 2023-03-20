#pragma once
#include "hjScene.h"
#include "hjGameObject.h"
#include "hjBackGround.h";

namespace hj
{
	class Bird;
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
		void makeBird(int time, Vector2 pos);
		void spawnBird(Bird* bird);
		Bird* nextBird(double time);

	private:
		double mTime;
		bool bir = false;
		std::vector<Bird*> mBirds;
	};
}