#include "hjTitleScene.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjTime.h"
#include "hjImage.h"
#include "hjBird.h"
#include "hjApplication.h"
#include "hjCamera.h"

extern hj::Application application;


namespace hj {

	TitleScene::TitleScene()
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Initialize()
	{
		float windowSizeY = (float)application.GetHeight();
		SetAsRatio(Vector2{ 1.0f, 1.0f } *(windowSizeY / 180.0f));
		Vector2 asRatio = GetAsRatio();

		BackGround* bg = new BackGround(L"TitleSky", L"..\\Resource\\Title\\TitleSky.bmp", Vector2::One, asRatio);
		AddGameObject(bg, eLayerType::BackBG);

		BackGround* bg2 = new BackGround(L"BackCloud", L"..\\Resource\\Title\\BackCloud.bmp", Vector2::One * 3.0f, asRatio, true);
		AddGameObject(bg2, eLayerType::BackBG);

		BackGround* bg3 = new BackGround(L"FrontCloud", L"..\\Resource\\Title\\FrontCloud.bmp", Vector2::One * 0.5f, asRatio, true);
		AddGameObject(bg3, eLayerType::FrontBG);

		BackGround* bg4 = new BackGround(L"MainLogo", L"..\\Resource\\Title\\MainLogo.bmp", Vector2::Zero, asRatio);
		AddGameObject(bg4, eLayerType::FrontBG);


		for (int i = -2; i < 3; i++)
		{
			makeBird((double)(11.0f + abs(i) * 1.0f), Vector2{ -20.0f,600.0f - (i * 30.0f) });
		}
		double temp = 3.1415926535f;
		int index = 1;
		double spawn = 1.0f;
		for (int i = 0; i < 5; i++)
		{
			if (index == 3)
				index = 1;
			while (temp < 1.0f)
			{
				temp *= 10.0f;
			}
			spawn += (double)(index++) * 4;
			makeBird(spawn, Vector2{ -20.0f,0.0f + ((UINT)temp * 100.0f) });
			temp = temp - (UINT)temp;
		}
		Camera::SetLookRange(Vector2{ (float)application.GetWidth() * 8, 0.f });
		Scene::Initialize();

	}
	void TitleScene::Update()
	{
		if (bir == false)
			mTime += Time::DeltaTime();
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
		}
		if (Input::GetKeyState(eKeyCode::Q) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Tool);
		}

		Scene::Update();
		Bird* next = nextBird(mTime);
		if (next != nullptr)
		{
			spawnBird(next);
		}
		if (Camera::GetPos().x >= application.GetWidth() * (0.5f + 2.0f / 0.5f))
			Camera::SetPos(Vector2{ Camera::GetPos().x - application.GetWidth() * (2.0f / 0.5f),Camera::GetPos().y });

		else
			Camera::SetPos(Camera::GetPos() + Vector2{ 1.0f, 0.0f });
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
		Camera::SetLookRange(Vector2{ (float)application.GetWidth() * 8, 0.f });
	}


	void TitleScene::OnExit()
	{
		Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
	}

	void TitleScene::makeBird(int time, Vector2 pos)
	{
		Bird* bird = new Bird(time, pos, L"bird", L"..\\Resource\\Title\\Bird.bmp", GetAsRatio());
		mBirds.push_back(bird);
		AddGameObject(bird, eLayerType::BGobject);

	}
	Bird* TitleScene::nextBird(double time)
	{

		for (std::vector<Bird*>::iterator bird
			= mBirds.begin(); bird != mBirds.end();)
		{
			if ((*bird)->GetState() == GameObject::eState::Death)
			{
				bird = mBirds.erase(bird);
			}
			else
			{
				bird++;
			}
		}
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
		Vector2 spawnPos = bird->getPos();
		bird->spawn();
		bird->GetComponent<Transform>()->SetPos(spawnPos);
	}
}