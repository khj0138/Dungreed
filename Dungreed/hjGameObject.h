#pragma once
#include "hjEntity.h"
#include "hjComponent.h"

namespace hj
{

	class GameObject : public Entity
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();

	protected:
		Vector2 mPos;

	private:
		std::vector<Component*> mComponents;

	};

}
