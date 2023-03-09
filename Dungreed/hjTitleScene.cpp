#include "hjTitleScene.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjTime.h"
#include "hjImage.h"
#include "hjBird.h"
#include "hjApplication.h"


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
		AddGameObject(bg, eLayerType::BackBG);
		bg->setAnimation(L"TitleSky", L"..\\Resource\\Title\\TitleSky.bmp", 0.0f);

		BackGround* bg2 = new BackGround();
		AddGameObject(bg2, eLayerType::BackBG);
		bg2->setAnimation(L"BackCloud", L"..\\Resource\\Title\\BackCloud.bmp", 15.f);

		BackGround* bg3 = new BackGround();
		AddGameObject(bg3, eLayerType::FrontBG);
		bg3->setAnimation(L"FrontCloud", L"..\\Resource\\Title\\FrontCloud.bmp", 25.f);

		BackGround* bg4 = new BackGround();
		AddGameObject(bg4, eLayerType::FrontBG);
		bg4->setAnimation(L"MainLogo", L"..\\Resource\\Title\\MainLogo.bmp", 0.0f);

		/*for (int i = 0; i < 7; i++)
		{
			int j = i;
			int k = i;
			if (i == 3)
				j = 1;
			if (i >= 4)
			{
				j = 6 - i;
				k = 6 - i;
			}
			makeBird((double)(1.0f + abs(i) * 1.0f), Vector2{ -20.0f,500.0f - (j * 70.0f) });
			makeBird((double)(1.0f + abs(i) * 1.0f), Vector2{ -20.0f,500.0f + (k * 70.0f) });
		}*/
		for (int i = -2; i < 3; i++)
		{
			makeBird((double)(1.0f + abs(i) * 1.0f), Vector2{ -20.0f,500.0f - (i * 30.0f) });
		}
		double temp = 3.1415926535f;
		int index = 1;
		for (int i = 0; i < 10; i++)
		{
			if (index == 3)
				index = 1;
			while (temp < 1.0f)
			{
				temp *= 10.0f;
			}
			makeBird((double)(5.0f + index++ * i * 1.0f), Vector2{ -20.0f,0.0f + ((UINT)temp * 100.0f) });
			temp = temp - (UINT)temp;
		}

		Scene::Initialize();

	}
	void TitleScene::Update()
	{
		if(bir == false)
			mTime += Time::DeltaTime();
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
		}
		Bird* next = nextBird(mTime);
		if (next != nullptr)
		{
			spawnBird(next);
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
	void TitleScene::makeBird(int time, Vector2 pos)
	{
		Bird* bird = new Bird(time, pos);
		mBirds.push_back(bird);
		AddGameObject(bird, eLayerType::BGobject);
		bird->GetComponent<Transform>()->SetScale(Vector2{ 5.0f, 5.0f });
		bird->setAnimation(L"bird", L"..\\Resource\\Title\\Bird.bmp");
	}
	Bird* TitleScene::nextBird(double time)
	{
		/*std::map<int, Bird*>::iterator iter
			= mBirds.find(index);*/
		for (Bird* bird : mBirds)
		{
			if (bird->getSpawn() == false && bird->getSpawnTime() <= time)
			{
				return bird;
			}
		}
		return nullptr;
	}
	void TitleScene::spawnBird(Bird* bird)
	{
		//Bird* bird = nextBird(mTime);
		Vector2 spawnPos = bird->getPos();
		bird->spawn();
		bird->GetComponent<Transform>()->SetPos(spawnPos);
	}
}