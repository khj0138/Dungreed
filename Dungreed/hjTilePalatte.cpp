#include "hjTilePalatte.h"
#include "hjRscManager.h"
#include "hjInput.h"
#include <commdlg.h>
#include "hjMouse.h"
#include "hjMath.h"

namespace hj
{
	Img* TilePalatte::mImage = nullptr;
	std::unordered_map<UINT64, Tile*> TilePalatte::mTiles = {};
	std::unordered_map<UINT64, Tile*> TilePalatte::mTiles2 = {};
	UINT TilePalatte::mIndex = -1;
	bool TilePalatte::tileRender = true;

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

	void TilePalatte::LoadTile(int index, Vector2 pos)
	{
		Vector2 tilePos(pos.x * TILE_SIZE_X, pos.y * TILE_SIZE_Y);

		TileID id;
		id.x = (UINT32)pos.x;
		id.y = (UINT32)pos.y;

		if (index == 2)
		{
			std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.find(id.id);
			if (iter != mTiles.end())
			{
				mTiles.erase(iter);
				return;
			}
			else
				return;
		}

		Tile* tile = object::Instantiate<Tile>(eLayerType::Tile);
		tile->InitializeTile(mImage, index);
		
		int m = 0;
		int n = 0;
		tilePos.x = tilePos.x +  m *  TILE_SIZE_X;
		tilePos.y = tilePos.y +  n *  TILE_SIZE_Y;
		tile->GetComponent<Transform>()->SetPos(tilePos);
		if (index == 0 || index == 1 || index == 3 || index == 4 || index == 5)
		{
			tile->AddComponent<Collider>();
			tile->GetComponent<Collider>()->SetPos(tilePos);
			tile->GetComponent<Collider>()->SetSize(Vector2{ TILE_SIZE_X, TILE_SIZE_Y });

			//tile->GetComponent<Collider>()->SetCenter(tile->GetComponent<Collider>()->GetSize() / 2.f);
		}

		std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.find(id.id);
		if (iter != mTiles.end())
		{
			//Tile* temp = iter->second;
			iter->second = tile;
			//delete temp;
		}
		else
		{

			id.x = (UINT32)pos.x + (UINT32)(m);
			id.y = (UINT32)pos.y + (UINT32)(n);
			mTiles.insert(std::make_pair(id.id, tile));
		}
	}
	
	void TilePalatte::CreateTile(int index, Vector2 pos)
	{
		Vector2 mousePos = Mouse::GetPos();
		if (mousePos.x >= 1600.0f || mousePos.x <= 0.0f)
			return;
		if (mousePos.y >= 900.0f || mousePos.y <= 0.0f)
			return;



		Vector2 tilePos(pos.x * TILE_SIZE_X, pos.y * TILE_SIZE_Y);

		TileID id;
		id.x = (UINT32)pos.x;
		id.y = (UINT32)pos.y;

		if (index == 2)
		{
			std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.find(id.id);
			if (iter != mTiles.end())
			{
				mTiles.erase(iter);
				return;
			}
			else
				return;
		}

		Tile* tile = object::Instantiate<Tile>(eLayerType::Tile);
		tile->InitializeTile(mImage, index);

		int m = 0;
		int n = 0;
		tilePos.x = tilePos.x + m * TILE_SIZE_X;
		tilePos.y = tilePos.y + n * TILE_SIZE_Y;
		tile->GetComponent<Transform>()->SetPos(tilePos);
		if (index == 0 || index == 1 || index == 3 || index == 4 || index == 5)
		{
			tile->AddComponent<Collider>();
			tile->GetComponent<Collider>()->SetPos(tilePos);
			tile->GetComponent<Collider>()->SetSize(Vector2{ TILE_SIZE_X, TILE_SIZE_Y });

			//tile->GetComponent<Collider>()->SetCenter(tile->GetComponent<Collider>()->GetSize() / 2.f);
		}

		std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.find(id.id);
		if (iter != mTiles.end())
		{
			//Tile* temp = iter->second;
			iter->second = tile;
			//delete temp;
		}
		else
		{

			id.x = (UINT32)pos.x + (UINT32)(m);
			id.y = (UINT32)pos.y + (UINT32)(n);
			mTiles.insert(std::make_pair(id.id, tile));
		}
	}

	void TilePalatte::CreateTiles(int index, UINT width, UINT height)
	{

	}

	void TilePalatte::Save(wchar_t path[256], UINT command)
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

		if (command == 0)
		{
			std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.begin();
			for (; iter != mTiles.end(); iter++)
			{
				int index = iter->second->Index();
				fwrite(&index, sizeof(int), 1, file);

				TileID id;
				id.id = iter->first;
				fwrite(&id.id, sizeof(TileID), 1, file);
			}
		}
		else if (command == 1)
		{
			std::unordered_map<UINT64, Tile*>::iterator iter = mTiles2.begin();
			for (; iter != mTiles2.end(); iter++)
			{
				int index = iter->second->Index();
				fwrite(&index, sizeof(int), 1, file);

				TileID id;
				id.id = iter->first;
				fwrite(&id.id, sizeof(TileID), 1, file);
			}
		}
		fclose(file);
	}

	void TilePalatte::Load(wchar_t path[256], UINT command, HDC hdc)
	{
		if (command == 0)
		{
			for (GameObject* obj : SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Tile))
			{
				obj = nullptr;
			}
			SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Tile).resize(0);
			SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Tile).shrink_to_fit();

		}
		TilePalatte::clear(command);
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
			//npath = ofn.lpstrFile;
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
			if (command == 0)
				LoadTile(index, Vector2{ id.x, id.y });
			/*else if (command == 1)
				RenderTile(index, Vector2{ id.x, id.y }, hdc);*/
			else if ((command == 1) && (hdc != NULL))
			{
				RenderTile(index, Vector2{ id.x, id.y }, hdc);
			}
		}

		fclose(file);
	}

	void TilePalatte::RenderTile(UINT index, Vector2 pos,  HDC hdc)
	{
		Vector2 tilePos(pos.x * TILE_SIZE_X, pos.y * TILE_SIZE_Y);

		int maxCol = mImage->GetWidth() / TILE_SIZE_X;
		
		int mY = index / maxCol;
		int mX = index % maxCol;

		if (mImage == nullptr)
			return;

		
		//Vector2 renderPos = Camera::CaluatePos(tilePos);
		Vector2 renderPos = tilePos;

		TransparentBlt(hdc
			, renderPos.x, renderPos.y
			, TILE_SIZE_X, TILE_SIZE_Y
			, mImage->GetHdc()
			, mX * TILE_SIZE_X, mY * TILE_SIZE_Y
			, TILE_SIZE_X, TILE_SIZE_Y
			, RGB(255, 0, 255)
		);
	}

	Vector2 TilePalatte::GetTilePos(Vector2 mousePos)
	{
		int indexY = mousePos.y / TILE_SIZE_Y;
		int indexX = mousePos.x / TILE_SIZE_X;

		return Vector2(indexX, indexY);
	}

	void TilePalatte::clear(UINT command)
	{
		if (command == 0)
		{
			std::unordered_map<UINT64, Tile*>::iterator iter = mTiles.begin();
			for (; iter != mTiles.end(); iter++)
			{
				if (iter->second == nullptr)
					continue;
				delete iter->second;
				iter->second = nullptr;
			}
			mTiles.clear();
		}
		else if (command == 1)
		{
			std::unordered_map<UINT64, Tile*>::iterator iter = mTiles2.begin();
			for (; iter != mTiles2.end(); iter++)
			{
				if (iter->second == nullptr)
					continue;
				delete iter->second;
				iter->second = nullptr;
			}
			mTiles2.clear();
		}
	}

}
