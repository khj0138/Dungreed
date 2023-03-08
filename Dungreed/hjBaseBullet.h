#pragma once
#include "hjGameObject.h"
namespace hj
{

	class BaseBullet : public GameObject
	{
	public:
		BaseBullet();
		~BaseBullet();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc)override;
		virtual void Release() override;
	};

}
