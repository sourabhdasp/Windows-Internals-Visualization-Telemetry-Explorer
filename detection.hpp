// detection.hpp
#pragma once
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

enum class EventType {
    ProcessStart,
    ProcessEnd,
    ThreadStart,
    ThreadStop,
    MemoryAlert,
    APCAlert,
    RiskAlert,
};

struct DetectionEvent {
    EventType type;
    std::wstring pid;        // Process ID as string
    std::wstring tid;        // Thread ID as string (if applicable)
    std::wstring description; // Human‑readable short description
    std::wstring details;    // Additional details (e.g., start address, module name)
};

// Thread‑safe queue API
void PushDetectionEvent(const DetectionEvent& ev);
bool PopDetectionEvent(DetectionEvent& ev); // returns false if queue empty
