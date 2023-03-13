#include "hjSceneManager.h"
#include "hjPlayScene.h"
#include "hjTitleScene.h"
#include "hjCamera.h"
#include "hjCollisionManager.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::Max);

		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		//mScenes[(UINT)eSceneType::Play]->SetName(L"PLAYER");
		mScenes[(UINT)eSceneType::Title] = new TitleScene();

		mActiveScene = mScenes[(UINT)eSceneType::Title];
		LoadScene(eSceneType::Title);
		//mActiveScene = mScenes[(UINT)eSceneType::Play];

		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Initialize();
		}
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::Render(HDC hdc)
	{
		mActiveScene->Render(hdc);
	}

	void SceneManager::Release()
	{
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;
			delete scene;
			scene = nullptr;
		}
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
	}

	void SceneManager::LoadScene(eSceneType type)
	{
		Camera::Clear();
		//ÇöÀç¾À
		if (mActiveScene != nullptr)
			mActiveScene->OnExit();

		//´ÙÀ½¾À
		CollisionManager::Clear();
		mActiveScene = mScenes[(UINT)type];
		mActiveScene->OnEnter();
	}

}