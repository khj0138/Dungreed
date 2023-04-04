#include "hjTile.h"
#include "hjTransform.h"
#include "hjCamera.h"
#include "hjCollider.h"
#include "hjHero.h"
#include "hjRigidBody.h"
#include "hjSceneManager.h"
#include "hjTilePalatte.h"
namespace hj {
	Tile::Tile()
		: mAtlas(nullptr)
		, mIndex(-1)
		, mX(-1)
		, mY(-1)
		, bRender(true)
	{
	}
	Tile::Tile(Vector2 pos)
		: mAtlas(nullptr)
		, mIndex(-1)
		, mX(-1)
		, mY(-1)
		, bCollider(false)
		, bRender(true)
	{
		GetComponent<Transform>()->SetPos(pos);
	}
	Tile::Tile(Vector2 pos, bool bCol)
		: mAtlas(nullptr)
		, mIndex(-1)
		, mX(-1)
		, mY(-1)
		, bCollider(bCol)
		, bRender(true)
	{
		GetComponent<Transform>()->SetPos(pos);
		AddComponent<Collider>();
		GetComponent<Collider>()->SetPos(pos);
	}
	Tile::~Tile()
	{
	}
	void Tile::InitializeTile(Img* atlas, int index)
	{
		mIndex = index;
		if (atlas == nullptr || index < 0)
			return;

		mAtlas = atlas;
		SetIndex(index);
	}
	void Tile::SetIndex(int index)
	{
		int maxCol = mAtlas->GetWidth() / TILE_SIZE_X;
		//int maxRow = mAtlas->GetHeight() / TILE_SIZE_Y;
		
		mY = index / maxCol;
		mX = index % maxCol;
	}
	void Tile::Update()
	{
		if (TilePalatte::getTileRender())
			bRender = true;
		else
			bRender = false;
		GameObject::Update();
	}
	void Tile::Render(HDC hdc)
	{
		if (mAtlas == nullptr || mIndex < 0)
			return;

		if (bRender)
		{

			Transform* tr = GetComponent<Transform>();
		
			Vector2 renderPos = Camera::CaluatePos(tr->GetPos());
		
			TransparentBlt(hdc
				, renderPos.x, renderPos.y
				, TILE_SIZE_X, TILE_SIZE_Y
				, mAtlas->GetHdc()
				, TILE_SIZE_X * mX, TILE_SIZE_Y * mY
				, TILE_SIZE_X, TILE_SIZE_X
				, RGB(255, 0, 255)
				);
		}
		GameObject::Render(hdc);
	}


	void Tile::OnCollisionEnter(Collider* other)
	{
		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero == nullptr)
			return;

		Rigidbody* rb = hero->GetComponent<Rigidbody>();

			Collider* heroCol = hero->GetComponent<Collider>();
			Vector2 heroPos = heroCol->GetPos();

			Collider* groundCol = this->GetComponent<Collider>();
			Vector2 groundPos = groundCol->GetPos();
			switch (mIndex)
			{
			case 0 :
			{
				float fLenY = (heroPos.y + heroCol->GetSize().y / 2.0f - groundPos.y - groundCol->GetSize().y / 2.0f);
				float fSizeY = (heroCol->GetSize().y / 2.0f) + (groundCol->GetSize().y / 2.0f);

				if (fabs(fLenY) <= fSizeY )
				{
					Transform* heroTr = hero->GetComponent<Transform>();
					Vector2 heroPos = heroTr->GetPos();

					if (fLenY <= 0.0f && rb->GetVelocity().y >= 0.0f)
					{
						rb->SetVelocity(Vector2{ rb->GetVelocity().x, 0.0f });
						rb->SetGround(true);
						heroPos = Vector2{ heroPos.x, groundPos.y + 1.f };
					}
					else if (fLenY > 0.0f && rb->GetVelocity().y <= 0.0f)
						heroPos = Vector2{ heroPos.x, groundPos.y + groundCol->GetSize().y + heroCol->GetSize().y + 1.f };
					heroTr->SetPos(heroPos);
				}
				break;
			}
			case 3:
			{
				if (rb->GetGravity())
				{

					float fLenY = (heroPos.y + heroCol->GetSize().y / 2.0f - groundPos.y - groundCol->GetSize().y / 2.0f);
					float fSizeY = (heroCol->GetSize().y / 2.0f) + (groundCol->GetSize().y / 2.0f);

					if ((fabs(fLenY)- fSizeY) <= 0.0f && (fabs(fLenY) - fSizeY) >= -20.f)
					{
						if (fLenY <= 0.0f && rb->GetVelocity().y >= 4.0f)
						{
							rb->SetVelocity(Vector2{ rb->GetVelocity().x, 0.0f });
							rb->SetGround(true);
							Transform* heroTr = hero->GetComponent<Transform>();
							Vector2 heroPos = heroTr->GetPos();
							heroPos = Vector2{ heroPos.x, groundPos.y + 1.f };
							heroTr->SetPos(heroPos);
						}
					}
				}
				break;
			}
			case 1:
			{
				float fLenX = (heroPos.x + heroCol->GetSize().x / 2.0f - groundPos.x - groundCol->GetSize().x / 2.0f);
				float fSizeX = (heroCol->GetSize().x / 2.0f) + (groundCol->GetSize().x / 2.0f);

				if (fabs(fLenX) <= fSizeX)
				{
					rb->SetVelocity(Vector2{ 0.0f, rb->GetVelocity().y });
					Transform* heroTr = hero->GetComponent<Transform>();
					Vector2 heroPos = heroTr->GetPos();

					if (fLenX <= 0.0f)
						heroPos = Vector2{ groundPos.x - heroCol->GetSize().x / 2.0f - 1.f, heroPos.y };
					else
						heroPos = Vector2{ groundPos.x + groundCol->GetSize().x + heroCol->GetSize().x / 2.0f + 1.f, heroPos.y };
					heroTr->SetPos(heroPos);
				}
				break;
			}

			}
		
	}
	void Tile::OnCollisionStay(Collider* other)
	{
		Tile::OnCollisionEnter(other);
	}
	void Tile::OnCollisionExit(Collider* other)
	{

		if (mIndex == 3)
		{

			Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
			if (hero == nullptr)
				return;
			Rigidbody* rb = hero->GetComponent<Rigidbody>();
			rb->SetGround(false);

		}
	}
}