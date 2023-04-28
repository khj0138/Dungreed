#pragma once
#include "hjPlayScene.h"
#include "hjHero.h"
#include "hjBackGround.h"
#include "hjGameObject.h"

namespace hj
{
	class Town : public PlayScene
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

		void DungeonEatOpenEvent();
		void DungeonEatCloseEvent();
		//void SetHero(Hero* hero) { mHero = hero; }
		/*virtual void AddGameObject(GameObject* obj, eLayerType layer) override
		{
			Scene::GetLayer(layer).AddGameObject(obj);
		}*/
	private:
		GameObject* DungeonEatOpen;
		GameObject* DungeonEatClose;


		//Hero* mHero;
		//BackGround* bg;
	};

}
