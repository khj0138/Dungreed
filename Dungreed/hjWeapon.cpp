#include "hjWeapon.h"
#include "hjCamera.h"
#include "hjWmanager.h"
#include "hjEmanager.h"

namespace hj
{
	void Weapon::Initialize()
	{
	}
	void Weapon::Update()
	{
	}
	void Weapon::Render(HDC hdc)
	{
	}
	void Weapon::Create()
	{
	}
	void Weapon::Idle()
	{
	}
	void Weapon::Attack()
	{
	}
	void Weapon::Reload()
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
	void Weapon::colRender(HDC hdc, Wmanager* mng, std::vector<Vector2> posCol, bool bCollision)
	{
		
		Collider* collider = mng->GetComponent<Collider>();
		collider->SetPos(collider->GetPos() - collider->GetCenter());
		collider->Render(hdc);
		collider->SetPos(collider->GetPos() + collider->GetCenter());

		Vector2 pos = collider->GetPos();
		float xtemp = 0.2f;
		float ytemp = -0.3f;
		float hands = 16.f / 4.f;
		Transform* tr = mng->GetOwner()->GetComponent<Transform>();
		Vector2 heroSize = tr->GetSize();

		// collider render ºÎºÐ
		Vector2 rect[4] = {};
		for (int i = 0; i < 4; i++)
		{
			rect[i] = math::Rotate(posCol[i], atan2(mng->GetDir().y, mng->GetDir().x) / PI * 180);
			rect[i] = Camera::CaluatePos(collider->GetPos() + rect[i], 1.f);
		}
		HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		if (bCollision)
		{
			HPEN redpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			DeleteObject(SelectObject(hdc, redpen));
		}
		for (int i = 0; i < 4; i++)
		{
			MoveToEx(hdc, (int)(rect[i].x), (int)(rect[i].y), nullptr);
			LineTo(hdc, (int)(rect[(i + 1) % 4].x), (int)(rect[(i + 1) % 4].y));
		}
		DeleteObject((HPEN)SelectObject(hdc, oldPen));
	}
}