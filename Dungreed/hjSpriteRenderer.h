#pragma once
#include "hjComponent.h"

namespace hj
{
	class SpriteRenderer : public Component
	{
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Render(HDC hdc);
		virtual void Release();

	private:

	};

}
