#include "hjSword.h"
extern GraphicsPath Path;
extern const GUID ImageFormatBMP;
namespace hj
{

	Sword::Sword()
		: mRadius(50.0f)
		, mWstate(eWeaponState::IDLE)
		, sState(SwordState::up)
		, isFlip(false)
	{
	}
	Sword::~Sword()
	{
	}
	void Sword::Initialize()
	{
	}
	void Sword::Update()
	{
		//Transform* tr = this->GetManager()->GetOwner()->GetComponent<Transform>();
		mWstate = Weapon::GetState();
		float length = mImage->GetHeight();
		Vector2 pos = this->GetManager()->GetPos();
		Vector2 dir = this->GetManager()->GetDir();
		if (sState == SwordState::up)
		{
			mSpawn = pos + math::Rotate((dir * mRadius), -90.0f);
			mSpawnDir = math::Rotate(dir, -10.0f);
		}
		else
		{
			mSpawn = pos + math::Rotate((dir * mRadius), -180.0f);
			mSpawnDir = math::Rotate(dir, -100.0f);
		}
		switch (mWstate)
		{
		case eWeaponState::IDLE:
		{
			Idle();
			break;
		}
		case eWeaponState::ATTACK:
		{
			Attack();
			break;
		}
		case eWeaponState::RELOAD:
		{
			Reload();
			break;
		}
		}
	}
	void Sword::Render(HDC hdc)
	{
		//make a graphics object from the empty bitmap
		
		//move rotation point to center of image
		//g.RotateTransform();
		////rotate
		//g.RotateTransform(angle);
		////move image back
		//g.TranslateTransform(-(float)b.Width / 2, -(float)b.Height / 2);
		////draw passed in image onto graphics object
		//g.DrawImage(b, new Point(0, 0));
		//

	/*	BitBlt(hdc, 0, 0
			, mImage->GetWidth(), mImage->GetHeight()
			, mImage->GetHdc()
			, 0, 0
			, SRCCOPY);*/
		

		//Gdiplus::Bitmap* p = Gdiplus::Bitmap::FromHBITMAP(mImage->GetBitmap(), NULL);

		/*HDC temp = CreateCompatibleDC(mImage->GetHdc());
		HBITMAP tempB = CreateCompatibleBitmap(mImage->GetHdc(), mImage->GetWidth(), mImage->GetHeight());

		HBITMAP oldBitmap = (HBITMAP)SelectObject(temp, tempB);
		DeleteObject(oldBitmap);



		BitBlt(temp, 0, 0
			, mImage->GetWidth(), mImage->GetHeight()
			, mImage->GetHdc()
			, 0, 0
			, SRCCOPY);*/
		//HBITMAP tempC = CreateCompatibleBitmap(mImage->GetHdc(), mImage->GetWidth(), mImage->GetHeight());
		//HBITMAP oldBitmap2 = (HBITMAP)SelectObject(temp, tempC);
		//HBITMAP oldBitmap3 = (HBITMAP)SelectObject(temp, tempB);
		//HBITMAP a = (HBITMAP)GetCurrentObject(temp, OBJ_BITMAP);
		


		//BITMAPINFOHEADER bih = { 0 };
		//bih.biSize = sizeof(BITMAPINFOHEADER);
		//bih.biWidth = mImage->GetWidth();
		//bih.biHeight = mImage->GetHeight();
		//bih.biPlanes = 1;
		//bih.biBitCount = 32; // set to 32 bits per pixel
		//bih.biCompression = BI_RGB;

		//// Allocate memory for the pixel data.
		//DWORD* pBits = new DWORD[mImage->GetWidth() * mImage->GetHeight()];
		//// Use GetDIBits to copy the pixel data from the original bitmap to the new pixel data buffer.
		//HDC hdc2 = mImage->GetHdc();
		//int nRet = GetDIBits(hdc2, (HBITMAP)mImage->GetBitmap() , 0, mImage->GetHeight(), pBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
		//

		//// Create a new bitmap using the BITMAPINFOHEADER structure and the new pixel data.
		//HBITMAP hBitmap = CreateDIBitmap(hdc2, (BITMAPINFOHEADER*)&bih, CBM_INIT, pBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

		//// Delete the pixel data buffer.
		//delete[] pBits;

		/*HBITMAP hBitmap = CreateCompatibleBitmap(mImage->GetHdc(), mImage->GetWidth(), mImage->GetHeight());
		HDC memDC = CreateCompatibleDC(mImage->GetHdc());
		HBITMAP oldmap = (HBITMAP)SelectObject(memDC, hBitmap);
		*/
		//BitBlt(hdc, mSpawn.x, mSpawn.y, mImage->GetWidth(), mImage->GetHeight(), mImage->GetHdc(), mImage->GetWidth(), mImage->GetHeight(), SRCCOPY);
		/*(HBITMAP)SelectObject(memDC, oldmap);
		
		DeleteDC(memDC);
		DeleteObject(oldmap);*/

		HBITMAP h_bitmap = (HBITMAP)GetCurrentObject(mImage->GetHdc(), OBJ_BITMAP);

		BITMAP bmp_info; 
		GetObject(h_bitmap, sizeof(BITMAP), &bmp_info);
		
		int pattern_size = bmp_info.bmWidth * bmp_info.bmHeight * bmp_info.bmBitsPixel / 8;
		
		BYTE* p_data = new BYTE[pattern_size];
		
		int d = GetBitmapBits(h_bitmap, pattern_size, p_data);

		//DWORD* pPixels = new DWORD[pattern_size / 3];
		//for (int i = 0; i < pattern_size / 3; i++) {
		//	// Extract the red, green, and blue color channels for the current pixel.
		//	BYTE r = p_data[i * 3];
		//	BYTE g = p_data[i * 3 + 1];
		//	BYTE b = p_data[i * 3 + 2];

		//	// Combine the color channels into a single pixel value.
		//	pPixels[i] = RGB(r, g, b);
		//}

		BITMAPINFO bInfo = { 0 };
		bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bInfo.bmiHeader.biWidth = bmp_info.bmWidth;
		bInfo.bmiHeader.biHeight = bmp_info.bmHeight;
		bInfo.bmiHeader.biPlanes = 1;
		bInfo.bmiHeader.biBitCount = 24;
		bInfo.bmiHeader.biCompression = BI_RGB;

		void* pBits = NULL;
		

		//HBITMAP a = CreateDIBSection(NULL, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
		HBITMAP temp = CreateDIBSection(NULL, &bInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

		memcpy(pBits, p_data, pattern_size);
		//if (temp) {
		//	// Successfully created DIB section
		//	BITMAPINFOHEADER bih;
		//	memset(&bih, 0, sizeof(bih));
		//	bih.biSize = sizeof(bih);
		//	if (GetDIBits(hdc, temp, 0, bmp_info.bmHeight, NULL, &bInfo, DIB_RGB_COLORS) != 0) {
		//		// GetDIBits succeeded, retrieve BITMAPINFOHEADER from BITMAPINFO
		//		bih = bInfo.bmiHeader;
		//		// Check pixel format and handle errors accordingly
		//		if (bih.biBitCount == 24 && bih.biCompression == BI_RGB) {
		//			// Pixel format is supported by GDI+
		//			int er = 0;
		//		}
		//		else {
		//			// Pixel format is not supported by GDI+
		//		}
		//	}
		//	else {
		//		// GetDIBits failed, handle error
		//	}
		//	// Cleanup
		//	DeleteObject(temp);
		//}
		//else {
		//	// Failed to create DIB section, handle error
		//}
		//if(temp!= NULL)
		//BYTE* ba = (BYTE*)(pBits);

		Gdiplus::Bitmap* b = Gdiplus::Bitmap::FromHBITMAP(temp, NULL);
		/*if (b == NULL) {
			DWORD error = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, NULL);
			MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION);
			LocalFree(lpMsgBuf);
		}*/
		
		int fd = 0;
		delete b;
		//b = NULL;
		//DeleteObject(a);
		//b->GetHBITMAP(Gdiplus::Color(0, 0, 0), &a);
		
		//HBITMAP hBitmap = temp;
		//BITMAP bitmap;
		//int result = GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		//MEMORYSTATUSEX memStatus;
		//memStatus.dwLength = sizeof(MEMORYSTATUSEX);
		//int check = 0;
		//if (GlobalMemoryStatusEx(&memStatus)) {
		//	DWORDLONG physMemFree = memStatus.ullAvailPhys;
		//	// Check if there is enough free physical memory for the bitmap
		//	if (physMemFree) {
		//		check = 50;
		//		// Create the Bitmap object here
		//	}
		//	else {
		//		check = 40;
		//		// Not enough free physical memory for the bitmap
		//	}
		//}
		//else {
		//	// Failed to retrieve memory status information
		//}
		//Gdiplus::Image::GetPixelFormat(temp);
		
		DeleteObject((mImage->GetHdc(), temp));
		//HPALETTE a = HPALETTE 
		BitBlt(hdc, mSpawn.x, mSpawn.y
			, mImage->GetWidth(), mImage->GetHeight()
			, mImage->GetHdc(), 0, 0
			, SRCCOPY);
		
		delete[] p_data;


		//DeleteObject(h_bitmap);
		//Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(bmp_info.bmWidth, bmp_info.bmHeight, bmp_info.bmWidthBytes, ImageFormatBMP, (BYTE)p_data);

		//HBITMAP b = (HBITMAP)CopyImage(mImage->GetBitmap(), IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
	
		//int a = 0;
		//Gdiplus::Status b = a->GetHBITMAP(Color(0, 0, 0), &hBitmap);
		//Graphics::GetHDC(mImage->GetHdc())
		//Graphics::GetHDC(mImage->GetHdc());
		//release(g);
		////Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromHBITMAP(tempB, NULL);
		//
		///*BitBlt(hdc, mSpawn.x, mSpawn.y
		//	, mImage->GetWidth(), mImage->GetHeight()
		//	, temp
		//	, 0, 0
		//	, SRCCOPY);*/
		//DeleteDC(temp);
		//DeleteObject(tempB);
		/*TransparentBlt(hdc, mSpawn.x - mImage->GetWidth() / 2.0f, mSpawn.y - mImage->GetHeight() / 2.0f
			, mImage->GetWidth(), mImage->GetHeight()
			, temp
			, 0.0f, 0.0f
			, mImage->GetWidth(), mImage->GetHeight(),
			RGB(255, 0, 255));*/
//		Gdiplus::Graphics g(temp);
		//g.RotateTransform(atan2((int)mSpawnDir.x, (int)mSpawnDir.y));
		//g.DrawCachedBitmap
		//g.DrawImage(mImage->GetBitmap(), Gdiplus::Rect{0, 0, (int)(mImage->GetWidth()), (int)(mImage->GetHeight())});


		
		/*BITMAP bmpInfo;
		::GetObject(mImage->GetBitmap(), sizeof(BITMAP), &bmpInfo);
		int cxBitmap = bmpInfo.bmWidth;
		int cyBitmap = bmpInfo.bmHeight;
		void* bits = bmpInfo.bmBits;
		HDC grapHdc = CreateCompatibleDC(mImage->GetHdc());
		HBITMAP bit = CreateCompatibleBitmap(mImage->GetHdc(), mImage->GetWidth(), mImage->GetHeight());
		DeleteObject(SelectObject(grapHdc, bit));
		BitBlt(grapHdc, 0, 0, mImage->GetWidth(), mImage->GetHeight(), mImage->GetHdc(), mImage->GetWidth(), mImage->GetHeight(), SRCCOPY);
		Gdiplus::Bitmap* target = new Bitmap("..\\Resource\\Char\\BambooSword2.bmp");
		Gdiplus::Bitmap target(mImage->GetWidth(), mImage->GetHeight());
		Gdiplus::Graphics G(&target);
		
		G.FillPath(&SolidBrush(Color(0, 0, 0)), &Path);
		*/

		//Gdiplus::Graphics graphics = Gdiplus::FromHdc();
		//Gdiplus::Bitmap bitmap(cxBitmap, cyBitmap, cxBitmap * 4, PixelFormat32bppARGB, (BYTE*)bits);
		//float f = atan2((int)(mSpawnDir.x * 1000.0f), (int)(mSpawnDir.y * 1000.0f)) * 180.0f / PI;
		////graphics.RotateTransform(atan2((int)mSpawnDir.x, (int)mSpawnDir.y));
		//graphics.RotateTransform((Gdiplus::REAL)(int)f);
		//graphics.DrawImage(&bitmap, 0, 0);
		//Gdiplus::Bitmap* p = &bitmap;
		//HBITMAP hBitmap2 = NULL;
		//Gdiplus::Status status = p->GetHBITMAP(Gdiplus::Color(0, 0, 0), &hBitmap2); 
		//SelectObject(mImage->GetHdc(), hBitmap2);

		
		//HBITMAP hBitmap = NULL;
		//pBitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &hBitmap);
		////Gdiplus::Status status =
		//oldBitmap = (HBITMAP)SelectObject(temp, hBitmap);
		//DeleteObject(oldBitmap);

		
	}

	void Sword::Create()
	{
		mImage = RscManager::Load<Img>(L"Sword", L"..\\Resource\\Char\\BambooSword2.bmp");
		//mImage = RscManager::Load<Image>(L"Sword", L"..\\Resource\\Char\\.bmp");
	}

	void Sword::Idle()
	{
		if (Mouse::GetLstate() == eKeyState::Down)
			Weapon::SetState((UINT)eWeaponState::ATTACK);
	}

	void Sword::Attack()
	{
		sState = (SwordState)(((UINT)sState + 1) % (UINT)SwordState::END);
		// collider »ý¼º
		Weapon::SetState((UINT)eWeaponState::RELOAD);
	}

	void Sword::Reload()
	{
		mTime += Time::DeltaTime();
		if (mTime > 3)
		{
			mTime = 0.0f;
			Weapon::SetState((UINT)eWeaponState::IDLE);
		}
	}
	
}