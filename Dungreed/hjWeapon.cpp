#include "hjWeapon.h"
#include "hjCamera.h"
#include "hjWmanager.h"
#include "hjEmanager.h"
#include "hjMouse.h"
#include "hjMath.h"

namespace hj
{
	void Weapon::Initialize()
	{
	}
	void Weapon::Update()
	{
		mPos = GetOwner()->GetComponent<Transform>()->GetPos()- Vector2{ 0.0f,GetOwner()->GetComponent<Collider>()->GetSize().y / 2.f };
		mDir = (Mouse::GetPos() - Camera::CaluatePos(mPos /*- Vector2{ 0.0f,GetOwner()->GetComponent<Collider>()->GetSize().y / 2.f }*/
			, Vector2::One)).Normalize();
		isFlip = GetOwner()->GetFlip();
	}
	void Weapon::Render(HDC hdc)
	{
	}
	void Weapon::Create()
	{
	}
	void Weapon::OnCollisionEnter(Collider* other)
	{
	}
	void Weapon::OnCollisionStay(Collider* other)
	{
	}
	void Weapon::OnCollisionExit(Collider* other)
	{
	}
	void Weapon::wCheckCol(Collider* target, Collider* other)
	{
	}
	void Weapon::colRender(HDC hdc, std::vector<Vector2> posCol, bool bCollision)
	{

		Collider* collider = GetComponent<Collider>();
		//collider->SetPos(collider->GetPos() - collider->GetCenter());
		collider->Render(hdc);
		//collider->SetPos(collider->GetPos() + collider->GetCenter());

		Vector2 pos = collider->GetPos();
		//float xtemp = 0.2f;
		//float ytemp = -0.3f;
		//float hands = 16.f / 4.f;
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		// collider render 부분
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{
			rect[i] = math::Rotate(posCol[i], atan2(GetDir().y, GetDir().x) / PI * 180);
			rect[i] = Camera::CaluatePos(collider->GetPos() + collider->GetCenter() + rect[i], Vector2::One);
		}
		HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		if (bCollision)
		{
			HPEN redpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			DeleteObject(SelectObject(hdc, redpen));
		}
		for (int i = 0; i < 4; i++)
		{
			MoveToEx(hdc, (int)(rect[i].x), (int)(rect[i].y), nullptr);
			LineTo(hdc, (int)(rect[(i + 1) % 4].x), (int)(rect[(i + 1) % 4].y));
		}
		DeleteObject((HPEN)SelectObject(hdc, oldPen));
	}
	void Weapon::Idle()
	{
		/*if (Mouse::GetLstate() == eKeyState::Down)
			SetState((UINT)eWeaponState::ATTACK);*/
	}

	void Weapon::Attack()
	{
		// 이펙트 생성 코드 필요
		//bAttack = true;
		//Weapon::SetState((UINT)eWeaponState::RELOAD);
	}
	void Weapon::Reload()
	{
		
	}
}