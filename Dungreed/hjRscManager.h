#pragma once
#include "hjResource.h"

namespace hj
{

	class RscManager
	{
	public:
		template <typename T>
		static T* Find(const std::wstring& key)
		{
			// 리소스맵 내부 데이터 탐색
			// 데이터 혹은 end 반환
			std::map<std::wstring, Resource*>::iterator iter = mRscManager.find(key);

			//찾는 데이터의 타입으로 형변환 후 반환
			if (iter != mRscManager.end())
			{
				return dynamic_cast<T*>(iter->second);
			}

			// 데이터 없을 시 null 반환
			return nullptr;
		}

		template <typename T>
		static T* Load(const std::wstring& key, const std::wstring& path)
		{
			// 키값으로 탐색
			T* resource = RscManager::Find<T>(key);
			if (resource != nullptr)
			{
				return resource;
			}

			// 해당 리소스 없을 시
			resource = new T();
			if (FAILED(resource->Load(path)))
			{
				assert(false);
				return nullptr;
			}

			resource->SetKey(key);
			resource->SetPath(path);
			mRscManager.insert(std::make_pair(key, resource));

			return dynamic_cast<T*>(resource);
		}

		template <typename T>
		static void Insert(const std::wstring& key, T* resource)
		{
			mRscManager.insert(std::make_pair(key, resource));
		}

		static void Release()
		{
			for (auto pair : mRscManager)
			{
				delete pair.second;
				pair.second = nullptr;
			}
		}
	private:
		static std::map<std::wstring, Resource*> mRscManager;
	};


}
