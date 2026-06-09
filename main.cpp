#include "telemetry.hpp"
#include "gui.hpp"
#include <windows.h>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

// define the JSON flag declared in telemetry.hpp
bool g_emitJson = false;

// ============================================================
// Command Line Options
// ============================================================
void PrintUsage() {
    std::wcout << L"\nTelemetryExplorer - Advanced Process Telemetry Tool" << std::endl;
    std::wcout << L"Usage: TelemetryExplorer [options]" << std::endl;
    std::wcout << L"\nOptions:" << std::endl;
    std::wcout << L"  --json           Run in JSON output mode (for external consumers)" << std::endl;
    std::wcout << L"  --list-processes List all current processes and exit" << std::endl;
    std::wcout << L"  --list-sessions  List all active sessions and exit" << std::endl;
    std::wcout << L"  --list-threads   List all current threads and exit" << std::endl;
    std::wcout << L"  --thread-dump    Dump thread database and exit" << std::endl;
    std::wcout << L"  --cmdlines       List all command lines and exit" << std::endl;
    std::wcout << L"  --cmdline-dump   Dump command line database and exit" << std::endl;
    std::wcout << L"  --injections     List all injection events and exit" << std::endl;
    std::wcout << L"  --injection-dump Dump injection database and exit" << std::endl;
    std::wcout << L"  --scan-inject    Scan all processes for injection and exit" << std::endl;
    std::wcout << L"  --dump           Dump process database and exit" << std::endl;
    std::wcout << L"  --tree           Show process tree and exit" << std::endl;
    std::wcout << L"  --syscalls       List all syscall events and exit" << std::endl;
    std::wcout << L"  --syscall-dump   Dump syscall database and exit" << std::endl;
    std::wcout << L"  --scan-hooks     Scan for hooked ntdll syscalls and exit" << std::endl;
    std::wcout << L"  --kernel         List all kernel events and exit" << std::endl;
    std::wcout << L"  --kernel-dump    Dump full kernel database with statistics" << std::endl;
    std::wcout << L"  --scan-kernel    Scan for kernel-level threats and exit" << std::endl;
    std::wcout << L"  --detections     List all detections and exit" << std::endl;
    std::wcout << L"  --detection-dump Dump full detection database with statistics" << std::endl;
    std::wcout << L"  --scan-detect    Scan for threats using detection engine and exit" << std::endl;
    std::wcout << L"  --correlations   List all correlated events and exit" << std::endl;
    std::wcout << L"  --correlation-dump Dump correlation statistics" << std::endl;
    std::wcout << L"  --graphs         List all behavioral graphs and exit" << std::endl;
    std::wcout << L"  --graph-dump     Dump graph statistics" << std::endl;
    std::wcout << L"  --help, -h, -?    Show this help message" << std::endl;
    std::wcout << L"\nExamples:" << std::endl;
    std::wcout << L"  TelemetryExplorer --json | python analyzer.py" << std::endl;
    std::wcout << L"  TelemetryExplorer --list-processes" << std::endl;
    std::wcout << L"  TelemetryExplorer --list-threads" << std::endl;
    std::wcout << L"  TelemetryExplorer --cmdlines" << std::endl;
    std::wcout << L"  TelemetryExplorer --injections" << std::endl;
    std::wcout << L"  TelemetryExplorer --scan-inject" << std::endl;
    std::wcout << L"  TelemetryExplorer --tree" << std::endl;
    std::wcout << L"  TelemetryExplorer --syscalls" << std::endl;
    std::wcout << L"  TelemetryExplorer --scan-hooks" << std::endl;
    std::wcout << L"  TelemetryExplorer --kernel" << std::endl;
    std::wcout << L"  TelemetryExplorer --kernel-dump" << std::endl;
    std::wcout << L"  TelemetryExplorer --scan-kernel" << std::endl;
    std::wcout << L"  TelemetryExplorer --detections" << std::endl;
    std::wcout << L"  TelemetryExplorer --detection-dump" << std::endl;
    std::wcout << L"  TelemetryExplorer --scan-detect" << std::endl;
    std::wcout << L"  TelemetryExplorer --correlations" << std::endl;
    std::wcout << L"  TelemetryExplorer --correlation-dump" << std::endl;
    std::wcout << L"  TelemetryExplorer --graphs" << std::endl;
    std::wcout << L"  TelemetryExplorer --graph-dump" << std::endl;
}

int main(int argc, char* argv[])
{
    // Parse command line arguments
    bool showHelp = false;
    bool listProcesses = false;
    bool listSessions = false;
    bool dumpDatabase = false;
    bool showTree = false;
    bool listThreads = false;
    bool threadDump = false;
    bool listCmdLines = false;
    bool cmdLineDump = false;
    bool listInjections = false;
    bool injectionDump = false;
    bool scanInjection = false;
    bool listSyscalls = false;
    bool syscallDump = false;
    bool scanHooks = false;
    bool listKernelEvents = false;
    bool kernelDump = false;
    bool scanKernel = false;
    bool listDetections = false;
    bool detectionDump = false;
    bool scanDetect = false;
    bool listCorrelations = false;
    bool correlationDump = false;
    bool listGraphs = false;
    bool graphDump = false;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--json") {
            g_emitJson = true;
        } else if (arg == "--list-processes") {
            listProcesses = true;
        } else if (arg == "--list-sessions") {
            listSessions = true;
        } else if (arg == "--list-threads") {
            listThreads = true;
        } else if (arg == "--thread-dump") {
            threadDump = true;
        } else if (arg == "--cmdlines") {
            listCmdLines = true;
        } else if (arg == "--cmdline-dump") {
            cmdLineDump = true;
        } else if (arg == "--injections") {
            listInjections = true;
        } else if (arg == "--injection-dump") {
            injectionDump = true;
        } else if (arg == "--scan-inject") {
            scanInjection = true;
        } else if (arg == "--dump") {
            dumpDatabase = true;
        } else if (arg == "--tree") {
            showTree = true;
        } else if (arg == "--syscalls") {
            listSyscalls = true;
        } else if (arg == "--syscall-dump") {
            syscallDump = true;
        } else if (arg == "--scan-hooks") {
            scanHooks = true;
        } else if (arg == "--kernel") {
            listKernelEvents = true;
        } else if (arg == "--kernel-dump") {
            kernelDump = true;
        } else if (arg == "--scan-kernel") {
            scanKernel = true;
        } else if (arg == "--detections") {
            listDetections = true;
        } else if (arg == "--detection-dump") {
            detectionDump = true;
        } else if (arg == "--scan-detect") {
            scanDetect = true;
        } else if (arg == "--correlations") {
            listCorrelations = true;
        } else if (arg == "--correlation-dump") {
            correlationDump = true;
        } else if (arg == "--graphs") {
            listGraphs = true;
        } else if (arg == "--graph-dump") {
            graphDump = true;
        } else if (arg == "--help" || arg == "-h" || arg == "-?") {
            showHelp = true;
        }
    }

    // Show help and exit
    if (showHelp) {
        PrintUsage();
        return 0;
    }

    // Initialize dump directory
    CreateDumpDirectory();

    // Initialize Process Intelligence Engine
    std::wcout << L"[+] Initializing Process Intelligence Engine..." << std::endl;
    
    // Initialize process database (this also initializes session tracker)
    g_ProcessDB.Initialize();
    
    // Initialize session tracker
    g_SessionTracker.Initialize();
    
    std::wcout << L"[+] Process Database: " << g_ProcessDB.Count() << L" processes loaded" << std::endl;
    std::wcout << L"[+] Sessions: " << g_SessionTracker.GetSessionCount() << L" active" << std::endl;

    // Initialize Thread Intelligence Engine
    std::wcout << L"[+] Initializing Thread Intelligence Engine..." << std::endl;
    
    // Initialize thread database
    g_ThreadDB.Initialize();
    
    // Initialize thread monitor
    g_ThreadMonitor.Initialize();
    
    std::wcout << L"[+] Thread Database: " << g_ThreadDB.Count() << L" threads loaded" << std::endl;

    // Initialize Command Line Intelligence Engine
    std::wcout << L"[+] Initializing Command Line Intelligence Engine..." << std::endl;
    
    // Initialize command line database
    g_CommandLineDB.Clear();
    
    // Initialize command line analyzer
    g_CommandLineAnalyzer.ResetStatistics();
    
    // Analyze existing process command lines
    g_CommandLineMonitor.Initialize();
    
    std::wcout << L"[+] Command Line Database: " << g_CommandLineDB.Count() << L" entries loaded" << std::endl;

    // Initialize Injection Intelligence Engine
    std::wcout << L"[+] Initializing Injection Intelligence Engine..." << std::endl;
    
    // Initialize injection database
    g_InjectionDB.Clear();
    
    // Initialize injection detector
    g_InjectionDetector.Initialize();
    
    // Initialize injection monitor
    g_InjectionMonitor.Initialize();
    
    std::wcout << L"[+] Injection Database: " << g_InjectionDB.Count() << L" events loaded" << std::endl;

    // Initialize Syscall Intelligence Engine (Phase 7)
    std::wcout << L"[+] Initializing Syscall Intelligence Engine..." << std::endl;
    
    // Initialize syscall database
    g_SyscallDB.Initialize();
    
    // Initialize syscall monitor
    InitializeSyscallMonitor();
    
    // Initialize syscall detector
    InitializeSyscallDetector();
    
    std::wcout << L"[+] Syscall Database: " << g_SyscallDB.Count() << L" events loaded" << std::endl;

    // Initialize Kernel Intelligence Engine
    std::wcout << L"[+] Initializing Kernel Intelligence Engine..." << std::endl;
    
    // Initialize kernel database
    g_KernelDB.Initialize();
    
    // Initialize kernel monitor
    InitializeKernelMonitor();
    
    // Initialize kernel detector
    InitializeKernelDetector();
    
    std::wcout << L"[+] Kernel Database: " << g_KernelDB.Count() << L" events loaded" << std::endl;

    // Initialize Detection Intelligence Engine
    std::wcout << L"[+] Initializing Detection Intelligence Engine..." << std::endl;
    
    // Initialize detection database
    g_DetectionDB.Initialize();
    
    // Initialize detection engine
    StartDetectionEngine();
    
    // Initialize event correlator
    InitializeDetectionCorrelator();
    
    // Initialize graph builder
    InitializeGraphBuilder();
    
    std::wcout << L"[+] Detection Database: " << g_DetectionDB.GetDetectionResultCount() << L" detections loaded" << std::endl;

    // Initialize Memory Intelligence Engine
    std::wcout << L"[+] Initializing Memory Intelligence Engine..." << std::endl;
    
    // Initialize memory database
    g_MemoryDB.Initialize();
    
    // Initialize memory monitor
    g_MemoryMonitor.Initialize();
    
    std::wcout << L"[+] Memory Database: " << g_MemoryDB.Count() << L" regions loaded" << std::endl;

    // Handle command-line mode operations
    if (listProcesses) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"CURRENT PROCESSES (" << g_ProcessDB.Count() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto processes = g_ProcessDB.GetAllProcesses();
        for (const auto& [pid, info] : processes) {
            std::wcout << L"[" << std::setw(6) << pid << L"] " 
                      << std::setw(25) << std::left << info.imageName 
                      << L"  Session: " << std::setw(3) << info.sessionId 
                      << L"  User: " << std::setw(20) << info.GetFullUserName()
                      << L"  Integrity: " << std::setw(12) << info.GetIntegrityString()
                      << L"  Elevated: " << (info.isElevated ? L"YES" : L"NO")
                      << std::endl;
        }
        return 0;
    }
    
    if (listSessions) {
        std::wcout << L"\n" << g_SessionTracker.GetSessionListAsString();
        return 0;
    }
    
    if (listThreads) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"CURRENT THREADS (" << g_ThreadDB.Count() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto allThreads = g_ThreadDB.GetAllThreads();
        for (const auto& [pid, threadMap] : allThreads) {
            ProcessInfo procInfo;
            if (g_ProcessDB.GetProcess(pid, procInfo)) {
                std::wcout << L"\nProcess: [" << pid << L"] " << procInfo.imageName << std::endl;
            } else {
                std::wcout << L"\nProcess: [" << pid << L"] (unknown)" << std::endl;
            }
            
            for (const auto& [tid, threadInfo] : threadMap) {
                std::wcout << L"  [" << std::setw(6) << tid << L"] "
                          << L"StartAddr: 0x" << std::hex << std::setw(16) << std::setfill(L'0') << threadInfo.startAddress << std::dec
                          << L"  State: " << std::setw(12) << threadInfo.GetStateString()
                          << L"  Risk: " << std::setw(3) << threadInfo.riskScore
                          << (threadInfo.HasRisk() ? L" [RISKY]" : L"")
                          << std::endl;
            }
        }
        
        // Show suspicious threads
        auto suspiciousThreads = g_ThreadDB.GetSuspiciousThreads();
        if (!suspiciousThreads.empty()) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"SUSPICIOUS THREADS (" << suspiciousThreads.size() << L"):" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
            for (const auto& thread : suspiciousThreads) {
                std::wcout << L"  [PID:" << thread.pid << L" TID:" << thread.tid << L"] "
                          << L"Risk: " << thread.riskScore
                          << L"  Flags: " << thread.GetRiskString()
                          << L"  StartAddr: 0x" << std::hex << thread.startAddress << std::dec
                          << std::endl;
            }
        }
        return 0;
    }
    
    if (threadDump) {
        std::wcout << L"\n" << g_ThreadDB.GetSuspiciousThreadsAsString() << std::endl;
        return 0;
    }
    
    if (listCmdLines) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"COMMAND LINES (" << g_CommandLineDB.Count() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto allCmdLines = g_CommandLineDB.GetAll();
        int suspiciousCount = 0;
        
        for (const auto& [pid, cmdLineInfo] : allCmdLines) {
            ProcessInfo procInfo;
            if (g_ProcessDB.GetProcess(pid, procInfo)) {
                std::wcout << L"\n[PID: " << pid << L"] " << procInfo.imageName << std::endl;
            } else {
                std::wcout << L"\n[PID: " << pid << L"] (unknown)" << std::endl;
            }
            
            std::wcout << L"  CMD: " << cmdLineInfo.originalCommandLine << std::endl;
            std::wcout << L"  Risk: " << cmdLineInfo.GetRiskLevelString() << L" (" << cmdLineInfo.riskScore << L")";
            
            if (cmdLineInfo.IsSuspicious()) {
                suspiciousCount++;
                std::wcout << L" [SUSPICIOUS]";
                if (!cmdLineInfo.detections.empty()) {
                    std::wcout << L" - " << cmdLineInfo.GetDetectionTypesString();
                }
            }
            std::wcout << std::endl;
        }
        
        if (suspiciousCount > 0) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"SUSPICIOUS: " << suspiciousCount << L" command lines" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
        }
        
        return 0;
    }
    
    if (cmdLineDump) {
        g_CommandLineDB.Dump();
        return 0;
    }
    
    if (listInjections) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"INJECTION EVENTS (" << g_InjectionDB.Count() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto allEvents = g_InjectionDB.GetAllEvents();
        int suspiciousCount = 0;
        
        for (const auto& [timestamp, event] : allEvents) {
            ProcessInfo procInfo;
            if (g_ProcessDB.GetProcess(event.pid, procInfo)) {
                std::wcout << L"\n[" << event.pid << L"] " << procInfo.imageName << std::endl;
            } else {
                std::wcout << L"\n[" << event.pid << L"] (unknown)" << std::endl;
            }
            
            std::wcout << L"  Type: " << event.GetTypeString() << std::endl;
            std::wcout << L"  Risk: " << event.GetRiskLevelString() << L" (" << event.riskScore << L")";
            
            if (event.IsSuspicious()) {
                suspiciousCount++;
                std::wcout << L" [SUSPICIOUS]";
            }
            std::wcout << std::endl;
            
            if (!event.apiCallChain.calls.empty()) {
                std::wcout << L"  Pattern: " << event.apiCallChain.GetPatternString() << std::endl;
            }
        }
        
        if (suspiciousCount > 0) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"SUSPICIOUS: " << suspiciousCount << L" injection events" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
        }
        
        return 0;
    }
    
    if (injectionDump) {
        g_InjectionDB.Dump();
        return 0;
    }
    
    if (scanInjection) {
        std::wcout << L"\n[+] Scanning all processes for injection..." << std::endl;
        auto events = ScanAllForInjection();
        
        std::wcout << L"[+] Found " << events.size() << L" potential injection events" << std::endl;
        
        if (!events.empty()) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"INJECTION DETECTIONS:" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
            
            for (const auto& event : events) {
                std::wcout << L"\n" << event.ToString() << std::endl;
            }
        }
        
        return 0;
    }
    
    if (dumpDatabase) {
        g_ProcessDB.Dump();
        return 0;
    }
    
    if (showTree) {
        std::wcout << L"\n" << g_ProcessDB.GetProcessTreeAsString() << std::endl;
        return 0;
    }

    if (listSyscalls) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"SYSCALL EVENTS (" << g_SyscallDB.Count() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto allSyscalls = g_SyscallDB.GetAllSyscalls();
        int highRiskCount = 0;
        
        for (const auto& info : allSyscalls) {
            ProcessInfo procInfo;
            if (g_ProcessDB.GetProcess(info->pid, procInfo)) {
                std::wcout << L"\n[Event " << info->eventId << L"] [PID: " << info->pid << L"] " << procInfo.imageName << std::endl;
            } else {
                std::wcout << L"\n[Event " << info->eventId << L"] [PID: " << info->pid << L"] (unknown)" << std::endl;
            }
            
            std::wcout << L"  Syscall: " << info->syscallName 
                      << L" (0x" << std::hex << static_cast<uint32_t>(info->syscallNumber) << std::dec << L")"
                      << L"  Type: " << SyscallUtils::GetSyscallName(info->syscallNumber)
                      << std::endl;
            std::wcout << L"  Time: " << info->timestampStr
                      << L"  Risk: " << info->riskScore
                      << (info->riskScore >= 70 ? L" [HIGH]" : (info->riskScore >= 40 ? L" [MEDIUM]" : L" [LOW]"))
                      << std::endl;
            
            if (info->riskFlags != 0) {
                std::wcout << L"  Flags: " << SyscallUtils::FormatRiskFlags(info->riskFlags) << std::endl;
            }
            
            if (!info->riskReason.empty()) {
                std::wcout << L"  Reason: " << info->riskReason << std::endl;
            }
            
            if (info->riskScore >= 70) {
                highRiskCount++;
            }
        }
        
        if (highRiskCount > 0) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"HIGH RISK: " << highRiskCount << L" syscall events" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
        }
        
        return 0;
    }
    
    if (syscallDump) {
        std::wcout << L"\n" << g_SyscallDB.GetStatisticsAsString() << std::endl;
        return 0;
    }
    
    if (scanHooks) {
        std::wcout << L"\n[+] Scanning ntdll for hooked syscall stubs..." << std::endl;
        
        bool scanned = ScanNtdllForHooks();
        
        if (scanned) {
            auto hooks = g_SyscallDetector.GetHookedSyscalls();
            auto stubs = g_SyscallDetector.GetNtdllStubInfo();
            
            std::wcout << L"[+] Found " << stubs.size() << L" syscall stubs, " 
                      << g_SyscallDetector.GetTotalHookedStubs() << L" hooked" << std::endl;
            
            if (g_SyscallDetector.GetTotalHookedStubs() > 0) {
                std::wcout << L"\n" << std::string(40, '-') << std::endl;
                std::wcout << L"HOOKED SYSCALL STUBS:" << std::endl;
                std::wcout << std::string(40, '-') << std::endl;
                
                for (const auto& stub : stubs) {
                    if (stub.isHooked) {
                        std::wcout << L"  [HOOKED] " << stub.functionName
                                  << L" (0x" << std::hex << stub.stubAddress << std::dec << L")"
                                  << L" Type: " << static_cast<int>(stub.hookType)
                                  << std::endl;
                        std::wcout << L"           Details: " << stub.hookDetails << std::endl;
                    }
                }
            } else {
                std::wcout << L"\n[+] No hooked syscall stubs detected" << std::endl;
            }
        } else {
            std::wcout << L"[-] Failed to scan ntdll" << std::endl;
        }
        
        return 0;
    }

    if (listKernelEvents) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"KERNEL EVENTS (" << g_KernelDB.Count() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto allEvents = g_KernelDB.GetAllEvents();
        int highRiskCount = 0;
        
        for (const auto& info : allEvents) {
            ProcessInfo procInfo;
            if (g_ProcessDB.GetProcess(info->pid, procInfo)) {
                std::wcout << L"\n[Event " << info->eventId << L"] [PID: " << info->pid << L"] " << procInfo.imageName << std::endl;
            } else {
                std::wcout << L"\n[Event " << info->eventId << L"] [PID: " << info->pid << L"] (unknown)" << std::endl;
            }
            
            std::wcout << L"  Type: " << info->eventTypeStr
                      << L"  Time: " << info->timestampStr
                      << L"  Risk: " << info->riskScore
                      << (info->riskScore >= 70 ? L" [HIGH]" : (info->riskScore >= 40 ? L" [MEDIUM]" : L" [LOW]"))
                      << std::endl;
            
            if (info->riskFlags != 0) {
                std::wcout << L"  Flags: " << KernelUtils::FormatRiskFlags(info->riskFlags) << std::endl;
            }
            
            if (!info->riskReason.empty()) {
                std::wcout << L"  Reason: " << info->riskReason << std::endl;
            }
            
            if (info->riskScore >= 70) {
                highRiskCount++;
            }
        }
        
        if (highRiskCount > 0) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"HIGH RISK: " << highRiskCount << L" kernel events" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
        }
        
        return 0;
    }
    
    if (kernelDump) {
        std::wcout << L"\n" << g_KernelDB.GetStatisticsAsString() << std::endl;
        return 0;
    }
    
    if (scanKernel) {
        std::wcout << L"\n[+] Scanning for kernel-level threats..." << std::endl;
        
        // Start kernel monitoring
        StartKernelMonitor();
        StartKernelDetector();
        
        // Perform scan
        auto threats = ScanForKernelThreats();
        
        std::wcout << L"[+] Found " << threats.size() << L" kernel threat events" << std::endl;
        
        if (!threats.empty()) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"KERNEL THREAT DETECTIONS:" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
            
            for (const auto& event : threats) {
                std::wcout << L"\nEvent ID: " << event->eventId << std::endl;
                std::wcout << L"  Type: " << event->eventTypeStr << std::endl;
                std::wcout << L"  PID: " << event->pid << L" (" << event->processName << L")" << std::endl;
                std::wcout << L"  Risk: " << event->riskScore << L"/100 [" 
                          << (event->riskScore >= 90 ? L"CRITICAL" : 
                              event->riskScore >= 70 ? L"HIGH" : 
                              event->riskScore >= 50 ? L"MEDIUM" : L"LOW") << L"]" << std::endl;
                
                if (!event->riskReason.empty()) {
                    std::wcout << L"  Reason: " << event->riskReason << std::endl;
                }
                
                if (event->riskFlags != 0) {
                    std::wcout << L"  Flags: " << KernelUtils::FormatRiskFlags(event->riskFlags) << std::endl;
                }
            }
        } else {
            std::wcout << L"[+] No kernel threats detected" << std::endl;
        }
        
        StopKernelMonitor();
        StopKernelDetector();
        
        return 0;
    }

    if (listDetections) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"DETECTIONS (" << g_DetectionDB.GetDetectionResultCount() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto allDetections = g_DetectionDB.GetAllDetectionResults();
        int highRiskCount = 0;
        
        for (const auto& detection : allDetections) {
            std::wcout << L"\n[" << detection.timestampStr << L"] " << detection.ruleName << L" (" << detection.ruleId << L")" << std::endl;
            std::wcout << L"  PID: " << detection.pid << L" (" << detection.processName << L")" << std::endl;
            std::wcout << L"  Severity: " << DetectionUtils::SeverityToString(detection.severity) << L"\n";
            std::wcout << L"  Risk Score: " << detection.finalRiskScore << L"/100" << std::endl;
            
            if (detection.finalRiskScore >= 70) {
                highRiskCount++;
            }
        }
        
        if (highRiskCount > 0) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"HIGH RISK: " << highRiskCount << L" detections" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
        }
        
        return 0;
    }
    
    if (detectionDump) {
        std::wcout << L"\n" << g_DetectionDB.GetStatisticsAsString() << std::endl;
        return 0;
    }
    
    if (scanDetect) {
        std::wcout << L"\n[+] Scanning for threats using detection engine..." << std::endl;
        
        // Evaluate all detection rules
        auto detections = ScanForDetections();
        
        std::wcout << L"[+] Found " << detections.size() << L" potential threats" << std::endl;
        
        if (!detections.empty()) {
            std::wcout << L"\n" << std::string(40, '-') << std::endl;
            std::wcout << L"THREAT DETECTIONS:" << std::endl;
            std::wcout << std::string(40, '-') << std::endl;
            
            for (const auto& detection : detections) {
                std::wcout << L"\nDetection ID: " << detection.detectionId << std::endl;
                std::wcout << L"  Rule: " << detection.ruleName << std::endl;
                std::wcout << L"  PID: " << detection.pid << L" (" << detection.processName << L")" << std::endl;
                std::wcout << L"  Risk: " << detection.finalRiskScore << L"/100 [" 
                          << DetectionUtils::SeverityToString(detection.severity) << L"]" << std::endl;
                
                if (!detection.detectionDescription.empty()) {
                    std::wcout << L"  Description: " << detection.detectionDescription << std::endl;
                }
                
                if (!detection.evidenceItems.empty()) {
                    std::wcout << L"  Evidence: " << std::endl;
                    for (const auto& evidence : detection.evidenceItems) {
                        std::wcout << L"    - " << evidence << std::endl;
                    }
                }
            }
        } else {
            std::wcout << L"[+] No threats detected" << std::endl;
        }
        
        return 0;
    }
    
    if (listCorrelations) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"CORRELATED EVENTS" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        DetectionQueryCriteria criteria;
        auto correlations = g_EventCorrelator.GetCorrelatedEvents(criteria);
        
        for (const auto& corr : correlations) {
            std::wcout << L"\n[" << corr.timestampStr << L"] " << corr.description << std::endl;
            std::wcout << L"  Correlation ID: " << corr.correlationId << std::endl;
            std::wcout << L"  Type: " << static_cast<int>(corr.correlationType) << std::endl;
            std::wcout << L"  Primary PID: " << corr.primaryPid << std::endl;
            std::wcout << L"  Risk Score: " << corr.riskScore << L"/100" << std::endl;
            std::wcout << L"  Source Events: " << corr.sourceEvents.size() << std::endl;
        }
        
        return 0;
    }
    
    if (correlationDump) {
        std::wcout << L"\n" << g_EventCorrelator.GetStatisticsAsString() << std::endl;
        return 0;
    }
    
    if (listGraphs) {
        std::wcout << L"\n" << std::string(80, '=') << std::endl;
        std::wcout << L"BEHAVIORAL GRAPHS (" << g_GraphBuilder.GetBehavioralGraphCount() << L")" << std::endl;
        std::wcout << std::string(80, '=') << std::endl;
        
        auto graphs = g_GraphBuilder.GetAllBehavioralGraphs();
        
        for (const auto& graph : graphs) {
            std::wcout << L"\nGraph: " << graph.graphName << std::endl;
            std::wcout << L"  ID: " << graph.graphId << std::endl;
            std::wcout << L"  Nodes: " << graph.nodes.size() << std::endl;
            std::wcout << L"  Edges: " << graph.edges.size() << std::endl;
        }
        
        return 0;
    }
    
    if (graphDump) {
        std::wcout << L"\n" << g_GraphBuilder.GetStatisticsAsString() << std::endl;
        return 0;
    }

    if (g_emitJson) {
        std::wcout << L"[+] Running in JSON output mode. Emitting detection events to stdout." << std::endl;
        // Run ETW in the foreground and emit JSON events for external GUI consumption
        StartETW();
        return 0;
    }

    std::wcout << L"\n[+] Starting GUI..." << std::endl;
    std::wcout << L"[+] Press Ctrl+C to exit" << std::endl;
    
    // Start GUI thread first
    StartGuiThread();
    JoinGuiThread();

    return 0;
}
