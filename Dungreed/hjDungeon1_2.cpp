#include "hjDungeon1_2.h"
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
	Dungeon1_2::Dungeon1_2()
	{
	}
	Dungeon1_2::~Dungeon1_2()
	{
	}
	void Dungeon1_2::Initialize()
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

		BackGround* Dungeon1_2 = new BackGround(L"Dungeon1_2", L"../Resource/Ice/Dungeon1_2.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_2, eLayerType::BackBG);
		BackGround* Dungeon1_2_foothold = new BackGround(L"Dungeon1_2_foothold", L"../Resource/Ice/Dungeon1_2_foothold.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_2_foothold, eLayerType::BackBG);
	

		PlaneObject* dungeonGate = new PlaneObject(L"DungeonGate_90", L"..\\Resource\\Ice\\DungeonGate\\DungeonGate_90.bmp", Vector2::One, Vector2{ 3360.0f, 166.0f }, Vector2::One);
		AddGameObject(dungeonGate, eLayerType::FrontBG);
		DungeonGate.push_back(dungeonGate);

		dungeonGate = new PlaneObject(L"DungeonGate_270", L"..\\Resource\\Ice\\DungeonGate\\DungeonGate_270.bmp", Vector2::One, Vector2{ 0.0f, 1126.0f }, Vector2::One);
		AddGameObject(dungeonGate, eLayerType::FrontBG);
		DungeonGate.push_back(dungeonGate);
		//Monster* mon = new Monster();
		//AddGameObject(mon, eLayerType::Monster);

		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void Dungeon1_2::Update()
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
		if (GetHero()->GetComponent<Transform>()->GetPos().x >= 3360.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y >= 160.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y <= 481.0f)
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
	void Dungeon1_2::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void Dungeon1_2::Release()
	{
		Scene::Release();
	}
	void Dungeon1_2::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ 3440.f, 1600.f }
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


		Ovibos* ovibos = new Ovibos();
		AddGameObject(ovibos, eLayerType::Monster);
		ovibos->Initialize();
		ovibos->GetComponent<Transform>()->SetPos(Vector2{ 2640.0f, 961.0f });

		SkelIceMagician* skelIceMagician = new SkelIceMagician();
		AddGameObject(skelIceMagician, eLayerType::Monster);
		skelIceMagician->Initialize();
		skelIceMagician->GetComponent<Transform>()->SetPos(Vector2{ 480.0f, 320.0f });

		SkelIceMagician* skelIceMagician2 = new SkelIceMagician();
		AddGameObject(skelIceMagician2, eLayerType::Monster);
		skelIceMagician2->Initialize();
		skelIceMagician2->GetComponent<Transform>()->SetPos(Vector2{ 3160.0f, 1120.0f });

		EliteIceSkelWarrior* eliteIceSkelWarrior = new EliteIceSkelWarrior();
		AddGameObject(eliteIceSkelWarrior, eLayerType::Monster);
		eliteIceSkelWarrior->Initialize();
		eliteIceSkelWarrior->GetComponent<Transform>()->SetPos(Vector2{ 1760.0f, 481.0f });


		EliteSkelWarrior* eliteSkelWarrior = new EliteSkelWarrior();
		AddGameObject(eliteSkelWarrior, eLayerType::Monster);
		eliteSkelWarrior->Initialize();
		eliteSkelWarrior->GetComponent<Transform>()->SetPos(Vector2{ 2640.0f, 1441.0f });


		SkeletonWarrior* skeletonWarrior = new SkeletonWarrior();
		AddGameObject(skeletonWarrior, eLayerType::Monster);
		skeletonWarrior->Initialize();
		skeletonWarrior->GetComponent<Transform>()->SetPos(Vector2{ 1120.0f, 1441.0f });
		
		SkeletonWarrior* skeletonWarrior2 = new SkeletonWarrior();
		AddGameObject(skeletonWarrior2, eLayerType::Monster);
		skeletonWarrior2->Initialize();
		skeletonWarrior2->GetComponent<Transform>()->SetPos(Vector2{ 1600.0f, 1441.0f });

		GetHero()->monsterNum = 7;
		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 0.0f, 1441.0f } +
				Vector2{ GetHero()->GetComponent<Transform>()->GetSize().x / 2.0f, 0.0f });
		wchar_t a[256] = L"../Resource/Ice/Dungeon1_2_Collider.Tile\0";
		//wchar_t b[256] = L"../Resource/Dungeon1_2Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);


	}
	void Dungeon1_2::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		wchar_t clean[256] = L"\0";
		TilePalatte::Load(clean);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}