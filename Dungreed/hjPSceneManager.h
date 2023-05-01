#pragma once
#include "hjScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"
#include "hjPlayScene.h"

namespace hj
{
	class Town;

	class PSceneManager : Scene
	{
	public:
		PSceneManager();
		~PSceneManager();

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


		PlayScene* GetPlayScene() { return mPlayScene; }
		PlayScene* FindScene(ePSceneType type) { return mPlayScenes[(UINT)type]; }
		void SetPlayScene(PlayScene* scene) { mPlayScene = scene; }
		Hero* GetHero() { return mHero; }
		void SetHero(Hero* hero) { mHero = hero; }
		void ReleaseHero();
		std::vector<PlayScene*>& GetPlayScenes() { return mPlayScenes; }

		template <typename T>
		void CreatePScene(ePSceneType type)
		{
			T* newPScene = new T();
			mPlayScenes.push_back(newPScene);
			newPScene->SetPManager(this);
			/*newPScene->AddGameObject(mHero, eLayerType::Player);
			newPScene->SetHero(mHero);*/
			return;
		}

		void ChangePlayScene(ePSceneType index) { 
			//mPlayScene->OnExit();
			PlayScene* prevScene = mPlayScene;
			mPlayScene = mPlayScenes[(UINT)index];
			SceneManager::ChangePlayScene(mPlayScene);
			SceneManager::LoadScene(eSceneType::Play);
			mPlayScene->GetHero()->ResetPrevPos();
			//delete prevScene;
			//mPlayScene->OnEnter();
		}
	private:
		Hero* mHero;
		BackGround* bg;
		std::vector<PlayScene*> mPlayScenes;
		PlayScene* mPlayScene;

	};
}


