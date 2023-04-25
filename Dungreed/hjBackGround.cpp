#include "hjBackGround.h"
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
	bool BackGround::bRender = true;
	BackGround::BackGround()
		: mTime(0.0f)
		, mRepeat(false)
		, mPos(Vector2::Zero)
	{

	}
	
	BackGround::BackGround(const std::wstring name, const std::wstring path, Vector2 moveRate, Vector2 asRatio, bool repeat, UINT command, Vector2 offset)
		: mTime(0.0f)
		, mRepeat(false)
		, mPos(offset)
	{
		if(command == 0)
			mImage = RscManager::Load<Img>(name, path);
		else if (command == 1)
		{
			mImage = Img::Create(name, application.GetWidth() * 3 , application.GetHeight() * 3 );
			wchar_t p[256];
			wcscpy_s(p, sizeof(wchar_t) * path.length(), path.c_str());
			TilePalatte::Load(p,1,mImage->GetHdc());
			/*TransparentBlt(application.GetHdc()
				, 0,0
				, 4800, 2700
				, mImage->GetHdc()
				, 0,0
				, 4800,2700
				, RGB(255, 0, 255)
			);*/
			int a = 0;
		}
		mImage->SetMoveRate(moveRate);
		mImage->SetRepeat(repeat);
		if (command == 0)
			mImage->MatchRatio(asRatio);
	}
	
	BackGround::~BackGround()
	{
		//delete mImage;
		//mImage = nullptr;
	}
	void BackGround::Initialize()
	{
		mLeftTop = Vector2{ 0.0f, 0.0f };
		GameObject::Initialize();
		Transform* tr = GetComponent<Transform>();

		tr->SetPos(Vector2{ 0.0f, 0.0f });

		Vector2 windowSize = Vector2{ (float)application.GetWidth(), (float)application.GetHeight() };


		Vector2 ImgSize = Vector2{ (float)mImage->GetWidth(), (float)mImage->GetHeight() };
		if (mPos.x != 0.0f)
			ImgSize.x = mPos.x;
		if (mPos.y != 0.0f)
			ImgSize.y = mPos.y;
		tr->SetPos(
			tr->GetPos()
			+ Vector2{ ImgSize.x / 2.0f, ImgSize.y }
			+ Vector2{ 0.0f, 0.0f }
		);

	}
	void BackGround::Update()
	{
		Transform* tr = GetComponent<Transform>();
		GameObject::Update();
	}
	void BackGround::Render(HDC hdc)
	{
		if (bRender == false)
			return;
		Transform* tr = GetComponent<Transform>();
		Vector2 scale = tr->GetScale();
		Vector2 pos = tr->GetPos();
		pos = Camera::CaluatePos(pos, mImage->GetMoveRate());
		pos.x -= (float)mImage->GetWidth() / 2.0f;
		pos.y -= mImage->GetHeight() ;


		if (mImage->GetRepeat() == true)
			if ((-1.f) * pos.x >= (mImage->GetWidth() / 2))
				pos.x +=
				(UINT)(fabs(pos.x)) / (mImage->GetWidth() / 2)
				* (float)(mImage->GetWidth() / 2);
		/*TransparentBlt(application.GetHdc()
			, pos.x, pos.y
			, mImage->GetWidth() * scale.x
			, mImage->GetHeight() * scale.y
			, mImage->GetHdc()
			, 0, 0
			, mImage->GetWidth(), mImage->GetHeight(),
			RGB(255, 0, 255));*/
		
		TransparentBlt(hdc, pos.x, pos.y
			, mImage->GetWidth() * scale.x
			, mImage->GetHeight() * scale.y
			, mImage->GetHdc()
			, 0, 0
			, mImage->GetWidth(), mImage->GetHeight(),
			RGB(255, 0, 255));


		/*TransparentBlt(hdc, pos.x, pos.y
			, mImage->GetWidth() * scale.x
			, mImage->GetHeight() * scale.y
			, mImage->GetHdc()
			, 0, 0
			, 1600, 900,
			RGB(255, 0, 255));*/
	}



	void BackGround::Release()
	{
		GameObject::Release();
	}

	void BackGround::SetPos(Vector2 pos)
	{
		tr = GetComponent<Transform>();
		Vector2 iPos = tr->GetPos(); // ���� ��ġ -3300

		UINT width = mImage->GetWidth(); // 3200
		if ((iPos.x + pos.x) < width) // 3200 + -3300 < 3200
			tr->SetPos(Vector2{ (float)(iPos.x + pos.x), iPos.y });
		else
			tr->SetPos(Vector2{ (float)(iPos.x + pos.x) - width, iPos.y });
	}

	void BackGround::SetImage(const std::wstring name, const std::wstring path, Vector2 moveRate, Vector2 asRatio, bool repeat)
	{
		mImage = RscManager::Load<Img>(name, path);
		mImage->SetMoveRate(moveRate);
		mImage->SetRepeat(repeat);
		mImage->MatchRatio(asRatio);
	}


	Vector2 BackGround::GetSize()
	{
		return Vector2{ (float)mImage->GetWidth(),(float)mImage->GetHeight() };
	}



}