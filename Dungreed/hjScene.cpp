#include "hjScene.h"

namespace hj
{

	hj::Scene::Scene()
	{
		mLayers.reserve(5);
		mLayers.resize((UINT)eLayerType::End);
	}

	hj::Scene::~Scene()
	{
	}

	void hj::Scene::Initialize()
	{
		for (Layer& layer : mLayers)
		{
			layer.Initialize();
		}
	}

	void hj::Scene::Update()
	{
		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}

	void hj::Scene::Render(HDC hdc)
	{
		for (Layer& layer : mLayers)
		{
			layer.Render(hdc);
		}
	}

	void hj::Scene::Release()
	{
		for (Layer& layer : mLayers)
		{
			layer.Release();
		}
	}

	void hj::Scene::AddGameObject(GameObject* obj, eLayerType layer)
	{
		mLayers[(UINT)layer].AddGameObject(obj);
	}

}