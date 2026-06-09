#pragma once

#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <thread>
#include <atomic>
#include "detection.hpp"
#include "etw.hpp"

// Global handles for UI elements
extern HWND hStartBtn, hStopBtn, hProgress;

// Thread‑state (atomic so UI and ETW thread can read/write safely)
extern std::atomic<bool> g_etwRunning;
extern std::thread       g_etwThread;

// Public API
void StartGuiThread();
int  RunMessageLoop();               // (optional helper)
void ProcessQueuedEvents(HWND hListView);
void InitControls(HWND parent);
void StartETWThread();
void StopETWThread();
void JoinGuiThread();
