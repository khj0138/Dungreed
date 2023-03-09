#include "hjPlayScene.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjMonster.h"
#include "hjCollisionManager.h"


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
		BackGround* bg = new BackGround();
		bg->setAnimation(L"back", L"..\\Resource\\SkyDay.bmp", 0.0f);
		AddGameObject(bg, eLayerType::BackBG);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);


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