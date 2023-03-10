#pragma once
#include "hjResource.h"

namespace hj
{


	class Image : public Resource
	{
	public:
		static Image* Create(const std::wstring& name, UINT width, UINT height);
		Image();
		~Image();

		virtual HRESULT Load(const std::wstring& path) override;

		HDC GetHdc() { return mHdc; }
		HBITMAP GetBitmap() { return  mBitmap; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

		void SetSize(Vector2 size) { mWidth = size.x; mHeight = size.y; }
		void SetOutputRatio(Vector2 ratio);
		void matchResolution(Vector2 Resolution = Vector2{ 0.0f, 0.0f });
		static void SetAsRatio(Vector2 ratio) { mAsRatio = ratio; }

	private:
		HBITMAP mBitmap;
		HDC mHdc;
		UINT mWidth;
		UINT mHeight;
		static Vector2 mAsRatio;
	};

}
