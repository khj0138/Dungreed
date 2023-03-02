#include "hjInput.h"


namespace hj
{
	int ASCII[(UINT)eKeyCode::END] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',VK_LBUTTON, VK_RBUTTON,
	};

	

	std::vector<Input::Key> Input::mKeys;
//	std::vector<Input::Mouse> Input::mMouse;

	void Input::Initialize()
	{
		for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
		{
			Key keyInfo;
			keyInfo.key = (eKeyCode)i;
			keyInfo.state = eKeyState::None;
			keyInfo.bPressed = false;

			mKeys.push_back(keyInfo);
		}
		/*for (UINT i = 0; i < (UINT)eMouseCode::END; i++)
		{
			Mouse mouseInfo;
			mouseInfo.pos = (eMouseCode)i;
			mouseInfo.state = eMouseState::None;
			mouseInfo.bPressed = false;

			mMouse.push_back(mouseInfo);
		}*/
		
	}

	void Input::Update()
	{
		for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
		{
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{
				if (mKeys[i].bPressed)
					mKeys[i].state = eKeyState::Pressed;
				else
					mKeys[i].state = eKeyState::Down;

				mKeys[i].bPressed = true;
			}
			else
			{
				if (mKeys[i].bPressed)
					mKeys[i].state = eKeyState::Up;
				else
					mKeys[i].state = eKeyState::None;

				mKeys[i].bPressed = false;
			}
		}
		/*for (UINT i = 0; i < (UINT)eMouseCode::END; i++)
		{
			GetMouse
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{
				if (mMouse[i].bPressed)
					mMouse[i].state = eMouseState::Pressed;
				else
					mMouse[i].state = eMouseState::Down;

				mMouse[i].bPressed = true;
			}
			else
			{
				if (mMouse[i].bPressed)
					mMouse[i].state = eMouseState::Up;
				else
					mMouse[i].state = eMouseState::None;

				mMouse[i].bPressed = false;
			}
		}*/
	}

	void Input::Render(HDC hdc)
	{

	}

}