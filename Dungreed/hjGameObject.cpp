#include "hjGameObject.h"


namespace hj
{

	hj::GameObject::GameObject()
	{
	}

	hj::GameObject::~GameObject()
	{
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
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Release();
		}
	}

}