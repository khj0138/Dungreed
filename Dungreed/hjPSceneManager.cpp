#include "hjPSceneManager.h"

#include "hjPlayScene.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjMonster.h"
#include "hjCollisionManager.h"
#include "hjObject.h"
#include "hjApplication.h"
#include "hjPlatform.h"
#include "hjCamera.h"
#include "hjGround.h"
#include "hjTilePalatte.h"

#include "hjPSceneList.h"


extern hj::Application application;

namespace hj
{
	
	PSceneManager::PSceneManager()
		: mPlayScene(nullptr)
	{
	}

	PSceneManager::~PSceneManager()
	{
	}

	void PSceneManager::Initialize()
	{
		mHero = new Hero();
		//Town* town = new Town();

		//mPlayScenes.push_back(town);
		//town->AddGameObject(mHero, eLayerType::Player);
		//town->SetHero(mHero);
		hj::PSceneManager::CreatePScene<Town>(ePSceneType::Town);
		hj::PSceneManager::CreatePScene<DungeonNiflheim>(ePSceneType::DungeonNiflheim);
		hj::PSceneManager::CreatePScene<Dungeon1_0>(ePSceneType::Dungeon1_0);
		hj::PSceneManager::CreatePScene<Dungeon1_1>(ePSceneType::Dungeon1_1);
		hj::PSceneManager::CreatePScene<Dungeon1_2>(ePSceneType::Dungeon1_2);
		//hj::PSceneManager::CreatePScene<DungeonNiflheim>(ePSceneType::Dungeon1_3);

		mPlayScene = mPlayScenes[0];

		//town->Initialize();
		for (auto pScene : mPlayScenes)
		{
			pScene->Initialize();
			pScene->AddGameObject(mHero, eLayerType::Player);
			pScene->SetHero(mHero);
		}
		SceneManager::ChangePlayScene(mPlayScene);
		mHero->Initialize();
	}

	void PSceneManager::Update()
	{
		mPlayScene->Update();
	}

	void PSceneManager::Render(HDC hdc)
	{
		mPlayScene->Render(hdc);
		//Scene::Render(hdc);
	}

	void PSceneManager::Release()
	{
		//Scene::Release();
		mPlayScene->Release();
	}

	void PSceneManager::OnEnter()
	{
		mPlayScene->OnEnter();
	}

	void PSceneManager::OnExit()
	{
		mPlayScene->OnExit();
	}



}