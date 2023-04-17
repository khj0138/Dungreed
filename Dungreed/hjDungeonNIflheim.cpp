#include "hjDungeonNiflheim.h"

#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjMonster.h"
#include "hjCollisionManager.h"
#include "hjObject.h"
#include "hjApplication.h"
#include "hjPlatform.h"
#include "hjCamera.h"
#include "hjGround.h"
#include "hjTilePalatte.h"
#include "hjPlaneObject.h"
#include "hjMonster.h"

extern hj::Application application;

namespace hj
{
	DungeonNiflheim::DungeonNiflheim()
	{
	}
	DungeonNiflheim::~DungeonNiflheim()
	{
	}
	void DungeonNiflheim::Initialize()
	{
		//float windowSizeY = (float)application.GetHeight();
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One * (windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();

		/*BackGround* bg = new BackGround(L"back", L"..\\Resource\\SkyDay.bmp", Vector2::Zero, asRatio);
		AddGameObject(bg, eLayerType::BackBG);*/

		asRatio = asRatio * 3.f;

		BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) });
		AddGameObject(bg2, eLayerType::BackBG);
		//BackGround* bg3 = new BackGround(L"TownBG2", L"..\\Resource\\Town\\TownBG_Day.bmp", Vector2{ 0.2f, 0.2f }, asRatio, true, 0, Vector2{ 0.0f, 1680.0f - 900.0f * (1.0f - 0.2f) });
		//AddGameObject(bg3, eLayerType::BackBG);
		BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1280.0f });
		AddGameObject(bg4, eLayerType::BackBG);

		BackGround* DungeonNiflheimTile = new BackGround(L"DungeonNiflheimTile", L"../Resource/Ice/NIflheimRoom.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(DungeonNiflheimTile, eLayerType::BackBG);
		/*PlaneObject* TownFloor = new PlaneObject(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", asRatio, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) }, Vector2{ 0.8f, 0.8f });
		AddGameObject(TownFloor, eLayerType::BGobject);
		PlaneObject* TownFloor22 = new PlaneObject(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", asRatio, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) }, Vector2{ 0.8f, 0.8f });
		AddGameObject(TownFloor22, eLayerType::BGobject);*/
		//BackGround* DungeonNiflheimTile = new BackGround(L"DungeonNiflheimTile", L"../Resource/Ice/Ice.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		//AddGameObject(DungeonNiflheimTile, eLayerType::FrontBG);

		/*PlaneObject* DungeonNiflheimFloor = new PlaneObject(L"DungeonNiflheimFloor", L"..\\Resource\\DungeonNiflheim\\DungeonNiflheimFloor.bmp", asRatio, Vector2{ 0.0f, 1600.0f }, Vector2::One);
		AddGameObject(DungeonNiflheimFloor, eLayerType::BGobject);*/

		Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);



		//Platform* floor = new Platform(L"DungeonNiflheimFloor", L"..\\Resource\\DungeonNiflheim\\DungeonNiflheimFloor.bmp", 1.0f,asRatio);
		//AddGameObject(floor, eLayerType::Platform);

		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void DungeonNiflheim::Update()
	{
		if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}
		/*if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Title);
			return;
		}*/
		/*if (Input::GetKeyState(eKeyCode::Q) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Tool);
			return;
		}*/
		Scene::Update();
	}
	void DungeonNiflheim::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void DungeonNiflheim::Release()
	{
		Scene::Release();
	}
	void DungeonNiflheim::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ 2560.f, 1280.f }
		);
		//GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 200.0f });
		Camera::SetTarget(GetHero());
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon, eLayerType::Monster, true);

		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 200.0f });
		wchar_t a[256] = L"../Resource/Ice/IceCollider.Tile\0";
		//wchar_t b[256] = L"../Resource/DungeonNiflheimTile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);
		

	}
	void DungeonNiflheim::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		TilePalatte::clear();
		//TilePalatte::clear(1);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}