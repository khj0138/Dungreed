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
			Gdiplus::Bitmap* bit = new Gdiplus::Bitmap(5400, 2700); //�����̾��� �� 500x500�� ��Ʈ�ʰ�ü����
		
		
			UINT num, size;
			Gdiplus::GetImageEncodersSize(&num, &size); //���ڴ�ũ�ⱸ�ϱ�
			Gdiplus::ImageCodecInfo* codec = new Gdiplus::ImageCodecInfo[size]; //Ư��ũ�⸸ŭ �ڵ���ü �����Ҵ�
			Gdiplus::GetImageEncoders(num, size, codec); //�̹������ڵ����� ���ϱ�

			for (int i = 0; i < num; i++) { //for������ ��ȸ�ϸ鼭

			

				if (!wcscmp(L"image/bmp", codec[i].MimeType)) { 
					clsid = codec[i].Clsid; 
					break; 
				}
			}

			delete[] codec; //�޸𸮴�������
		
			bit = bit->FromHBITMAP(hBit, 0); //hbitmap������ ���Ͽ� bit��� ��Ʈ�ʰ�ü�� ������
			bit->Save(L"../Resource/test.bmp\0", &clsid); //���Ϸ� ����
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
