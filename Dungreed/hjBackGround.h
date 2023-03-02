#pragma once
#include "hjGameObject.h"
#include "hjImage.h"
#include "hjSpriteRenderer.h"
#include "hjTransform.h"


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

		void setBG(const std::wstring& key, const std::wstring& path);
		void setPos(Vector2 pos);
		void setV(double v) { mV = v; }

	private:
		SpriteRenderer* spr;
		Transform* tr;
		double mV;

	};

}