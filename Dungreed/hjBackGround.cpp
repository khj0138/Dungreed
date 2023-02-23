#include "hjBackGround.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"


namespace hj
{
	BackGround::BackGround()
	{
	}
	BackGround::~BackGround()
	{
	}
	void BackGround::Initialize()
	{
		mImage = RscManager::Load<Image>(L"back", L"..\\Resource\\BackCloud.bmp");
		GameObject::Initialize();
	}
	void BackGround::Update()
	{
		GameObject::Update();
	}
	void BackGround::Render(HDC hdc)
	{
		GameObject::Render(hdc);
		UINT width = mImage->GetWidth();
		UINT height = mImage->GetHeight();
		HDC Ihdc = mImage->GetHdc();
		BitBlt(hdc, mPos.x, mPos.y, width,height,Ihdc, 0, 0, SRCCOPY);
		//GdiTransparentBlt(hdc, mPos.x, mPos.y, width, height, mImage->GetHdc(), 0, 0, width, height, RGB(255, 0, 255));
	}
	void BackGround::Release()
	{
		GameObject::Release();
	}
}