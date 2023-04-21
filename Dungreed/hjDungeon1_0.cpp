#include "hjDungeon1_0.h"
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
#include "hjSkeletonWarrior.h"
#include "hjEliteSkelWarrior.h"

extern hj::Application application;

namespace hj
{
	Dungeon1_0::Dungeon1_0()
	{
	}
	Dungeon1_0::~Dungeon1_0()
	{
	}
	void Dungeon1_0::Initialize()
	{
		//float windowSizeY = (float)application.GetHeight();
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One * (windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();

		asRatio = asRatio * 3.f;


		/*BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) });
		AddGameObject(bg2, eLayerType::BackBG);*/
		//BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1280.0f });
		//AddGameObject(bg4, eLayerType::BackBG);
		

		BackGround* Dungeon1_0_foothold = new BackGround(L"Dungeon1_0_foothold", L"../Resource/Ice/Dungeon1_0.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_0_foothold, eLayerType::BackBG);
		BackGround* Dungeon1_0 = new BackGround(L"Dungeon1_0", L"../Resource/Ice/Dungeon1_0_foothold.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_0, eLayerType::BackBG);

		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);*/



		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void Dungeon1_0::Update()
	{
		if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			GetPManager()->ChangePlayScene(ePSceneType::Dungeon1_1);
			//GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
			return;
		}
		/*if (Input::GetKeyState(eKeyCode::Q) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Tool);
			return;
		}*/
		Scene::Update();
	}
	void Dungeon1_0::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void Dungeon1_0::Release()
	{
		Scene::Release();
	}
	void Dungeon1_0::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ 1600.f, 1520.f }
		);
		Camera::SetTarget(GetHero());
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, true);

		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 100.0f, 1200.0f });
		wchar_t a[256] = L"../Resource/Ice/Dungeon1_0_Collider.Tile\0";
		//wchar_t b[256] = L"../Resource/Dungeon1_0Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);


	}
	void Dungeon1_0::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		TilePalatte::clear();
		//TilePalatte::clear(1);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}