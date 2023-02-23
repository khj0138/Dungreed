#include "hjGameObject.h"
#include "hjTransform.h"
#include "hjSpriteRenderer.h"


namespace hj
{

	hj::GameObject::GameObject()
		: index(0)
	{
		mComponents.resize((UINT)eComponentType::End);
		AddComponent<Transform>();
		AddComponent<SpriteRenderer>();
	}

	hj::GameObject::~GameObject()
	{
		for (Component* comp : mComponents)
		{
			delete comp;
			comp = nullptr;
		}
	}

	void hj::GameObject::Initialize()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Initialize();
		}
	}

	void hj::GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Update();
		}
	}

	void hj::GameObject::Render(HDC hdc)
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Render(hdc);
		}
	}

	void hj::GameObject::Release()
	{

	}

}