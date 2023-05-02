#include "hjEmanager.h"
#include "hjMouse.h"
#include "hjMath.h"
#include "hjMouse.h"
#include "hjEffect.h"
#include "hjComponent.h"
#include "hjTransform.h"
#include "hjAnimator.h"
#include "hjImage.h"
#include "hjRscManager.h"
#include "hjSceneManager.h"
#include "hjSpriteRenderer.h"


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
		bEffects.clear();
		for (auto effect : mEffects)
		{
			delete effect.second;
			effect.second = nullptr;
		}
		mEffects.clear();
	}

	void Emanager::Initialize()
	{
	}

	void Emanager::Update()
	{
		mPos = GetOwner()->GetComponent<Transform>()->GetPos();
		mDir = (Mouse::GetPos() - Camera::CaluatePos(mPos, Vector2::One)).Normalize();
		isFlip = Mouse::GetPos().x < Camera::CaluatePos(mPos, Vector2::One).x;
		if (mActiveEffect != nullptr)
		{
			mActiveEffect->Update();
		}
	}

	void Emanager::Render(HDC hdc)
	{
		/*if (mActiveEffect != nullptr)
		{
			mActiveEffect->Render(hdc);
		}*/
	}

	void Emanager::Release()
	{
	}

	void Emanager::CreateEffect(const std::wstring& name, Vector2 direction)
	{
		effectInfo* eInfo = FindEffect(name);
		if (eInfo != nullptr)
		{
			if (!(eInfo->bCreate))
				return;
			/*if (bEffects.find(eInfo) != bEffects.end())
				if(bEffects.find(eInfo)->second == false)
					return;*/
			Effect* newEffect = nullptr;
			Vector2 pos = GetOwner()->GetComponent<Transform>()->GetPos();
			newEffect = new Effect(GetOwner()->GetComponent<Transform>()->GetPos());

			if (eInfo->bRotate)
			{
				newEffect->Create(eInfo->mImage, eInfo->mName, eInfo->mFrame, 0, eInfo->mOffset, eInfo->mPlayRate, false);
			}
			else if (!(eInfo->bFlip))
			{
				newEffect->Create(eInfo->mImage, eInfo->mName, eInfo->mFrame, 0, eInfo->mOffset, eInfo->mPlayRate, false);
			}
			else if (GetFlip())
			{
				//newEffect->SetName(eInfo->mName);
				//std::wstring flipName = L"Flipped";
				//flipName.append(eInfo->mName);
				newEffect->Create(eInfo->mImage, eInfo->mName, eInfo->mFrame, 1, eInfo->mOffset * (Vector2::Right * -1.f), eInfo->mPlayRate);
			}
			else
			{
				//newEffect->SetName(eInfo->mName);
				newEffect->Create(eInfo->mImage, eInfo->mName, eInfo->mFrame, 0, eInfo->mOffset, eInfo->mPlayRate);
			}

			newEffect->SetEmanager(this);
			SceneManager::FindScene(eSceneType::Play)->AddGameObject(newEffect, eLayerType::Effect);
			newEffect->SetState(eState::Active);
			if (eInfo->bRotate)
			{
				newEffect->GetComponent<SpriteRenderer>()->SetRotate(true);
				if (GetFlip())
				{
					newEffect->GetComponent<SpriteRenderer>()->SetDir(direction);
				}
				else
				{
					newEffect->GetComponent<SpriteRenderer>()->SetDir(direction);

				}
				//nnewEffect->GetComponent<SpriteRenderer>()->SetDir(direction);

			}
			if (!(eInfo->bDuplicate))
				eInfo->bCreate = false;
		}
	}

	void Emanager::RegisterEffect(const std::wstring& name, const std::wstring& path,
		bool loop, bool rotate, UINT frame, Vector2 offset, float playRate, Vector2 size, bool flip)
	{
		//SetAsRatio(Vector2::One * ((float)application.GetWidth() / 960.0f));
		SetAsRatio(size);
		effectInfo* newEffect = new effectInfo;
		newEffect->mImage = RscManager::Load<Img>(name, path);
		newEffect->mImage->MatchRatio(mAsRatio);
		newEffect->mImage->SetMoveRate(Vector2::One);
		newEffect->mName = name;
		newEffect->mFrame = frame;
		newEffect->mOffset = offset;
		newEffect->mPlayRate = playRate;
		newEffect->bDuplicate = loop;
		newEffect->bCreate = true;
		newEffect->bRotate = rotate;
		newEffect->bFlip = flip;
		if (newEffect != nullptr)
		{
			mEffects.insert(std::make_pair(name, newEffect));
			bEffects.insert(std::make_pair(newEffect, true));
		}

	}

	Emanager::effectInfo* Emanager::FindEffect(const std::wstring& name)
	{
		std::map<std::wstring, effectInfo*>::iterator iter
			= mEffects.find(name);

		if (iter == mEffects.end())
			return nullptr;
		return iter->second;
	}

	/*void Emanager::PlayEffect(const std::wstring& name)
	{
		mActiveEffect = FindEffect(name);
		mActiveEffect->Play();
	}*/

	void Emanager::Clear()
	{
		//mActiveEffect->GetComponent<Animator>()->
		mActiveEffect->GetAnimator()->Reset();
		mActiveEffect = nullptr;
	}

	//	return wfuncs->Wrender.mWfunc;
	//}
}