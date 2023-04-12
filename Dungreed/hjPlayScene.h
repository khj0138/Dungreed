#pragma once
#include "hjScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{
	class Town;
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


		Scene* GetPlayScene() { return mPlayScene; }
		void SetPlayScene(Scene* scene) { mPlayScene = scene; }

		template <typename T>
		static inline void CreatePScene(ePSceneType type)
		{
			T* newPScene = new T();
			mPlayScenes.push_back(newPScene);
			newPScene->AddGameObject(hero, eLayerType::Player);
			newPScene->SetHero(hero);
			return;
		}
	private:
		Hero* hero;
		BackGround* bg;
		std::vector<Scene*> mPlayScenes;
		Scene* mPlayScene;
	};

}
