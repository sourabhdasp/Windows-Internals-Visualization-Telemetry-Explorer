#include <windows.h>
#include <evntrace.h>
#include <tdh.h>
#include <psapi.h>
#include <tlhelp32.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <mutex>
#include <iomanip>

#include "telemetry.hpp"
#include "detection.hpp"
#include "gui.hpp"

#pragma comment(lib, "tdh.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Psapi.lib")

// --- JSON output helpers (used when g_emitJson is true) -----------------

static std::string WStringToUtf8(const std::wstring& w)
{
    if (w.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), NULL, 0, NULL, NULL);
    if (size_needed <= 0) return std::string();
    std::string out(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), &out[0], size_needed, NULL, NULL);
    return out;
}

static std::string JsonEscape(const std::string& s)
{
    std::string o;
    o.reserve(s.size());
    for (unsigned char c : s) {
        switch (c) {
        case '"': o += "\\\""; break;
        case '\\': o += "\\\\"; break;
        case '\b': o += "\\b"; break;
        case '\f': o += "\\f"; break;
        case '\n': o += "\\n"; break;
        case '\r': o += "\\r"; break;
        case '\t': o += "\\t"; break;
        default:
            if (c < 0x20) {
                char buf[8];
                sprintf_s(buf, "\\u%04x", c);
                o += buf;
            } else {
                o += c;
            }
        }
    }
    return o;
}

static std::wstring GetCurrentTimestampW()
{
    SYSTEMTIME st; GetLocalTime(&st);
    wchar_t buf[64];
    swprintf_s(buf, L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return std::wstring(buf);
}

static std::wstring EventTypeToWString(EventType t)
{
    switch (t) {
    case EventType::ProcessStart: return L"ProcessStart";
    case EventType::ProcessEnd:   return L"ProcessEnd";
    case EventType::ThreadStart:  return L"ThreadStart";
    case EventType::ThreadStop:   return L"ThreadStop";
    case EventType::MemoryAlert:  return L"MemoryAlert";
    case EventType::APCAlert:     return L"APCAlert";
    case EventType::RiskAlert:    return L"RiskAlert";
    default: return L"Unknown";
    }
}

// Augment LogDetection to emit JSON-lines if requested
void LogDetection(EventType type, const std::wstring& pid, const std::wstring& tid, const std::wstring& desc, const std::wstring& details)
{
    DetectionEvent ev;
    ev.type = type;
    ev.pid = pid;
    ev.tid = tid;
    ev.description = desc;
    ev.details = details;
    PushDetectionEvent(ev);

    // Emit JSON to stdout for external GUI consumers when enabled
    if (g_emitJson) {
        std::wstring tsW = GetCurrentTimestampW();
        std::wstring typeW = EventTypeToWString(type);

        std::string ts = JsonEscape(WStringToUtf8(tsW));
        std::string typeS = JsonEscape(WStringToUtf8(typeW));
        std::string pidS = JsonEscape(WStringToUtf8(pid));
        std::string tidS = JsonEscape(WStringToUtf8(tid));
        std::string descS = JsonEscape(WStringToUtf8(desc));
        std::string detS = JsonEscape(WStringToUtf8(details));

        std::ostringstream ss;
        ss << "{"
           << "\"timestamp\":\"" << ts << "\",";
        ss << "\"type\":\"" << typeS << "\",";
        ss << "\"pid\":\"" << pidS << "\",";
        ss << "\"tid\":\"" << tidS << "\",";
        ss << "\"description\":\"" << descS << "\",";
        ss << "\"details\":\"" << detS << "\"";
        ss << "}";

        std::cout << ss.str() << std::endl;
        std::cout.flush();
    }
}


// =====================================================
// SYSTEM TRACE GUID
// =====================================================

static const GUID SystemTraceControlGuid =
{
    0x9e814aad,
    0x3204,
    0x11d2,
    {0x9a,0x82,0x00,0x60,0x08,0xa8,0x69,0x39}
};

static const GUID ProcessProviderGuid = 
{ 0x22fb2cd6, 0x0e7b, 0x422b, { 0xa0, 0xc7, 0x2f, 0xad, 0x1f, 0xd0, 0xe7, 0x16 } };

static const GUID ThreadProviderGuid = 
{ 0xe13b14c5, 0x05e8, 0x466d, { 0xbb, 0x47, 0xed, 0x7d, 0x24, 0x72, 0xf2, 0x16 } };

// =====================================================
// GLOBAL TRACE HANDLES
// =====================================================

TRACEHANDLE g_Session = 0;

TRACEHANDLE g_Trace = 0;

// =====================================================
// PRINT UNICODE STRING
// =====================================================

void PrintUnicodeString(
    PBYTE data,
    ULONG size)
{
    if (!data || size == 0)
    {
        return;
    }

    std::wstring str(
        (PWCHAR)data,
        size / sizeof(WCHAR));

    std::wcout << str;
}

// =====================================================
// EXTRACT PROPERTY
// =====================================================

bool ExtractProperty(
    PEVENT_RECORD pEvent,
    LPCWSTR propertyName,
    std::wstring& output)
{
    PROPERTY_DATA_DESCRIPTOR desc;

    ZeroMemory(
        &desc,
        sizeof(desc));

    desc.PropertyName =
        (ULONGLONG)propertyName;

    desc.ArrayIndex =
        ULONG_MAX;

    DWORD propertySize = 0;

    ULONG status =
        TdhGetPropertySize(
            pEvent,
            0,
            NULL,
            1,
            &desc,
            &propertySize);

    if (status != ERROR_SUCCESS ||
        propertySize == 0)
    {
        return false;
    }

    std::vector<BYTE> propertyBuffer(
        propertySize);

    status =
        TdhGetProperty(
            pEvent,
            0,
            NULL,
            1,
            &desc,
            propertySize,
            propertyBuffer.data());

    if (status != ERROR_SUCCESS)
    {
        return false;
    }

    output.assign(
        (PWCHAR)propertyBuffer.data(),
        propertySize / sizeof(WCHAR));

    return true;
}

// =====================================================
// LOWERCASE STRING
// =====================================================

std::wstring ToLower(
    std::wstring str)
{
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        towlower);

    return str;
}

// =====================================================
// BASIC DETECTION ENGINE
// =====================================================

void AnalyzeCommandLine(
    const std::wstring& cmdLine)
{
    std::wstring lower =
        ToLower(cmdLine);

    if (lower.find(L"-enc")
        != std::wstring::npos)
    {
        std::wcout
            << L"[!!!] ENCODED POWERSHELL DETECTED"
            << std::endl;
    }

    if (lower.find(L"rundll32")
        != std::wstring::npos)
    {
        std::wcout
            << L"[!!!] RUNDLL32 EXECUTION DETECTED"
            << std::endl;
    }

    if (lower.find(L"mshta")
        != std::wstring::npos)
    {
        std::wcout
            << L"[!!!] MSHTA DETECTED"
            << std::endl;
    }

    if (lower.find(L"regsvr32")
        != std::wstring::npos)
    {
        std::wcout
            << L"[!!!] REGSVR32 DETECTED"
            << std::endl;
    }

    if (lower.find(L"powershell")
        != std::wstring::npos)
    {
        std::wcout
            << L"[+] PowerShell Execution"
            << std::endl;
    }
}

// =====================================================
// EXTRACT UINT32 PROPERTY
// =====================================================

bool ExtractUInt32Property(
    PEVENT_RECORD pEvent,
    LPCWSTR propertyName,
    DWORD& value)
{
    PROPERTY_DATA_DESCRIPTOR desc;

    ZeroMemory(
        &desc,
        sizeof(desc));

    desc.PropertyName =
        (ULONGLONG)propertyName;

    desc.ArrayIndex =
        ULONG_MAX;

    DWORD propertySize = 0;

    ULONG status =
        TdhGetPropertySize(
            pEvent,
            0,
            NULL,
            1,
            &desc,
            &propertySize);

    if (status != ERROR_SUCCESS ||
        propertySize != sizeof(DWORD))
    {
        return false;
    }

    status =
        TdhGetProperty(
            pEvent,
            0,
            NULL,
            1,
            &desc,
            sizeof(DWORD),
            (PBYTE)&value);

    return status == ERROR_SUCCESS;
}

// =====================================================
// EXTRACT POINTER PROPERTY
// =====================================================

bool ExtractPointerProperty(
    PEVENT_RECORD pEvent,
    LPCWSTR propertyName,
    uintptr_t& value)
{
    PROPERTY_DATA_DESCRIPTOR desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.PropertyName = (ULONGLONG)propertyName;
    desc.ArrayIndex = ULONG_MAX;

    DWORD propertySize = 0;
    ULONG status = TdhGetPropertySize(pEvent, 0, NULL, 1, &desc, &propertySize);
    if (status != ERROR_SUCCESS || propertySize == 0)
    {
        return false;
    }

    if (propertySize == sizeof(uintptr_t))
    {
        status = TdhGetProperty(pEvent, 0, NULL, 1, &desc, propertySize, (PBYTE)&value);
        return status == ERROR_SUCCESS;
    }
    else if (propertySize == sizeof(DWORD))
    {
        DWORD val32 = 0;
        status = TdhGetProperty(pEvent, 0, NULL, 1, &desc, sizeof(DWORD), (PBYTE)&val32);
        if (status == ERROR_SUCCESS)
        {
            value = val32;
            return true;
        }
    }
    else if (propertySize == sizeof(ULONGLONG))
    {
        ULONGLONG val64 = 0;
        status = TdhGetProperty(pEvent, 0, NULL, 1, &desc, sizeof(ULONGLONG), (PBYTE)&val64);
        if (status == ERROR_SUCCESS)
        {
            value = (uintptr_t)val64;
            return true;
        }
    }
    return false;
}

// =====================================================
// GET PROCESS INTEGRITY LEVEL
// =====================================================

std::wstring GetProcessIntegrityLevel(HANDLE hProcess)
{
    HANDLE hToken = NULL;
    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
    {
        return L"N/A (Access Denied)";
    }

    DWORD dwSize = 0;
    GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwSize);
    if (dwSize == 0)
    {
        CloseHandle(hToken);
        return L"N/A";
    }

    std::vector<BYTE> buffer(dwSize);
    PTOKEN_MANDATORY_LABEL pEntry = (PTOKEN_MANDATORY_LABEL)buffer.data();

    if (!GetTokenInformation(hToken, TokenIntegrityLevel, pEntry, dwSize, &dwSize))
    {
        CloseHandle(hToken);
        return L"N/A";
    }

    CloseHandle(hToken);

    DWORD subAuthority = *GetSidSubAuthority(
        pEntry->Label.Sid,
        (DWORD)(*GetSidSubAuthorityCount(pEntry->Label.Sid) - 1));

    if (subAuthority >= SECURITY_MANDATORY_SYSTEM_RID)
    {
        return L"System";
    }
    else if (subAuthority >= SECURITY_MANDATORY_HIGH_RID)
    {
        return L"High";
    }
    else if (subAuthority >= SECURITY_MANDATORY_MEDIUM_RID)
    {
        return L"Medium";
    }
    else if (subAuthority >= SECURITY_MANDATORY_LOW_RID)
    {
        return L"Low";
    }
    else
    {
        return L"Untrusted";
    }
}

// =====================================================
// FORMAT LARGE_INTEGER TIMESTAMP
// =====================================================

std::wstring FormatTimestamp(LARGE_INTEGER timestamp)
{
    FILETIME ft;
    ft.dwLowDateTime = timestamp.LowPart;
    ft.dwHighDateTime = timestamp.HighPart;

    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    WCHAR buf[64];
    swprintf_s(
        buf,
        L"%04d-%02d-%02d %02d:%02d:%02d.%03d",
        stLocal.wYear,
        stLocal.wMonth,
        stLocal.wDay,
        stLocal.wHour,
        stLocal.wMinute,
        stLocal.wSecond,
        stLocal.wMilliseconds);

    return buf;
}

// =====================================================
// ANALYZE PROCESS LINEAGE
// =====================================================

void AnalyzeProcessLineage(
    const ProcessInfo& parent,
    const ProcessInfo& child)
{
    std::wstring pName = ToLower(parent.imageName);
    std::wstring cName = ToLower(child.imageName);
    std::wstring cCmd = ToLower(child.commandLine);

    // Rule 1: Office spawning PowerShell or Scripting Engines
    if (pName.find(L"winword.exe") != std::wstring::npos ||
        pName.find(L"excel.exe") != std::wstring::npos ||
        pName.find(L"powerpnt.exe") != std::wstring::npos)
    {
        if (cName.find(L"powershell.exe") != std::wstring::npos ||
            cName.find(L"cmd.exe") != std::wstring::npos ||
            cName.find(L"wscript.exe") != std::wstring::npos ||
            cName.find(L"cscript.exe") != std::wstring::npos ||
            cName.find(L"mshta.exe") != std::wstring::npos ||
            cName.find(L"rundll32.exe") != std::wstring::npos)
        {
            std::wcout
                << L"\n[!!!] ALERT: OFFICE APPLICATION SPAWNED SCRIPT/SHELL HOST"
                << L"\n      Parent : " << parent.imageName
                << L"\n      Child  : " << child.imageName
                << L"\n      CmdLine: " << child.commandLine
                << std::endl;
        }
    }

    // Rule 2: Browser spawning Command Interpreter
    if (pName.find(L"chrome.exe") != std::wstring::npos ||
        pName.find(L"msedge.exe") != std::wstring::npos ||
        pName.find(L"firefox.exe") != std::wstring::npos ||
        pName.find(L"iexplore.exe") != std::wstring::npos)
    {
        if (cName.find(L"cmd.exe") != std::wstring::npos ||
            cName.find(L"powershell.exe") != std::wstring::npos)
        {
            std::wcout
                << L"\n[!!!] ALERT: WEB BROWSER SPAWNED COMMAND INTERPRETER"
                << L"\n      Parent : " << parent.imageName
                << L"\n      Child  : " << child.imageName
                << L"\n      CmdLine: " << child.commandLine
                << std::endl;
        }
    }

    // Rule 3: LOLBins spawning LOLBins
    bool isParentLolbin = false;
    if (pName.find(L"powershell.exe") != std::wstring::npos ||
        pName.find(L"cmd.exe") != std::wstring::npos ||
        pName.find(L"rundll32.exe") != std::wstring::npos ||
        pName.find(L"mshta.exe") != std::wstring::npos ||
        pName.find(L"regsvr32.exe") != std::wstring::npos ||
        pName.find(L"certutil.exe") != std::wstring::npos)
    {
        isParentLolbin = true;
    }

    bool isChildLolbin = false;
    if (cName.find(L"powershell.exe") != std::wstring::npos ||
        cName.find(L"cmd.exe") != std::wstring::npos ||
        cName.find(L"rundll32.exe") != std::wstring::npos ||
        cName.find(L"mshta.exe") != std::wstring::npos ||
        cName.find(L"regsvr32.exe") != std::wstring::npos ||
        cName.find(L"certutil.exe") != std::wstring::npos)
    {
        isChildLolbin = true;
    }

    if (isParentLolbin && isChildLolbin)
    {
        std::wcout
            << L"\n[!] WARNING: LOLBIN SPAWNED ANOTHER LOLBIN (POTENTIAL CHAIN)"
            << L"\n    Parent : " << parent.imageName
            << L"\n    Child  : " << child.imageName
            << L"\n    CmdLine: " << child.commandLine
            << std::endl;
    }

    // Rule 4: Explorer anomalies (explorer.exe spawned by unusual parent)
    if (cName == L"explorer.exe")
    {
        if (pName != L"userinit.exe")
        {
            std::wcout
                << L"\n[!!!] ALERT: EXPLORER.EXE SPAWNED BY UNUSUAL PARENT"
                << L"\n      Parent : " << parent.imageName
                << L"\n      Child  : " << child.imageName
                << std::endl;
        }
    }
}

// =====================================================
// GET BACKING MODULE QUIET
// =====================================================

std::wstring GetBackingModuleQuiet(HANDLE hProcess, uintptr_t address)
{
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            MODULEINFO modInfo;
            if (GetModuleInformation(hProcess, hMods[i], &modInfo, sizeof(modInfo)))
            {
                uintptr_t start = (uintptr_t)modInfo.lpBaseOfDll;
                uintptr_t end = start + modInfo.SizeOfImage;
                if (address >= start && address < end)
                {
                    WCHAR moduleName[MAX_PATH];
                    if (GetModuleBaseNameW(hProcess, hMods[i], moduleName, MAX_PATH))
                    {
                        return moduleName;
                    }
                    return L"Unknown Module";
                }
            }
        }
    }
    return L"";
}

// =====================================================
// ETW EVENT CALLBACK
// =====================================================

VOID WINAPI EventCallback(
    PEVENT_RECORD pEvent)
{
    DWORD eventId =
        pEvent->EventHeader
        .EventDescriptor.Id;

    if (IsEqualGUID(pEvent->EventHeader.ProviderId, ProcessProviderGuid))
    {
        // =================================================
        // PROCESS START
        // =================================================
        if (eventId == 1)
        {
            std::wcout
                << L"\n===================================================="
                << std::endl;

            std::wcout
                << L"[PROCESS START]"
                << std::endl;

            std::wcout
                << L"PID : "
                << pEvent->EventHeader.ProcessId
                << std::endl;

            std::wcout
                << L"TID : "
                << pEvent->EventHeader.ThreadId
                << std::endl;

            // =============================================
            // GET EVENT METADATA
            // =============================================

            DWORD bufferSize = 0;
            TdhGetEventInformation(pEvent, 0, NULL, NULL, &bufferSize);
            if (bufferSize == 0)
            {
                return;
            }

            std::vector<BYTE> buffer(bufferSize);
            PTRACE_EVENT_INFO eventInfo = (PTRACE_EVENT_INFO)buffer.data();

            if (TdhGetEventInformation(pEvent, 0, NULL, eventInfo, &bufferSize) != ERROR_SUCCESS)
            {
                return;
            }

            // =============================================
            // ENUMERATE PROPERTIES
            // =============================================

            std::wcout
                << L"\n[ETW PROPERTIES]"
                << std::endl;

            for (ULONG i = 0; i < eventInfo->TopLevelPropertyCount; i++)
            {
                PWSTR propertyName = (PWSTR)((PBYTE)eventInfo + eventInfo->EventPropertyInfoArray[i].NameOffset);
                std::wcout << L"Property : " << propertyName << std::endl;
            }

            // =============================================
            // IMAGE NAME
            // =============================================

            std::wstring imageName;
            if (ExtractProperty(pEvent, L"ImageFileName", imageName))
            {
                std::wcout
                    << L"\nImage Name : "
                    << imageName
                    << std::endl;
            }

            // =============================================
            // COMMAND LINE
            // =============================================

            std::wstring commandLine;
            if (ExtractProperty(pEvent, L"CommandLine", commandLine))
            {
                std::wcout
                    << L"CommandLine : "
                    << commandLine
                    << std::endl;

                AnalyzeCommandLine(commandLine);
            }

            // =============================================
            // PROCESS STRUCT & DATA
            // =============================================

            ProcessInfo proc;
            proc.pid = pEvent->EventHeader.ProcessId;
            proc.imageName = imageName;
            proc.commandLine = commandLine;

            // Retrieve StartTime from Event Header Timestamp
            proc.startTime = FormatTimestamp(pEvent->EventHeader.TimeStamp);

            // Open live process handle to retrieve Integrity level
            proc.integrity = L"N/A";
            HANDLE hProcTmp = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, proc.pid);
            if (hProcTmp)
            {
                proc.integrity = GetProcessIntegrityLevel(hProcTmp);
                CloseHandle(hProcTmp);
            }

            // Retrieve Session ID
            proc.sessionId = 0;
            if (!ProcessIdToSessionId(proc.pid, &proc.sessionId))
            {
                ExtractUInt32Property(pEvent, L"SessionId", proc.sessionId);
            }

            // =============================================
            // STORE REAL PARENT PID
            // =============================================

            DWORD parentPid = 0;
            if (ExtractUInt32Property(pEvent, L"ParentProcessId", parentPid))
            {
                proc.parentPid = parentPid;
                std::wcout
                    << L"Parent PID : "
                    << parentPid
                    << std::endl;
            }
            else
            {
                proc.parentPid = 0;
            }

            // Print Session Correlation & Integrity Telemetry
            std::wcout
                << L"Session ID : " << proc.sessionId
                << L" (" << (proc.sessionId == 0 ? L"Service Session" : L"Interactive Session") << L")"
                << std::endl;

            std::wcout
                << L"Integrity  : " << proc.integrity
                << std::endl;

            std::wcout
                << L"Start Time : " << proc.startTime
                << std::endl;

            // =============================================
            // SAVE PROCESS INTO DATABASE
            // =============================================

            g_ProcessMap[proc.pid] = proc;

            // =============================================
            // BUILD LIVE PROCESS TREE & LINEAGE
            // =============================================

            auto parentIt = g_ProcessMap.find(proc.parentPid);
            if (parentIt != g_ProcessMap.end())
            {
                std::wcout
                    << L"\n[PROCESS LINEAGE]"
                    << std::endl;

                std::wcout
                    << L"Parent : "
                    << parentIt->second.imageName
                    << std::endl;

                std::wcout
                    << L"Child  : "
                    << proc.imageName
                    << std::endl;

                // Perform behavioral lineage detection
                AnalyzeProcessLineage(parentIt->second, proc);
            }

            // =============================================
            // LIVE PROCESS INSPECTION
            // =============================================

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pEvent->EventHeader.ProcessId);
            if (hProcess)
            {
                std::wcout
                    << L"\n[+] Performing Live Telemetry Inspection..."
                    << std::endl;

                EnumerateModules(hProcess);
                EnumerateMemory(hProcess);
                GetRemotePEB(hProcess);
                EnumerateThreads(pEvent->EventHeader.ProcessId);
                CloseHandle(hProcess);
            }
            else
            {
                std::wcout
                    << L"[!] Failed To Open Process"
                    << std::endl;
            }
        }
        // =================================================
        // PROCESS END
        // =================================================
        else if (eventId == 2)
        {
            std::wcout
                << L"\n===================================================="
                << std::endl;

            std::wcout
                << L"[PROCESS END]"
                << std::endl;

            std::wcout
                << L"PID : "
                << pEvent->EventHeader.ProcessId
                << std::endl;
        }
    }
    else if (IsEqualGUID(pEvent->EventHeader.ProviderId, ThreadProviderGuid))
    {
        // =================================================
        // THREAD START
        // =================================================
        if (eventId == 1)
        {
            uintptr_t startAddr = 0;
            if (ExtractPointerProperty(pEvent, L"Win32StartAddr", startAddr))
            {
                std::wcout
                    << L"\n===================================================="
                    << L"\n[THREAD START]"
                    << L"\nPID          : " << pEvent->EventHeader.ProcessId
                    << L"\nTID          : " << pEvent->EventHeader.ThreadId
                    << L"\nStart Address: 0x" << std::hex << startAddr << std::dec
                    << std::endl;

                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pEvent->EventHeader.ProcessId);
                if (hProcess)
                {
                    MEMORY_BASIC_INFORMATION mbi;
                    if (VirtualQueryEx(hProcess, (LPCVOID)startAddr, &mbi, sizeof(mbi)))
                    {
                        std::wstring memTypeStr = L"MEM_UNKNOWN";
                        if (mbi.Type == MEM_IMAGE) memTypeStr = L"MEM_IMAGE";
                        else if (mbi.Type == MEM_PRIVATE) memTypeStr = L"MEM_PRIVATE";
                        else if (mbi.Type == MEM_MAPPED) memTypeStr = L"MEM_MAPPED";

                        std::wstring protectionStr = L"UNKNOWN";
                        if (mbi.Protect & PAGE_EXECUTE) protectionStr = L"EXECUTE";
                        else if (mbi.Protect & PAGE_EXECUTE_READ) protectionStr = L"EXECUTE_READ";
                        else if (mbi.Protect & PAGE_EXECUTE_READWRITE) protectionStr = L"EXECUTE_READWRITE";
                        else if (mbi.Protect & PAGE_EXECUTE_WRITECOPY) protectionStr = L"EXECUTE_WRITECOPY";

                        std::wcout
                            << L"Memory Type  : " << memTypeStr
                            << L"\nProtection   : " << protectionStr
                            << std::endl;

                        std::wstring backingModule = GetBackingModuleQuiet(hProcess, startAddr);
                        if (!backingModule.empty())
                        {
                            std::wcout << L"Backing DLL  : " << backingModule << std::endl;
                        }
                        else
                        {
                            std::wcout << L"Backing DLL  : None (Not backed by module)" << std::endl;

            // Build detection event for suspicious private memory execution, include backing module if available
            DetectionEvent ev{};
            ev.type = EventType::MemoryAlert;
            ev.pid = std::to_wstring(pEvent->EventHeader.ProcessId);
            ev.tid = std::to_wstring(pEvent->EventHeader.ThreadId);
            ev.description = L"Suspicious Private Memory Execution";
            wchar_t details[512];
            std::wstring moduleInfo = backingModule.empty() ? L"None" : backingModule;
            swprintf_s(details, L"StartAddr=0x%llX; PID=%llu; TID=%llu; Module=%s", startAddr, (unsigned long long)pEvent->EventHeader.ProcessId, (unsigned long long)pEvent->EventHeader.ThreadId, moduleInfo.c_str());
            ev.details = details;
            PushDetectionEvent(ev);
                        }
                    }
                    CloseHandle(hProcess);
                }
            }
        }
        // =================================================
        // THREAD STOP
        // =================================================
        else if (eventId == 2)
        {
            std::wcout
                << L"\n===================================================="
                << L"\n[THREAD STOP]"
                << L"\nPID : " << pEvent->EventHeader.ProcessId
                << L"\nTID : " << pEvent->EventHeader.ThreadId
                << std::endl;
        }
        // =================================================
        // QUEUE APC (APC INJECTION DETECTION)
        // =================================================
        else if (eventId == 12)
        {
            uintptr_t apcRoutine = 0;
            if (ExtractPointerProperty(pEvent, L"ApcRoutine", apcRoutine))
            {
                std::wcout
                    << L"\n===================================================="
                    << L"\n[APC QUEUED]"
                    << L"\nPID         : " << pEvent->EventHeader.ProcessId
                    << L"\nTarget TID  : " << pEvent->EventHeader.ThreadId
                    << L"\nApcRoutine  : 0x" << std::hex << apcRoutine << std::dec
                    << std::endl;

                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pEvent->EventHeader.ProcessId);
                if (hProcess)
                {
                    MEMORY_BASIC_INFORMATION mbi;
                    if (VirtualQueryEx(hProcess, (LPCVOID)apcRoutine, &mbi, sizeof(mbi)))
                    {
                        std::wstring backingModule = GetBackingModuleQuiet(hProcess, apcRoutine);
                        // Build detection event for unbacked APC injection
                        {
                            std::wstring details;
                            wchar_t buf[256];
                            swprintf_s(buf, L"APC Routine=0x%llX; ProcessPID=%llu; ThreadTID=%llu; MemoryType=MEM_PRIVATE (Executable)", (unsigned long long)apcRoutine, (unsigned long long)pEvent->EventHeader.ProcessId, (unsigned long long)pEvent->EventHeader.ThreadId);
                            details = buf;
                            LogDetection(EventType::APCAlert,
                                         std::to_wstring(pEvent->EventHeader.ProcessId),
                                         std::to_wstring(pEvent->EventHeader.ThreadId),
                                         L"Suspicious APC Injection",
                                         details);
                        }
                    }
                    CloseHandle(hProcess);
                }
            }
        }
    }
}

void StartETW()
{
    // =================================================
    // ADMIN CHECK
    // =================================================

    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(
            &ntAuthority, 2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &adminGroup))
    {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    if (!isAdmin)
    {
        std::wcout
            << L"[!!!] WARNING: TelemetryExplorer requires ADMINISTRATOR privileges."
            << L"\n      Run this executable as Administrator."
            << std::endl;
        return;
    }

    // =================================================
    // STOP ANY STALE SESSION FIRST
    // =================================================

    {
        EVENT_TRACE_PROPERTIES* staleProps =
            (EVENT_TRACE_PROPERTIES*)
            calloc(1, sizeof(EVENT_TRACE_PROPERTIES) + sizeof(WCHAR) * 1024);

        if (staleProps)
        {
            staleProps->Wnode.BufferSize =
                sizeof(EVENT_TRACE_PROPERTIES) + sizeof(WCHAR) * 1024;
            staleProps->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);

            ULONG stopStatus = ControlTrace(
                0,
                L"MyTelemetrySession",
                staleProps,
                EVENT_TRACE_CONTROL_STOP);

            if (stopStatus == ERROR_SUCCESS)
            {
                std::wcout
                    << L"[+] Stopped stale ETW session."
                    << std::endl;
            }

            free(staleProps);
        }
    }

    // =================================================
    // ALLOCATE SESSION PROPERTIES
    // =================================================

    EVENT_TRACE_PROPERTIES* props =
        (EVENT_TRACE_PROPERTIES*)
        calloc(
            1,
            sizeof(EVENT_TRACE_PROPERTIES)
            + sizeof(WCHAR) * 1024);

    if (!props)
    {
        std::wcout
            << L"[!] Memory Allocation Failed"
            << std::endl;

        return;
    }

    props->Wnode.BufferSize =
        sizeof(EVENT_TRACE_PROPERTIES)
        + sizeof(WCHAR) * 1024;

    props->Wnode.Flags =
        WNODE_FLAG_TRACED_GUID;

    props->LogFileMode =
        EVENT_TRACE_REAL_TIME_MODE;

    props->LoggerNameOffset =
        sizeof(EVENT_TRACE_PROPERTIES);

    // =================================================
    // START SESSION
    // =================================================

    ULONG status =
        StartTrace(
            &g_Session,
            L"MyTelemetrySession",
            props);

    if (status != ERROR_SUCCESS)
    {
        if (status == ERROR_ACCESS_DENIED)
        {
            std::wcout
                << L"[!!!] StartTrace Failed: ACCESS DENIED."
                << L"\n      Run this tool as Administrator."
                << std::endl;
        }
        else
        {
            std::wcout
                << L"[!] StartTrace Failed : "
                << status
                << std::endl;
        }

        free(props);

        return;
    }

    std::wcout
        << L"[+] ETW Session Started"
        << std::endl;

    // =================================================
    // ENABLE KERNEL PROCESS & THREAD PROVIDERS
    // =================================================

    status =
        EnableTraceEx2(
            g_Session,
            &ProcessProviderGuid,
            EVENT_CONTROL_CODE_ENABLE_PROVIDER,
            TRACE_LEVEL_VERBOSE,
            0,
            0,
            0,
            NULL);

    if (status != ERROR_SUCCESS)
    {
        std::wcout
            << L"[!] EnableTraceEx2 (Process) Failed : "
            << status
            << std::endl;
    }
    else
    {
        std::wcout
            << L"[+] Kernel Process Provider Enabled"
            << std::endl;
    }

    status =
        EnableTraceEx2(
            g_Session,
            &ThreadProviderGuid,
            EVENT_CONTROL_CODE_ENABLE_PROVIDER,
            TRACE_LEVEL_VERBOSE,
            0,
            0,
            0,
            NULL);

    if (status != ERROR_SUCCESS)
    {
        std::wcout
            << L"[!] EnableTraceEx2 (Thread) Failed : "
            << status
            << std::endl;
    }
    else
    {
        std::wcout
            << L"[+] Kernel Thread Provider Enabled"
            << std::endl;
    }

    // =================================================
    // OPEN TRACE
    // =================================================

    EVENT_TRACE_LOGFILE trace;

    ZeroMemory(
        &trace,
        sizeof(trace));

    trace.LoggerName =
        (LPWSTR)L"MyTelemetrySession";

    trace.ProcessTraceMode =
        PROCESS_TRACE_MODE_REAL_TIME
        |
        PROCESS_TRACE_MODE_EVENT_RECORD;

    trace.EventRecordCallback =
        EventCallback;

    g_Trace =
        OpenTrace(&trace);

    if (g_Trace ==
        INVALID_PROCESSTRACE_HANDLE)
    {
        std::wcout
            << L"[!] OpenTrace Failed"
            << std::endl;

        free(props);

        return;
    }

    std::wcout
        << L"[+] Waiting For ETW Events..."
        << std::endl;

    // =================================================
    // START PROCESSING EVENTS
    // =================================================

    ProcessTrace(
        &g_Trace,
        1,
        0,
        0);

    // =================================================
    // CLEANUP
    // =================================================

    CloseTrace(g_Trace);

    ControlTrace(
        g_Session,
        L"MyTelemetrySession",
        props,
        EVENT_TRACE_CONTROL_STOP);

    free(props);
}
