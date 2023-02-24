#include "hjPlayScene.h"
#include "hjSceneManager.h"
#include "hjInput.h"


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
		hero = new Hero();
		AddGameObject(hero, eLayerType::Player);
		bg = new BackGround();
		AddGameObject(bg, eLayerType::BG);
		bg->setBG(L"back", L"..\\Resource\\SkyDay.bmp");


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

	}

	void PlayScene::OnExit()
	{

	}
}