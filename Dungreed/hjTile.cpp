#include "hjTile.h"
#include "hjTransform.h"
#include "hjCamera.h"
#include "hjCollider.h"
#include "hjHero.h"
#include "hjRigidBody.h"
#include "hjSceneManager.h"
#include "hjTilePalatte.h"
#include "hjImage.h"
#include "hjMonster.h"
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
		//Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		GameObject* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero == nullptr)
		{
			hero = dynamic_cast<Monster*>(other->GetOwner());
			if(hero == nullptr)
				return;
		}

		Rigidbody* rb = hero->GetComponent<Rigidbody>();

		Collider* heroCol = hero->GetComponent<Collider>();
		Vector2 heroPos = heroCol->GetPos();

		Collider* groundCol = this->GetComponent<Collider>();
		Vector2 groundPos = groundCol->GetPos();
		switch (mIndex)
		{
		case 0:
		{
			float fLenY = (heroPos.y + heroCol->GetSize().y / 2.0f - groundPos.y - groundCol->GetSize().y / 2.0f);
			float fSizeY = (heroCol->GetSize().y / 2.0f) + (groundCol->GetSize().y / 2.0f);

			if (fabs(fLenY) <= fSizeY)
			{
				Transform* heroTr = hero->GetComponent<Transform>();
				Vector2 heroPos = heroTr->GetPos();

				if (fLenY <= 0.0f && rb->GetVelocity().y >= 0.0f)
				{
					rb->SetVelocity(Vector2{ rb->GetVelocity().x, 0.0f });
					rb->SetGround(true);
					heroPos = Vector2{ heroPos.x, groundPos.y };
				}
				else if (fLenY > 0.0f && rb->GetVelocity().y <= 0.0f)
					heroPos = Vector2{ heroPos.x, groundPos.y + groundCol->GetSize().y + heroCol->GetSize().y };
				heroTr->SetPos(heroPos);
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
		case 3:
		{
			if (rb->GetGravity())
			{

				Vector2 a1 = (*hero).prevPos;
				Transform* heroTr = hero->GetComponent<Transform>();
				Vector2 a2 = heroTr->GetPos();

				float a22 = GetComponent<Collider>()->GetSize().x;

				Vector2 b1 = this->GetComponent<Collider>()->GetPos() + Vector2{ 0.0f,0.0f };
				Vector2 b2 = this->GetComponent<Collider>()->GetPos() + Vector2{ GetComponent<Collider>()->GetSize().x ,0.0f };

				Vector2 a = a2 - a1;
				Vector2 b = b2 - b1;
				Vector2 target = Vector2::Zero;

					if (math::Intersection_Lines(a1 , a2, b1, b2, target))
					{
						Vector2 f = (target - a1) / (a2 - a1);
						Vector2 e = a2 - target;
						if (rb->GetGround())
						{

							float dir = (math::Dot(e, b) > 0.0f) ? 1.0f : -1.0f;
							float len = e.Length();
							Vector2 result = target + b.Normalize() * len * dir;
								

							hero->GetComponent<Transform>()->SetPos(result + Vector2{ 0.0f, 1.0f });
						}
						else if (rb->GetVelocity().y >= 0.0f)
						{

							float len = math::Dot(e, b);
							hero->GetComponent<Transform>()->SetPos(target + Vector2{ 0.0f, 1.0f });
							rb->SetGround(true);
						}
							
					}
				
			}
			break;
		}
		case 4:
		{
			if (rb->GetGravity())

			{
				Vector2 a1 = (*hero).prevPos;
				Transform* heroTr = hero->GetComponent<Transform>();
				Vector2 a2 = heroTr->GetPos();
				

				a1 -= Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };
				a2 -= Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };

				Vector2 b1 = this->GetComponent<Collider>()->GetPos() + Vector2{ 0.0f,1.0f };
				Vector2 b2 = this->GetComponent<Collider>()->GetPos() + Vector2{ GetComponent<Collider>()->GetSize().x ,GetComponent<Collider>()->GetSize().y  + 1.0f };
				
				Vector2 a = a2 - a1;
				Vector2 b = b2 - b1;

				Vector2 target = Vector2::Zero;
				for (int i = -2; i < 3; i++)
				{
					bool check = false;
					if (math::Intersection_Lines(a1 + Vector2{ (float)i,0.0f }, a2 + Vector2{ (float)i, 0.0f }, b1, b2, target))
					{

						Vector2 f = (target - a1) / (a2 - a1);
						Vector2 e = a2 - target;
						if (rb->GetGround())
						{

							float dir = (math::Dot(e, b) > 0.0f) ? 1.0f : -1.0f;
							float len = e.Length();
							Vector2 result = target + b.Normalize() * len * dir;
							if (result.x >= b1.x && result.x + a.x <= b1.x)
							{

								if (rb->GetVelocity().x < 0.0f)
									result = Vector2{ b1.x, b1.y };
							}
							else if (a1.x == b1.x || (a1.y == b1.y))
							{
								if (rb->GetVelocity().x < 0.0f)
									break;
								else if (i > 0)
									continue;
							}
							if (result.x <= b2.x && result.x + a.x >= b2.x)
							{
								if (rb->GetVelocity().x > 0.0f)
									result = Vector2{ b2.x, b2.y };
							}
							else if (a1.x == b2.x || (a1.y == b2.y))
							{
								if (rb->GetVelocity().x > 0.0f)
									break;
								else if (i < 0)
									continue;
							}

							if (rb->GetVelocity().x > 0.0f)
								hero->GetComponent<Transform>()->SetPos(result +
									Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							else if (rb->GetVelocity().x < 0.0f)
								hero->GetComponent<Transform>()->SetPos(result +
									Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							else
								hero->GetComponent<Transform>()->SetPos(result +
									Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
						}
						else if (rb->GetVelocity().y >= 0.0f)
						{

							float len = math::Dot(e, b);
							//target = target - Vector2{ (float)i, (float)j };
							hero->GetComponent<Transform>()->SetPos(target +
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							rb->SetGround(true);
						}
						break;

					}
				}

			}
			break;
		}
		case 5:
		{
			if (rb->GetGravity())

			{
				Vector2 a1 = (*hero).prevPos;
				Transform* heroTr = hero->GetComponent<Transform>();
				Vector2 a2 = heroTr->GetPos();


				a1 += Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };
				a2 += Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };

				Vector2 b1 = this->GetComponent<Collider>()->GetPos() + Vector2{ 0.0f,GetComponent<Collider>()->GetSize().y + 1.0f };
				Vector2 b2 = this->GetComponent<Collider>()->GetPos() + Vector2{ GetComponent<Collider>()->GetSize().x ,1.0f };

				Vector2 a = a2 - a1;
				Vector2 b = b2 - b1;

				Vector2 target = Vector2::Zero;
				for (int i = -2; i < 3; i++)
				{
					bool check = false;
					if (math::Intersection_Lines(a1 + Vector2{ (float)i,0.0f }, a2 + Vector2{ (float)i, 0.0f }, b1, b2, target))
					{

						Vector2 f = (target - a1) / (a2 - a1);
						Vector2 e = a2 - target;
						if (rb->GetGround())
						{

							float dir = (math::Dot(e, b) > 0.0f) ? 1.0f : -1.0f;
							float len = e.Length();
							Vector2 result = target + b.Normalize() * len * dir;
							if (result.x >= b1.x && result.x + a.x <= b1.x)
							{

								if (rb->GetVelocity().x < 0.0f)
									result = Vector2{ b1.x, b1.y };
							}
							else if ((a1.x == b1.x) || (a1.y == b1.y))
							{
								if (rb->GetVelocity().x < 0.0f)
									break;
								else if (i > 0)
									continue;
							}
							if (result.x <= b2.x && result.x + a.x >= b2.x)
							{
								if (rb->GetVelocity().x > 0.0f)
									result = Vector2{ b2.x, b2.y };
							}
							else if ((a1.x == b2.x) || (a1.y == b2.y))
							{
								if (rb->GetVelocity().x > 0.0f)
									break;
								else if (i < 0)
									continue;
									
							}

							if (rb->GetVelocity().x > 0.0f)
								hero->GetComponent<Transform>()->SetPos(result -
									Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							else if (rb->GetVelocity().x < 0.0f)
								hero->GetComponent<Transform>()->SetPos(result -
									Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							else
								hero->GetComponent<Transform>()->SetPos(result -
									Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
						}
						else if (rb->GetVelocity().y >= 0.0f)
						{

							float len = math::Dot(e, b);
							//target = target - Vector2{ (float)i, (float)j };
							hero->GetComponent<Transform>()->SetPos(target -
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							rb->SetGround(true);
						}
						break;

					}
					int a = 0;
				}

			}
			break;
		}

		case 6:
		{
			if (true)

			{

				Vector2 a1 = (*hero).prevPos;
				Transform* heroTr = hero->GetComponent<Transform>();
				Vector2 a2 = heroTr->GetPos();


				a1 -= Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };
				a2 -= Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };

				Vector2 b1 = this->GetComponent<Collider>()->GetPos() + Vector2{ 0.0f,0.0f };
				Vector2 b2 = this->GetComponent<Collider>()->GetPos() + Vector2{ GetComponent<Collider>()->GetSize().x ,GetComponent<Collider>()->GetSize().y };

				Vector2 a = a2 - a1;
				Vector2 b = b2 - b1;
				double det = math::Cross(a, b);
				if (fabs(det) < 0.01f) return;
				else
				{
					Vector2 c = b1 - a1;
					Vector2 d = b2 - b1;
					Vector2 p = a1 + (a2 - a1) * (math::Cross(c, d) / det);
					Vector2 f = (p - a1) / (a2 - a1);

					if ((f.x >= -0.5f || f.y >= -0.5f) && f.Length() <= 1.0f)
					{

						Vector2 e = a2 - p;

						if (rb->GetGround() || !(rb->GetGravity()))
						{

							float dir = (math::Dot(e, b) > 0.0f) ? 1.0f : -1.0f;
							float len = e.Length();
							Vector2 result = p + b.Normalize() * len * dir;

							if ((result.x + a.x >= b2.x))
							{
								if (rb->GetVelocity().x > 0.0f)
									result = b2 + Vector2{ 0.0f,1.0f };
							}

							hero->GetComponent<Transform>()->SetPos(result +
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f, 0.0f });
						}
						else if (rb->GetVelocity().y >= 0.0f)
						{

							float len = math::Dot(e, b);
							hero->GetComponent<Transform>()->SetPos(p +
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							rb->SetGround(true);
						}
					}
				}
			}
			break;
		}
		case 7:
		{
			if (true)

			{




				Vector2 a1 = (*hero).prevPos;
				Transform* heroTr = hero->GetComponent<Transform>();
				Vector2 a2 = heroTr->GetPos();


				a1 += Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };
				a2 += Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f,0.0f };

				Vector2 b1 = this->GetComponent<Collider>()->GetPos() + Vector2{ 0.0f,GetComponent<Collider>()->GetSize().y };
				Vector2 b2 = this->GetComponent<Collider>()->GetPos() + Vector2{ GetComponent<Collider>()->GetSize().x ,0.0f };

				Vector2 a = a2 - a1;
				Vector2 b = b2 - b1;
				double det = math::Cross(a, b);
				if (fabs(det) < 0.01f) return;
				else
				{
					Vector2 c = b1 - a1;
					Vector2 d = b2 - b1;
					Vector2 p = a1 + (a2 - a1) * (math::Cross(c, d) / det);
					Vector2 f = (p - a1) / (a2 - a1);

					if ((f.x >= -0.5f || f.y >= -0.5f) && f.Length() <= 1.0f)
					{

						Vector2 e = a2 - p;

						if (rb->GetGround() || !(rb->GetGravity()))
						{

							float dir = (math::Dot(e, b) > 0.0f) ? 1.0f : -1.0f;
							float len = e.Length();
							Vector2 result = p + b.Normalize() * len * dir;

							if (result.x + a.x <= b1.x)
							{
								if (rb->GetVelocity().x < 0.0f)
									result = b1 + Vector2{ 0.0f,1.0f };
							}

							hero->GetComponent<Transform>()->SetPos(result -
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f, 0.0f });
						}
						else if (rb->GetVelocity().y >= 0.0f)
						{

							float len = math::Dot(e, b);
							hero->GetComponent<Transform>()->SetPos(p -
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							rb->SetGround(true);
						}
					}
				}

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

		//if (mIndex == 3/* || mIndex == 4 || mIndex == 5*/)
		//{
		//	if (!(other->CollisionCheck()))
		//	{

		//		Hero* hero = dynamic_cast<Hero*>(other->GetOwner());
		//		if (hero == nullptr)
		//			return;
		//		Rigidbody* rb = hero->GetComponent<Rigidbody>();

		//		rb->SetGround(false);
		//	}

		//}
	}
}