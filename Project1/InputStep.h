#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <windows.h>

struct ClickRecord { int x; int y; std::string button; int delay_ms; };

class InputStep
{
public:
    explicit InputStep(const char* outFile = "defined_clicks.json");
    // Call this regularly from the main loop. It will handle recording and playback triggers.
    void Update();

private:
    void SaveToFile(const char* filename);
    void PlaybackFromFile(const char* filename);
    // recording handlers (integrated)
    void HandleRecord();
    void HandleSaveTrigger();
    void HandlePlaybackTrigger();

    bool recording_;
    std::vector<ClickRecord> records_;
    bool lb_was_down_;
    bool rb_was_down_;
    bool mb_was_down_;
    std::chrono::steady_clock::time_point last_event_time_;

    bool y_was_down_;
    bool p_was_down_;
    bool f12_was_down_;

    std::string output_file_;
};
