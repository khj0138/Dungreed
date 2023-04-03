#pragma once
#include "HjHjEngine.h"

namespace hj
{

	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hWnd);
		void Run();
		void Update();
		void Render();

		void SetMenuBar(bool power);

		HWND GetHwnd() { return mHwnd; }
		HWND GetToolHwnd() { return mToolHwnd; }
		HDC GetHdc() { return mHdc; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

		void SetToolHwnd(HWND hwnd) { mToolHwnd = hwnd; }

	private:
		void clear();

	private:
		HWND mHwnd;
		HDC mHdc;
		HMENU mMenubar;

		// tool
		HWND mToolHwnd;

		// back buffer
		HBITMAP mBackBuffer;
		HDC mBackHDC;

		UINT mWidth;
		UINT mHeight;

		Vector2 mPos;
	};
}