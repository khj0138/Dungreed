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
		
		
		BackGround* bg2 = new BackGround();
		AddGameObject(bg2, eLayerType::BG);
		bg2->setBG(L"backTitle", L"..\\Resource\\Title\\backCloud.bmp");
		bg2->setV(1.f);
		BackGround* bg3 = new BackGround();
		AddGameObject(bg3, eLayerType::BG);
		bg3->setBG(L"backTitle", L"..\\Resource\\Title\\FrontCloud.bmp");
		bg3->setV(1.f);
		BackGround* bg = new BackGround();
		AddGameObject(bg, eLayerType::BG);
		bg->setBG(L"backTitle", L"..\\Resource\\Title\\TitleSky.bmp");
		bg->setV(0.0f);
	}
	void TitleScene::Update()
	{
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
		}
		
		mTime += Time::Deltatime();
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