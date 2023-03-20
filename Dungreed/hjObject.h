#pragma once
#include "hjGameObject.h"
#include "hjScene.h"
#include "hjSceneManager.h"
#include "hjTransform.h"
#include "hjCollider.h"

namespace hj::object
{
	template <typename T>
	static inline T* Instantiate(eLayerType type)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(gameObj, type);
		gameObj->Initialize();
		return gameObj;
	}

	template <typename T>
	static inline T* Instantiate(Vector2 pos, eLayerType type)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(gameObj, type);
		gameObj->Initialize();
		gameObj->GameObject::GetComponent<Transform>()->SetPos(pos);
		return gameObj;
	}

	template <typename T>
	static inline T* Instantiate(Vector2 pos, eLayerType type, eSceneType sType)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::FindScene(sType);
		scene->AddGameObject(gameObj, type);
		gameObj->Initialize();
		gameObj->GameObject::GetComponent<Transform>()->SetPos(pos);
		return gameObj;
	}

	template <typename T>
	static inline T* Instantiate(Vector2 pos, eLayerType type, Vector2 col)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(gameObj, type);
		gameObj->Initialize();
		gameObj->GameObject::GetComponent<Transform>()->SetPos(pos);

		Collider* collider = new Collider();
		collider->SetSize(col);
		collider->SetCenter();
		collider->SetPos(pos);
		gameObj->GameObject::AddComponent<Collider>();
		return gameObj;
	}
	template <typename T>
	static inline T* Instantiate(eLayerType type, const std::wstring name, const std::wstring path, float moveRate = 0.0f)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(gameObj, type);
		//gameObj->setAnimation(name, path, moveRate);
		gameObj->Initialize();
		return gameObj;
	}

	static void Destroy(GameObject* obj)
	{
		obj->SetState(GameObject::eState::Death);
	}
}