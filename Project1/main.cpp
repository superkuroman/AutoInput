#pragma once
#include<fstream>
#include"common/mouse.h"
#include"common/keyboard.h"
#include<vector>
#include<string>
#include<chrono>
#include<ctime>
#include<iomanip>
#include<sstream>
#include<windows.h>
#include "InputStep.h"
#ifdef USE_ARCHE_LAND
#include"GameAuto\InArcheLand.h"
#endif
#ifdef USE_TDJ
#include"GameAuto\TDJ.h"
#endif
#ifdef USE_OTHER
#include"GameAuto\Other.h"
#endif

using namespace std;
static const DWORD FPS = (DWORD)30.0f;
static const DWORD SEC = (DWORD)1000.0f;
static const DWORD WAIT_MILL_SEC = static_cast<DWORD>((double)SEC / (double)FPS);
static const DWORD NEXT_WAIT = WAIT_MILL_SEC * 1;
static const float _4KRATE = 2.5f;
static const float FULLHD_RATE = 1.0f;
static const float SETTING_RATE = _4KRATE;

KeyBoard m_keyboard;
MOUSE m_mouse;
int main()
{
#ifdef MousePosCustom

#define JSON_OUTPUT_FILE "TargetClicks.json"
	InputStep input(JSON_OUTPUT_FILE);
	bool is_stop = false;
	while (true)
	{
		input.Update();
	}
#else

#ifdef USE_OTHER
	HWND hWnd = FindWindow(NULL, TEXT("未設定です"));
	if (hWnd != nullptr)
	{
		Other other(nullptr);
		other.Main();
		return 0;
	}
#endif
#ifdef USE_TDJ
	HWND hWnd = FindWindow(NULL, TEXT("天地劫：幽城再临"));
	if (hWnd == nullptr)
	{
		hWnd = FindWindow(NULL, TEXT("天地劫"));
	}
	if (hWnd != nullptr)
	{
		TDJ tdj(hWnd);
		tdj.Main();
		return 0;
	}
#endif
#ifdef USE_ARCHE_LAND
	HWND hWnd = FindWindow(NULL, TEXT("아르케랜드"));
	if (hWnd != nullptr)
	{
		InArcheLand arche_land(hWnd);
		arche_land.Main();
		return 0;
	}
#endif
	

	while (true)
	{
		const unsigned int c_end_key = VK_5;
		if (m_keyboard.IsKeyInput(VK_1) || m_keyboard.IsKeyInput(VK_2))
		{
			while (true)
			{
				static const double rate = 0.3f;
				static const DWORD wait_sec = static_cast<DWORD>( (double)NEXT_WAIT * rate );
				Sleep(wait_sec);
				m_mouse.LBDown();
				Sleep(wait_sec);
				m_mouse.LBUp();
				Sleep(wait_sec);
				if (m_keyboard.IsKeyInput(VK_1))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(VK_3))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(VK_4))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(c_end_key))
				{
					return 0;
				}
			}
		}
		else if (m_keyboard.IsKeyInput(VK_3) || m_keyboard.IsKeyInput(VK_4))
		{
			while (true)
			{
				static const double rate = 0.3f;
				static const DWORD wait_sec = static_cast<DWORD>((double)NEXT_WAIT * rate);
				Sleep(wait_sec);
				m_keyboard.KeyInput(VK_Z);
				Sleep(wait_sec);
				if (m_keyboard.IsKeyInput(VK_1))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(VK_3))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(VK_4))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(c_end_key))
				{
					return 0;
				}
			}
		}
		else if (m_keyboard.IsKeyInput(c_end_key))
		{
			return 0;
		}

		
	}
#endif // DEBUGTEST
	return 0;
}

