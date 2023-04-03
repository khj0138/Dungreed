#pragma once
#include "hjGameObject.h"
#include "hjImage.h"

namespace hj
{
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
	private:
		Img* mAtlas;
		UINT mIndex;
		UINT mY;
		UINT mX;
		bool bCollider;
	};
}

