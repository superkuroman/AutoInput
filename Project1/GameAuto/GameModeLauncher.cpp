#include "GameModeLauncher.h"

#include <cstddef>
#include <cstdio>
#include <windows.h>

#ifdef USE_ARCHE_LAND
#include "InArcheLand.h"
#endif

#ifdef USE_OTHER
#include "Other.h"
#endif

#ifdef USE_TDJ
#include "TDJ.h"
#endif

namespace
{
struct GameMode
{
	const wchar_t* name;
	const wchar_t* const* window_titles;
	std::size_t window_title_count;
	void (*run)(HWND hwnd);
};

HWND FindModeWindow(const GameMode& mode)
{
	for (std::size_t index = 0; index < mode.window_title_count; ++index)
	{
		HWND hwnd = FindWindowW(nullptr, mode.window_titles[index]);
		if (hwnd != nullptr)
		{
			return hwnd;
		}
	}

	return nullptr;
}

bool TryRunMode(const GameMode& mode)
{
	HWND hwnd = FindModeWindow(mode);
	if (hwnd == nullptr)
	{
		return false;
	}

	wprintf(L"Auto mode: %s\n", mode.name);
	mode.run(hwnd);
	return true;
}

#ifdef USE_OTHER
void RunOther(HWND hwnd)
{
	Other other(hwnd);
	other.Main();
}

const wchar_t* const kOtherWindowTitles[] = {
	L"\x672a\x8a2d\x5b9a\x3067\x3059",
};

const GameMode kOtherMode = {
	L"Other",
	kOtherWindowTitles,
	sizeof(kOtherWindowTitles) / sizeof(kOtherWindowTitles[0]),
	RunOther,
};
#endif

#ifdef USE_TDJ
void RunTDJ(HWND hwnd)
{
	TDJ tdj(hwnd);
	tdj.Main();
}

const wchar_t* const kTDJWindowTitles[] = {
	L"\x5929\x5730\x52ab\xff1a\x5e7d\x57ce\x518d\x4e34",
	L"\x5929\x5730\x52ab",
};

const GameMode kTDJMode = {
	L"TDJ",
	kTDJWindowTitles,
	sizeof(kTDJWindowTitles) / sizeof(kTDJWindowTitles[0]),
	RunTDJ,
};
#endif

#ifdef USE_ARCHE_LAND
void RunArcheLand(HWND hwnd)
{
	InArcheLand arche_land(hwnd);
	arche_land.Main();
}

const wchar_t* const kArcheLandWindowTitles[] = {
	L"\xc544\xb974\xcf00\xb79c\xb4dc",
};

const GameMode kArcheLandMode = {
	L"ArcheLand",
	kArcheLandWindowTitles,
	sizeof(kArcheLandWindowTitles) / sizeof(kArcheLandWindowTitles[0]),
	RunArcheLand,
};
#endif
}

bool RunFirstAvailableGameMode()
{
#ifdef USE_OTHER
	if (TryRunMode(kOtherMode))
	{
		return true;
	}
#endif

#ifdef USE_TDJ
	if (TryRunMode(kTDJMode))
	{
		return true;
	}
#endif

#ifdef USE_ARCHE_LAND
	if (TryRunMode(kArcheLandMode))
	{
		return true;
	}
#endif

	return false;
}
