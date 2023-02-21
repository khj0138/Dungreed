#pragma once
#include "hjEntity.h"

namespace hj
{
	class Component : public Entity
	{
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();

	private:

	};

}
