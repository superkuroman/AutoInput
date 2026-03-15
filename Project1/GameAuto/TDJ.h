#ifdef USE_TDJ

#pragma once
#include<fstream>
#include<mouse.h>
#include<keyboard.h>

class TDJ
{
public:
	TDJ(HWND hwnd);
	~TDJ();

	void Main();
	
private:
	void MouseLBClick(POINT click_point, DWORD delay = 0.0);
	void UpdateRectInfo();

private:
	HWND m_hwnd;
	POINT m_input_data_screen_size; // 入力データを取る時のスクリーンサイズ
	RECT m_rect;
	double m_rate_x;
	double m_rate_y;
	KeyBoard m_keyboard;
	MOUSE m_mouse;
};

#endif

