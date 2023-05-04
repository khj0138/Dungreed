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


		asRatio = asRatio * 3.f;

		BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.4f, 0.6f }, asRatio, false, 0, Vector2{ 0.0f, 1600.0f - 900.0f * (1.0f - 0.6f) });
		AddGameObject(bg2, eLayerType::BackBG);
		BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.6f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1600.0f - 900.0f * (1.0f - 0.6f) });
		AddGameObject(bg4, eLayerType::BackBG);

		BackGround* Dungeon1_3 = new BackGround(L"Dungeon1_3", L"../Resource/Ice/Dungeon1_3.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_3, eLayerType::BackBG);
		BackGround* Dungeon1_3_foothold = new BackGround(L"Dungeon1_3_foothold", L"../Resource/Ice/Dungeon1_3_foothold.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_3_foothold, eLayerType::BackBG);


		PlaneObject* dungeonGate = new PlaneObject(L"DungeonGate_90", L"..\\Resource\\Ice\\DungeonGate\\DungeonGate_90.bmp", Vector2::One, Vector2{ 1520.0f, 166.0f }, Vector2::One);
		AddGameObject(dungeonGate, eLayerType::FrontBG);
		DungeonGate.push_back(dungeonGate);

		dungeonGate = new PlaneObject(L"DungeonGate_270", L"..\\Resource\\Ice\\DungeonGate\\DungeonGate_270.bmp", Vector2::One, Vector2{ 0.0f, 166.0f }, Vector2::One);
		AddGameObject(dungeonGate, eLayerType::FrontBG);
		DungeonGate.push_back(dungeonGate);
		//Monster* mon = new Monster();
		//AddGameObject(mon, eLayerType::Monster);

		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void Dungeon1_3::Update()
	{
		/*if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}*/
		//if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		//{
		//	GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
		//	//GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
		//	return;
		//}
		if (GetHero()->monsterNum == 0)
		{
			for (auto gate : DungeonGate)
				gate->SetState(GameObject::eState::Pause);
		}
		if (GetHero()->GetComponent<Transform>()->GetPos().x >= 1520.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y >= 160.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y <= 481.0f)
		{
			GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
			return;
		}
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
			return;
		}
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
			Vector2{ 1600.f, 1600.f }
		);
		//GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 200.0f });
		Camera::SetTarget(GetHero());
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Player, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Bullet_Player, true);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Bullet_Monster, true);

		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 120.0f, 481.0f });
		SkeletonArcher* skeletonArcher;
		SkeletonArcher* skeletonArcher2;
		SkelDog* skeldog;
		for (int i = 0; i < 4; i++)
		{
			skeletonArcher = new SkeletonArcher();
			AddGameObject(skeletonArcher, eLayerType::Monster);
			skeletonArcher->Initialize();
			skeletonArcher->GetComponent<Transform>()->SetPos(Vector2{ 200.0f, 481.0f + 240.0f * (float)i});

			skeletonArcher2 = new SkeletonArcher();
			AddGameObject(skeletonArcher2, eLayerType::Monster);
			skeletonArcher2->Initialize();
			skeletonArcher2->GetComponent<Transform>()->SetPos(Vector2{ 1400.0f, 481.0f + 240.0f * (float)i });

			skeldog = new SkelDog();
			AddGameObject(skeldog, eLayerType::Monster);
			skeldog->Initialize();
			skeldog->GetComponent<Transform>()->SetPos(Vector2{ 200.0f + 300.0f * (float)i, 1441.0f });
		}

		Ovibos* ovibos = new Ovibos();
		AddGameObject(ovibos, eLayerType::Monster);
		ovibos->Initialize();
		ovibos->GetComponent<Transform>()->SetPos(Vector2{ 1280.0f, 1441.0f });

		Ovibos* ovibos2 = new Ovibos();
		AddGameObject(ovibos2, eLayerType::Monster);
		ovibos2->Initialize();
		ovibos2->GetComponent<Transform>()->SetPos(Vector2{ 320.0f, 1441.0f });

		

		GetHero()->monsterNum = 14;
		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 120.0f, 481.0f } +
				Vector2{ GetHero()->GetComponent<Transform>()->GetSize().x / 2.0f, 0.0f });
		wchar_t a[256] = L"../Resource/Ice/Dungeon1_3_Collider.Tile\0";
		//wchar_t b[256] = L"../Resource/Dungeon1_3Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);


	}
	void Dungeon1_3::OnExit()
	{
		PlayScene::OnExit();
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		wchar_t clean[256] = L"\0";
		TilePalatte::Load(clean);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}