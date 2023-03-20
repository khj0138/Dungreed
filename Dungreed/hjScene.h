#pragma once
#include "hjEntity.h"
#include "hjLayer.h"

namespace hj
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Destroy();
		virtual void Release();

		virtual void OnEnter();
		virtual void OnExit();

		void AddGameObject(GameObject* obj, eLayerType layer);
		std::vector<GameObject*>& GetGameObjects(eLayerType layer);
		void SetAsRatio(Vector2 ratio) { asRatio = ratio; }
		Vector2 GetAsRatio() { return asRatio; }
	private:
		std::vector<Layer> mLayers;
		Vector2 asRatio;
	};
}