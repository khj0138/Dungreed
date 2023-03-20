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
		float GetMoveRate() { return mMoveRate; }

		void SetMoveRate(float rate) { mMoveRate = rate; }
		void SetRepeat(bool repeat) { mRepeat = repeat; }
		void SetSize(Vector2 size) { mWidth = size.x; mHeight = size.y; }
		void SetOutputSize(Vector2 size);
		void MatchRatio(Vector2 ratio);

	private:
		HBITMAP mBitmap;
		HDC mHdc;
		UINT mWidth;
		UINT mHeight;
		bool mRepeat;
		float mMoveRate;
	};

}
