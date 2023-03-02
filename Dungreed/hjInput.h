#pragma once
#include "HjHjEngine.h"

namespace hj
{
	enum class eKeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M, MLEFT, MRIGHT,
		END,
	};

	enum class eKeyState
	{
		Down,
		Pressed,
		Up,
		None,
	};

	/*enum class eMouseCode
	{
		left,
		right,
		END
	};*/

	/*enum class eMouseState
	{
		Down,
		Pressed,
		Up,
		None,
	};*/

	class Input
	{
	public:
		struct Key
		{
			eKeyCode key;
			eKeyState state;
			bool bPressed;
		};
		//struct Mouse
		//{
		//	eMouseCode pos;
		//	eMouseState state;
		//	bool bPressed;
		//};

		static void Initialize();
		static void Update();
		static void Render(HDC hdc);

		inline static eKeyState GetKeyState(eKeyCode keyCode)
		{
			return mKeys[(UINT)keyCode].state;
		};

		//GetKey()		Ű�� ������ �ð���ŭ true�� ��ȯ
		//GetKeyDown()	Ű�� ������ ��, �� �ѹ� true�� ��ȯ
		//GetKeyUp()	Ű�� ������ ���� ��, �� �ѹ� true�� ��ȯ

		static __forceinline bool GetKey(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::Pressed;
		}
		
		static __forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::Down;
		}

		static __forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::Up;
		}
		static __forceinline bool GetKeyNone(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].state == eKeyState::None;
		}
		//
		//static __forceinline bool GetKey(eMouseCode mouseCode)
		//{
		//	return mMouse[static_cast<UINT>(mouseCode)].state == eMouseState::Pressed;
		//}
		//
		//static __forceinline bool GetKeyDown(eMouseCode mouseCode)
		//{
		//	return mMouse[static_cast<UINT>(mouseCode)].state == eMouseState::Down;
		//}

		//static __forceinline bool GetKeyUp(eMouseCode mouseCode)
		//{
		//	return mMouse[static_cast<UINT>(mouseCode)].state == eMouseState::Up;
		//}
		//static __forceinline bool GetKeyNone(eMouseCode mouseCode)
		//{
		//	return mMouse[static_cast<UINT>(mouseCode)].state == eMouseState::None;
		//}


		
	private:
		static std::vector<Key> mKeys;
//		static std::vector<Mouse> mMouse;
	};

}
