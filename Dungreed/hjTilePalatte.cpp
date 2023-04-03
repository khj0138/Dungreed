#include "hjTilePalatte.h"
#include "hjRscManager.h"
#include "hjInput.h"
#include <commdlg.h>
#include "hjMouse.h"

namespace hj
{
	Img* TilePalatte::mImage = nullptr;
	std::unordered_map<UINT64, Tile*> TilePalatte::mTiles = {};
	UINT TilePalatte::mIndex = -1;

	void TilePalatte::Initiailize()
	{
		mImage = RscManager::Find<Img>(L"TileAtlas");
	}

	void TilePalatte::Update()
	{
	}

	void TilePalatte::Render(HDC hdc)
	{
	}

	void TilePalatte::CreateTile(int index, Vector2 pos)
	{
		Vector2 mousePos = Mouse::GetPos();
		if (mousePos.x >= 1600.0f || mousePos.x <= 0.0f)
			return;
		if (mousePos.y >= 900.0f || mousePos.y <= 0.0f)
			return;


		Tile* tile = object::Instantiate<Tile>(eLayerType::Tile);
		tile->InitializeTile(mImage, index);


		Vector2 tilePos(pos.x * TILE_SIZE_X, pos.y * TILE_SIZE_Y);
		tile->GetComponent<Transform>()->SetPos(tilePos);
		if (index == 0 || index == 1 || index == 3)
		{
			tile->AddComponent<Collider>();
			tile->GetComponent<Collider>()->SetPos(tilePos);
			tile->GetComponent<Collider>()->SetSize(Vector2{ TILE_SIZE_X, TILE_SIZE_Y });
			
			//tile->GetComponent<Collider>()->SetCenter(tile->GetComponent<Collider>()->GetSize() / 2.f);
		}

		TileID id;
		id.x = (UINT32)pos.x;
		id.y = (UINT32)pos.y;

		std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.find(id.id);
		if (iter != mTiles.end())
		{
			//Tile* temp = iter->second;
			iter->second = tile;
			//delete temp;
		}
		else
		{
			mTiles.insert(std::make_pair(id.id, tile));
		}
	}

	void TilePalatte::CreateTiles(int index, UINT width, UINT height)
	{

	}

	void TilePalatte::Save(wchar_t path[256])
	{
		// open a file name
		OPENFILENAME ofn = {};

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


		FILE* file = nullptr;
		if (path == nullptr)
		{
			ofn.lpstrFile = szFilePath;
			ofn.lpstrFile[0] = '\0';
			if (false == GetOpenFileName(&ofn))
				return;

			_wfopen_s(&file, szFilePath, L"wb");
		}
		else
		{
			ofn.lpstrFile = path;

			_wfopen_s(&file, path, L"wb");
		}

		if (file == nullptr)
			return;
		

		

		std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.begin();
		for (; iter != mTiles.end(); iter++)
		{
			int index = iter->second->Index();
			fwrite(&index, sizeof(int), 1, file);

			TileID id;
			id.id = iter->first;
			fwrite(&id.id, sizeof(TileID), 1, file);
		}

		fclose(file);
	}

	void TilePalatte::Load(wchar_t path[256])
	{

		SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Tile).clear();
		TilePalatte::clear();
		OPENFILENAME ofn = {};

		wchar_t szFilePath[256];
		//wchar_t szFilePath[256] = L"C:\\Users\\kang\\Desktop\\assortRock\\khj\\46th_winAPI\\Dungreed\\Resource\\Tile2.Tile\0";

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


		FILE* file = nullptr;
		if (path == nullptr)
		{
			ofn.lpstrFile = szFilePath;
			ofn.lpstrFile[0] = '\0';
			if (false == GetOpenFileName(&ofn))
				return;

			_wfopen_s(&file, szFilePath, L"rb");
		}
		else
		{
			ofn.lpstrFile = path;

			_wfopen_s(&file, path, L"rb");
		}

		if (file == nullptr)
			return;

		while (true)
		{
			int index = -1;
			TileID id;

			if (fread(&index, sizeof(int), 1, file) == NULL)
				break;

			if (fread(&id.id, sizeof(TileID), 1, file) == NULL)
				break;

			CreateTile(index, Vector2(id.x, id.y));
		}

		fclose(file);
	}

	Vector2 TilePalatte::GetTilePos(Vector2 mousePos)
	{
		int indexY = mousePos.y / TILE_SIZE_Y;
		int indexX = mousePos.x / TILE_SIZE_X;

		return Vector2(indexX, indexY);
	}
	
	void TilePalatte::clear()
	{
		std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.begin();
		for (; iter != mTiles.end(); iter++)
		{
			delete iter->second;
			iter->second = nullptr;
		}
		mTiles.clear();
	}

}
