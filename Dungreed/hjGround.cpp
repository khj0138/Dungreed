#include "hjGround.h"
#include "hjCollider.h"
#include "hjHero.h"
#include "hjRigidbody.h"
#include "hjTransform.h"


namespace hj
{
	Ground::Ground()
	{
	}
	Ground::~Ground()
	{
	}
	void Ground::Initialize()
	{
		mCollider = AddComponent<Collider>();
		mCollider->SetSize(Vector2(9280.0f, 50.0f));
		GameObject::Initialize();
	}
	void Ground::Update()
	{
		GameObject::Update();
	}
	void Ground::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
	void Ground::Release()
	{
	}
	void Ground::OnCollisionEnter(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero == nullptr)
			return;

		Rigidbody* rb = hero->GetComponent<Rigidbody>();
		rb->SetGround(true);

		Collider* heroCol = hero->GetComponent<Collider>();
		Vector2 heroPos = heroCol->GetPos();

		Collider* groundCol = this->GetComponent<Collider>();
		Vector2 groundPos = groundCol->GetPos();

		float fLen = fabs(heroPos.y - groundPos.y);
		float fSize = (heroCol->GetSize().y / 2.0f) + (groundCol->GetSize().y / 2.0f);

		if (fLen < fSize)
		{
			Transform* heroTr = hero->GetComponent<Transform>();
			Transform* grTr = this->GetComponent<Transform>();

			Vector2 heroPos = heroTr->GetPos();
			Vector2 grPos = grTr->GetPos();

			heroPos = Vector2{ heroPos.x, grPos.y -1.0f};
			heroTr->SetPos(heroPos);
		}
	}
	void Ground::OnCollisionStay(Collider* other)
	{
	}
	void Ground::OnCollisionExit(Collider* other)
	{
	}
}