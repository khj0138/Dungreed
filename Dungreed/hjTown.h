#pragma once
#include "hjScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{
	class Town : public Scene
	{
	public:
		Town();
		~Town();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

		void SetHero(Hero* hero) { mHero = hero; }
		/*virtual void AddGameObject(GameObject* obj, eLayerType layer) override
		{
			Scene::GetLayer(layer).AddGameObject(obj);
		}*/
	private:
		Hero* mHero;
		BackGround* bg;
	};

}
