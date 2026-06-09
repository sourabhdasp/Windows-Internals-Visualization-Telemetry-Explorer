// gui.cpp
#include "gui.hpp"
#include "detection.hpp"
#include "thread_intelligence/thread_monitor.hpp"
#include "memory_intelligence/memory_monitor.hpp"
#include <windows.h>
#include <commctrl.h>
#include <thread>
#include <atomic>
#include <cstring>
#include <string>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Uxtheme.lib")

static const wchar_t* WINDOW_CLASS = L"TelemetryExplorerGUI";
static const int    WINDOW_WIDTH = 1000;
static const int    WINDOW_HEIGHT = 600;
static HWND hListView = nullptr;
static HWND hStartBtn = nullptr;
static HWND hStopBtn = nullptr;
static HWND hProgress = nullptr;
static std::thread guiThread;

// Forward declarations
LRESULT CALLBACK GuiWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitListView(HWND parent);
void InitControls(HWND parent);
void AppendLogItem(const DetectionEvent& ev);
void StartETWThread();
void StopETWThread();

// ETW thread management
std::atomic<bool> g_etwRunning{false};
std::thread g_etwThread;


void StartGuiThread()
{
    // Ensure common controls are loaded (for ListView)
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icex);

    guiThread = std::thread([]() {
        // Register window class
        WNDCLASSEXW wc = { 0 };
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = GuiWndProc;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.lpszClassName = WINDOW_CLASS;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        RegisterClassExW(&wc);

        HWND hwnd = CreateWindowExW(
            0,
            WINDOW_CLASS,
            L"Telemetry Explorer – Detection Dashboard",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            nullptr, nullptr,
            wc.hInstance,
            nullptr);

        if (!hwnd) return;

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        // Message loop – a short timer will drive UI updates
        MSG msg;
        while (GetMessageW(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    });
    // Detach so the app can exit cleanly when main finishes
    // guiThread.detach(); // removed detach; will join later
}

LRESULT CALLBACK GuiWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        InitListView(hwnd);
        InitControls(hwnd);
        // Set a timer to poll detection queue every 150 ms
        SetTimer(hwnd, 1, 150, nullptr);
        return 0;
    case WM_TIMER:
        // Pull all pending events and add to UI
        {
            DetectionEvent ev;
            while (PopDetectionEvent(ev)) {
                AppendLogItem(ev);
            }
        }
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case 1001: // Start
                if (!g_etwRunning) {
                    StartETWThread();
                }
                break;
            case 1002: // Stop
                if (g_etwRunning) {
                    StopETWThread();
                }
                break;
        }
        return 0;
    case WM_SIZE:
        if (hListView) {
            MoveWindow(hListView, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

void InitListView(HWND parent)
{
    // Create a full‑size ListView in report mode
    hListView = CreateWindowExW(
        0,
        WC_LISTVIEWW,
        nullptr,
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS,
        0, 0, 0, 0,
        parent, nullptr,
        GetModuleHandleW(nullptr),
        nullptr);

    // Columns: Time, Type, PID, TID, Description, Details
    LVCOLUMNW col = { 0 };
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.fmt = LVCFMT_LEFT;
    const wchar_t* titles[] = { L"Time", L"Type", L"PID", L"TID", L"Description", L"Details" };
    int widths[] = { 120, 120, 80, 80, 250, 250 };
    for (int i = 0; i < 6; ++i) {
        col.pszText = (LPWSTR)titles[i];
        col.cx = widths[i];
        col.iSubItem = i;
        ListView_InsertColumn(hListView, i, &col);
    }

    // Enable dark theme if OS supports it (Windows 10+)
    SetWindowTheme(hListView, L"Explorer", nullptr);
    // Ensure controls are on top
    if (hStartBtn) SetWindowPos(hStartBtn, HWND_TOP, 10, 10, 0, 0, SWP_NOSIZE);
    if (hStopBtn) SetWindowPos(hStopBtn, HWND_TOP, 100, 10, 0, 0, SWP_NOSIZE);
    SetWindowPos(hProgress, HWND_TOP, 190, 10, 300, 30, 0);
    return;
}


void AppendLogItem(const DetectionEvent& ev)
{
    // Build row items
    LVITEMW item = { 0 };
    item.mask = LVIF_TEXT;
    item.iItem = ListView_GetItemCount(hListView);
    // Time column (simple HH:MM:SS)
    SYSTEMTIME st; GetLocalTime(&st);
    wchar_t timeStr[16];
    swprintf_s(timeStr, L"%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    item.iSubItem = 0; item.pszText = timeStr; ListView_InsertItem(hListView, &item);
    // Helper to set subitem text
    auto setSub = [&](int col, const std::wstring& txt) {
        ListView_SetItemText(hListView, item.iItem, col, (LPWSTR)txt.c_str());
    };
    // Type string
    std::wstring typeStr;
    switch (ev.type) {
    case EventType::ProcessStart: typeStr = L"ProcessStart"; break;
    case EventType::ProcessEnd:   typeStr = L"ProcessEnd";   break;
    case EventType::ThreadStart:  typeStr = L"ThreadStart";  break;
    case EventType::ThreadStop:   typeStr = L"ThreadStop";   break;
    case EventType::MemoryAlert:  typeStr = L"MemoryAlert";  break;
    case EventType::APCAlert:     typeStr = L"APCAlert";     break;
    case EventType::RiskAlert:    typeStr = L"RiskAlert";    break;
    }
    setSub(1, typeStr);
    setSub(2, ev.pid);
    setSub(3, ev.tid);
    setSub(4, ev.description);
    setSub(5, ev.details);
    // Auto‑scroll to newest entry
    ListView_EnsureVisible(hListView, item.iItem, FALSE);
}

// Simple wrapper used by ETW code – currently a no‑op because we already push via LogDetection
void EnqueueGuiLog(const std::wstring& description, const std::wstring& details)
{
    // In this minimal version we log as a generic RiskAlert with empty PID/TID.
    DetectionEvent ev{};
    ev.type = EventType::RiskAlert;
    ev.pid = L"";
    ev.tid = L"";
    ev.description = description;
    ev.details = details;
    PushDetectionEvent(ev);
}
// ----------------------------------------------------------------
// GUI helper implementations (previously missing)
// ----------------------------------------------------------------

// Create the UI controls (Start/Stop buttons and progress bar).
void InitControls(HWND parent)
{
    // Start button
    hStartBtn = CreateWindowExW(
        0, L"BUTTON", L"Start",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 80, 30,
        parent, (HMENU)1001,
        GetModuleHandleW(nullptr), nullptr);

    // Stop button
    hStopBtn = CreateWindowExW(
        0, L"BUTTON", L"Stop",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 10, 80, 30,
        parent, (HMENU)1002,
        GetModuleHandleW(nullptr), nullptr);

    // Progress bar (marquee style)
    InitCommonControls();
    hProgress = CreateWindowExW(
        0, PROGRESS_CLASS, nullptr,
        WS_CHILD | WS_VISIBLE,
        190, 10, 300, 30,
        parent, nullptr,
        GetModuleHandleW(nullptr), nullptr);
    // Enable marquee animation (30 ms interval)
    SendMessageW(hProgress, PBM_SETMARQUEE, TRUE, 30);
}

// Start the ETW processing in a background thread.
void StartETWThread()
{
    if (g_etwRunning) return; // already running
    g_etwRunning = true;
    g_etwThread = std::thread([](){
        // Start Thread Monitor for thread-level telemetry
        StartThreadMonitor();
        
        // Start Memory Monitor for memory-level telemetry
        StartMemoryMonitor();
        
        StartETW();            // defined in etw.cpp
        g_etwRunning = false; // clear flag when finished
    });
    g_etwThread.detach(); // detach so GUI can exit cleanly
}

// Stop the ETW thread by clearing the running flag.
void StopETWThread()
{
    g_etwRunning = false; // ETW loop should honor this flag
    // In a full implementation we would also call ControlTrace to stop the session.
}
// Join the GUI thread – blocks until the window is closed.
void JoinGuiThread()
{
    if (guiThread.joinable())
        guiThread.join();
}
