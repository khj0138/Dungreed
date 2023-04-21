#pragma once
#include "hjScene.h"

namespace hj
{
	class PSceneManager;
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void Render(HDC hdc);
		static void Release();
		static void Destroy();

		static void LoadScene(eSceneType type);
		static Scene* GetActiveScene() { return mActiveScene; }
		static Scene* FindScene(eSceneType type) { return mScenes[(UINT)type]; }
		static void ChangePlayScene(Scene* scene) {
			mScenes[(UINT)eSceneType::Play] = scene;
			//LoadScene(eSceneType::Play);
		}
		static PSceneManager* GetPManager() { return PManager; }

	private:
		static std::vector<Scene*> mScenes;
		static Scene* mActiveScene;
		static PSceneManager* PManager;
	};
}
