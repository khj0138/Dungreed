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
#include "hjTime.h"

#include "hjMonster.h"
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

		BackGround* bg2 = new BackGround(L"DungeonNiflheimBackBG", L"../Resource/Ice/IceBackBG.bmp", Vector2{ 0.8f, 0.8f }, asRatio, false, 0, Vector2{ 0.0f, 1280.0f - 900.0f * (1.0f - 0.8f) });
		AddGameObject(bg2, eLayerType::BackBG);
		BackGround* bg4 = new BackGround(L"DungeonNiflheimFrontBG", L"../Resource/Ice/IceFrontBG.bmp", Vector2{ 0.8f, 0.6f }, asRatio, true, 0, Vector2{ 0.0f, 1280.0f });
		AddGameObject(bg4, eLayerType::BackBG);

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
		if (mTime < 1.0f)
			mTime += Time::DeltaTime();
		else if (mTime < 2.0f)
		{
			Camera::SetTarget(GetHero());
			GetHero()->SetState(GameObject::eState::Active);
			niflheim->SetState(GameObject::eState::Active);
			Camera::SetBVelocity(false);
			mTime = 2.0f;
		}
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

		
		if (niflheim->GetState() == GameObject::eState::Pause && mTime >= 2)
		{
			for (auto gate : DungeonGate)
				gate->SetState(GameObject::eState::Pause);
		}
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
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Tile, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Monster, eLayerType::Player, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Player, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Bullet_Player, true);
		//CollisionManager::SetLayer(eLayerType::Tile, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Bullet_Monster, true);
		CollisionManager::SetLayer(eLayerType::Weapon_Player, eLayerType::Bullet_Monster, true);

		PlaneObject* dungeonGate = new PlaneObject(L"DungeonGate_90", L"..\\Resource\\Ice\\DungeonGate\\DungeonGate_90.bmp", Vector2::One, Vector2{ 2480.0f, 166.0f }, Vector2::One);
		AddGameObject(dungeonGate, eLayerType::FrontBG);
		DungeonGate.push_back(dungeonGate);

		dungeonGate = new PlaneObject(L"DungeonGate_270", L"..\\Resource\\Ice\\DungeonGate\\DungeonGate_270.bmp", Vector2::One, Vector2{ 0.0f, 806.0f }, Vector2::One);
		AddGameObject(dungeonGate, eLayerType::FrontBG);
		DungeonGate.push_back(dungeonGate);

		Niflheim* mon2 = new Niflheim();
		AddGameObject(mon2, eLayerType::Monster);
		mon2->Initialize();
		mon2->GetComponent<Transform>()->SetPos(Vector2{ 1280.0f, 600.0f });
		niflheim = (GameObject*)mon2;

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
		
		GetHero()->SetState(GameObject::eState::Wait);
		mon2->SetState(GameObject::eState::Wait);
		Camera::SetBVelocity(true);
		Camera::SetTarget(mon2);
		mTime = 0.0f;
	}
	void DungeonNiflheim::OnExit()
	{
		//Camera::SetLookRange(Vector2{ 0.0f, 0.0f });
		//PlayScene::OnExit();
		Camera::SetTarget(nullptr);

		TilePalatte::clear();
		//TilePalatte::clear(1);

		//CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, false);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Tile, false);
	}
}