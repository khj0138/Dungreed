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
		Transform* tr = GetComponent<Transform>();
		Vector2 pos = tr->GetPos();
		setPos(Vector2{ (float)mV * 1.0f, 0.0f });
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
		//BitBlt(hdc, 0, 0, 1600, 900, iHdc, 0, 0, SRCCOPY);

		float scale = (float)width / 1600.f;

		float moveX = (float)pos.x / (float)width;
		float outputX = (float)height / 900.f * 1600.f;
		float moveRate = (1 - moveX) * (float)width / outputX;
		if (moveRate > 1)
		{
			GdiTransparentBlt(hdc, 0, 0, (UINT)(1600), (UINT)(900), iHdc, moveX * width, 0, (UINT)(outputX ), height, RGB(255, 0, 255));
		}
		else
		{
			GdiTransparentBlt(hdc, 0, 0, (UINT)(1600.f * moveRate), (UINT)(900), iHdc, moveX* width, 0, moveRate * outputX, height, RGB(255, 0, 255));
			GdiTransparentBlt(hdc, (UINT)(1600.f * moveRate), 0, (UINT)(1600.f * (1-moveRate)), (UINT)(900.f), iHdc, 0, 0, (UINT)(outputX * (1- moveRate)), height, RGB(255, 0, 255));
		}

	}
	void BackGround::setBG(const std::wstring& key, const std::wstring& path)
	{
		spr = GetComponent<SpriteRenderer>();
		spr->SetSprite(key, path);
		spr->SetState((UINT)eStateType::Idle);
		spr->SetIndex(0);
		int a = 0;
	}
	void BackGround::setPos(Vector2 pos)
	{
		tr = GetComponent<Transform>();
		Vector2 iPos = tr->GetPos();

		spr = GetComponent < SpriteRenderer>();
		UINT width = spr->GetSprite()->GetWidth();
		tr->SetPos(Vector2{ (float)((UINT)(iPos.x + pos.x) % width), iPos.y });
	}

	void BackGround::Release()
	{
		GameObject::Release();
	}
}