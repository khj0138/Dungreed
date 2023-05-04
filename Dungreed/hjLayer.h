#pragma once
#include "hjEntity.h"
#include "hjGameObject.h"


namespace hj
{
	class Layer : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();

		void AddGameObject(GameObject* gameObj);
		std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }

		void ClearAll() { mGameObjects.clear(); }
	private:
		std::vector<GameObject*> mGameObjects;
	};
}

