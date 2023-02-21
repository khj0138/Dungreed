#include "hjPlayScene.h"
#include "hjHero.h"

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