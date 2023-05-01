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
#include "hjSkeletonWarrior.h"
#include "hjEliteSkelWarrior.h"
#include "hjNiflheim.h"

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
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One * (windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();


		asRatio = asRatio * 3.f;

		/*BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) });
		AddGameObject(bg2, eLayerType::BackBG);*/
		//BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1280.0f });
		//AddGameObject(bg4, eLayerType::BackBG);

		BackGround* Ice_NIflheim_foothold = new BackGround(L"Ice_NIflheim_foothold", L"../Resource/Ice/Ice_NIflheim_foothold.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Ice_NIflheim_foothold, eLayerType::BackBG);
		BackGround* Ice_NIflheim = new BackGround(L"Ice_NIflheim", L"../Resource/Ice/Ice_NIflheim.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Ice_NIflheim, eLayerType::BackBG);

		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);*/

		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void DungeonNiflheim::Update()
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
		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);
		mon->Initialize();*/

		Camera::SetLookRange(
			Vector2{ 2560.f, 1280.f }
		);
		//GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 200.0f });
		Camera::SetTarget(GetHero());
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Player, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Bullet_Player, true);
		//CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Bullet_Monster, true);

		Niflheim* mon2 = new Niflheim();
		AddGameObject(mon2, eLayerType::Monster);
		mon2->Initialize();
		mon2->GetComponent<Transform>()->SetPos(Vector2{ 1280.0f, 640.0f });
		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 0.0f, 1121.0f } +
				Vector2{ GetHero()->GetComponent<Transform>()->GetSize().x / 2.0f, 0.0f });
		wchar_t a[256] = L"../Resource/Ice/Ice_NIflheim_Collider.Tile\0";
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