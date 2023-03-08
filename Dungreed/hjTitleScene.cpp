#include "hjTitleScene.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjTime.h"
#include "hjImage.h"
#include "hjBird.h"

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
		bg->setAnimation(L"TitleSky", L"..\\Resource\\Title\\TitleSky.bmp", 0.0f);
		AddGameObject(bg, eLayerType::BackBG);

		BackGround* bg2 = new BackGround();
		bg2->setAnimation(L"BackCloud", L"..\\Resource\\Title\\BackCloud.bmp", 3.f);
		AddGameObject(bg2, eLayerType::BackBG);

		BackGround* bg3 = new BackGround();
		bg3->setAnimation(L"FrontCloud", L"..\\Resource\\Title\\FrontCloud.bmp", 8.f);
		AddGameObject(bg3, eLayerType::FrontBG);

		BackGround* bg4 = new BackGround();
		bg4->setAnimation(L"MainLogo", L"..\\Resource\\Title\\MainLogo.bmp", 0.0f);
		AddGameObject(bg4, eLayerType::FrontBG);


		Scene::Initialize();

		Vector2 size = bg->GetSize();
		bg->setScale(Vector2{ (1600.f / size.x), (900.f / size.y) });

		size = bg2->GetSize();
		bg2->setScale(Vector2{ (1600.f / size.x), (900.f / size.y) });

		size = bg3->GetSize();
		bg3->setScale(Vector2{ (1600.f / size.x), (900.f / size.y) });
		
		size = bg4->GetSize();
		bg4->setScale(Vector2{ (1600.f / size.x), (900.f / size.y) });
	}
	void TitleScene::Update()
	{
		if(bir == false)
			mTime += Time::DeltaTime();
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
		}
		if (mTime > 2)
		{
			mTime = 0.0f;
			makeBird();
			
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
	void TitleScene::makeBird()
	{
		Bird* bird = new Bird();
		AddGameObject(bird, eLayerType::BGobject);
		bird->GetComponent<Transform>()->SetPos(Vector2{ 800.0f, 500.0f });
		bird->GetComponent<Transform>()->SetScale(Vector2{ 5.0f, 5.0f });
		bird->setAnimation(L"bird", L"..\\Resource\\Title\\Bird.bmp");
	}
}