#include "hjPlayScene.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjMonster.h"
#include "hjCollisionManager.h"
#include "hjObject.h"
#include "hjApplication.h"
#include "hjPlatform.h"
#include "hjCamera.h"
#include "hjGround.h"

extern hj::Application application;

namespace hj
{

	PlayScene::PlayScene()
	{
	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{

		//float windowSizeY = (float)application.GetHeight();
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One *(windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();
		

		hero = new Hero();
		AddGameObject(hero, eLayerType::Player);
		

		
		BackGround* bg = new BackGround(L"back", L"..\\Resource\\SkyDay.bmp", 0.0f, asRatio);
		AddGameObject(bg, eLayerType::BackBG);

		asRatio = asRatio * 3.f;

		BackGround* bg2 = new BackGround(L"TownBG", L"..\\Resource\\Town\\TownBG_Day.bmp", 0.2f, asRatio, true);
		AddGameObject(bg2, eLayerType::BackBG);

		BackGround* bg3 = new BackGround(L"TownLayer", L"..\\Resource\\Town\\TownLayer_Day.bmp", 0.5f, asRatio, true);
		AddGameObject(bg3, eLayerType::BackBG);

		Platform* floor = new Platform(L"TownFloor", L"..\\Resource\\Town\\TownFloor.bmp", 1.0f, asRatio);
		AddGameObject(floor, eLayerType::Platform);
		
		asRatio = asRatio / 3.f;
		
		Scene::Initialize();
		object::Instantiate<Ground>(Vector2(0.0f, 800.0f), eLayerType::Ground, eSceneType::Play);
	}

	void PlayScene::Update()
	{
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Title);
		}

		Scene::Update();
	}

	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void PlayScene::Release()
	{
		Scene::Release();
	}

	void PlayScene::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ (float)application.GetWidth() * 5.0f, -(float)application.GetHeight() * 5.0f }
		);
		Camera::SetTarget(hero);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
	}

	void PlayScene::OnExit()
	{
		Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
	}
}