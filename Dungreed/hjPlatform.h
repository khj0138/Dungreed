#pragma once
#include "hjGameObject.h"
#include "hjImage.h"

namespace hj
{
	class Animator;
	class Platform : public GameObject
	{
	public:
		Platform();
		~Platform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;
		void setAnimation(const std::wstring name, const std::wstring path);

	private:
		Animator* mAnimator;
		Img* mImage;
	};

}
