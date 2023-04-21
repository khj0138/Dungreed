#pragma once
#include "hjScene.h"

namespace hj
{
	class ToolScene : public Scene
	{
	public:
		ToolScene();
		~ToolScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

		/*virtual void AddGameObject(GameObject* obj, eLayerType layer) override
		{
			Scene::GetLayer(layer).AddGameObject(obj);
		}*/
	private:
		wchar_t path[256];
	};
}

