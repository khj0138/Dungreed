#pragma once
#include "hjGameObject.h"

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
		void StateChange(UINT prev, UINT cur);

	private:
		std::vector<int> mStates;
	};

}