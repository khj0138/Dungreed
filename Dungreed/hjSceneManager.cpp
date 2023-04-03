#include "hjSceneManager.h"
#include "hjPlayScene.h"
#include "hjTitleScene.h"
#include "hjCamera.h"
#include "hjCollisionManager.h"
#include "hjToolScene.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);

		mScenes[(UINT)eSceneType::Title] = new TitleScene();
		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		mScenes[(UINT)eSceneType::Tool] = new ToolScene();

		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Initialize();
		}
		mActiveScene = mScenes[(UINT)eSceneType::Title];
		//mActiveScene = mScenes[(UINT)eSceneType::Tool];
		//LoadScene(eSceneType::Title);
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::Render(HDC hdc)
	{
		mActiveScene->Render(hdc);
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
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

	void SceneManager::LoadScene(eSceneType type)
	{
		//Camera::Clear();

		//ÇöÀç¾À
		if (mActiveScene != nullptr)
			mActiveScene->OnExit();

		CollisionManager::Clear();
		
		//´ÙÀ½¾À
		mActiveScene = mScenes[(UINT)type];
		mActiveScene->OnEnter();
	}

}