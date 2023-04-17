#pragma once
#include "hjScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{
	class Town;
	class PSceneManager;
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

		/*virtual void AddGameObject(GameObject* obj, eLayerType layer) override
		{
			GetPlayScene()->AddGameObject(obj, layer);
		}*/


		/*Scene* GetPlayScene() { return mPlayScene; }
		void SetPlayScene(Scene* scene) { mPlayScene = scene; }*/
		Hero* GetHero() { return mHero; }
		void SetHero(Hero* hero) { mHero = hero; }
		PSceneManager* GetPManager() { return PManager; }
		void SetPManager(PSceneManager* manager) { PManager = manager; }


		
	private:
		Hero* mHero;
		PSceneManager* PManager;
	};

}
