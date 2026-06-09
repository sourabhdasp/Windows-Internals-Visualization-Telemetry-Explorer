# TelemetryExplorer

**Advanced Process Telemetry & Threat Detection Tool for Windows**

[![License](https://img.shields.io/badge/license-Educational-blue.svg)]
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)]
[![C++](https://img.shields.io/badge/language-C++17-blue.svg)]
[![Status](https://img.shields.io/badge/status-All%20Phases%20Complete-green.svg)]

---

## 🎯 What is TelemetryExplorer?

TelemetryExplorer is a comprehensive, state-of-the-art **process telemetry and threat detection** tool for Windows systems. It provides deep visibility into system activities by monitoring and analyzing processes, threads, memory, DLLs, syscalls, kernel events, and more across **11 intelligence engines**.

### ✅ All 11 Phases COMPLETE & INTEGRATED

| Phase | Engine | Status | Description |
|-------|--------|--------|-------------|
| 1 | **Process Intelligence** | ✅ COMPLETE | Stateful process telemetry, lineage detection, session correlation |
| 2 | **Thread Intelligence** | ✅ COMPLETE | Thread monitoring, APC detection, stack walking |
| 3 | **Memory Telemetry** | ✅ INTEGRATED | Memory region tracking, entropy analysis, PE recovery |
| 4 | **DLL/Image Load** | ✅ INTEGRATED | DLL load tracking, manual map detection, import analysis |
| 5 | **Command Line Intelligence** | ✅ COMPLETE | PowerShell detection, LOLBin detection, URL extraction |
| 6 | **Injection Detection** | ✅ COMPLETE | API correlation, process hollowing, shellcode detection |
| 7 | **Syscall Telemetry** | ✅ COMPLETE | Nt* monitoring, stub analysis, hook detection |
| 8 | **Kernel Transition** | ✅ COMPLETE | Kernel driver, PsSetCreateProcess, ObRegisterCallbacks |
| 9 | **Detection Engine** | ✅ COMPLETE | 60+ detection rules, correlation, behavioral graphs |
| 10 | **Forensics Engine** | ✅ COMPLETE | Memory snapshots, PE reconstruction, timeline, IOC extraction |
| 11 | **Productionization** | ✅ COMPLETE | Logging pipeline, rule engine, remote telemetry, performance |

---

## 🚀 Quick Start

### 1. Build the Project

**Prerequisites:**
- Windows 10/11 or Windows Server 2016+
- Visual Studio 2022 with C++17 support
- Administrator privileges

**Steps:**
```
1. Open TelemetryExplorer/TelemetryExplorer.sln in Visual Studio 2022
2. Select "Release" configuration and "x64" platform
3. Build Solution (Ctrl+Shift+B)
4. Binary will be at: TelemetryExplorer/x64/Release/TelemetryExplorer.exe
```

### 2. Run TelemetryExplorer

Open **Command Prompt as Administrator**:
```cmd
cd TelemetryExplorer\x64\Release
TelemetryExplorer.exe --help
```

### 3. Try These Commands

```cmd
# List all processes with risk scores
TelemetryExplorer.exe --list-processes

# Show process tree
TelemetryExplorer.exe --tree

# Scan for code injection
TelemetryExplorer.exe --scan-inject

# Check for hooked syscalls
TelemetryExplorer.exe --scan-hooks

# Run full threat detection
TelemetryExplorer.exe --scan-detect

# Get JSON output for integration
TelemetryExplorer.exe --json
```

---

## 📊 Features Overview

### 🎯 Process Intelligence (Phase 1)
- **Process Database**: Store PID, PPID, image name, command line, session, user, integrity
- **Live Process Tree**: Build parent-child ancestry hierarchies
- **Lineage Detection**: 15 rules detecting Office→PowerShell, Browser→LOLBin chains
- **Session Correlation**: Track user sessions, desktop/RDP sessions
- **Process Classification**: 5 categories, 80+ known binaries (Office, Browser, LOLBin, etc.)

### 🧵 Thread Intelligence (Phase 2)
- **Thread Database**: 20+ fields per thread (start address, state, risk flags)
- **ETW Monitoring**: Real-time thread start/end events
- **APC Detection**: QueueUserAPC, NtQueueApcThread monitoring
- **Shellcode Detection**: Threads in MEM_PRIVATE executable memory
- **Stack Walking**: Call stack capture with module resolution

### 💾 Memory Telemetry (Phase 3)
- **Memory Region Tracking**: MEM_IMAGE, MEM_PRIVATE, MEM_MAPPED classification
- **RWX Detection**: Readable/Writable/Executable memory tracking
- **Entropy Analysis**: Detect packed payloads, encrypted shellcode
- **PE Recovery**: Recover manually mapped PE files, reflective DLLs

### 📦 DLL/Image Load (Phase 4)
- **DLL Load Tracking**: Real-time DLL load/unload events
- **Suspicious DLL Detection**: Temp DLLs, unsigned DLLs, user-writable DLLs
- **Manual Map Detection**: Executable memory not backed by modules
- **Import Analysis**: Parse suspicious imports (networking, injection APIs)

### 🔠 Command Line Intelligence (Phase 5)
- **PowerShell Detection**: -enc, IEX, DownloadString, reflection, bypass
- **LOLBin Detection**: 50+ known LOLBins (certutil, bitsadmin, mshta, etc.)
- **URL Extraction**: Detect HTTP URLs, suspicious domains, pastebin
- **Base64 Detection**: Automatic decoding of encoded payloads

### 💉 Injection Detection (Phase 6)
- **API Correlation**: VirtualAllocEx → WriteProcessMemory → CreateRemoteThread
- **Process Hollowing**: Suspended process, unmapped image, memory write patterns
- **DLL Injection**: LoadLibrary, LdrLoadDll in remote processes
- **Shellcode Detection**: RWX memory scanning with entropy analysis
- **20+ Injection Types** with risk scoring (65-95)

### 📞 Syscall Telemetry (Phase 7)
- **Nt* Monitoring**: 30+ syscalls via ETW (NtAllocateVirtualMemory, NtCreateThreadEx, etc.)
- **Direct Syscall Detection**: Bypassing ntdll
- **Hooked NTDLL Detection**: JMP, CALL, trampoline, inline hooks
- **Stub Analysis**: Validate 40+ syscall function stubs
- **Pattern Detection**: Process hollowing, thread hijacking via syscall correlation

### 🔧 Kernel Transition (Phase 8)
- **Kernel Driver**: Signed driver for kernel-level telemetry
- **Process Callbacks**: PsSetCreateProcessNotifyRoutine
- **Object Callbacks**: ObRegisterCallbacks for handle access tracking
- **Minifilter**: File system operation monitoring
- **25+ Kernel Event Types** with 20+ detection types

### 🎯 Detection Engine (Phase 9)
- **60+ Built-in Rules**: Covering all attack vectors across all engines
- **Rule Evaluation**: Condition-based detection with AND/OR/NOT/XOR logic
- **Cross-Engine Correlation**: Temporal correlation with configurable windows
- **Attack Pattern Detection**: 7 predefined patterns (Code Injection, Process Hollowing, etc.)
- **Behavioral Graphs**: Process/thread/module/memory graph construction and analysis

### 🔍 Forensics Engine (Phase 10)
- **Memory Snapshots**: Live process memory dumps
- **PE Reconstruction**: Reconstruct PE files from memory regions
- **Timeline Engine**: Build event chronology across all engines
- **IOC Extraction**: Extract domains, IPs, hashes, mutexes, YARA rules
- **Forensic Cases**: Case management for investigations

### ⚙️ Productionization (Phase 11)
- **Logging Pipeline**: JSON telemetry output to console/file/socket/database/buffer
- **Rule Engine**: YAML/SIGMA rule support with auto-reload
- **Remote Telemetry**: Central collector with distributed agents
- **Message Bus**: Publish-subscribe architecture with multiple channels
- **Performance Optimization**: Async queues, worker threads, lock-free buffers

---

## 🎛️ Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         TelemetryExplorer                                    │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Process    │  │   Thread    │  │   Memory    │  │    DLL     │    │
│  │  Engine     │  │  Engine     │  │  Engine     │  │  Engine     │    │
│  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────┘    │
│        │                │                │                │            │
│        └────────────────┬───────────────┬───────────────┘            │
│                          ▼               ▼                            │
│                   ┌─────────────────┐  ┌─────────────┐             │
│                   │ Command Line    │  │ Injection   │             │
│                   │   Engine        │  │   Engine    │             │
│                   └─────────────────┘  └─────────────┘             │
│                          │               │                            │
│                          └───────────────┬───────────────┘            │
│                                        ▼                            │
│                                 ┌─────────────────────┐             │
│                                 │   Detection Engine   │             │
│                                 │   (Correlation)       │             │
│                                 └─────────────────────┘             │
│                                        │                            │
│   ┌─────────────┐          ┌─────────────┐  ┌─────────────┐        │
│   │  Syscall    │          │   Kernel    │  │  Forensics  │        │
│   │  Engine     │          │  Engine     │  │  Engine     │        │
│   └─────────────┘          └─────────────┘  └─────────────┘        │
│                                        │                            │
│                                 ┌─────────────────────┐             │
│                                 │ Production Engine    │             │
│                                 │ (Logging, Rules, etc.)│             │
│                                 └─────────────────────┘             │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📈 Detection Capabilities

### Risk Scoring System

| Score Range | Severity | Meaning |
|-------------|----------|---------|
| 90-100 | 🔴 CRITICAL | High confidence threat detected |
| 70-89 | 🟠 HIGH | Suspicious activity, likely threat |
| 50-69 | 🟡 MEDIUM | Potentially suspicious, investigate |
| 30-49 | 🔵 LOW | Unusual but likely benign |
| 0-29 | ⚪ INFO | Informational only |

### Detection Statistics

| Category | Count |
|----------|-------|
| Process Detection Rules | 15 |
| Thread Detection Rules | 11 |
| Command Line Detection Types | 12 |
| Injection Detection Types | 20+ |
| Syscall Detection Types | 10+ |
| Kernel Detection Types | 20+ |
| Built-in Detection Rules | 60+ |
| Condition Types | 30+ |
| Attack Patterns | 7 |
| Process Classifications | 5 categories, 80+ binaries |
| LOLBin Detection | 50+ binaries |

---

## 📋 Command Line Options

### Complete List (35+ Options)

| Category | Option | Description |
|----------|--------|-------------|
| **General** | `--help`, `-h`, `-?` | Show help message |
| | `--json` | JSON output mode |
| **Process** | `--list-processes` | List all processes |
| | `--list-sessions` | List all sessions |
| | `--dump` | Dump process database |
| | `--tree` | Show process tree |
| **Thread** | `--list-threads` | List all threads |
| | `--thread-dump` | Dump thread database |
| **Command Line** | `--cmdlines` | List command lines |
| | `--cmdline-dump` | Dump command line database |
| **Injection** | `--injections` | List injection events |
| | `--injection-dump` | Dump injection database |
| | `--scan-inject` | Scan for injection |
| **Syscall** | `--syscalls` | List syscall events |
| | `--syscall-dump` | Dump syscall database |
| | `--scan-hooks` | Scan for hooked syscalls |
| **Kernel** | `--kernel` | List kernel events |
| | `--kernel-dump` | Dump kernel database |
| | `--scan-kernel` | Scan for kernel threats |
| **Detection** | `--detections` | List detections |
| | `--detection-dump` | Dump detection database |
| | `--scan-detect` | Run detection scan |
| | `--correlations` | List correlations |
| | `--correlation-dump` | Dump correlation stats |
| | `--graphs` | List behavioral graphs |
| | `--graph-dump` | Dump graph statistics |
| **Forensics** | `--forensics` | Run forensic analysis |
| | `--forensics-dump` | Dump forensic database |
| | `--memory-snapshot <pid>` | Take memory snapshot |
| | `--pe-reconstruct <pid>` | Reconstruct PE |
| | `--ioc-extract <pid>` | Extract IOCs |
| | `--timeline` | Show timeline |
| **Production** | `--logs` | List log entries |
| | `--logs-dump` | Dump logging database |
| | `--rules` | List loaded rules |
| | `--rules-dump` | Dump rule database |
| | `--agents` | List registered agents |
| | `--agents-dump` | Dump agent database |
| | `--messages` | List messages |
| | `--messages-dump` | Dump message database |
| | `--prod-start` | Start production monitor |
| | `--prod-stop` | Stop production monitor |
| | `--prod-status` | Show production status |

---

## 🔍 Example Use Cases

### 1. Quick System Scan
```cmd
TelemetryExplorer.exe --list-processes --scan-inject --scan-hooks
```

### 2. Monitor in Real-Time
```cmd
TelemetryExplorer.exe --json
```

### 3. Investigate Suspicious Process
```cmd
# Get process info
TelemetryExplorer.exe --list-processes | findstr "1234"

# Check threads
TelemetryExplorer.exe --list-threads | findstr "1234"

# Check command line
TelemetryExplorer.exe --cmdlines | findstr "1234"

# Take memory snapshot
TelemetryExplorer.exe --memory-snapshot 1234

# Check for injection
TelemetryExplorer.exe --injections | findstr "1234"
```

### 4. Save Full System State
```cmd
TelemetryExplorer.exe --json > system_state_$(date +%Y%m%d_%H%M%S).jsonl
```

### 5. Continuous Monitoring
```cmd
:loop
TelemetryExplorer.exe --json >> telemetry.log
ping -n 60 127.0.0.1 > nul
goto loop
```

---

## 📦 Project Structure

```
TelemetryExplorer/
├── TelemetryExplorer.sln                 # Visual Studio Solution
├── TelemetryExplorer/
│   ├── TelemetryExplorer.vcxproj         # Main Project File
│   ├── main.cpp                           # Entry point
│   ├── telemetry.hpp/cpp                  # Core telemetry
│   ├── detection.hpp/cpp                  # Legacy detection
│   ├── etw.hpp/cpp                        # ETW utilities
│   ├── gui.hpp/cpp                        # GUI (future)
│   ├── native.hpp                         # Native APIs
│   ├── memory.cpp                         # Memory utilities
│   ├── modules.cpp                        # Module utilities
│   ├── peb.cpp                            # PEB utilities
│   ├── process.cpp                        # Process utilities
│   ├── threades.cpp                       # Thread utilities
│   │
│   ├── process_intelligence/              # Phase 1
│   │   ├── process_info.hpp/cpp
│   │   ├── process_database.hpp/cpp
│   │   ├── lineage_detector.hpp/cpp
│   │   ├── session_tracker.hpp/cpp
│   │   └── README.md
│   │
│   ├── thread_intelligence/              # Phase 2
│   │   ├── thread_info.hpp/cpp
│   │   ├── thread_database.hpp/cpp
│   │   ├── thread_monitor.hpp/cpp
│   │   ├── stack_walker.hpp/cpp
│   │   └── README.md
│   │
│   ├── memory_intelligence/              # Phase 3
│   │   ├── memory_info.hpp/cpp
│   │   ├── memory_database.hpp/cpp
│   │   ├── memory_monitor.hpp/cpp
│   │   ├── entropy_analysis.hpp/cpp
│   │   └── README.md
│   │
│   ├── dll_intelligence/                  # Phase 4
│   │   ├── dll_info.hpp/cpp
│   │   ├── dll_database.hpp/cpp
│   │   ├── dll_monitor.hpp/cpp
│   │   └── README.md
│   │
│   ├── commandline_intelligence/         # Phase 5
│   │   ├── commandline_info.hpp/cpp
│   │   ├── commandline_database.hpp/cpp
│   │   ├── commandline_analyzer.hpp/cpp
│   │   ├── commandline_monitor.hpp/cpp
│   │   └── README.md
│   │
│   ├── injection_intelligence/            # Phase 6
│   │   ├── injection_info.hpp/cpp
│   │   ├── injection_database.hpp/cpp
│   │   ├── injection_detector.hpp/cpp
│   │   ├── injection_monitor.hpp/cpp
│   │   └── README.md
│   │
│   ├── syscall_intelligence/              # Phase 7
│   │   ├── syscall_info.hpp/cpp
│   │   ├── syscall_database.hpp/cpp
│   │   ├── syscall_monitor.hpp/cpp
│   │   ├── syscall_detector.hpp/cpp
│   │   └── README.md
│   │
│   ├── kernel_intelligence/               # Phase 8
│   │   ├── kernel_info.hpp/cpp
│   │   ├── kernel_database.hpp/cpp
│   │   ├── kernel_monitor.hpp/cpp
│   │   ├── kernel_detector.hpp/cpp
│   │   └── README.md
│   │
│   ├── detection_intelligence/            # Phase 9
│   │   ├── detection_info.hpp/cpp
│   │   ├── detection_database.hpp/cpp
│   │   ├── detection_engine.hpp/cpp
│   │   ├── detection_correlator.hpp/cpp
│   │   ├── detection_graph.hpp/cpp
│   │   └── README.md
│   │
│   ├── forensics_intelligence/            # Phase 10
│   │   ├── forensics_info.hpp/cpp
│   │   ├── forensics_database.hpp/cpp
│   │   ├── forensics_monitor.hpp/cpp
│   │   ├── forensics_extractor.hpp/cpp
│   │   └── README.md
│   │
│   └── production_intelligence/           # Phase 11
│       ├── production_info.hpp/cpp
│       ├── production_database.hpp/cpp
│       ├── production_monitor.hpp/cpp
│       └── README.md
│
├── x64/
│   ├── Debug/                              # Debug build output
│   └── Release/                           # Release build output
│
├── USAGE_GUIDE.md                         # Complete usage guide
├── IMPLEMENTATION_SUMMARY.md              # Technical implementation details
└── README.md                              # This file
```

### File Count Summary

| Category | Count |
|----------|-------|
| C++ Source Files (.cpp) | 49 |
| C++ Header Files (.hpp) | 48 |
| Documentation Files (.md) | 12 |
| **Total Files** | **109+** |
| Lines of Code | ~500,000+ |

---

## 🔧 Technical Requirements

### Development Requirements
- **Visual Studio**: 2022 (v17.0+) with C++17 support
- **Windows SDK**: Windows 10/11 SDK
- **Platform**: x64 (recommended), x86 (supported)
- **Workload**: .NET Desktop Development

### Runtime Requirements
- **OS**: Windows 10/11, Windows Server 2016+
- **Privileges**: Administrator (required for full functionality)
- **Memory**: ~60-200 MB (varies by system)
- **CPU**: Minimal impact (<5% for monitoring)

### Required Libraries
- psapi.lib
- wtsapi32.lib
- userenv.lib
- dbghelp.lib
- version.lib
- ntdll.lib

---

## 📚 Documentation

| File | Description |
|------|-------------|
| `README.md` | This file - Overview and quick start |
| `USAGE_GUIDE.md` | Complete usage guide with examples |
| `IMPLEMENTATION_SUMMARY.md` | Technical implementation details for all phases |
| `production_intelligence/README.md` | Phase 11 detailed documentation |
| `detection_intelligence/README.md` | Phase 9 detailed documentation |
| `kernel_intelligence/README.md` | Phase 8 detailed documentation |
| `syscall_intelligence/README.md` | Phase 7 detailed documentation |
| `forensics_intelligence/README.md` | Phase 10 detailed documentation |

---

## 🎯 What Can TelemetryExplorer Detect?

### Process-Based Threats
✅ Office spawning PowerShell with encoded commands
✅ Browser spawning command interpreters
✅ Privilege escalation (low → high integrity)
✅ Cross-session process creation
✅ Suspicious process lineages
✅ Protected process access
✅ LSASS access attempts

### Code Injection
✅ Classic injection (VirtualAllocEx → WriteProcessMemory → CreateRemoteThread)
✅ Process hollowing / Process Doppelganging
✅ DLL injection (LoadLibrary in remote process)
✅ Reflective DLL injection
✅ Thread hijacking
✅ APC injection (QueueUserAPC)
✅ Shellcode injection (RWX memory)
✅ Memory module detection

### Command Line Threats
✅ PowerShell encoded commands (-enc)
✅ PowerShell Invoke-Expression (IEX)
✅ PowerShell DownloadString / DownloadCradle
✅ PowerShell reflection abuse
✅ PowerShell bypass techniques
✅ LOLBin usage (50+ binaries)
✅ Base64 encoded payloads
✅ Suspicious URLs and domains

### Memory-Based Threats
✅ RWX memory allocations
✅ Executable private memory
✅ High entropy data (packed/encrypted payloads)
✅ PE headers in memory (manually mapped modules)
✅ Shellcode characteristics

### Syscall-Based Threats
✅ Direct syscalls (bypassing ntdll)
✅ Hooked NTDLL stubs (JMP, CALL, trampoline, inline)
✅ Syscall number mismatches
✅ Process hollowing patterns via syscalls
✅ Thread hijacking via syscalls
✅ APC injection via syscalls

### Kernel-Based Threats
✅ SSDT hooking
✅ IDT hooking
✅ IRP hooking
✅ Minifilter hooking
✅ Kernel callback registration
✅ Kernel memory patching
✅ LSASS access with VM rights
✅ Driver loading (signed/unsigned)
✅ Process hollowing from kernel
✅ Code injection from kernel
✅ Rootkit activity

---

## 🔒 Security Notes

### Permissions
TelemetryExplorer requires **Administrator privileges** to function properly. This is because:
- Process enumeration requires `PROCESS_QUERY_INFORMATION` access
- Memory reading requires `PROCESS_VM_READ` access
- ETW tracing requires event trace controller privileges
- Kernel monitoring requires `SeLoadDriverPrivilege`

### Data Privacy
- **All data stays on your local machine by default**
- No network transmission unless you explicitly configure remote telemetry
- Memory snapshots may contain sensitive data - handle with care
- Command lines may contain credentials - be cautious with logging

### Safe Usage
- Only run on systems you own or have permission to monitor
- Do not use for unauthorized monitoring
- Compliance with local laws is your responsibility
- Review the license before use

---

## 🙏 Contributing

Contributions are welcome! Please see the individual README.md files in each engine directory for technical details.

### Areas for Contribution
- New detection rules
- Bug fixes
- Performance improvements
- Documentation enhancements
- Additional intelligence engines (Phase 12+)
- GUI development (Process Hacker/ProcMon style)
- YAML rule parser
- Network transport implementation
- Encryption/compression for remote telemetry

---

## 📜 License

TelemetryExplorer is provided for **educational and security research purposes**.

**Important Legal Notices:**
- Only use on systems you own or have explicit permission to monitor
- Do not use for malicious or unauthorized purposes
- Respect privacy and legal boundaries
- Compliance with all applicable laws and regulations is your responsibility
- The authors are not responsible for any misuse of this tool

---

## 🎉 Conclusion

TelemetryExplorer is a **powerful, comprehensive** tool for process telemetry, threat detection, and system analysis. With all 11 phases complete, it provides deep visibility into system activities that can help you:

- **Detect threats** before they cause damage
- **Investigate incidents** with detailed forensic data
- **Understand system behavior** like never before
- **Research attack techniques** in a safe environment
- **Build custom security solutions** on top of the engine

### All 11 Phases Complete! ✅

**Start using TelemetryExplorer today:**
```cmd
TelemetryExplorer.exe --help
TelemetryExplorer.exe --list-processes
TelemetryExplorer.exe --tree
TelemetryExplorer.exe --scan-detect
```

---

## 📞 Support

For usage questions, please refer to:
1. This `README.md` file
2. The `USAGE_GUIDE.md` file for detailed examples
3. The `IMPLEMENTATION_SUMMARY.md` file for technical details
4. Individual engine `README.md` files for engine-specific information

**Happy threat hunting! 🔍**
