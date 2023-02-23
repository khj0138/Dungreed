#pragma once
#include "hjGameObject.h"
#include "hjImage.h"

namespace hj
{

	class Hero : public GameObject
	{
	public:
		Hero();
		~Hero();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:
		//std::vector<Image*> mImage;
		Image* mImage;
		bool flip;
	};

}