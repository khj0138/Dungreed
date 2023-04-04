#pragma once
#include "hjTile.h"
#include "hjImage.h"
#include "hjObject.h"


namespace hj
{
	union TileID
	{
		struct
		{
			UINT32 x;
			UINT32 y;
		};
		UINT64 id;
	};

	class TilePalatte
	{
	public:
		static void Initiailize();
		static void Update();
		static void Render(HDC hdc);

		static void CreateTile(int index, Vector2 pos);
		static void CreateTiles(int index, UINT width, UINT height);
		static void Save(wchar_t path[256] = nullptr);
		static void Load(wchar_t path[256] = nullptr);

		static Vector2 GetTilePos(Vector2 mousePos);
		static void SetIndex(UINT index) { mIndex = index; }
		static UINT GetIndex() { return mIndex; }
		static void clear();

		static void tRenderChange(int tf = -1)
		{
			tileRender = (bool)(((int)tileRender + 1) % 2);
			/*if (tf == -1)
			else
				tileRender = (bool)tf;*/
		}
		static bool getTileRender() { return tileRender; }
	private:
		static std::unordered_map<UINT64, Tile*> mTiles;
		static std::unordered_map<UINT64, Tile*> mTiles2;
		static Img* mImage;
		static UINT mIndex;
		static bool tileRender;

	};
}
