#include "hjEmanager.h"
#include "hjMouse.h"
#include "hjMath.h"
#include "hjMouse.h"
#include "hjEffect.h"
#include "hjComponent.h"
#include "hjTransform.h"
#include "hjAnimator.h"


namespace hj
{

	Emanager::Emanager()
		: GameObject()
		, mOwner(nullptr)
		, mDir(Vector2::Zero)
		, mActiveEffect(nullptr)
		, mPos(Vector2::Zero)
		, isFlip(false)
	{
	}

	Emanager::~Emanager()
	{
		for (auto effect : mEffects)
		{
			delete effect.second;
			effect.second = nullptr;
		}
	}

	void Emanager::Initialize()
	{
	}

	void Emanager::Update()
	{
		mPos = GetOwner()->GetComponent<Transform>()->GetPos();
		mDir = (Mouse::GetPos() - Camera::CaluatePos(mPos, 1.f)).Normalize();
		isFlip = Mouse::GetPos().x < Camera::CaluatePos(mPos, 1.f).x;
		if (mActiveEffect != nullptr)
		{
			mActiveEffect->Update();
		}
	}

	void Emanager::Render(HDC hdc)
	{
		if (mActiveEffect != nullptr)
		{
			mActiveEffect->Render(hdc);
		}
	}

	void Emanager::Release()
	{
	}

	void Emanager::CreateEffect(const std::wstring& name, const std::wstring&path, bool loop, bool bDir, UINT frame)
	{
		Effect* newEffect = nullptr;
		newEffect = new Effect();
		if (newEffect != nullptr)
		{
			newEffect->Create(name, path, loop, bDir,frame);
			newEffect->SetEmanager(this);
			mEffects.insert(std::make_pair(name, newEffect));
		}
	}

	Effect* Emanager::FindEffect(const std::wstring& name)
	{
		std::map<std::wstring, Effect*>::iterator iter
			= mEffects.find(name);

		if (iter == mEffects.end())
			return nullptr;
		return iter->second;
	}

	void Emanager::PlayEffect(const std::wstring& name)
	{
		mActiveEffect = FindEffect(name);
	}

	void Emanager::Clear()
	{
		//mActiveEffect->GetComponent<Animator>()->
		//mActiveEffect = nullptr;
	}

	//	return wfuncs->Wrender.mWfunc;
	//}
}