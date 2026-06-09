#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <unordered_map>

// ============================================================================
// Include Process Intelligence Engine (Phase 1)
// ============================================================================
#include "process_intelligence/process_info.hpp"
#include "process_intelligence/process_database.hpp"
#include "process_intelligence/lineage_detector.hpp"
#include "process_intelligence/session_tracker.hpp"

// ============================================================================
// Include Thread Intelligence Engine (Phase 2)
// ============================================================================
#include "thread_intelligence/thread_info.hpp"
#include "thread_intelligence/thread_database.hpp"
#include "thread_intelligence/thread_monitor.hpp"
#include "thread_intelligence/stack_walker.hpp"

// ============================================================================
// Include Memory Intelligence Engine (Phase 3)
// ============================================================================
#include "memory_intelligence/memory_info.hpp"
#include "memory_intelligence/memory_database.hpp"
#include "memory_intelligence/memory_monitor.hpp"
#include "memory_intelligence/entropy_analysis.hpp"

// ============================================================================
// Include DLL Intelligence Engine (Phase 4)
// ============================================================================
#include "dll_intelligence/dll_info.hpp"
#include "dll_intelligence/dll_database.hpp"
#include "dll_intelligence/dll_monitor.hpp"

// ============================================================================
// Include Command Line Intelligence Engine (Phase 5)
// ============================================================================
#include "commandline_intelligence/commandline_info.hpp"
#include "commandline_intelligence/commandline_database.hpp"
#include "commandline_intelligence/commandline_analyzer.hpp"
#include "commandline_intelligence/commandline_monitor.hpp"

// ============================================================================
// Include Injection Intelligence Engine (Phase 6)
// ============================================================================
#include "injection_intelligence/injection_info.hpp"
#include "injection_intelligence/injection_database.hpp"
#include "injection_intelligence/injection_detector.hpp"
#include "injection_intelligence/injection_monitor.hpp"

// ============================================================================
// Include Syscall Intelligence Engine (Phase 7)
// ============================================================================
#include "syscall_intelligence/syscall_info.hpp"
#include "syscall_intelligence/syscall_database.hpp"
#include "syscall_intelligence/syscall_monitor.hpp"
#include "syscall_intelligence/syscall_detector.hpp"

// ============================================================================
// Include Kernel Intelligence Engine (Phase 8)
// ============================================================================
#include "kernel_intelligence/kernel_info.hpp"
#include "kernel_intelligence/kernel_database.hpp"
#include "kernel_intelligence/kernel_monitor.hpp"
#include "kernel_intelligence/kernel_detector.hpp"

// ============================================================================
// Include Detection Intelligence Engine (Phase 9)
// ============================================================================
#include "detection_intelligence/detection_info.hpp"
#include "detection_intelligence/detection_database.hpp"
#include "detection_intelligence/detection_engine.hpp"
#include "detection_intelligence/detection_correlator.hpp"
#include "detection_intelligence/detection_graph.hpp"

// ============================================================================
// Include Forensics Intelligence Engine (Phase 10)
// ============================================================================
#include "forensics_intelligence/forensics_info.hpp"
#include "forensics_intelligence/forensics_database.hpp"
#include "forensics_intelligence/forensics_monitor.hpp"
#include "forensics_intelligence/forensics_extractor.hpp"

// ============================================================================
// Legacy Module Info (for backward compatibility)
// ============================================================================

struct ModuleInfo
{
    std::wstring name;
    uintptr_t base;
    DWORD size;
};

extern std::vector<ModuleInfo> g_Modules;

// When true the process will emit detection events as JSON-lines to stdout
extern bool g_emitJson;

// ========================================
// Legacy ProcessInfo (for backward compatibility - use new ProcessInfo in process_info.hpp)
// ========================================
using LegacyProcessInfo = ProcessInfo;

// Legacy global process map (now replaced by g_ProcessDB)
// Note: g_ProcessMap is declared in process_intelligence/process_info.hpp

// ========================================
// GLOBAL FUNCTIONS
// ========================================

void CreateDumpDirectory();

void EnumerateProcesses();

void EnumerateModules(HANDLE hProcess);

void EnumerateMemory(HANDLE hProcess);

void EnumerateThreads(DWORD pid);

void GetRemotePEB(HANDLE hProcess);

bool IsMemoryBackedByModule(uintptr_t address);

void DumpRegionToDisk(
    HANDLE hProcess,
    LPVOID baseAddress,
    SIZE_T regionSize
);

void ParsePEHeader(
    HANDLE hProcess,
    LPVOID baseAddress
);

void DumpMemoryBytes(
    HANDLE hProcess,
    LPVOID address
);

void StartETW();

// Updated to use new ProcessDatabase
void InitializeProcessDatabase();

// Initialize Thread Database
void InitializeThreadDatabase();

// Start Thread Monitoring
void StartThreadMonitor();

// Stop Thread Monitoring
void StopThreadMonitor();

// Start Command Line Monitoring
void StartCommandLineMonitor();

// Stop Command Line Monitoring
void StopCommandLineMonitor();

// Initialize Command Line Database
void InitializeCommandLineDatabase();

// Start Injection Monitoring
void StartInjectionMonitor();

// Stop Injection Monitoring
void StopInjectionMonitor();

// Initialize Injection Database
void InitializeInjectionDatabase();

// Scan for injection
std::vector<InjectionEvent> ScanAllForInjection();

// ============================================================================
// Syscall Intelligence Engine Functions
// ============================================================================

// Initialize Syscall Database
inline void InitializeSyscallDatabase() {
    g_SyscallDB.Initialize();
}

// Start Syscall Monitoring
inline void StartSyscallMonitor() {
    SyscallMonitorUtils::Start();
}

// Stop Syscall Monitoring
inline void StopSyscallMonitor() {
    SyscallMonitorUtils::Stop();
}

// Initialize Syscall Detector
inline void InitializeSyscallDetector() {
    SyscallDetectorUtils::Initialize();
}

// Start Syscall Detection
inline void StartSyscallDetector() {
    SyscallDetectorUtils::Start();
}

// Stop Syscall Detection
inline void StopSyscallDetector() {
    SyscallDetectorUtils::Stop();
}

// Scan for hooked ntdll stubs
inline bool ScanNtdllForHooks() {
    return SyscallDetectorUtils::ScanNtdll();
}

// Analyze a syscall event
inline std::vector<SyscallDetectionResult> AnalyzeSyscallEvent(const std::shared_ptr<SyscallInfo>& info) {
    return SyscallDetectorUtils::Analyze(info);
}

// Check if a specific syscall stub is hooked
inline bool IsSyscallStubHooked(SyscallNumber syscall) {
    return SyscallDetectorUtils::IsStubHooked(syscall);
}

// ============================================================================
// Kernel Intelligence Engine Functions
// ============================================================================

// Initialize Kernel Database
inline void InitializeKernelDatabase() {
    g_KernelDB.Initialize();
}

// Start Kernel Monitoring
inline void StartKernelMonitor() {
    KernelMonitorUtils::Start();
}

// Stop Kernel Monitoring
inline void StopKernelMonitor() {
    KernelMonitorUtils::Stop();
}

// Initialize Kernel Detector
inline void InitializeKernelDetector() {
    KernelDetectorUtils::Initialize();
}

// Start Kernel Detection
inline void StartKernelDetection() {
    KernelDetectorUtils::Start();
}

// Stop Kernel Detection
inline void StopKernelDetector() {
    KernelDetectorUtils::Stop();
}

// Scan for kernel threats
inline std::vector<KernelDetectionResult> ScanForKernelThreats() {
    // Perform a full kernel scan
    KernelDetectorUtils::ScanForHooks();
    return {}; // Return empty vector for now
}

// Load kernel driver
inline bool LoadKernelDriver() {
    return KernelMonitorUtils::LoadDriver();
}

// Unload kernel driver
inline bool UnloadKernelDriver() {
    return KernelMonitorUtils::UnloadDriver();
}

// ============================================================================
// Detection Intelligence Engine Functions
// ============================================================================

// Initialize Detection Database
inline void InitializeDetectionDatabase() {
    g_DetectionDB.Initialize();
}

// Start Detection Engine
inline void StartDetectionEngine() {
    DetectionEngineUtils::Initialize();
}

// Stop Detection Engine
inline void StopDetectionEngine() {
    DetectionEngineUtils::Shutdown();
}

// Initialize Event Correlator
inline void InitializeDetectionCorrelator() {
    CorrelatorUtils::Initialize();
}

// Start Event Correlator
inline void StartDetectionCorrelator() {
    CorrelatorUtils::Initialize();
}

// Stop Event Correlator
inline void StopDetectionCorrelator() {
    CorrelatorUtils::Shutdown();
}

// Initialize Graph Builder
inline void InitializeGraphBuilder() {
    GraphBuilderUtils::Initialize();
}

// Start Graph Builder
inline void StartGraphBuilder() {
    GraphBuilderUtils::Initialize();
}

// Stop Graph Builder
inline void StopGraphBuilder() {
    GraphBuilderUtils::Shutdown();
}

// Evaluate all detection rules
inline std::vector<DetectionResult> EvaluateAllDetectionRules() {
    return DetectionEngineUtils::EvaluateAll();
}

// Evaluate detection rules for a specific process
inline std::vector<DetectionResult> EvaluateDetectionRulesForProcess(DWORD pid) {
    return DetectionEngineUtils::EvaluateForProcess(pid);
}

// Correlate process tree
inline CorrelatedEvent CorrelateDetectionProcessTree(DWORD pid) {
    return CorrelatorUtils::CorrelateProcessTree(pid);
}

// Detect patterns
inline std::vector<DetectionResult> DetectDetectionPatterns() {
    return DetectionEngineUtils::DetectPatterns();
}

// Build process behavioral graph
inline BehavioralGraph BuildDetectionProcessGraph(DWORD pid) {
    return GraphBuilderUtils::BuildProcessGraph(pid);
}

// Get detection statistics
inline DetectionStatistics GetDetectionStatistics() {
    return DetectionDBUtils::GetStatistics();
}

// Scan for detections
inline std::vector<DetectionResult> ScanForDetections() {
    return DetectionEngineUtils::EvaluateAll();
}

// ============================================================================
// Forensics Intelligence Engine Functions
// ============================================================================

// Initialize Forensics Database
inline void InitializeForensicsDatabase() {
    g_ForensicDB.Initialize();
}

// Start Forensics Monitor
inline void StartForensicsMonitor() {
    ForensicMonitorUtils::Start();
}

// Stop Forensics Monitor
inline void StopForensicsMonitor() {
    ForensicMonitorUtils::Stop();
}

// Initialize Forensics Extractor
inline void InitializeForensicsExtractor() {
    ForensicExtractorUtils::Initialize();
}

// Take memory snapshot of a process
inline bool TakeMemorySnapshot(DWORD pid) {
    return ForensicMonitorUtils::TakeMemorySnapshot(pid);
}

// Take memory snapshot of all processes
inline bool TakeMemorySnapshotAllProcesses() {
    return ForensicMonitorUtils::TakeMemorySnapshotAllProcesses();
}

// Reconstruct PE from process
inline bool ReconstructPEFromProcess(DWORD pid) {
    return ForensicMonitorUtils::ReconstructPEFromProcess(pid);
}

// Extract IOCs from process
inline bool ExtractIOCsFromProcess(DWORD pid) {
    return ForensicMonitorUtils::ExtractIOCsFromProcess(pid);
}

// Create forensic case for process
inline ForensicCase CreateForensicCase(DWORD pid) {
    return ForensicMonitorUtils::CreateCaseForProcess(pid);
}

// Perform full forensic analysis
inline size_t FullForensicAnalysisOfProcess(DWORD pid) {
    return ForensicMonitorUtils::FullForensicAnalysisOfProcess(pid);
}

// Get forensic statistics
inline ForensicStatistics GetForensicStatistics() {
    return ForensicDBUtils::GetStatistics();
}

// ============================================================================
// Include Production Intelligence Engine (Phase 11)
// ============================================================================
#include "production_intelligence/production_info.hpp"
#include "production_intelligence/production_database.hpp"
#include "production_intelligence/production_monitor.hpp"

// ============================================================================
// Production Intelligence Engine Functions
// ============================================================================

namespace ProductionIntelligenceUtils {
    inline void InitializeProductionMonitor() {
        ProductionIntelligence::InitializeProductionMonitor();
    }

    inline void StartProductionMonitor() {
        ProductionIntelligence::g_ProductionMonitor.Start();
    }

    inline void StopProductionMonitor() {
        ProductionIntelligence::g_ProductionMonitor.Stop();
    }

    inline bool IsProductionMonitorRunning() {
        return ProductionIntelligence::g_ProductionMonitor.IsRunning();
    }

    inline bool ProductionLog(
        ProductionIntelligence::LogLevel level,
        const std::wstring& logger,
        const std::wstring& message,
        const std::wstring& processName = L"",
        DWORD processId = 0,
        const std::unordered_map<std::wstring, std::wstring>& context = {}) {
        return ProductionIntelligence::g_ProductionMonitor.Log(
            level, logger, message, processName, processId, context);
    }

    inline std::vector<ProductionIntelligence::RuleMatchPtr> EvaluateProductionRules(
        const std::unordered_map<std::wstring, std::wstring>& event) {
        return ProductionIntelligence::g_ProductionMonitor.EvaluateRules(event);
    }

    inline bool PublishProductionMessage(const ProductionIntelligence::TelemetryMessage& message) {
        return ProductionIntelligence::g_ProductionMonitor.PublishMessage(message);
    }

    inline bool RegisterProductionAgent(const ProductionIntelligence::AgentConfig& agent) {
        return ProductionIntelligence::g_ProductionMonitor.RegisterAgent(agent);
    }

    inline ProductionIntelligence::ProductionStats GetProductionStatistics() {
        return ProductionIntelligence::g_ProductionMonitor.GetStats();
    }

    inline ProductionIntelligence::LoggingDatabase& GetLoggingDB() {
        return ProductionIntelligence::g_LoggingDB;
    }

    inline ProductionIntelligence::RuleDatabase& GetRuleDB() {
        return ProductionIntelligence::g_RuleDB;
    }

    inline ProductionIntelligence::AgentDatabase& GetAgentDB() {
        return ProductionIntelligence::g_AgentDB;
    }

    inline ProductionIntelligence::MessageDatabase& GetMessageDB() {
        return ProductionIntelligence::g_MessageDB;
    }

    inline ProductionIntelligence::LoggingPipeline& GetLoggingPipeline() {
        return ProductionIntelligence::g_ProductionMonitor.GetLoggingPipeline();
    }

    inline ProductionIntelligence::RuleEngine& GetRuleEngine() {
        return ProductionIntelligence::g_ProductionMonitor.GetRuleEngine();
    }

    inline ProductionIntelligence::AgentManager& GetAgentManager() {
        return ProductionIntelligence::g_ProductionMonitor.GetAgentManager();
    }

    inline ProductionIntelligence::MessageBus& GetMessageBus() {
        return ProductionIntelligence::g_ProductionMonitor.GetMessageBus();
    }

    inline ProductionIntelligence::PerformanceMonitor& GetPerformanceMonitor() {
        return ProductionIntelligence::g_ProductionMonitor.GetPerformanceMonitor();
    }
}

// Initialize Production Database
inline void InitializeProductionDatabase() {
    ProductionIntelligence::InitializeProductionDatabases();
}

// Start Production Monitor
inline void StartProductionMonitor() {
    ProductionIntelligenceUtils::StartProductionMonitor();
}

// Stop Production Monitor
inline void StopProductionMonitor() {
    ProductionIntelligenceUtils::StopProductionMonitor();
}

// Log to production pipeline
inline bool ProductionLog(
    ProductionIntelligence::LogLevel level,
    const std::wstring& logger,
    const std::wstring& message,
    const std::wstring& processName = L"",
    DWORD processId = 0,
    const std::unordered_map<std::wstring, std::wstring>& context = {}) {
    return ProductionIntelligenceUtils::ProductionLog(level, logger, message, processName, processId, context);
}

// Get production statistics
inline ProductionIntelligence::ProductionStats GetProductionStatistics() {
    return ProductionIntelligenceUtils::GetProductionStatistics();
}
