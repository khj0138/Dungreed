#include "hjDungeon1_3.h"
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
#include "hjPSceneManager.h"

#include "hjMonster.h"
#include "hjmonsterHeader.h"

extern hj::Application application;

namespace hj
{
	Dungeon1_3::Dungeon1_3()
	{
	}
	Dungeon1_3::~Dungeon1_3()
	{
	}
	void Dungeon1_3::Initialize()
	{
		//float windowSizeY = (float)application.GetHeight();
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One * (windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();

		/*BackGround* bg = new BackGround(L"back", L"..\\Resource\\SkyDay.bmp", Vector2::Zero, asRatio);
		AddGameObject(bg, eLayerType::BackBG);*/

		asRatio = asRatio * 3.f;

		BackGround* bg2 = new BackGround(L"Dungeon1_3BackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) });
		AddGameObject(bg2, eLayerType::BackBG);
		//BackGround* bg4 = new BackGround(L"Dungeon1_3FrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1280.0f });
		//AddGameObject(bg4, eLayerType::BackBG);

		BackGround* Dungeon1_3Tile = new BackGround(L"Dungeon1_3Tile", L"../Resource/Ice/NIflheimRoom.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_3Tile, eLayerType::BackBG);


		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);*/
		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void Dungeon1_3::Update()
	{
		/*if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}*/
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
	void Dungeon1_3::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void Dungeon1_3::Release()
	{
		Scene::Release();
	}
	void Dungeon1_3::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ 2560.f, 1280.f }
		);
		//GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 200.0f });
		Camera::SetTarget(GetHero());
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, true);

		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 200.0f });
		wchar_t a[256] = L"../Resource/Ice/IceCollider.Tile\0";
		//wchar_t b[256] = L"../Resource/Dungeon1_3Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);


	}
	void Dungeon1_3::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		TilePalatte::clear();
		//TilePalatte::clear(1);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}