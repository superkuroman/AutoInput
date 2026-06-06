#ifdef USE_TDJ

#include "TDJ.h"

#include <cstddef>

namespace
{
constexpr DWORD kFps = 60;
constexpr DWORD kSec = 1000;
constexpr DWORD kWaitMillSec = kSec / kFps;
constexpr DWORD kNextWait = kWaitMillSec;
constexpr DWORD kIdleWait = 16;
constexpr unsigned int kEndKey = VK_2;

struct ClickAction
{
	int key;
	TDJ::RatioPoint point;
};

const ClickAction kClickActions[] = {
	{ VK_Q, { 0.78750000, 0.88263889 } },
	{ VK_W, { 0.81757813, 0.69166667 } },
	{ VK_E, { 0.92382813, 0.62291667 } },
	{ VK_SPACE, { 0.93750000, 0.86736111 } },
	{ VK_R, { 0.72703125, 0.90416667 } },
};
}

TDJ::TDJ(HWND hwnd)
	: m_hwnd(hwnd)
	, m_rect{}
	, m_mouse()
{
	UpdateRectInfo();
}

TDJ::~TDJ()
{
}

void TDJ::Main()
{
	m_keyboard.Update();
	while (!m_keyboard.IsKeyInput(kEndKey))
	{
		m_keyboard.Update();
		if (m_keyboard.IsKeyInput(VK_1))
		{
			while (true)
			{
				m_keyboard.Update();
				m_mouse.LBClick();
				Sleep(kNextWait * 10);
				if (m_keyboard.IsKeyInput(VK_1))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(kEndKey))
				{
					return;
				}
			}
		}
		if (m_keyboard.IsKeyInput(VK_3))
		{
			while (true)
			{
				m_keyboard.Update();
				m_mouse.LBClick();
				Sleep(kNextWait);
				if (m_keyboard.IsKeyInput(VK_3))
				{
					break;
				}
				if (m_keyboard.IsKeyInput(kEndKey))
				{
					return;
				}
			}
		}
		else
		{
			for (std::size_t index = 0; index < sizeof(kClickActions) / sizeof(kClickActions[0]); ++index)
			{
				if (m_keyboard.IsKeyInput(kClickActions[index].key))
				{
					MouseLBClick(kClickActions[index].point);
					break;
				}
			}
		}

		Sleep(kIdleWait);
	}
}

void TDJ::MouseLBClick(RatioPoint click_point, DWORD delay)
{
	ShowCursor(FALSE);
	UpdateRectInfo();
	m_mouse.SavePos();

	const POINT move_after_pos = ToWindowPoint(click_point);
	m_mouse.MoveLock(
		move_after_pos.x,
		move_after_pos.y
	);
	Sleep(kNextWait * delay);
	Sleep(kNextWait);
	m_mouse.LBDown();
	Sleep(kNextWait);
	m_mouse.LBUp();
	Sleep(kNextWait);
	m_mouse.RestorePos();
	Sleep(kNextWait);
	m_mouse.Unlock();
	ShowCursor(TRUE);
}

void TDJ::UpdateRectInfo()
{
	GetWindowRect(m_hwnd, &m_rect);
}

POINT TDJ::ToWindowPoint(RatioPoint click_point) const
{
	const double size_x = static_cast<double>(m_rect.right - m_rect.left);
	const double size_y = static_cast<double>(m_rect.bottom - m_rect.top);
	return {
		m_rect.left + static_cast<int>(size_x * click_point.x),
		m_rect.top + static_cast<int>(size_y * click_point.y),
	};
}

#endif
