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
		static void LoadTile(int index, Vector2 pos);
		static void CreateTiles(int index, UINT width, UINT height);
		static void Save(wchar_t path[256] = nullptr, UINT command = 0);
		static void Load(wchar_t path[256] = nullptr, UINT command = 0, HDC hdc = NULL);

		static void RenderTile(UINT index, Vector2 pos, HDC hdc);

		static Vector2 GetTilePos(Vector2 mousePos);
		static void SetIndex(UINT index) { mIndex = index; }
		static UINT GetIndex() { return mIndex; }
		static void clear(UINT command = 0);

		static void tRenderChange(int tf = -1)
		{
			tileRender = (bool)(((int)tileRender + 1) % 2);
			/*if (tf == -1)
			else
				tileRender = (bool)tf;*/
		}
		static bool getTileRender() { return tileRender; }
		static bool HDCToFile(HBITMAP hBit)
		{
			CLSID clsid; //clsid
			Gdiplus::Bitmap* bit = new Gdiplus::Bitmap(5400, 2700); //내용이없는 빈 500x500의 비트맵객체생성
		
		
			UINT num, size;
			Gdiplus::GetImageEncodersSize(&num, &size); //인코더크기구하기
			Gdiplus::ImageCodecInfo* codec = new Gdiplus::ImageCodecInfo[size]; //특정크기만큼 코덱객체 동적할당
			Gdiplus::GetImageEncoders(num, size, codec); //이미지인코딩정보 구하기

			for (int i = 0; i < num; i++) { //for문으로 순회하면서

			

				if (!wcscmp(L"image/bmp", codec[i].MimeType)) { 
					clsid = codec[i].Clsid; 
					break; 
				}
			}

			delete[] codec; //메모리누수방지
		
			bit = bit->FromHBITMAP(hBit, 0); //hbitmap내용을 통하여 bit라는 비트맵객체에 값대입
			bit->Save(L"../Resource/test.bmp\0", &clsid); //파일로 저장
			return true;
		}
	private:
		static std::unordered_map<UINT64, Tile*> mTiles;
		static std::unordered_map<UINT64, Tile*> mTiles2;
		static Img* mImage;
		static UINT mIndex;
		static bool tileRender;

	};

	
}
