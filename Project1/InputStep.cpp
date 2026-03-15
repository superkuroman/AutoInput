#include "InputStep.h"
#include <fstream>
#include <iostream>
#include <sstream>

InputStep::InputStep(const char* outFile)
	: recording_(false),
	  records_(),
	  lb_was_down_(false),
	  rb_was_down_(false),
	  mb_was_down_(false),
	  last_event_time_(std::chrono::steady_clock::now()),
	  y_was_down_(false),
	  p_was_down_(false),
	  output_file_(outFile)
{
}

void InputStep::SaveToFile(const char* filename)
{
    // resolve full path and report it
    char fullpath[MAX_PATH] = {0};
    DWORD res = GetFullPathNameA(filename, MAX_PATH, fullpath, nullptr);
    if (res == 0)
    {
        printf("Failed to resolve full path for '%s'\n", filename);
        // fall back to given filename
        strcpy_s(fullpath, MAX_PATH, filename);
    }
    else
    {
        printf("Saving to full path: %s\n", fullpath);
    }

    std::ofstream ofs(fullpath);
    if (!ofs)
    {
        printf("Failed to open %s for writing\n", fullpath);
        return;
    }
	ofs << "{\n  \"clicks\": [\n";
	for (size_t i = 0; i < records_.size(); ++i)
	{
		ofs << "    { \"x\": " << records_[i].x << ", \"y\": " << records_[i].y
			<< ", \"button\": \"" << records_[i].button << "\", \"delay_ms\": " << records_[i].delay_ms << " }";
		if (i + 1 < records_.size()) ofs << ",";
		ofs << "\n";
	}
	ofs << "  ]\n}\n";
	ofs.close();
	printf("Saved %zu clicks to %s\n", records_.size(), filename);
}

void InputStep::PlaybackFromFile(const char* filename)
{
	std::ifstream ifs(filename);
	if (!ifs)
	{
		printf("Failed to open %s for reading\n", filename);
		return;
	}
	std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	// naive parse
	std::vector<ClickRecord> play;
	size_t pos = 0;
	while (true)
	{
		size_t obj = content.find('{', pos);
		if (obj == std::string::npos) break;
		size_t xk = content.find("\"x\"", obj);
		if (xk == std::string::npos) break;
		size_t colon = content.find(':', xk);
		size_t comma = content.find_first_of(",}\n", colon);
		if (colon == std::string::npos || comma == std::string::npos) break;
		int x = atoi(content.substr(colon+1, comma-colon-1).c_str());

		size_t yk = content.find("\"y\"", comma);
		if (yk == std::string::npos) break;
		colon = content.find(':', yk);
		comma = content.find_first_of(",}\n", colon);
		int y = atoi(content.substr(colon+1, comma-colon-1).c_str());

		size_t bk = content.find("\"button\"", comma);
		if (bk == std::string::npos) break;
		colon = content.find(':', bk);
		size_t q1 = content.find('"', colon);
		size_t q2 = content.find('"', q1+1);
		std::string button = content.substr(q1+1, q2-q1-1);

		size_t dk = content.find("\"delay_ms\"", q2);
		if (dk == std::string::npos) break;
		colon = content.find(':', dk);
		comma = content.find_first_of(",}\n", colon);
		int delay = atoi(content.substr(colon+1, comma-colon-1).c_str());

		play.push_back({ x, y, button, delay });
		pos = comma;
	}

	if (play.empty())
	{
		printf("No playable clicks found in %s\n", filename);
		return;
	}

	printf("Playing back %zu clicks from %s\n", play.size(), filename);
	for (size_t i = 0; i < play.size(); ++i)
	{
		Sleep(play[i].delay_ms);
		SetCursorPos(play[i].x, play[i].y);
		if (play[i].button == "left")
		{
			INPUT inputs[2] = {};
			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			inputs[1].type = INPUT_MOUSE;
			inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(2, inputs, sizeof(INPUT));
		}
		else if (play[i].button == "right")
		{
			INPUT inputs[2] = {};
			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			inputs[1].type = INPUT_MOUSE;
			inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			SendInput(2, inputs, sizeof(INPUT));
		}
		else if (play[i].button == "middle")
		{
			INPUT inputs[2] = {};
			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			inputs[1].type = INPUT_MOUSE;
			inputs[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			SendInput(2, inputs, sizeof(INPUT));
		}
		printf("click %zu ms:%d \n", i, play[i].delay_ms);
	}
	printf("Playback finished\n");
}

void InputStep::Update()
{
	HandleRecord();
	HandleSaveTrigger();
	HandlePlaybackTrigger();
}
void InputStep::HandleRecord()
{
	// Handle toggle (F12)
	bool f12_down = (GetAsyncKeyState(VK_F12) & 0x8000) != 0;
	if (f12_down && !f12_was_down_)
	{
		if (!recording_)
		{
			recording_ = true;
			records_.clear();
			last_event_time_ = std::chrono::steady_clock::now();
			printf("Recording started. Click mouse to record positions. Press F12 again to stop.\n");
		}
		else
		{
			recording_ = false;
			SaveToFile("clicks.json");
		}
	}
	f12_was_down_ = f12_down;

	// If recording, process mouse button down edges
	if (!recording_) return;

	bool lb_down = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	bool rb_down = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
	bool mb_down = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

	if (lb_down && !lb_was_down_)
	{
		POINT p; GetCursorPos(&p);
		auto now = std::chrono::steady_clock::now();
		int delay = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - last_event_time_).count();
		last_event_time_ = now;
		records_.push_back({ p.x, p.y, "left", delay });
		printf("Recorded click %zu -> left x=%d y=%d delay=%d\n", records_.size(), p.x, p.y, delay);
	}
	if (rb_down && !rb_was_down_)
	{
		POINT p; GetCursorPos(&p);
		auto now = std::chrono::steady_clock::now();
		int delay = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - last_event_time_).count();
		last_event_time_ = now;
		records_.push_back({ p.x, p.y, "right", delay });
		printf("Recorded click %zu -> right x=%d y=%d delay=%d\n", records_.size(), p.x, p.y, delay);
	}
	if (mb_down && !mb_was_down_)
	{
		POINT p; GetCursorPos(&p);
		auto now = std::chrono::steady_clock::now();
		int delay = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - last_event_time_).count();
		last_event_time_ = now;
		records_.push_back({ p.x, p.y, "middle", delay });
		printf("Recorded click %zu -> middle x=%d y=%d delay=%d\n", records_.size(), p.x, p.y, delay);
	}

	lb_was_down_ = lb_down;
	rb_was_down_ = rb_down;
	mb_was_down_ = mb_down;
}

void InputStep::HandleSaveTrigger()
{
	bool y_down = (GetAsyncKeyState('Y') & 0x8000) != 0;
	if (y_down && !y_was_down_)
	{
		SaveToFile(output_file_.c_str());
	}
	y_was_down_ = y_down;
}

void InputStep::HandlePlaybackTrigger()
{
	bool p_down = (GetAsyncKeyState('P') & 0x8000) != 0;
	if (p_down && !p_was_down_)
	{
		PlaybackFromFile(output_file_.c_str());
	}
	p_was_down_ = p_down;
}
