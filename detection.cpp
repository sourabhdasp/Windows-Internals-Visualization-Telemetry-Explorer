#include "detection.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

static std::queue<DetectionEvent> g_eventQueue;
static std::mutex g_queueMutex;
static std::condition_variable g_queueCV;

void PushDetectionEvent(const DetectionEvent& ev)
{
    {
        std::lock_guard<std::mutex> lock(g_queueMutex);
        g_eventQueue.push(ev);
    }
    g_queueCV.notify_one();
}

bool PopDetectionEvent(DetectionEvent& ev)
{
    std::lock_guard<std::mutex> lock(g_queueMutex);
    if (g_eventQueue.empty())
        return false;
    ev = g_eventQueue.front();
    g_eventQueue.pop();
    return true;
}
