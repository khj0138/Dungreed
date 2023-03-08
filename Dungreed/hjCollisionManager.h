#pragma once
#include "CommonInclude.h"
#include "hjEnums.h"
#include "hjCollider.h"
namespace hj
{
	class Scene;

	class CollisionManager
	{
	public:
		static void Update();
		static void LayerCollision(Scene* scene, eLayerType left, eLayerType right);
		static bool Intersect(Collider* left, Collider* right);

		static void SetLayer(eLayerType left, eLayerType right, bool value);
		static void Clear();

	private:
		static WORD mMatrix[(UINT)eLayerType::End];
	};

}
