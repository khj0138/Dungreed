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
		if (rb == nullptr)
			return;
		Collider* heroCol = hero->GetComponent<Collider>();
		Vector2 heroPos = heroCol->GetPos();

		Collider* groundCol = this->GetComponent<Collider>();
		Vector2 groundPos = groundCol->GetPos();
		switch (mIndex)
		{
		case 0:
		{
			
			Vector2 target = Vector2::Zero;
			for (int i = 0; i < 4; i++)
			{
				if (!(other->CollisionCheck()))
				{
					if(CollisionCheck(target, hero, (UINT)i))
					{
						//other->SetCollision(true);
						if(i < 2)
							rb->SetVelocity(Vector2{ rb->GetVelocity().x,0.0f });

						else
							rb->SetVelocity(Vector2{ 0.0f, rb->GetVelocity().y });
						if (i == 0)
							rb->SetGround(true);
						hero->GetComponent<Transform>()->SetPos(target);
						break;
					}
				}
			}
			break;
		}
		case 1:
		{
			/*float fLenX = (heroPos.x + heroCol->GetSize().x / 2.0f - groundPos.x - groundCol->GetSize().x / 2.0f);
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
			}*/
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
							hero->prevPos = hero->GetComponent<Transform>()->GetPos();
						}
						else if (rb->GetVelocity().y >= 0.0f)
						{

							float len = math::Dot(e, b);
							hero->GetComponent<Transform>()->SetPos(p +
								Vector2{ hero->GetComponent<Collider>()->GetSize().x / 2.f , 0.0f });
							hero->prevPos = hero->GetComponent<Transform>()->GetPos();
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
		case 3:
		{
			if (rb->GetGravity())
			{
				Vector2 target = Vector2::Zero;
				if (rb->GetVelocity().y > 0.0f)
				{
					if (CollisionCheck(target, hero))
					{
						bCollision = true;
						rb->SetVelocity(Vector2{ rb->GetVelocity().x,0.0f });
						rb->SetGround(true);
						hero->GetComponent<Transform>()->SetPos(target);
					}
				}

			}
			break;
		}

		case 4:
		{
			if (rb->GetGravity())
			{
				Vector2 target = Vector2::Zero;
				if (rb->GetVelocity().y >= 0.0f)
				{
					if (CollisionCheck(target, hero))
					{
						bCollision = true;
						rb->SetVelocity(Vector2{ rb->GetVelocity().x,0.0f });
						rb->SetGround(true);
						hero->GetComponent<Transform>()->SetPos(target);
					}
				}
			}
			break;
		}
		case 5:
		{
			if (rb->GetGravity())
			{
				Vector2 target = Vector2::Zero;
				if (CollisionCheck(target, hero))
				{
					if (rb->GetVelocity().y >= 0.0f)
					{
						bCollision = true;
						rb->SetVelocity(Vector2{ rb->GetVelocity().x,0.0f });
						rb->SetGround(true);
						//hero->GetComponent<Transform>()->SetPos(hero->prevPos);
						hero->GetComponent<Transform>()->SetPos(target);
					}
				}
			}
			break;
		}
		}

	}
	void Tile::OnCollisionStay(Collider* other)
	{
		GameObject* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero == nullptr)
		{
			hero = dynamic_cast<Monster*>(other->GetOwner());
			if (hero == nullptr)
				return;
		}
		if ((mIndex == 3 || mIndex == 4 || mIndex == 5)&&(!(other->CollisionCheck())))
		{
			Rigidbody* rb = hero->GetComponent<Rigidbody>();
			if (rb == nullptr)
				return;
			if (rb->GetGravity())
			{
				GameObject* hero = dynamic_cast<Hero*>(other->GetOwner());
				if (hero == nullptr)
				{
					hero = dynamic_cast<Monster*>(other->GetOwner());
					if (hero == nullptr)
						return;
				}


				Collider* heroCol = hero->GetComponent<Collider>();
				//Vector2 heroPos = heroCol->GetPos();

				if ((bCollision == true) && (rb->GetVelocity().y >= 0))
				{
					rb->SetVelocity(Vector2{ rb->GetVelocity().x,0.0f });
					rb->SetGround(true);
				}
				if ((bCollision == false) && (rb->GetVelocity().y >= 0))
				{
						Vector2 target = Vector2::Zero;
						if (CollisionCheck(target, hero))
						{
							bCollision = true;
							rb->SetVelocity(Vector2{ rb->GetVelocity().x,0.0f });
							rb->SetGround(true);
							//hero->GetComponent<Transform>()->SetPos(Vector2{ target });
						}
				}
				if ((bCollision == true) && (rb->GetVelocity().y == 0) && (rb->GetGround()))
				{	
					Rigidbody* rb = hero->GetComponent<Rigidbody>();

					Vector2 prevHPos = (*hero).prevPos;
					Transform* heroTr = hero->GetComponent<Transform>();
					Vector2 curHPos = heroTr->GetPos();

					Vector2 colPos = this->GetComponent<Collider>()->GetPos();
					Vector2 colSize = this->GetComponent<Collider>()->GetSize();

					Vector2 lineStart = Vector2::Zero;
					Vector2 lineEnd = Vector2::Zero;

					if (mIndex == 3)
					{
						lineStart = colPos + Vector2{ 0.0f,1.0f };
						lineEnd = colPos + Vector2{ colSize.x ,1.0f };
					}
					else if (mIndex == 4)
					{
						//other->SetCollision(true);
						lineStart = colPos + Vector2{ 0.0f,1.0f };
						lineEnd = colPos + Vector2{ colSize.x ,colSize.y + 1.0f };
					}
					else if (mIndex == 5)
					{
						//other->SetCollision(true);
						lineStart = colPos + Vector2{ 0.0f,colSize.y + 1.0f };
						lineEnd = colPos + Vector2{ colSize.x ,1.0f };
					}
					Vector2 lineVector = lineEnd - lineStart;
					if (curHPos.x <= lineEnd.x && curHPos.x >= lineStart.x)
					{

						if (((curHPos.x + (curHPos.x - prevHPos.x)) >= lineEnd.x) && rb->GetVelocity().x > 0.0f)
							curHPos = lineEnd;
						else if (((curHPos.x + (curHPos.x - prevHPos.x)) <= lineStart.x) && rb->GetVelocity().x < 0.0f)
							curHPos = lineStart;
						//float length = curHPos.x - prevHPos.x;
						Vector2 heroVector = Vector2{ curHPos.x, lineStart.y + (curHPos.x - lineStart.x) / lineVector.x * lineVector.y };
					
						hero->GetComponent<Transform>()->SetPos(Vector2{ curHPos.x, lineStart.y + (curHPos.x- lineStart.x) / lineVector.x * lineVector.y});

					}

					//if(bCollision == true)
					//	bCollision = false;
				}
			}
			else
			{
				bCollision = false;
			}
		}
		else
			Tile::OnCollisionEnter(other);
	}
	void Tile::OnCollisionExit(Collider* other)
	{
		GameObject* hero = dynamic_cast<Hero*>(other->GetOwner());
		if (hero == nullptr)
		{
			hero = dynamic_cast<Monster*>(other->GetOwner());
			if (hero == nullptr)
				return;
		}
		if (mIndex == 3 || mIndex == 4 || mIndex == 5)
		{
			bCollision = false;
		}
	}
	bool Tile::CollisionCheck(Vector2& target, GameObject* hero)
	{
		Rigidbody* rb = hero->GetComponent<Rigidbody>();

		Vector2 prevHPos = (*hero).prevPos;
		Transform* heroTr = hero->GetComponent<Transform>();
		Vector2 curHPos = heroTr->GetPos();

		Vector2 colPos = this->GetComponent<Collider>()->GetPos();
		Vector2 colSize = this->GetComponent<Collider>()->GetSize();

		Vector2 lineStart = Vector2::Zero;
		Vector2 lineEnd = Vector2::Zero;

		
		if (mIndex == 3)
		{
			lineStart = colPos + Vector2{ 0.0f,1.0f };
			lineEnd = colPos + Vector2{ colSize.x ,1.0f };
		}
		else if (mIndex == 4)
		{
			lineStart = colPos + Vector2{ 0.0f,1.0f };
			lineEnd = colPos + Vector2{ colSize.x ,colSize.y + 1.0f };
		}
		else if (mIndex == 5)
		{
			lineStart = colPos + Vector2{ 0.0f,colSize.y + 1.0f };
			lineEnd = colPos + Vector2{ colSize.x ,1.0f };
		}

		Vector2 heroVector = curHPos - prevHPos;
		Vector2 lineVector = lineEnd - lineStart;

		if (heroVector == Vector2::Zero)
			return false;
		for (int i = -4; i < 5; i++)
		{
			bool check = false;
			for (int j = -4; j < 5; j++)
			{
				if (math::Intersection_Lines(prevHPos + Vector2{ (float)i,(float)j }, curHPos + Vector2{ (float)i, (float)j }, lineStart, lineEnd, target))
				{

					Vector2 f = (target - prevHPos) / (curHPos - prevHPos);
					Vector2 e = curHPos - target;

					if (target == Vector2::Zero)
						target = curHPos;
					if (rb->GetVelocity().y >= 0.0f)
					{
						return true;
					}
				}
			}
		}
		return false;

	}
	bool Tile::CollisionCheck(Vector2& target, GameObject* hero, UINT index)
	{
		Rigidbody* rb = hero->GetComponent<Rigidbody>();

		Vector2 prevHPos = (*hero).prevPos;
		Transform* heroTr = hero->GetComponent<Transform>();
		Vector2 heroColSIze = hero->GetComponent<Collider>()->GetSize();
		Vector2 curHPos = heroTr->GetPos();
		Vector2 tempPos = curHPos;

		Vector2 colPos = this->GetComponent<Collider>()->GetPos();
		Vector2 colSize = this->GetComponent<Collider>()->GetSize();

		Vector2 lineStart = Vector2::Zero;
		Vector2 lineEnd = Vector2::Zero;


		if (index == 0)
		{
			lineStart = colPos + Vector2{ 0.0f,1.0f };
			lineEnd = colPos + Vector2{ colSize.x ,1.0f };
			
		}
		else if (index == 1)
		{
			lineStart = colPos + Vector2{ 0.0f,colSize.y - 1.0f };
			lineEnd = colPos + Vector2{ colSize.x ,colSize.y - 1.0f };
			curHPos = curHPos + Vector2{ 0.0f, heroColSIze.y * -1.0f };
			prevHPos = prevHPos + Vector2{ 0.0f, heroColSIze.y * -1.0f };
		}
		else if (index == 2)
		{
			lineStart = colPos + Vector2{ 1.0f, 0.0f  };
			lineEnd = colPos + Vector2{ 1.0f , colSize.y };
			curHPos = curHPos + Vector2{ heroColSIze.x / 2.0f, heroColSIze.y / -2.0f };
			prevHPos = prevHPos + Vector2{ heroColSIze.x / 2.0f, heroColSIze.y / -2.0f };
		}
		else if (index == 3)
		{
			lineStart = colPos + Vector2{ colSize.x - 1.0f,0.0f };
			lineEnd = colPos + Vector2{ colSize.x - 1.0f,colSize.y };
			curHPos = curHPos + Vector2{ heroColSIze.x / -2.0f, heroColSIze.y / -2.0f };
			prevHPos = prevHPos + Vector2{ heroColSIze.x / -2.0f, heroColSIze.y / -2.0f };
		}

		Vector2 heroVector = curHPos - prevHPos;
		Vector2 lineVector = lineEnd - lineStart;

		if (heroVector == Vector2::Zero)
		{
			target = curHPos;
			return true;
		}
		if (math::Intersection_Lines(prevHPos , curHPos , lineStart, lineEnd, target))
		{

			Vector2 f = (target - prevHPos) / (curHPos - prevHPos);
			Vector2 e = curHPos - target;
			if (target == Vector2::Zero)
				return false;
			if (index == 0 && rb->GetVelocity().y >= 0.0f)
			{
				if (!(rb->GetGravity()))
				{
					target = Vector2{ curHPos.x, target.y };
				}
				if ((rb->GetVelocity().y == 0.0f))
				{
					target = Vector2{ curHPos.x, target.y };
				}

				return true;
			}
			if (index == 1 && rb->GetVelocity().y <= 0.0f)
			{
				if (!(rb->GetGravity()))
				{
					target = Vector2{ curHPos.x, target.y };
				}
				target = Vector2{ target.x, target.y + 1.0f }
				- Vector2{ 0.0f, heroColSIze.y * -1.0f };
				/*if (rb->GetVelocity().y == 0.0f)
					target = Vector2{ curHPos.x, target.y };*/
				return true;
			}
			if (index == 2 && rb->GetVelocity().x >= 0.0f)
			{
				target = Vector2{ target.x - 1.0f, curHPos.y }
				-Vector2{ heroColSIze.x / 2.0f, heroColSIze.y / -2.0f };
				/*if (rb->GetVelocity().x == 0.0f)
					target = Vector2{ target.x, curHPos.y };*/
				return true;
			}
			if (index == 3 && rb->GetVelocity().x <= 0.0f)
			{
				target = Vector2{ target.x + 1.0f, curHPos.y }
				- Vector2{ heroColSIze.x / -2.0f, heroColSIze.y / -2.0f };
				/*if (rb->GetVelocity().x == 0.0f)
					target = Vector2{ target.x, curHPos.y };*/
				return true;
			}
			else
			{
				//target = tempPos;
				return false;
			}
		}
		
		return false;

	}
}