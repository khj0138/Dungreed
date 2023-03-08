#pragma once
#include "hjGameObject.h"

namespace hj
{
	class Weapon : public GameObject
	{
	public:
		Weapon();
		~Weapon();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:

	};
}
