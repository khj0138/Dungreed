
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjBackGround.h"

namespace hj
{

	hj::PlayScene::PlayScene()
	{
	}

	hj::PlayScene::~PlayScene()
	{
	}

	void hj::PlayScene::Initialize()
	{
		Hero* hero = new Hero();
		AddGameObject(hero, eLayerType::Player);
		BackGround* backGround = new BackGround();
		AddGameObject(backGround, eLayerType::BG);

		Scene::Initialize();
	}

	void hj::PlayScene::Update()
	{
		Scene::Update();
	}

	void hj::PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void hj::PlayScene::Release()
	{
		Scene::Release();
	}
}