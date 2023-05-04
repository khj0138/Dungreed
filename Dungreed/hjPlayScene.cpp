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
#include "hjPSceneManager.h"

#include "hjTown.h"


extern hj::Application application;

namespace hj
{

	PlayScene::PlayScene()
		: mHero(nullptr)
	{
	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{
		
		//mHero = new Hero();
		//Town* town = new Town();
		//mPlayScenes.push_back(town);
		//town->AddGameObject(mHero, eLayerType::Player);
		//town->SetHero(mHero);
		//hj::PlayScene::CreatePScene<Town>(ePSceneType::Town);

		//mPlayScene = mPlayScenes[0];
		//SceneManager::ChangePlayScene(mPlayScene);
		//town->Initialize();
		/*for (auto pScene : mPlayScenes)
		{
			pScene->Initialize();
		}*/
	}

	void PlayScene::Update()
	{
		
	}

	void PlayScene::Render(HDC hdc)
	{
		
		//Scene::Render(hdc);
	}

	void PlayScene::Release()
	{
		//Scene::Release();
		
	}

	void PlayScene::OnEnter()
	{
		
		//Camera::SetLookRange(
		//	Vector2{ (float)application.GetWidth() * 5.0f, -(float)application.GetHeight() * 5.0f }
		//);
		//Camera::SetTarget(hero);
		////CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		////CollisionManager::SetLayer(eLayerType::Bullet, eLayerType::Ground, true);
		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);

		///*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		//{
		//	delete gameObj;
		//	gameObj = nullptr;
		//}*/
		////SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		//wchar_t a[256] = L"../Resource/Tile2.Tile\0";
		//TilePalatte::Load(a);
	}

	void PlayScene::OnExit()
	{
		Scene::ClearLayer(eLayerType::Weapon_Player);
		Scene::ClearLayer(eLayerType::Bullet_Monster);
		Scene::ClearLayer(eLayerType::Bullet_Player);
		Scene::ClearLayer(eLayerType::Tile);
		////Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		//Camera::SetTarget(nullptr);

		////TilePalatte::clear();

		////CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}



}