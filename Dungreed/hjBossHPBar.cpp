#include "hjBossHPBar.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjCamera.h"
#include "hjTilePalatte.h"

#include "hjMonster.h"

extern hj::Application application;

namespace hj
{
	BossHPBar::BossHPBar()
		: mTime(0.0f)
		, mPos(Vector2::Zero)
	{
	}

	BossHPBar::BossHPBar(const std::wstring name, const std::wstring path, Vector2 asRatio, Vector2 offset)
		: mTime(0.0f)
		, mPos(offset)
	{
		std::wstring full = L"_Full.bmp";
		std::wstring fullPath = path;
		fullPath.append(full);
		std::wstring fullName = name;
		fullName.append(full);

		std::wstring empty = L"_Empty.bmp";
		std::wstring emptyPath = path;
		emptyPath.append(empty);
		std::wstring emptyName = name;
		emptyName.append(empty);

		image_full = RscManager::Load<Img>(fullName, fullPath);
		image_empty = RscManager::Load<Img>(emptyName, emptyPath);

		image_full->SetMoveRate(Vector2::Zero);
		image_empty->SetMoveRate(Vector2::Zero);
		image_full->SetRepeat(false);
		image_empty->SetRepeat(false);
		image_full->MatchRatio(asRatio);
		image_empty->MatchRatio(asRatio);
	}
	BossHPBar::~BossHPBar()
	{
		//delete image_full;
		//delete image_empty;
	}
	void BossHPBar::Initialize()
	{
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();
		Vector2 ImgSize = Vector2{ (float)image_empty->GetWidth(), (float)image_empty->GetHeight() };
		tr->SetPos(
			mPos
		);
	}
	void BossHPBar::Update()
	{
		//GameObject::Update();
	}
	void BossHPBar::Render(HDC hdc)
	{

		Transform* tr = GetComponent<Transform>();
		Vector2 scale = tr->GetScale();
		Vector2 pos = tr->GetPos();
		pos.x -= (float)image_empty->GetWidth() / 2.0f;
		pos.y -= image_empty->GetHeight();

		TransparentBlt(hdc, pos.x, pos.y
			, image_empty->GetWidth() * scale.x
			, image_empty->GetHeight() * scale.y
			, image_empty->GetHdc()
			, 0, 0
			, image_empty->GetWidth(), image_empty->GetHeight(),
			RGB(255, 0, 255));

		UINT monsterHP = GetMonster()->GetStat().HP;
		UINT monsterMaxHP = GetMonster()->GetStat().maxHP;
		float hpRate = (float)monsterHP / (float)monsterMaxHP;
		UINT outputHP = 0;
		if ((hpRate * 100.f) >= 0.0f)
			outputHP = (UINT)(hpRate * 100.f);
		if (outputHP > 0)
			TransparentBlt(hdc, pos.x + 106, pos.y
				, outputHP * 5, image_full->GetHeight()
				, image_full->GetHdc()
				, 106, 0
				, outputHP * 5, image_full->GetHeight(),
				RGB(255, 0, 255));
	}

	void BossHPBar::Release()
	{
		GameObject::Release();
	}
}