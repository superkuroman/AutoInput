#ifdef USE_TDJ

#include "TDJ.h"

static const DWORD FPS = (DWORD)60.0f;
static const DWORD SEC = (DWORD)1000.0f;
static const DWORD WAIT_MILL_SEC = SEC / FPS;
static const DWORD NEXT_WAIT = WAIT_MILL_SEC * 1;


TDJ::TDJ(HWND hwnd)
	:m_hwnd(hwnd)
	, m_mouse()
{
	m_input_data_screen_size.x = 2560;
	m_input_data_screen_size.y = 1440;
	UpdateRectInfo();
}
TDJ::~TDJ()
{
}

void TDJ::Main()
{
	const int change_wait_count = 20;
	int change_count = 0;
	const unsigned int c_end_key = VK_2;
	while (true)
	{
		m_keyboard.Update();
		if (m_keyboard.IsKeyInput(VK_1))
		{
			while (true)
			{
				m_mouse.LBClick();
				Sleep(NEXT_WAIT * 10);
				if (m_keyboard.IsKeyInput(VK_1))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(c_end_key))
				{
					return;
				}
			}
		}
		if (m_keyboard.IsKeyInput(VK_3))
		{
			while (true)
			{
				m_mouse.LBClick();
				Sleep(NEXT_WAIT);
				if (m_keyboard.IsKeyInput(VK_3))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(c_end_key))
				{
					return;
				}
			}
		}
		else if (m_keyboard.IsKeyInput(VK_LMENU))
		{
			// 左ALTキー
			while (true)
			{
				if (m_keyboard.IsKeyInput(VK_LMENU))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(c_end_key))
				{
					return;
				}
			}
			change_count = 0;
		}
		else if (m_keyboard.IsKeyInput(VK_Q))
		{
			// 1スキル
			POINT clikc_point;
			clikc_point.x = 2016;
			clikc_point.y = 1271;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_W))
		{
			// 2スキル
			POINT clikc_point;
			clikc_point.x = 2093;
			clikc_point.y = 996;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_E))
		{
			// 3スキル
			POINT clikc_point;
			clikc_point.x = 2365;
			clikc_point.y = 897;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_SPACE))
		{
			// 決定
			POINT clikc_point;
			clikc_point.x = 2400;
			clikc_point.y = 1249;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_R))
		{
			// 取り消し
			POINT clikc_point;
			clikc_point.x = 1861;
			clikc_point.y = 1302;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_D))
		{
			Sleep(NEXT_WAIT);
			m_mouse.LBDown();
			Sleep(NEXT_WAIT);
			m_mouse.LBUp();
			Sleep(NEXT_WAIT);
			// 石 捨てる インベントリ
			POINT clikc_point;
			clikc_point.x = 917;
			clikc_point.y = 270;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_F))
		{
			Sleep(NEXT_WAIT);
			m_mouse.LBDown();
			Sleep(NEXT_WAIT);
			m_mouse.LBUp();
			Sleep(NEXT_WAIT * 15);
			// 石 留め キャラ選択 石ない状態
			POINT clikc_point;
			clikc_point.x = 1884;
			clikc_point.y = 368;
			MouseLBClick(clikc_point);
		}
		else if (m_keyboard.IsKeyInput(VK_C))
		{
			Sleep(NEXT_WAIT);
			m_mouse.LBDown();
			Sleep(NEXT_WAIT);
			m_mouse.LBUp();
			Sleep(NEXT_WAIT);

			// 派遣戻す
			POINT clikc_point;
			clikc_point.x = 1552;
			clikc_point.y = 1079;
			MouseLBClick(clikc_point,static_cast<DWORD>(38.0));
		}

		if (m_keyboard.IsKeyInput(c_end_key))
		{
			return;
		}
	}
}

void TDJ::MouseLBClick(POINT click_point, DWORD delay)
{
	ShowCursor(FALSE);
	UpdateRectInfo();
	m_mouse.SavePos();
	
	POINT move_after_pos;
	move_after_pos.x = m_rect.left + static_cast<int>(m_rate_x * click_point.x);
	move_after_pos.y = m_rect.top + static_cast<int>(m_rate_y * click_point.y);
	m_mouse.MoveLock(
		move_after_pos.x,
		move_after_pos.y
	);
	if (delay >= 0.0f)
	{
		Sleep(NEXT_WAIT * delay);
	}
	Sleep(NEXT_WAIT);
	m_mouse.LBDown();
	Sleep(NEXT_WAIT);
	m_mouse.LBUp();
	Sleep(NEXT_WAIT);
	m_mouse.RestorePos();
	Sleep(NEXT_WAIT);
	m_mouse.Unlock();
	ShowCursor(TRUE);
}

void TDJ::UpdateRectInfo()
{
	GetWindowRect(m_hwnd, &m_rect);
	double size_x =
		static_cast<double>(m_rect.right) - static_cast<double>(m_rect.left);
	double size_y =
		static_cast<double>(m_rect.bottom) - static_cast<double>(m_rect.top);
	m_rate_x = size_x / m_input_data_screen_size.x;
	m_rate_y = size_y / m_input_data_screen_size.y;
}

#endif