//origin
#include "hjApplication.h"
#include "hjSceneManager.h"
#include "hjTime.h"
#include "hjInput.h"

namespace hj
{
	hj::Application::Application()
		: mHwnd(NULL)
		, mHdc(NULL)
		, mWidth(1600)
		, mHeight(900)
	{
	}

	hj::Application::~Application()
	{
		SceneManager::Release();
	}

	void hj::Application::Initialize(HWND hWnd)
	{
		mHwnd = hWnd;
		mHdc = GetDC(hWnd);

		// ��Ʈ�� �ػ� ������ ���� ���� ������ ũ�� ���
		RECT rect = { 0, 0, mWidth, mHeight };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		// ������ ũ�� ���� �� ��� ����
		SetWindowPos(mHwnd
			, nullptr, 100, 50
			, rect.right - rect.left
			, rect.bottom - rect.top
			, 0);
		ShowWindow(hWnd, true);

		mBackBuffer = CreateCompatibleBitmap(mHdc, mWidth, mHeight);
		mBackHDC = CreateCompatibleDC(mHdc);

		HBITMAP defaultBitmap
			= (HBITMAP)SelectObject(mBackHDC, mBackBuffer);
		DeleteObject(defaultBitmap);

		Time::Initialize();
		Input::Initialize();
		SceneManager::Initialize();
	}

	void hj::Application::Run()
	{
		Update();
		Render();
	}

	void hj::Application::Update()
	{
		Time::Update();
		Input::Update();
		SceneManager::Update();
	}

	void hj::Application::Render()
	{
		// clear
		Time::Render(mBackHDC);
		Input::Render(mBackHDC);
		SceneManager::Render(mBackHDC);

		BitBlt(mHdc, 0, 0, mWidth, mHeight, mBackHDC, 0, 0, SRCCOPY);
	}
}