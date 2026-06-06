#ifdef USE_TDJ

#pragma once
#include "..\common\mouse.h"
#include "..\common\keyboard.h"

class TDJ
{
public:
	struct RatioPoint
	{
		double x;
		double y;
	};

	TDJ(HWND hwnd);
	~TDJ();

	void Main();

private:
	void MouseLBClick(RatioPoint click_point, DWORD delay = 0);
	void UpdateRectInfo();
	POINT ToWindowPoint(RatioPoint click_point) const;

private:
	HWND m_hwnd;
	RECT m_rect;
	KeyBoard m_keyboard;
	MOUSE m_mouse;
};

#endif
