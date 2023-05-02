#include "hjPlaneObject.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjInput.h"
#include "hjRscManager.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjCamera.h"
#include "hjTilePalatte.h"

extern hj::Application application;

namespace hj
{
	PlaneObject::PlaneObject()
		: mTime(0.0f)
		, mPos(Vector2::Zero)
	{
	}

	PlaneObject::PlaneObject(const std::wstring name, const std::wstring path, Vector2 asRatio, Vector2 offset, Vector2 moveRate)
		: mTime(0.0f)
		, mPos(offset)
	{
		mImage = RscManager::Load<Img>(name, path);
		mImage->SetMoveRate(moveRate);
		mImage->SetRepeat(false);
		mImage->MatchRatio(asRatio);
	}
	PlaneObject::~PlaneObject()
	{
	}
	void PlaneObject::Initialize()
	{
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();
		Vector2 ImgSize = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };
		/*if (mPos.x != 0.0f)
			ImgSize.x = mPos.x;
		if (mPos.y != 0.0f)
			ImgSize.y = mPos.y;*/
		tr->SetPos(
			mPos
			+ Vector2{ ImgSize.x / 2.0f, ImgSize.y }
			+ Vector2{ 0.0f, 0.0f }
		);

	}
	void PlaneObject::Update()
	{
		GameObject::Update();
	}
	void PlaneObject::Render(HDC hdc)
	{

		Transform* tr = GetComponent<Transform>();
		Vector2 scale = tr->GetScale();
		Vector2 pos = tr->GetPos();
		pos = Camera::CaluatePos(pos, mImage->GetMoveRate());
		pos.x -= (float)mImage->GetWidth() / 2.0f;
		pos.y -= mImage->GetHeight();
		TransparentBlt(hdc, pos.x, pos.y
			, mImage->GetWidth() * scale.x
			, mImage->GetHeight() * scale.y
			, mImage->GetHdc()
			, 0, 0
			, mImage->GetWidth(), mImage->GetHeight(),
			RGB(255, 0, 255));
	}

	void PlaneObject::Release()
	{
		GameObject::Release();
	}
}