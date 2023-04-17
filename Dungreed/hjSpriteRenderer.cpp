#include "hjSpriteRenderer.h"
#include "hjCamera.h"


namespace hj
{
	SpriteRenderer::SpriteRenderer()
		:Component(eComponentType::SpriteRenderer)
		, mFlip(false)
		, bRotate(false)
		, mDir(Vector2{ 0.0f,0.0f })
	{
	}
	SpriteRenderer::~SpriteRenderer()
	{
	}
	void SpriteRenderer::Initialize()
	{
	}

	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::Render(HDC hdc)
	{
		/*if (bRotate)
			rotateRender(hdc, image)*/
	}

	void SpriteRenderer::Release()
	{
	}

	void SpriteRenderer::rotateRender(HDC hdc, HDC ImgHdc, Vector2 size, float degree, Vector2 pos)
	{
		HBITMAP mem_bitmap = CreateCompatibleBitmap(ImgHdc, size.x, size.y);
		HDC mem_dc = (HDC)CreateCompatibleDC(ImgHdc);

		
		DeleteObject(SelectObject(mem_dc, mem_bitmap));


		TransparentBlt(mem_dc, 0, 0
			, size.x, size.y
			, ImgHdc, 0, 0
			, size.x, size.y
			, RGB(255, 0, 255));


		HBITMAP h_bitmap = (HBITMAP)GetCurrentObject(mem_dc, OBJ_BITMAP);

		BITMAP bmp_info;
		GetObject(h_bitmap, sizeof(BITMAP), &bmp_info);

		int h_bmp_size = (bmp_info.bmWidth) * (bmp_info.bmHeight) * (bmp_info.bmBitsPixel) / 8;
		BYTE* h_bmp_data = new BYTE[h_bmp_size];
		GetBitmapBits(h_bitmap, h_bmp_size, h_bmp_data);

		HBITMAP copied_hBmp = NULL;
		BITMAPINFO bInfo = { 0 };
		bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bInfo.bmiHeader.biWidth = bmp_info.bmWidth;
		bInfo.bmiHeader.biHeight = (-1) * bmp_info.bmHeight;
		bInfo.bmiHeader.biPlanes = 1;
		bInfo.bmiHeader.biBitCount = 24;
		bInfo.bmiHeader.biCompression = BI_RGB;

		void* copied_hBits = NULL;
		copied_hBmp = CreateDIBSection(NULL, &bInfo, DIB_RGB_COLORS, &copied_hBits, NULL, 0);

		memcpy(copied_hBits, h_bmp_data, h_bmp_size);

 		Gdiplus::Bitmap* GdiP_bmp = Gdiplus::Bitmap::FromHBITMAP(copied_hBmp, NULL);


		DeleteDC(mem_dc);
		DeleteObject(mem_bitmap);

		if (GdiP_bmp != NULL)
		{
			/////////////////////////////////////////////////////////////////////////////
			/*
			Gdiplus::Graphics G2(hdc);
			
			ImageAttributes att;

			att.SetColorKey(Color(255, 0, 255), ColorAdjustTypeDefault);
			G2.DrawImage(GdiP_bmp, RectF(0, 0, (REAL)(*GdiP_bmp).GetWidth(), (REAL)(*GdiP_bmp).GetHeight()),
				0, 0, (REAL)(*GdiP_bmp).GetWidth(), (REAL)(*GdiP_bmp).GetHeight(), UnitPixel, &att);*/
			//G.ResetTransform();
			//s = G.SetSmoothingMode(SmoothingModeNone);

			Gdiplus::Graphics G(GdiP_bmp);

			G.RotateTransform((REAL)degree);
			G.TranslateTransform(size.x / 2.f, size.y / 2.f, Gdiplus::MatrixOrderAppend);
			G.DrawImage(GdiP_bmp, -(INT)size.x / 2, -(INT)size.y / 2);
			Gdiplus::Bitmap* pBmp = new Gdiplus::Bitmap(size.x, size.y, PixelFormat24bppRGB);
			
			//G.DrawImage(pBmp, size.x, size.y);

			HBITMAP hBitmap = NULL;
			GdiP_bmp->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);
			//pBmp->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);
			HDC memDC = G.GetHDC();
			HBITMAP oldMap = (HBITMAP)SelectObject(memDC, hBitmap);
			Vector2 realSpawn = Camera::CaluatePos(pos, Vector2::One);
			TransparentBlt(hdc, (int)realSpawn.x, (int)realSpawn.y
				, size.x, size.y
				, memDC, 0, 0
				, size.x, size.y
				, RGB(0, 0, 0));
			DeleteObject((HBITMAP)SelectObject(memDC, oldMap));
			DeleteDC(memDC);

			G.ReleaseHDC(memDC);
			G.~Graphics();

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		delete GdiP_bmp;
		DeleteObject(copied_hBmp);
		DeleteObject(h_bitmap);
		delete[] h_bmp_data;
		h_bmp_data = nullptr;
	}

}