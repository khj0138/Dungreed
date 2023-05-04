#include "hjToolScene.h"
#include "hjApplication.h"
#include "hjImage.h"
#include "hjRscManager.h"
#include "hjTile.h"
#include "hjObject.h"
#include "hjCamera.h"
#include "hjMouse.h"
#include "hjTilePalatte.h"

extern hj::Application application;

namespace hj
{

	ToolScene::ToolScene()
	{
	}

	ToolScene::~ToolScene()
	{
	}

	void ToolScene::Initialize()
	{
		Scene::Initialize();
	}

	void ToolScene::Update()
	{
		/*if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
			return;
		}*/
		Scene::Update();
		//Vector2 temp = Mouse::GetPos();
		if (Mouse::GetLstate() == (eKeyState::Down))
		{
			Vector2 pos = Mouse::GetPos();
			pos -= Camera::CaluatePos(Vector2::Zero);

			pos = TilePalatte::GetTilePos(pos);

			UINT tileIndex = TilePalatte::GetIndex();
			TilePalatte::CreateTile(tileIndex, pos);
		}
		if (Input::GetKeyDown(eKeyCode::S))
		{
			TilePalatte::Save();
		}
		if (Input::GetKeyDown(eKeyCode::L))
		{
			//TilePalatte::Load();
			//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";
			TilePalatte::Load();
		}
		
	}

	void ToolScene::Render(HDC hdc)
	{
		
		HPEN colorPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		HPEN oldPen = (HPEN)SelectObject(hdc, colorPen);


		Vector2 startPos(0, 0);
		startPos = Camera::CaluatePos(startPos);

		int maxRow = application.GetHeight() / TILE_SIZE_Y + 1;
		for (size_t y = 0; y < maxRow * 3; y++)
		{
			MoveToEx(hdc, startPos.x, TILE_SIZE_Y * y + startPos.y, NULL);
			LineTo(hdc, application.GetWidth(), TILE_SIZE_Y * y + startPos.y);
		}
		int maxColumn = application.GetWidth() / TILE_SIZE_X + 1;

		for (size_t x = 0; x < maxColumn * 3; x++)
		{
			MoveToEx(hdc, TILE_SIZE_X * x + startPos.x, startPos.y, NULL);
			LineTo(hdc, TILE_SIZE_X * x + startPos.x, application.GetHeight());
		}
		(HPEN)SelectObject(hdc, oldPen);
		DeleteObject(colorPen);
		Scene::Render(hdc);

		if (Input::GetKeyDown(eKeyCode::R))
		{
			Img* a = Img::Create(L"test", 8000, 1800);
			wchar_t p[256] = L"../Resource/Ice/Dungeon1_3_foothold.Tile\0";
			//wchar_t p[256] = L"../Resource/Ice/Dungeon1_2_Tile.Tile\0";
			TilePalatte::Load(p, 1, a->GetHdc());
			//TilePalatte::Load(p);
			HBITMAP b = (HBITMAP)GetCurrentObject(a->GetHdc(), OBJ_BITMAP);
			TilePalatte::HDCToFile(b);
			delete a;
			wchar_t c[256] = L"../Resource/Ice/Dungeon1_3_foothold.Tile\0";
			//wchar_t c[256] = L"../Resource/Ice/Dungeon1_2_Tile.Tile\0";
			TilePalatte::Load(c);
		}
		/*if (Input::GetKeyState(eKeyCode::N) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Play);
			return;
		}*/
	}

	void ToolScene::Release()
	{
		Scene::Release();
	}

	void ToolScene::OnEnter()
	{

		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile.Tile\0";
		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";
		wchar_t a[256] = L"../Resource/Tile2.Tile\0";
		wchar_t b[256] = L"../Resource/TownTile.Tile\0";
		wchar_t c[256] = L"../Resource/TownCollider.Tile\0";
		//TilePalatte::Load(a);
		//TilePalatte::Load(b);
		TilePalatte::Load(c);
		Camera::SetLookRange(Vector2{ (float)application.GetWidth() * 8, (float)application.GetHeight() * 8 });
	}

	void ToolScene::OnExit()
	{

		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile.Tile\0";
		wchar_t a[256] = L"../Resource/Tile2.Tile\0";
		wchar_t b[256] = L"../Resource/TownTile.Tile\0";
		wchar_t c[256] = L"../Resource/TownCollider.Tile\0";

		//wchar_t a[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";
		//TilePalatte::Save(a);
		//TilePalatte::Save(b);
		TilePalatte::Save(c);
		TilePalatte::clear();
		//Camera::SetLookRange(Vector2{ 0.f, 0.f });
	}

}

#include "Resource.h"
LRESULT CALLBACK AtlasWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		//512 384
		//HMENU mMenubar = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_CLIENT));
		//SetMenu(hWnd, mMenubar);
		//hj::Img* tile = hj::RscManager::Load<hj::Img>(L"TileAtlas", L"..\\Resource\\Tile.bmp");
		//hj::Img* tile = hj::RscManager::Load<hj::Img>(L"TileAtlas", L"..\\Resource\\TileTown.bmp");
		hj::Img* tile = hj::RscManager::Load<hj::Img>(L"TileAtlas", L"..\\Resource\\TileIce.bmp");
		RECT rect = { 0, 0, tile->GetWidth(), tile->GetHeight() };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		
		// 윈도우 크기 변경및 출력 설정
		SetWindowPos(hWnd
			, nullptr, 1600, 0
			, rect.right - rect.left
			, rect.bottom - rect.top
			, 0);
		ShowWindow(hWnd, true);
	}

	case WM_LBUTTONDOWN:
	{

		if (GetFocus())
		{
			::POINT mousePos = {};
			::GetCursorPos(&mousePos);
			::ScreenToClient(application.GetToolHwnd(), &mousePos);

			int x = mousePos.x / TILE_SIZE_X;
			int y = mousePos.y / TILE_SIZE_Y;

			int index = (y * 9) + (x % 9);

			hj::TilePalatte::SetIndex(index);
		}
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
			//case IDM_ABOUT:
			//    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			//    break;
			//case IDM_EXIT:
			//    DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		hj::Img* tile = hj::RscManager::Find<hj::Img>(L"TileAtlas");
		BitBlt(hdc, 0, 0, tile->GetWidth(), tile->GetHeight(), tile->GetHdc(), 0, 0, SRCCOPY);

		//HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		//HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

		//Ellipse(hdc, 0, 0, 100, 100);
		////RoundRect(hdc, 200, 200, 300, 300, 500, 500);
		//(HPEN)SelectObject(hdc, oldPen);
		//DeleteObject(redPen);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}