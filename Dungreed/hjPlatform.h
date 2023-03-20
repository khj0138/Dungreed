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
		Platform(const std::wstring name, const std::wstring path, float moveRate, Vector2 asRatio, bool Repeat = false);
		~Platform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;
		void SetImage(const std::wstring name, const std::wstring path, float moveRate, Vector2 asRatio, bool Repeat = false);

	private:
		Animator* mAnimator;
		Img* mImage;
	};

}
