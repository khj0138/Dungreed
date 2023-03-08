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
			// ���ҽ��� ���� ������ Ž��
			// ������ Ȥ�� end ��ȯ
			std::map<std::wstring, Resource*>::iterator iter = mRscManager.find(key);

			//ã�� �������� Ÿ������ ����ȯ �� ��ȯ
			if (iter != mRscManager.end())
			{
				return dynamic_cast<T*>(iter->second);
			}

			// ������ ���� �� null ��ȯ
			return nullptr;
		}

		template <typename T>
		static T* Load(const std::wstring& key, const std::wstring& path)
		{
			// Ű������ Ž��
			T* resource = RscManager::Find<T>(key);
			if (resource != nullptr)
			{
				return resource;
			}

			// �ش� ���ҽ� ���� ��
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
