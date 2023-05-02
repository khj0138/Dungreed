#include "hjDungeon1_1.h"
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
#include "hjSkeletonArcher.h"
#include "hjEliteSkelWarrior.h"
#include "hjEliteIceSkelWarrior.h"

extern hj::Application application;

namespace hj
{
	Dungeon1_1::Dungeon1_1()
	{
	}
	Dungeon1_1::~Dungeon1_1()
	{
	}
	void Dungeon1_1::Initialize()
	{
		//float windowSizeY = (float)application.GetHeight();
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One * (windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();

		/*BackGround* bg = new BackGround(L"back", L"..\\Resource\\SkyDay.bmp", Vector2::Zero, asRatio);
		AddGameObject(bg, eLayerType::BackBG);*/

		asRatio = asRatio * 3.f;

		/*BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) });
		AddGameObject(bg2, eLayerType::BackBG);*/
		//BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1280.0f });
		//AddGameObject(bg4, eLayerType::BackBG);

		BackGround* Dungeon1_1 = new BackGround(L"Dungeon1_1", L"../Resource/Ice/Dungeon1_1.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_1, eLayerType::BackBG);
		BackGround* Dungeon1_1_foothold = new BackGround(L"Dungeon1_1_foothold", L"../Resource/Ice/Dungeon1_1_foothold.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_1_foothold, eLayerType::BackBG);
		PlaneObject* Dungeon1_1_model = new PlaneObject(L"Dungeon1_1_model", L"..\\Resource\\Ice\\Dungeon1_1_model.bmp", Vector2::One, Vector2{ 860.0f, 960.0f } - Vector2{75.f, 150.f}, Vector2::One);
		AddGameObject(Dungeon1_1_model, eLayerType::FrontBG);
		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);*/

		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void Dungeon1_1::Update()
	{
		/*if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}*/
		//if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		//{
		//	GetPManager()->ChangePlayScene(ePSceneType::Dungeon1_2);
		//	//GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
		//	return;
		//}
		if (GetHero()->GetComponent<Transform>()->GetPos().x >= 2800.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y >= 400.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y <= 720.0f)
		{
			GetPManager()->ChangePlayScene(ePSceneType::Dungeon1_2);
			//GetHero()->GameObject::SetState(GameObject::eState::Pause);
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
	void Dungeon1_1::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void Dungeon1_1::Release()
	{
		Scene::Release();
	}
	void Dungeon1_1::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ 2880.f, 1120.f }
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


		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();

		/*EliteSkelWarrior* mon = new EliteSkelWarrior();
		AddGameObject(mon, eLayerType::Monster);
		mon->Initialize();*/

		SkeletonWarrior* skelWarrior1 = new SkeletonWarrior();
		AddGameObject(skelWarrior1, eLayerType::Monster);
		skelWarrior1->Initialize();
		skelWarrior1->GetComponent<Transform>()->SetPos(
			Vector2{ 960.0f, 721.0f });

		SkeletonWarrior* skelWarrior2 = new SkeletonWarrior();
		AddGameObject(skelWarrior2, eLayerType::Monster);
		skelWarrior2->Initialize();
		skelWarrior2->GetComponent<Transform>()->SetPos(
			Vector2{ 2000.0f, 721.0f });

		SkeletonArcher* skelArcher1 = new SkeletonArcher();
		AddGameObject(skelArcher1, eLayerType::Monster);
		skelArcher1->Initialize();
		skelArcher1->GetComponent<Transform>()->SetPos(
			 Vector2{ 760.0f, 401.0f });

		SkeletonArcher* skelArcher2 = new SkeletonArcher();
		AddGameObject(skelArcher2, eLayerType::Monster);
		skelArcher2->Initialize();
		skelArcher2->GetComponent<Transform>()->SetPos(
			Vector2{ 2200.0f, 401.0f });

		EliteIceSkelWarrior* eliteSkelIce1 = new EliteIceSkelWarrior();
		AddGameObject(eliteSkelIce1, eLayerType::Monster);
		eliteSkelIce1->Initialize();
		eliteSkelIce1->GetComponent<Transform>()->SetPos(
			Vector2{ 1480.0f, 721.0f });
		
		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 120.0f, 721.0f });

		wchar_t a[256] = L"../Resource/Ice/Dungeon1_1_Collider.Tile\0";
		//wchar_t b[256] = L"../Resource/Dungeon1_1Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);
		

	}
	void Dungeon1_1::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		wchar_t clean[256] = L"\0";
		TilePalatte::Load(clean);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}