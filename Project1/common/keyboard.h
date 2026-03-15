#pragma once
#include "Me_include.h"


class KeyBoard
{
private:
	static const int MAX = 512;
	bool  m_key_in[MAX];

public:
	inline void Update()
	{
		for (int ii = 0; ii < MAX; ii++)
		{
			GetAsyncKeyState(ii);
		}
	}
	inline void init()
	{
		for (int ii = 0; ii < MAX; ii++)
		{
			m_key_in[ii] = false;
		}
	}

	inline bool IsKeyInput(int key)
	{
		if (GetAsyncKeyState(key))
		{
			if (m_key_in[key])
			{
				return false;
			}
			else
			{
				m_key_in[key] = true;
				return true;
			}
		}

		m_key_in[key] = false;
		return false;
	}
	inline void KeyInput(int key)
	{
		m_key_in[key] = true;
		keybd_event(key, 0, 0, 0);
		keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
	}
	inline void KeyInput(int key, bool is_up)
	{
		m_key_in[key] = true;
		if (is_up == true)
		{
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
		}
		else
		{
			keybd_event(key, 0, 0, 0);
		}
	}
	inline void KeyInput(int key, HWND hwnd)
	{
		m_key_in[key] = true;
		SendMessage(hwnd, WM_KEYDOWN, key, 0);
		SendMessage(hwnd, WM_KEYUP, key, 0);
	}
};