#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjSpriteRenderer.h"
#include "hjTransform.h"
#include "hjAnimator.h"


namespace hj
{

	class BackGround : public GameObject
	{
	public:
		BackGround();
		BackGround(const std::wstring name, const std::wstring path, Vector2 moveRate, Vector2 asRatio, bool Repeat = false, UINT command = 0, Vector2 offset = Vector2::Zero);
		~BackGround();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void SetPos(Vector2 pos);
		//void setV(double v) { mV = v; }
		void SetImage(const std::wstring name, const std::wstring path, Vector2 moveRate, Vector2 asRatio, bool repeat = false);
		bool GetRepeat() { return mRepeat; }
		Vector2 GetSize();
		static bool bRender;
		static void SetBRender(bool render) { bRender = render; }
		static void bRenderChange() {
			bRender = (bool)(((int)bRender + 1) % 2);
		}
	private:
		SpriteRenderer* spr;
		Transform* tr;
		double mTime;
		Img* mImage;
		Vector2 mLeftTop;
		bool mRepeat;
		Vector2 mPos;
	};

}