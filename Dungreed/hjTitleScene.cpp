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
		Vector2 asRatio = Vector2{ 1.0f, 1.0f } *(windowSizeY / 180.0f);

		//Scene::setAsRatio(asRatio);
		Image::SetAsRatio(asRatio);
		BackGround* bg = new BackGround();
		AddGameObject(bg, eLayerType::BackBG);
		bg->setAnimation(L"TitleSky", L"..\\Resource\\Title\\TitleSky.bmp", 0.0f);
		
		BackGround* bg2 = new BackGround();
		AddGameObject(bg2, eLayerType::BackBG);
		bg2->setAnimation(L"BackCloud", L"..\\Resource\\Title\\BackCloud.bmp", 3.f, true);
		
		BackGround* bg3 = new BackGround();
		AddGameObject(bg3, eLayerType::FrontBG);
		bg3->setAnimation(L"FrontCloud", L"..\\Resource\\Title\\FrontCloud.bmp", 0.5f, true);
		
		BackGround* bg4 = new BackGround();
		AddGameObject(bg4, eLayerType::FrontBG);
		bg4->setAnimation(L"MainLogo", L"..\\Resource\\Title\\MainLogo.bmp", 0.0f);


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
		
		Scene::Update();
		Bird* next = nextBird(mTime);
		if (next != nullptr)
		{
			spawnBird(next);
		}
		if (Camera::GetPos().x >= application.GetWidth() * 4.5f)
			Camera::SetPos(Vector2{ Camera::GetPos().x- application.GetWidth() * 4.0f,Camera::GetPos().y});
			//Camera::SetPos(Vector2{ Camera::GetPos().x- application.GetWidth() / 2.0f, application.GetHeight() / 2.0f });
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
		Camera::SetLookRange(
			Vector2{ (float)application.GetWidth() * 5.0f, -(float)application.GetHeight() * 5.0f }
		);
	}
	void TitleScene::OnExit()
	{
		Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
	}
	void TitleScene::makeBird(int time, Vector2 pos)
	{
		Bird* bird = new Bird(time, pos);
		mBirds.push_back(bird);
		AddGameObject(bird, eLayerType::BGobject);
		
		bird->setAnimation(L"bird", L"..\\Resource\\Title\\Bird.bmp");
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
		//Bird* bird = nextBird(mTime);
		Vector2 spawnPos = bird->getPos();
		bird->spawn();
		bird->GetComponent<Transform>()->SetPos(spawnPos);
	}
}