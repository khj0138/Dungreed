#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjAnimator.h"

#include "hjTransform.h"

namespace hj
{
	class Bird : public GameObject
	{
	public:
		Bird();
		~Bird();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;
		void setAnimation(const std::wstring name, const std::wstring path);
	private:
		Image* mImage;
		Animator* mAnimator;
	};

}
