#include "hjBackGround.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"


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
		GameObject::Render(hdc);
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		Vector2 size = tr->GetSize();

		SpriteRenderer* spr = GetComponent<SpriteRenderer>();
		Image* img = spr->GetSprite();
		bool flip = spr->checkFlip();

		UINT width = img->GetWidth();
		UINT height = img->GetHeight();
		HDC iHdc = img->GetHdc();
		BitBlt(hdc, pos.x, pos.y, width,height, iHdc, 0, 0, SRCCOPY);
		//GdiTransparentBlt(hdc, mPos.x, mPos.y, width, height, mImage->GetHdc(), 0, 0, width, height, RGB(255, 0, 255));
	}
	void BackGround::setBG(const std::wstring& key, const std::wstring& path)
	{
		spr = GetComponent<SpriteRenderer>();
		spr->SetSprite(key, path);
		spr->SetState((UINT)eStateType::Idle);
		spr->SetIndex(0);
		int a = 0;
	}
	void BackGround::Release()
	{
		GameObject::Release();
	}
}