#pragma once
#include "hjGameObject.h"
#include "hjImage.h"

namespace hj
{

	class BackGround : public GameObject
	{
	public:
		BackGround();
		~BackGround();
		
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:
		Image* mImage;
	};

}