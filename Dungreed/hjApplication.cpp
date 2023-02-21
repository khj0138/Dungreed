#include "hjApplication.h"
#include "hjSceneManager.h"
#include "hjTime.h"
#include "hjInput.h"

namespace hj
{
	hj::Application::Application()
		: mHwnd(NULL)
		, mHdc(NULL)
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
		Time::Render(mHdc);
		Input::Render(mHdc);
		SceneManager::Render(mHdc);
	}
}