#pragma once
#include "hjResource.h"

namespace hj
{


	class Img : public Resource
	{
	public:
		static Img* Create(const std::wstring& name, UINT width, UINT height);
		Img();
		~Img();

		virtual HRESULT Load(const std::wstring& path) override;

		HDC GetHdc() { return mHdc; }
		HBITMAP GetBitmap() { return  mBitmap; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		bool GetRepeat() { return mRepeat; }
		Vector2 GetMoveRate() { return mMoveRate; }

		void SetMoveRate(Vector2 rate) { mMoveRate = rate; }
		void SetRepeat(bool repeat) { mRepeat = repeat; }
		void SetSize(Vector2 size) { mWidth = size.x; mHeight = size.y; }
		void SetOutputSize(Vector2 size);
		void MatchRatio(Vector2 ratio);
		void makeMask(const std::wstring& path);

	private:
		HBITMAP mBitmap;
		HDC mHdc;
		UINT mWidth;
		UINT mHeight;
		bool mRepeat;
		Vector2 mMoveRate;
		HDC mask;
		HBITMAP mask_bmp;

	};

}
