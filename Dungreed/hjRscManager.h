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
			std::wstring key2;
			key2.append(key);
			T* resource = RscManager::Find<T>(key2);
			if (resource != nullptr)
			{
				wchar_t s[65];
				_itow_s(rIndex++, s, 10);
				std::wstring s2(s);
				key2.append(s2);
			}

			// �ش� ���ҽ� ���� ��
			resource = new T();
			if (FAILED(resource->Load(path)))
			{
				assert(false);
				return nullptr;
			}

			resource->SetKey(key2);
			resource->SetPath(path);
			mRscManager.insert(std::make_pair(key2, resource));

			return dynamic_cast<T*>(resource);
		}

		template <typename T>
		static void Insert(const std::wstring& key, T* resource)
		{
			std::wstring key2;
			key2.append(key);
			T* temp = RscManager::Find<T>(key2);
			if (temp != nullptr)
			{
				key2 = key2 + ((wchar_t)(rIndex++));
			}
			mRscManager.insert(std::make_pair(key2, resource));
		}

		static void Release()
		{
			for (auto& pair : mRscManager)
			{
				delete pair.second;
				pair.second = nullptr;
			}
		}

	public:
		static UINT rIndex;
	private:
		static std::map<std::wstring, Resource*> mRscManager;
	};


}
