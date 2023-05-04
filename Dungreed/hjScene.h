#pragma once
#include "hjEntity.h"
#include "hjLayer.h"
#include "hjEnums.h"
#define NAME(n) L#n
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
		bool LayerEmpty(eLayerType layer) { return mLayers.empty(); }
		void SetAsRatio(Vector2 ratio) { asRatio = ratio; }
		Vector2 GetAsRatio() { return asRatio; }

		void ClearLayer(eLayerType layer) 
		{
			mLayers[(int)layer].ClearAll();
		}
	private:
		std::vector<Layer> mLayers;
		Vector2 asRatio;
	};
}