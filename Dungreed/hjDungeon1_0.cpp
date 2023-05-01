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
#include "hjAnimObject.h"

#include "hjRigidBody.h"
#include "hjMonster.h"
#include "hjSkeletonWarrior.h"
#include "hjGameObject.h"
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


		BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, false, 0, Vector2{ 0.0f, 1520.0f - (1200.0f) * (1.0f - 0.6f) });
		AddGameObject(bg2, eLayerType::BackBG);
		BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1520.0f - (1200.0f) * (1.0f - 0.8f) });
		AddGameObject(bg4, eLayerType::BackBG);
		

		BackGround* Dungeon1_0_foothold = new BackGround(L"Dungeon1_0_foothold", L"../Resource/Ice/Dungeon1_0.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_0_foothold, eLayerType::BackBG);
		BackGround* Dungeon1_0 = new BackGround(L"Dungeon1_0", L"../Resource/Ice/Dungeon1_0_foothold.bmp", Vector2::One * 1.0f, asRatio / 5.f, false);
		AddGameObject(Dungeon1_0, eLayerType::BackBG);
		AnimObject* IceDoorClose = new AnimObject(L"IceDoorClose", L"../Resource/Ice/IceDoorClose.bmp", asRatio, Vector2{ 10.0f, 1.0f }, Vector2{ 500.0f, 1360.0f });
		AddGameObject(IceDoorClose, eLayerType::BackBG);

		/*BackGround* Dungeon1_0 = new BackGround(L"IceDoorClose", L"../Resource/Ice/IceDoorClose.bmp", Vector2::One * 1.0f, asRatio / 5.f, false, 0, Vector2{ 0.0f, 1280.0f });
		AddGameObject(Dungeon1_0, eLayerType::BackBG);*/
		
		/*Monster* mon = new Monster();
		AddGameObject(mon, eLayerType::Monster);*/



		asRatio = asRatio / 3.f;

		Scene::Initialize();
	}
	void Dungeon1_0::Update()
	{
		/*if (Input::GetKeyDown(eKeyCode::T))
		{
			TilePalatte::tRenderChange();
			BackGround::bRenderChange();
		}*/
		if (GetHero()->GetComponent<Transform>()->GetPos().x >= 1520.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y >= 640.0f
			&& GetHero()->GetComponent<Transform>()->GetPos().y <= 881.0f)
		{
			GetPManager()->ChangePlayScene(ePSceneType::Dungeon1_1);
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
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Player, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Bullet_Player, true);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Bullet_Monster, true);

		if (GetHero() != nullptr)
			GetHero()->GetComponent<Transform>()->SetPos(Vector2{ 500.0f, 1361.0f });
		GetHero()->StateChange(Hero::eHeroState::Idle, L"Idle", true);
		GetHero()->GetComponent<Rigidbody>()->SetVelocity(Vector2::Zero);
		wchar_t a[256] = L"../Resource/Ice/Dungeon1_0_Collider.Tile\0";
		//wchar_t b[256] = L"../Resource/Dungeon1_0Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		TilePalatte::Load(a);


	}
	void Dungeon1_0::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		Camera::SetTarget(nullptr);

		wchar_t clean[256] = L"\0";
		TilePalatte::Load(clean);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, false);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, false);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, false);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Player, false);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Bullet_Player, false);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Monster, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Bullet_Monster, false);
		CollisionManager::Clear();
	}
}