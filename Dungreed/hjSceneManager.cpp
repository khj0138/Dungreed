#include "hjSceneManager.h"
#include "hjPlayScene.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::Max);

		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		mScenes[(UINT)eSceneType::Play]->SetName(L"PLAYER");

		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Initialize();
		}
	}

	void SceneManager::Update()
	{
		//for (size_t i = 0; i < (UINT)eSceneType::Max; i++)
		//{
		//	if (mScenes[i] == nullptr)
		//		continue;

		//	mScenes[i]->Update();
		//}
		//read only
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Update();
		}
	}

	void hj::SceneManager::Render(HDC hdc)
	{
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Render(hdc);
		}
	}

	void hj::SceneManager::Release()
	{
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Release();
		}
		delete mScenes[(UINT)eSceneType::Play];
	}

}