#include "hjTitleScene.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjTime.h"
#include "hjImage.h"

namespace hj {

	TitleScene::TitleScene()
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Initialize()
	{
		BackGround* bg = new BackGround();
		AddGameObject(bg, eLayerType::BG);
		bg->setBG(L"TitleSky", L"..\\Resource\\Title\\TitleSky.bmp");
		bg->setV(0.0f);

		bg = new BackGround();
		AddGameObject(bg, eLayerType::BG);
		bg->setBG(L"backCloud", L"..\\Resource\\Title\\backCloud.bmp");
		bg->setV(1.f);
		
		bg = new BackGround();
		AddGameObject(bg, eLayerType::BG);
		bg->setBG(L"FrontCloud", L"..\\Resource\\Title\\FrontCloud.bmp");
		bg->setV(2.f);
		
	}
	void TitleScene::Update()
	{
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
		}
		
		mTime += Time::DeltaTime();
		if (mTime > 0.01)
		{
			mTime = 0.0f;
		}
		Scene::Update();
	}
	void TitleScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void TitleScene::Release()
	{
		Scene::Release();
	}
	void TitleScene::OnEnter()
	{
	}
	void TitleScene::OnExit()
	{
	}
}