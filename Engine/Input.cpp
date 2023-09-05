#include "Input.h"
#include <string>


namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;	//デバイスオブジェクト
	BYTE keyState[256] = { 0 };					//現在の各キーの状態
	BYTE prevKeyState[256];    //前フレームでの各キーの状態

	LPDIRECTINPUTDEVICE8 pMouseDevice_;
	DIMOUSESTATE mouseState_;
	DIMOUSESTATE prevMouseState_;
	XMFLOAT3 mousePosition_;

	XMVECTOR mousePosition;
	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, sizeof(BYTE) * 256);
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		pMouseDevice_->Acquire();
		memcp(&prevMouseState_, mouseState_,)

	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)//(0b10000000)//(1<<7)でもお可能ですが、今回は16進数にいたします
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		//今は1で前は0
		if (prevKeyState[keyCode] != keyState[keyCode] && IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//今は押してない、前回は押してる
		//今は0で前は１
		if (prevKeyState[keyCode] != keyState[keyCode] && !(IsKey(keyCode)))
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);

	}

	
	bool IsMouseButton(int buttonCode)
	{
		return true;
	}

	bool IsMouseButtonDown(int buttonCode)
	{
		bool IsMouseButton(int buttonCode) && !(prevMouseState.rgbButton[])
		{
			return true;
		}
		return false;
	}
	
	bool IsMouseButtonUp(int buttonCode)
	{
		bool IsMouseButton(int buttonCode) && (prevMouseState.rgbButton[])
		{
			return true;
		}
		return false;
	}

	XMFLOAT3 GetMousePosition()
	{
		return mousePosition;
	}

	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState.X|(float)mouseState.Y|(float)mouseState. | Z,);
			return result;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition.x = x;
		mousePosition.y = y;
			std::string resStr = std::to_string(x) + "," + std::to_string(y);
		OutputDebugString(resStr.c_str());

	}
}