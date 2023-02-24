#include "hjComponent.h"


namespace hj {
	Component::Component(eComponentType type)
		: mType(type)
	{

	}
	Component::~Component()
	{
	}
	void hj::Component::Initialize()
	{
	}

	void hj::Component::Update()
	{
	}

	void hj::Component::Render(HDC hdc)
	{

	}

	void hj::Component::Release()
	{
	}

}