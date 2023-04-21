#pragma once
#include "hjGameObject.h"


namespace hj
{
	class Img;
	class Tile : public GameObject
	{
	public:
		Tile();
		Tile(Vector2 pos);
		Tile(Vector2 pos, bool bCol);
		~Tile();

		void InitializeTile(Img* atlas, int index);
		void SetIndex(int index);

		virtual void Update() override;
		virtual void Render(HDC hdc) override;

		UINT Y() { return mY; }
		UINT X() { return mX; }
		UINT Index() { return mIndex; }

		void OnCollisionEnter(Collider* other);
		void OnCollisionStay(Collider* other);
		void OnCollisionExit(Collider* other);
		void SetBCollision(bool col) { bCollision = col; }
		bool CollisionCheck(Vector2& target, GameObject* hero);
		
	private:
		Img* mAtlas;
		UINT mIndex;
		UINT mY;
		UINT mX;
		bool bCollider;
		bool bRender;
		bool bCollision;
	};
}

