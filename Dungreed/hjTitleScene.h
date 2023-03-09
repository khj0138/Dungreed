#pragma once
#include "hjScene.h"
#include "hjBackGround.h";
#include "hjGameObject.h"
#include "hjBird.h"

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
		void makeBird(int time, Vector2 pos);
		void spawnBird(Bird* bird);
		Bird* nextBird(double time);

		/*std::map<int, Vector2>::iterator::iter;*/
	private:
		double mTime;
		bool bir = false;
		/*std::map<std::wstring, Vector2>::iterator iter;*/
		//std::map<int, Bird*> mBirds;
		std::vector<Bird*> mBirds;
	};
}