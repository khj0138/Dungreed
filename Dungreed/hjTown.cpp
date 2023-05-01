#include "hjTown.h"
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
#include "hjRigidBody.h"

#include "hjMonster.h"
#include "hjSkeletonWarrior.h"
#include "hjEliteSkelWarrior.h"
#include "hjAnimObject.h"
#include "hjNiflheim.h"
extern hj::Application application;

namespace hj
{
	Town::Town()
	{
	}
	Town::~Town()
	{
	}
	void Town::Initialize()
	{
		//float windowSizeY = (float)application.GetHeight();
		float windowSizeX = (float)application.GetWidth();
		Scene::SetAsRatio(Vector2::One * (windowSizeX / 960.0f));
		Vector2 asRatio = Scene::GetAsRatio();

		BackGround* bg = new BackGround(L"back", L"..\\Resource\\SkyDay.bmp", Vector2::Zero, asRatio);
		AddGameObject(bg, eLayerType::BackBG);

		asRatio = asRatio * 3.f;
		
		BackGround* bg2 = new BackGround(L"TownBG", L"..\\Resource\\Town\\TownBG_Day.bmp", Vector2{ 0.2f, 0.2f }, asRatio, true, 0, Vector2{0.0f, 1680.0f - 900.0f * (1.0f - 0.2f)});
		AddGameObject(bg2, eLayerType::BackBG);

		BackGround* bg3 = new BackGround(L"TownLayer", L"..\\Resource\\Town\\TownLayer_Day.bmp", Vector2{0.5f, 0.5f}, asRatio, true, 0, Vector2{0.0f, 1680.0f - 900.0f * (1.0f - 0.5f)});
		AddGameObject(bg3, eLayerType::BackBG);
		
		BackGround* TownTile = new BackGround(L"TownTile", L"../Resource/Town/TownGround.bmp", Vector2::One * 1.0f, asRatio/5.f , false);
		AddGameObject(TownTile, eLayerType::FrontBG);

		//BackGround* TownTile = new BackGround(L"TownTile", L"../Resource/Ice/Ice.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		//AddGameObject(TownTile, eLayerType::FrontBG);

		PlaneObject* TownFloor = new PlaneObject(L"TownFloor", L"..\\Resource\\Town\\TownFloor.bmp", asRatio, Vector2{ 0.0f, 1600.0f}, Vector2::One);
		AddGameObject(TownFloor, eLayerType::Ground);

		DungeonEatOpen = new AnimObject(L"DungeonEatOpen", L"../Resource/Town/DungeonEatOpen.bmp", asRatio, Vector2{ 8.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, Vector2::One, 0.05f);
		AddGameObject(DungeonEatOpen, eLayerType::Ground);
		DungeonEatClose = new AnimObject(L"DungeonEatClose", L"../Resource/Town/DungeonEatClose.bmp", asRatio, Vector2{ 20.0f, 1.0f }, Vector2{ 0.0f, 0.0f }, Vector2::One, 0.05f);
		AddGameObject(DungeonEatClose, eLayerType::Ground);

		DungeonEatOpen->SetState(GameObject::eState::Pause);
		DungeonEatClose->SetState(GameObject::eState::Pause);
		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);*/
		
		

		//Platform* floor = new Platform(L"TownFloor", L"..\\Resource\\Town\\TownFloor.bmp", 1.0f,asRatio);
		//AddGameObject(floor, eLayerType::Platform);

		asRatio = asRatio / 3.f;

		Scene::Initialize();
		DungeonEatOpen->GetComponent<Animator>()->GetCompleteEvent(L"anim") = std::bind(&Town::DungeonEatOpenEvent, this);
		DungeonEatClose->GetComponent<Animator>()->GetCompleteEvent(L"anim") = std::bind(&Town::DungeonEatCloseEvent, this);
	}
	void Town::Update()
	{
		/*if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
		}*/
		if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			GetPManager()->ChangePlayScene(ePSceneType::Dungeon1_0);
			//GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
			return;
		}
		if (
			GetHero()->GetComponent<Transform>()->GetPos().x >= 4000.0f &&
			GetHero()->GetComponent<Transform>()->GetPos().x <= 5320.0f &&
			GetHero()->GetComponent<Transform>()->GetPos().y >= 1590.0f &&
			GetHero()->GetComponent<Rigidbody>()->GetGround() &&
			DungeonEatOpen->GetState() == GameObject::eState::Pause &&
			DungeonEatClose->GetState() == GameObject::eState::Pause
			)
		{
			DungeonEatOpen->SetState(GameObject::eState::Active);
			GetHero()->SetState(GameObject::eState::Wait);
			GetHero()->StateChange(Hero::eHeroState::Idle, L"Idle", true);
			GetHero()->GetComponent<Rigidbody>()->SetVelocity(Vector2::Zero);
			Transform* tr = DungeonEatOpen->GetComponent<Transform>();
			tr->SetPos(GetHero()->GetComponent<Transform>()->GetPos()
				- Vector2{ tr->GetSize().x / 2.f, tr->GetSize().y }
			);
			//GetPManager()->ChangePlayScene(ePSceneType::DungeonNiflheim);
			return;
		}
		/*if (Input::GetKeyState(eKeyCode::Q) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Tool);
			return;
		}*/
		if (Input::GetKeyDown(eKeyCode::L))
		{
			//TilePalatte::Load();
			//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";
			TilePalatte::Load();
		}

		Scene::Update();
	}
	void Town::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
	void Town::Release()
	{
		Scene::Release();
	}
	void Town::OnEnter()
	{
		Camera::SetLookRange(
			Vector2{ 9040.f, 1840.f }
		);
		Camera::SetTarget(GetHero());
		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		//CollisionManager::SetLayer(eLayerType::Bullet, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Player, true);


		/*SkeletonWarrior* mon = new SkeletonWarrior();
		AddGameObject(mon, eLayerType::Monster);
		mon->Initialize();*/

		/*EliteSkelWarrior* mon = new EliteSkelWarrior();
		AddGameObject(mon, eLayerType::Monster);
		mon->Initialize();

		SkeletonWarrior* mon2 = new SkeletonWarrior();
		AddGameObject(mon2, eLayerType::Monster);
		mon2->Initialize();
		mon2->GetComponent<Transform>()->SetPos(
			mon2->GetComponent<Transform>()->GetPos() + Vector2{ 200.0f, 0.0f });*/

		/*Niflheim* mon2 = new Niflheim();
		AddGameObject(mon2, eLayerType::Monster);
		mon2->Initialize();
		mon2->GetComponent<Transform>()->SetPos(Vector2{ 3200.0f, 450.0f });*/
		/*for (GameObject* gameObj : SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile))
		{
			delete gameObj;
			gameObj = nullptr;
		}*/
		//SceneManager::FindScene(eSceneType::Play)->GetGameObjects(eLayerType::Tile).clear();
		wchar_t a[256] = L"../Resource/Town/TownCollider.Tile\0";
		//wchar_t b[256] = L"../Resource/TownTile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);
		//TilePalatte::Load(b);

	}
	void Town::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		//TilePalatte::clear();
		//TilePalatte::clear(1);
		wchar_t clean[256] = L"\0";
		TilePalatte::Load(clean);
		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);

		GetHero()->SetState(GameObject::eState::Active);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, false);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, false);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, false);
	}
	void Town::DungeonEatOpenEvent()
	{
		GetHero()->SetState(GameObject::eState::Pause);
		DungeonEatClose->SetState(GameObject::eState::Active);
		DungeonEatOpen->SetState(GameObject::eState::Pause);
		Transform* tr = DungeonEatClose->GetComponent<Transform>();
		tr->SetPos(GetHero()->GetComponent<Transform>()->GetPos()
			- Vector2{ tr->GetSize().x / 2.f, tr->GetSize().y }
		);
		//return;
	}
	void Town::DungeonEatCloseEvent()
	{
		GetPManager()->ChangePlayScene(ePSceneType::Dungeon1_0);
		//return;
	}
}