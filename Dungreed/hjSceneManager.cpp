#include "hjSceneManager.h"
#include "hjPlayScene.h"
#include "hjTitleScene.h"
#include "hjCamera.h"
#include "hjCollisionManager.h"
#include "hjToolScene.h"
#include "hjTilePalatte.h"
#include "hjPSceneManager.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;
	PSceneManager* SceneManager::PManager = nullptr;

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);

		TilePalatte::Initiailize();


		/*PlayScene* playScene = new PlayScene();
		mScenes[(UINT)eSceneType::Play] = playScene->;*/
		mScenes[(UINT)eSceneType::Title] = new TitleScene();
		mScenes[(UINT)eSceneType::Tool] = new ToolScene();

		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Initialize();
		}
		mScenes[(UINT)eSceneType::Play] = nullptr;
		PManager = new PSceneManager();
		PManager->Initialize();
		
		
		LoadScene(eSceneType::Title);
		//mActiveScene = mScenes[(UINT)eSceneType::Tool];
		//LoadScene(eSceneType::Title);
	}

	void SceneManager::Update()
	{
		if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}
		if (Input::GetKeyDown(eKeyCode::B))
		{
			BackGround::bRenderChange();
		}
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
		mScenes[(UINT)eSceneType::Play] = nullptr;
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;
			delete scene;
			scene = nullptr;
		}
		delete PManager;

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