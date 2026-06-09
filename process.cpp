#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>

#include "telemetry.hpp"
#include "process_intelligence/process_database.hpp"

// ============================================================
// Legacy Global Process Map (for backward compatibility)
// Note: Use g_ProcessDB for new code
// ============================================================
ProcessMap g_ProcessMap;

// ============================================================
// Helper Functions (Legacy - for backward compatibility)
// ============================================================

std::wstring GetProcessArchitecture(HANDLE hProcess)
{
    ProcessInfo info;
    g_ProcessDB.GetProcessUserInfo(hProcess, info);
    return info.GetArchitectureString();
}

bool IsProcessElevated(HANDLE hProcess)
{
    ProcessInfo info;
    HANDLE hToken = NULL;
    
    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
    {
        return false;
    }
    
    TOKEN_ELEVATION elevation;
    DWORD size = sizeof(TOKEN_ELEVATION);
    BOOL success = GetTokenInformation(
        hToken,
        TokenElevation,
        &elevation,
        sizeof(elevation),
        &size);
    
    CloseHandle(hToken);
    return success && elevation.TokenIsElevated;
}

std::wstring GetProcessPath(HANDLE hProcess)
{
    WCHAR path[MAX_PATH];
    DWORD size = MAX_PATH;
    
    if (QueryFullProcessImageNameW(hProcess, 0, path, &size))
    {
        return path;
    }
    
    return L"Access Denied";
}

void CreateDumpDirectory()
{
    CreateDirectoryW(L"dumps", NULL);
    CreateDirectoryW(L"TelemetryExplorer/dumps", NULL);
}

// ============================================================
// Enumerate All Processes (Legacy - for backward compatibility)
// ============================================================
void EnumerateProcesses()
{
    // Now uses the new ProcessDatabase
    g_ProcessDB.Dump();
}

// ============================================================
// Initialize Process Database (Updated to use new ProcessDatabase)
// ============================================================
void InitializeProcessDatabase()
{
    // The global g_ProcessDB is already initialized in its constructor
    // But we'll call Initialize() to ensure it's populated
    g_ProcessDB.Initialize();
    
    // Copy to legacy g_ProcessMap for backward compatibility
    g_ProcessMap = g_ProcessDB.GetAllProcesses();
}

// ============================================================
// Additional Helper Functions
// ============================================================

// Get process information by PID using new database
bool GetProcessInfo(DWORD pid, ProcessInfo& info)
{
    return g_ProcessDB.GetProcess(pid, info);
}

// Get all processes
ProcessMap GetAllProcesses()
{
    return g_ProcessDB.GetAllProcesses();
}

// Refresh process database
void RefreshProcessDatabase()
{
    g_ProcessDB.Refresh();
    g_ProcessMap = g_ProcessDB.GetAllProcesses();
}

// Get process children
std::vector<ProcessInfo> GetProcessChildren(DWORD pid)
{
    return g_ProcessDB.GetChildren(pid);
}

// Get process parent
ProcessInfo GetProcessParent(DWORD pid)
{
    return g_ProcessDB.GetParent(pid);
}

// Get process ancestry
std::vector<ProcessInfo> GetProcessAncestry(DWORD pid)
{
    return g_ProcessDB.GetAncestry(pid);
}

// Get process descendants
std::vector<ProcessInfo> GetProcessDescendants(DWORD pid)
{
    return g_ProcessDB.GetDescendants(pid);
}

// Analyze process lineage
std::vector<LineageDetectionResult> AnalyzeProcessLineage(DWORD parentPid, DWORD childPid)
{
    return g_LineageDetector.AnalyzeLineage(parentPid, childPid);
}

// Analyze process lineage with ProcessInfo objects
std::vector<LineageDetectionResult> AnalyzeProcessLineage(const ProcessInfo& parent, const ProcessInfo& child)
{
    return g_LineageDetector.AnalyzeLineage(parent, child);
}

// Get session information
SessionInfo GetSessionInfo(DWORD sessionId)
{
    SessionInfo info;
    if (g_SessionTracker.GetSession(sessionId, info)) {
        return info;
    }
    return SessionInfo();
}

// Get session for process
SessionInfo GetSessionForProcess(DWORD pid)
{
    return g_SessionTracker.GetSessionForProcess(pid);
}
