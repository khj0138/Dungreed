#include "hjBackGround.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"


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
		GameObject::Initialize();
	}
	void BackGround::Update()
	{
		GameObject::Update();
	}
	void BackGround::Render(HDC hdc)
	{
		Rectangle(hdc, -1, -1, 1602, 902);
		GameObject::Render(hdc);
	}
	void BackGround::Release()
	{
		GameObject::Release();
	}
}