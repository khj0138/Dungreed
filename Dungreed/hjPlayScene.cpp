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
		Vector2 asRatio = Vector2{ 1.0f, 1.0f } * (windowSizeX / 960.0f);
		//asRatio = Vector2{ 1.0f, 1.0f } *(windowSizeX / 1600.0f);
		//Vector2 asRatio = Vector2{ 1.0f, 1.0f } *(windowSizeY / 146.0f);
		Img::SetAsRatio(asRatio);

		hero = new Hero();
		AddGameObject(hero, eLayerType::Player);
		//object::Instantiate<Hero>(Vector2{ 100.0f, 500.0f }, eLayerType::Player, Vector2{ 36.0f, 56.0f });
		//BackGround * bg = object::Instantiate<BackGround>(eLayerType::BackBG, L"back", L"..\\Resource\\SkyDay.bmp", 0.0f);


		
		BackGround* bg = new BackGround();
		AddGameObject(bg, eLayerType::BackBG);
		bg->setAnimation(L"back", L"..\\Resource\\SkyDay.bmp", 0.0f);

		Img::SetAsRatio(asRatio * 3.0f);

		BackGround* bg2 = new BackGround();
		AddGameObject(bg2, eLayerType::BackBG);
		bg2->setAnimation(L"TownBG", L"..\\Resource\\Town\\TownBG_Day.bmp", 0.2f, true);

		BackGround* bg3 = new BackGround();
		AddGameObject(bg3, eLayerType::BackBG);
		bg3->setAnimation(L"TownLayer", L"..\\Resource\\Town\\TownLayer_Day.bmp", 0.5f, true);

		/*BackGround* bg4 = new BackGround();
		AddGameObject(bg4, eLayerType::BackBG);
		bg4->setAnimation(L"TownFloor", L"..\\Resource\\Town\\TownFloor.bmp", 0.0f);*/
		
		Platform* floor = new Platform();
		AddGameObject(floor, eLayerType::Platform);
		floor->setAnimation(L"TownFloor", L"..\\Resource\\Town\\TownFloor.bmp");
		
		Img::SetAsRatio(asRatio);

		/*hero = new Hero();
		AddGameObject(hero, eLayerType::Player);*/
		
		Scene::Initialize();
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
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);
	}

	void PlayScene::OnExit()
	{
		Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);
	}
}