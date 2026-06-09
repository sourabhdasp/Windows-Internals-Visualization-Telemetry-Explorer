# TelemetryExplorer - Complete Usage Guide

## 🎯 Overview

TelemetryExplorer is a comprehensive process telemetry and threat detection tool that provides deep visibility into system activities across 11 intelligence engines. It monitors processes, threads, memory, DLLs, command lines, injections, syscalls, kernel events, detections, forensics, and production features.

**All 11 Phases are COMPLETE and INTEGRATED:**
- ✅ Phase 1: Process Intelligence Engine
- ✅ Phase 2: Thread Intelligence Engine
- ✅ Phase 3: Memory Telemetry Engine
- ✅ Phase 4: DLL/Image Load Engine
- ✅ Phase 5: Command Line Intelligence
- ✅ Phase 6: Injection Detection Engine
- ✅ Phase 7: Syscall Telemetry Engine
- ✅ Phase 8: Kernel Transition Engine
- ✅ Phase 9: Detection Engine
- ✅ Phase 10: Forensics Engine
- ✅ Phase 11: Productionization Engine

---

## 🚀 Quick Start

### Prerequisites

- **Visual Studio 2022** (Recommended)
- **Windows 10/11** or **Windows Server 2016+**
- **Administrator Privileges** (Required for full functionality)
- **C++17** support
- **.NET Desktop Development** workload

### Building the Project

1. **Open the Solution**
   ```
   Open: TelemetryExplorer/TelemetryExplorer.sln
   ```

2. **Build Configuration**
   - Select **Release** configuration
   - Select **x64** platform
   - Click **Build Solution** (Ctrl+Shift+B)

3. **Output Location**
   ```
   Binary: TelemetryExplorer/x64/Release/TelemetryExplorer.exe
   ```

### Running TelemetryExplorer

Open **Command Prompt as Administrator** and navigate to the output directory:

```cmd
cd TelemetryExplorer\x64\Release
TelemetryExplorer.exe --help
```

---

## 📋 Command Line Options

### General Options

| Option | Description |
|--------|-------------|
| `--help`, `-h`, `-?` | Show help message |
| `--json` | Run in JSON output mode (for external consumers) |

### Process Intelligence (Phase 1)

| Option | Description |
|--------|-------------|
| `--list-processes` | List all current processes and exit |
| `--list-sessions` | List all active sessions and exit |
| `--dump` | Dump process database and exit |
| `--tree` | Show process tree and exit |

### Thread Intelligence (Phase 2)

| Option | Description |
|--------|-------------|
| `--list-threads` | List all current threads and exit |
| `--thread-dump` | Dump thread database and exit |

### Command Line Intelligence (Phase 5)

| Option | Description |
|--------|-------------|
| `--cmdlines` | List all command lines and exit |
| `--cmdline-dump` | Dump command line database and exit |

### Injection Detection (Phase 6)

| Option | Description |
|--------|-------------|
| `--injections` | List all injection events and exit |
| `--injection-dump` | Dump injection database and exit |
| `--scan-inject` | Scan all processes for injection and exit |

### Syscall Telemetry (Phase 7)

| Option | Description |
|--------|-------------|
| `--syscalls` | List all syscall events and exit |
| `--syscall-dump` | Dump syscall database with statistics |
| `--scan-hooks` | Scan for hooked ntdll syscalls and exit |

### Kernel Intelligence (Phase 8)

| Option | Description |
|--------|-------------|
| `--kernel` | List all kernel events and exit |
| `--kernel-dump` | Dump kernel database with statistics |
| `--scan-kernel` | Scan for kernel-level threats and exit |

### Detection Engine (Phase 9)

| Option | Description |
|--------|-------------|
| `--detections` | List all detections and exit |
| `--detection-dump` | Dump full detection database with statistics |
| `--scan-detect` | Scan for threats using detection engine and exit |
| `--correlations` | List all correlated events and exit |
| `--correlation-dump` | Dump correlation statistics |
| `--graphs` | List all behavioral graphs and exit |
| `--graph-dump` | Dump graph statistics |

### Forensics Engine (Phase 10)

| Option | Description |
|--------|-------------|
| `--forensics` | Perform forensic analysis on all processes |
| `--forensics-dump` | Dump forensic database |
| `--memory-snapshot <pid>` | Take memory snapshot of a process |
| `--pe-reconstruct <pid>` | Reconstruct PE from process memory |
| `--ioc-extract <pid>` | Extract IOCs from process |
| `--timeline` | Show process timeline |

### Productionization (Phase 11)

| Option | Description |
|--------|-------------|
| `--logs` | List all log entries and exit |
| `--logs-dump` | Dump full logging database with statistics |
| `--rules` | List all loaded rules and exit |
| `--rules-dump` | Dump full rule database with statistics |
| `--agents` | List all registered agents and exit |
| `--agents-dump` | Dump full agent database with statistics |
| `--messages` | List all messages and exit |
| `--messages-dump` | Dump full message database with statistics |
| `--prod-start` | Start production monitor and exit |
| `--prod-stop` | Stop production monitor and exit |
| `--prod-status` | Show production monitor status and exit |

---

## 🎯 Common Use Cases

### 1. Monitor All System Activity in Real-Time

```cmd
TelemetryExplorer.exe --json
```

This will output all telemetry events as JSON lines to stdout, which you can pipe to other tools:

```cmd
TelemetryExplorer.exe --json | python analyzer.py
```

### 2. Get Full Process Tree

```cmd
TelemetryExplorer.exe --tree
```

Shows hierarchical process tree with risk scores and tags.

### 3. Scan for Suspicious Processes

```cmd
TelemetryExplorer.exe --list-processes
```

Lists all processes with integrity levels, elevation status, and risk tags.

### 4. Detect Code Injection

```cmd
TelemetryExplorer.exe --scan-inject
```

Scans all running processes for injection patterns (API correlation, memory artifacts, etc.).

### 5. Check for Hooked Syscalls

```cmd
TelemetryExplorer.exe --scan-hooks
```

Scans ntdll for hooked syscall stubs (indicative of malware or rootkits).

### 6. Run Full Threat Detection

```cmd
TelemetryExplorer.exe --scan-detect
```

Runs all 60+ detection rules across all engines to identify threats.

### 7. Forensic Analysis

```cmd
TelemetryExplorer.exe --memory-snapshot 1234
```

Takes a memory snapshot of process with PID 1234.

```cmd
TelemetryExplorer.exe --pe-reconstruct 1234
```

Reconstructs PE files from process memory.

### 8. Start Production Monitoring

```cmd
TelemetryExplorer.exe --prod-start
```

Starts the production monitor with logging pipeline, rule engine, and agent management.

---

## 📊 Understanding the Output

### Process Listing (`--list-processes`)

```
CURRENT PROCESSES (123)
================================================================================
[     0] svchost.exe             Session:   0  User: NT AUTHORITY\SYSTEM   Integrity: System       Elevated: YES
[   124] explorer.exe            Session:   1  User: DOMAIN\User1          Integrity: Medium      Elevated: NO 
[  1234] winword.exe             Session:   1  User: DOMAIN\User1          Integrity: Medium      Elevated: NO 
[  5678] powershell.exe          Session:   1  User: DOMAIN\User1          Integrity: Medium      Elevated: YES
      ⚠️ DETECTED: Office Spawning Script/LOLBin [HIGH RISK +85]
      ⚠️ DETECTED: PowerShell with -enc flag [CRITICAL RISK +100]
      ⚠️ TOTAL RISK SCORE: 185 (capped at 100)
```

**Columns:**
- PID: Process ID
- Name: Executable name
- Session: Session ID
- User: User account
- Integrity: Integrity level (System, High, Medium, Low, Untrusted)
- Elevated: Whether process is elevated
- Risk Score: 0-100 (higher = more suspicious)
- Tags: Classification tags (office_app, browser, lolbin, etc.)

### Process Tree (`--tree`)

```
Process Tree:
├─ [0] System Idle Process
├─ [4] System
  ├─ [280] smss.exe
  ├─ [324] csrss.exe
  ├─ [332] wininit.exe
    ├─ [408] services.exe
      ├─ [500] svchost.exe
      └─ [512] svchost.exe
    └─ [416] lsass.exe
├─ [1000] explorer.exe (DOMAIN\User1)
  └─ [1234] winword.exe (DOMAIN\User1)
    └─ [5678] powershell.exe (DOMAIN\User1) [RISK: 85]
      ⚠️ DETECTED: Office Spawning Script/LOLBin
      ⚠️ DETECTED: PowerShell with -enc flag
```

### Thread Listing (`--list-threads`)

```
Process: [1234] svchost.exe
  [  5678] StartAddr: 0x0000000012345678  State: RUNNING  Risk:  85 [RISKY]
      ⚠️ DETECTED: Thread in MEM_PRIVATE Executable Memory
      Reason: Thread starts in private executable memory (potential shellcode)
  [  9012] StartAddr: 0x00007FFE12340000  State: RUNNING  Risk: 100 [RISKY]
      ⚠️ DETECTED: APC Injection
      Reason: QueueUserAPC called with routine in non-system module
```

### Injection Detection (`--injections`)

```
INJECTION EVENTS (5)
================================================================================
[1] Type: Classic Injection (VirtualAllocEx -> WriteProcessMemory -> CreateRemoteThread)
    Source: explorer.exe (PID: 1000)
    Target: svchost.exe (PID: 1234)
    Risk Score: 95/100 [CRITICAL]
    Timestamp: 2024-01-15 10:30:45
    
[2] Type: Shellcode Injection
    Source: powershell.exe (PID: 5678)
    Target: powershell.exe (PID: 5678)
    Risk Score: 90/100 [CRITICAL]
    Timestamp: 2024-01-15 10:35:22
```

### Detection Results (`--detections`)

```
DETECTION RESULTS (12)
================================================================================
[1] Rule: Office → PowerShell → RWX Memory Chain
    Severity: CRITICAL (100/100)
    Process: winword.exe → powershell.exe
    Details: Classic malware execution pattern detected
    
[2] Rule: Process Hollowing Detected
    Severity: HIGH (90/100)
    Process: svchost.exe (PID: 1234)
    Details: Suspended process with unmapped image and RWX memory
```

---

## 🔍 Detection Capabilities

### Process Lineage Detection (15 Rules)

| Severity | Detection | Description | Risk Score |
|----------|-----------|-------------|------------|
| 🔴 CRITICAL | Office → Script with -enc/IEX | Office spawning PowerShell with encoded commands | +100 |
| 🔴 CRITICAL | Privilege Escalation | Unelevated process spawning elevated child | +95 |
| 🔴 CRITICAL | Long LOLBin Chain | 3+ LOLBins in process ancestry | +95 |
| 🔴 CRITICAL | Office → LOLBin → Script | Classic malware execution pattern | +98 |
| 🟠 HIGH | Office → Script/LOLBin | Office spawning PowerShell/CMD | +85 |
| 🟠 HIGH | Browser → Command Interpreter | Browser spawning CMD/PowerShell | +80 |
| 🟡 MEDIUM | LOLBin → LOLBin | Same LOLBin spawning another instance | +40 |

### Thread Risk Detection (11 Rules)

| Severity | Detection | Description | Risk Score |
|----------|-----------|-------------|------------|
| 🔴 CRITICAL | Thread in MEM_PRIVATE Executable | Shellcode indicator | 85-100 |
| 🔴 CRITICAL | APC Injection | QueueUserAPC with suspicious routine | 90 |
| 🔴 CRITICAL | Remote Thread Creation | Thread created in another process | 90 |
| 🔴 CRITICAL | Shellcode-like Thread | Thread with shellcode characteristics | 100 |
| 🟠 HIGH | Thread in RWX Memory | Readable/writable/executable memory | 85 |
| 🟠 HIGH | Start Address Not In Module | Thread outside loaded modules | 80 |

### Command Line Detection (12 Types)

| Type | Description | Risk Score |
|------|-------------|------------|
| PowerShell -enc | Encoded command | 100 |
| PowerShell IEX | Invoke-Expression | 95 |
| PowerShell DownloadString | Download cradle | 90 |
| PowerShell Reflection | Reflection abuse | 85 |
| LOLBin | Known living-off-the-land binary | 70 |
| URL | Suspicious domain detected | 60-80 |
| Base64 | Encoded payload | 75 |

### Injection Detection (20+ Types)

| Type | Description | Risk Score |
|------|-------------|------------|
| Process Hollowing | Classic hollowing pattern | 90 |
| DLL Injection | LoadLibrary in remote process | 80 |
| Remote Thread | CreateRemoteThread in another process | 85 |
| Shellcode Injection | RWX memory with high entropy | 95 |
| APC Injection | QueueUserAPC with malicious routine | 85 |
| Thread Hijacking | SetThreadContext on other thread | 90 |

### Syscall Detection (10+ Types)

| Type | Description | Risk Score |
|------|-------------|------------|
| Direct Syscall | Bypassing ntdll | 85 |
| Hooked NTDLL | Syscall stub modified | 95 |
| RWX Allocation | Executable memory allocation | 80 |
| Process Hollowing Pattern | VirtualAlloc → Write → CreateThread | 90 |
| Thread Hijacking | SetContextThread on other thread | 90 |

### Kernel Detection (20+ Types)

| Type | Description | Risk Score |
|------|-------------|------------|
| LSASS Access | LSASS memory access | 85 |
| SSDT Hook | System Service Descriptor Table modified | 95 |
| IDT Hook | Interrupt Descriptor Table modified | 95 |
| Unsigned Driver | Unsigned driver loaded | 85 |
| Rootkit Activity | Kernel-mode rootkit detected | 95 |
| Process Hollowing (Kernel) | Kernel-level hollowing | 90 |

---

## 🎛️ Engine Integration

### All Engines Working Together

When you run TelemetryExplorer, all 11 engines work together:

```
┌─────────────────────────────────────────────────────────────┐
│                    TelemetryExplorer                          │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│  │ Process  │  │ Thread   │  │ Memory   │  │ DLL      │   │
│  │ Engine   │  │ Engine   │  │ Engine   │  │ Engine   │   │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘   │
│       │             │             │             │          │
│       └─────────────┬─────────────┬─────────────┘          │
│                     ▼             ▼                          │
│              ┌─────────────────┐  ┌─────────────┐          │
│              │ Command Line    │  │ Injection   │          │
│              │ Engine          │  │ Engine      │          │
│              └─────────────────┘  └─────────────┘          │
│                     │             │                          │
│                     └─────────────┬─────────────┘          │
│                                   ▼                          │
│                    ┌────────────────────────────┐           │
│                    │        Detection Engine     │           │
│                    │  (Correlates all events)    │           │
│                    └────────────────────────────┘           │
│                                   │                          │
│              ┌────────────────────┼────────────────────┐   │
│              ▼                    ▼                    ▼    │
│       ┌─────────────┐     ┌─────────────┐     ┌─────────────┐ │
│       │ Syscall     │     │ Kernel      │     │ Forensics   │ │
│       │ Engine      │     │ Engine      │     │ Engine      │ │
│       └─────────────┘     └─────────────┘     └─────────────┘ │
│                                                                 │
│                    ┌────────────────────────────┐           │
│                    │     Production Engine       │           │
│                    │  (Logging, Rules, Agents)    │           │
│                    └────────────────────────────┘           │
└─────────────────────────────────────────────────────────────┘
```

### Data Flow

```
ETW Events → Process/Thread/Memory/DLL/Syscall Monitoring
              ↓
         Engine Databases (ProcessDB, ThreadDB, MemoryDB, etc.)
              ↓
         Detection Engine (Rule Evaluation, Correlation)
              ↓
         Forensics Engine (Snapshots, PE Reconstruction, IOC Extraction)
              ↓
         Production Engine (Logging, Remote Telemetry, Performance)
              ↓
         Output: Console / JSON / Files / Network
```

---

## 🔧 Configuration

### Visual Studio Project Configuration

1. **Required Libraries** (already configured):
   - `psapi.lib` - Process Status API
   - `wtsapi32.lib` - Windows Terminal Services API
   - `userenv.lib` - User Environment API
   - `dbghelp.lib` - Debug Help Library
   - `version.lib` - Version Information
   - `ntdll.lib` - NT DLL Library

2. **Preprocessor Definitions:**
   - `WIN32`
   - `_CONSOLE`
   - `_CRT_SECURE_NO_WARNINGS`
   - `UNICODE`
   - `_UNICODE`

3. **C++ Standard:** C++17

### Runtime Requirements

- **Administrator Privileges**: Required for:
  - Process enumeration
  - Memory access
  - Syscall monitoring
  - Kernel driver loading
  - ETW session creation

- **ETW Providers**: TelemetryExplorer uses these ETW providers:
  - Microsoft-Windows-Kernel-Process-Power
  - Microsoft-Windows-Kernel-Thread
  - Microsoft-Windows-Kernel-Memory
  - Microsoft-Windows-Kernel-Syscall
  - Microsoft-Windows-Process-Creation
  - Microsoft-Windows-Thread-Naming

---

## 📊 Performance Considerations

### Memory Usage

| Engine | Memory Usage | Notes |
|--------|--------------|-------|
| Process Intelligence | ~10-20 MB | Scales with process count |
| Thread Intelligence | ~5-10 MB | Scales with thread count |
| Memory Intelligence | ~5-15 MB | Scales with memory regions |
| Detection Engine | ~20-50 MB | Depends on rule count |
| Forensics Engine | ~10-100 MB | Memory snapshots |
| Production Engine | ~5-10 MB | Logging buffers |

**Total Estimated**: ~60-200 MB (varies by system)

### CPU Usage

| Activity | CPU Usage |
|----------|-----------|
| Idle | <1% |
| Monitoring (ETW) | 1-5% |
| Scanning | 10-30% |
| Full Analysis | 30-70% |

### Optimization Tips

1. **Use Release Build** for production (Debug has more overhead)
2. **Limit Monitoring Scope** with filters if needed
3. **Batch Operations** for better throughput
4. **Disable Unused Engines** if not needed
5. **Adjust Buffer Sizes** based on your system

---

## 🛠️ Troubleshooting

### Common Issues

#### Issue: Access Denied
**Symptoms:** Errors when enumerating processes or memory
**Solution:** Run as Administrator

```cmd
Run: cmd as Administrator, then TelemetryExplorer.exe
```

#### Issue: ETW Events Not Received
**Symptoms:** No events appearing in output
**Solution:** Check ETW session permissions, ensure admin rights

#### Issue: Compilation Errors
**Symptoms:** Build fails with C++ errors
**Solution:** 
- Ensure Visual Studio 2022 with C++17 support
- Check all file paths in project
- Clean and rebuild solution

#### Issue: Kernel Driver Loading Failed
**Symptoms:** Kernel monitoring doesn't work
**Solution:** 
- Requires signed driver
- Disable driver signature enforcement (for testing only)
- Run: `bcdedit /set testsigning on` then reboot

#### Issue: Memory Access Violations
**Symptoms:** Crash when accessing certain processes
**Solution:** 
- Some protected processes (LSASS, etc.) require special privileges
- Use SeDebugPrivilege
- Expected behavior - some processes cannot be accessed

### Debugging Options

```cmd
# Verbose output
TelemetryExplorer.exe --list-processes 2>&1 | more

# Save to file
TelemetryExplorer.exe --json > output.jsonl

# Filter by risk
TelemetryExplorer.exe --list-processes | findstr "RISK"

# Check specific process
TelemetryExplorer.exe --list-processes | findstr "1234"
```

---

## 🔒 Security Considerations

### Permissions Required

| Feature | Required Permission |
|---------|---------------------|
| Process Enumeration | PROCESS_QUERY_INFORMATION |
| Memory Reading | PROCESS_VM_READ |
| Thread Enumeration | THREAD_QUERY_INFORMATION |
| ETW Tracing | Event trace controller |
| Kernel Monitoring | SeLoadDriverPrivilege |
| Debugging | SeDebugPrivilege |

### Data Collection

TelemetryExplorer collects:
- Process information (name, path, command line, user, integrity)
- Thread information (start address, state, context)
- Memory information (regions, permissions, entropy)
- DLL/module information (name, path, load address)
- Syscall information (number, type, arguments)
- Kernel events (process/thread/image callbacks)

**No data is transmitted** over the network by default. All data stays on the local machine.

### Network Communication (Phase 11)

If you enable the Production Engine with remote telemetry:
- Agent communication uses configurable ports (default: 8080)
- SSL/TLS encryption can be enabled
- Authentication tokens are supported
- Data is only sent to configured collectors

---

## 📚 Learning Resources

### Understanding Concepts

#### Process Integrity Levels
- **System**: Highest privilege (kernel, drivers)
- **High**: Administrator
- **Medium**: Standard user
- **Low**: Restricted (Internet Explorer protected mode)
- **Untrusted**: Very restricted (sandboxed processes)

#### Memory Region Types
- **MEM_IMAGE**: Backed by a PE file (executable)
- **MEM_PRIVATE**: Private to the process (heap, stack)
- **MEM_MAPPED**: Memory-mapped file
- **RWX**: Readable, Writable, Executable (suspicious!)

#### Common LOLBins
LOLBins (Living Off The Land Binaries) are legitimate tools that can be abused:
- **PowerShell**: Script execution
- **cmd.exe**: Command execution
- **rundll32**: Load and execute DLLs
- **regsvr32**: Register COM servers
- **mshta**: HTML Application execution
- **certutil**: Certificate utility (can decode base64)
- **bitsadmin**: Background Intelligent Transfer Service

### Attack Patterns Detected

#### Code Injection
```
Attacker Process → VirtualAllocEx (RWX) → WriteProcessMemory → CreateRemoteThread
                    ↓
              Target Process (now running malicious code)
```

#### Process Hollowing
```
1. CreateProcess (suspended)
2. VirtualAllocEx (RWX in target)
3. WriteProcessMemory (malicious payload)
4. ResumeThread (execute payload)
```

#### Privilege Escalation
```
Low Integrity Process → High Integrity Process
   (e.g., browser → system process)
```

#### APC Injection
```
1. OpenProcess (target)
2. VirtualAllocEx (RWX in target)
3. WriteProcessMemory (shellcode)
4. QueueUserAPC (with shellcode address)
5. NtTestAlert (trigger APC execution)
```

#### Syscall Hooking
```
Original: ntdll!NtCreateThreadEx → kernel32!CreateThreadEx

Hooked: ntdll!NtCreateThreadEx → Malicious code → kernel32!CreateThreadEx
                            (JMP, CALL, or inline hook)
```

---

## 🎓 Example Scenarios

### Scenario 1: Detecting a PowerShell Attack

**Attack:**
```powershell
powershell -enc JABzAGUAcgB... (base64 encoded malicious payload)
```

**Detection:**
```
TelemetryExplorer.exe --list-processes
```

**Output:**
```
[ 5678] powershell.exe          Session:   1  User: DOMAIN\User1
    ⚠️ DETECTED: PowerShell with -enc flag [CRITICAL RISK +100]
    ⚠️ DETECTED: Office Spawning Script/LOLBin [HIGH RISK +85]
    ⚠️ TOTAL RISK SCORE: 185 (capped at 100)
```

**Investigation:**
```cmd
# Get command line
TelemetryExplorer.exe --cmdlines | findstr "5678"

# Check threads
TelemetryExplorer.exe --list-threads | findstr "5678"

# Check memory regions
TelemetryExplorer.exe --memory-snapshot 5678
```

### Scenario 2: Detecting Process Injection

**Attack:**
```c
// Malicious code injecting into svchost
VirtualAllocEx(hTarget, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
WriteProcessMemory(hTarget, pRemote, payload, size, NULL);
CreateRemoteThread(hTarget, NULL, 0, pRemote, NULL, 0, NULL);
```

**Detection:**
```
TelemetryExplorer.exe --injections
```

**Output:**
```
[1] Type: Classic Injection
    Source: malicious.exe (PID: 9999)
    Target: svchost.exe (PID: 1234)
    Risk Score: 95/100 [CRITICAL]
    API Chain: VirtualAllocEx → WriteProcessMemory → CreateRemoteThread
```

**Investigation:**
```cmd
# Get process info
TelemetryExplorer.exe --list-processes | findstr "9999"

# Get thread info
TelemetryExplorer.exe --list-threads | findstr "1234"

# Check syscalls
TelemetryExplorer.exe --syscalls | findstr "1234"
```

### Scenario 3: Detecting Hooked Syscalls

**Attack:**
```c
// Malware hooks NtCreateThreadEx
unsigned char patch[] = { 0xE9, ... }; // JMP to malicious code
WriteProcessMemory(GetModuleHandle("ntdll"), NtCreateThreadEx, patch, sizeof(patch), NULL);
```

**Detection:**
```
TelemetryExplorer.exe --scan-hooks
```

**Output:**
```
Syscall Hook Detection Results:
================================================================================
[1] Syscall: NtCreateThreadEx (0x24)
    Status: HOOKED
    Hook Type: JMP (0xE9)
    Original Address: 0x7FFExxxxxxxx
    Hooked Address: 0x0000yyyyyyyy
    Risk Score: 95/100 [CRITICAL]
```

---

## 📝 JSON Output Format

When using `--json` flag, all output is formatted as JSON Lines (one JSON object per line).

### Process Event
```json
{
  "event_type": "process_create",
  "timestamp": "2024-01-15T10:30:45.1234567Z",
  "pid": 1234,
  "ppid": 1000,
  "name": "powershell.exe",
  "path": "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe",
  "command_line": "powershell -enc JABzAGUAcgB...",
  "user": "DOMAIN\\User1",
  "session": 1,
  "integrity": "Medium",
  "elevated": false,
  "risk_score": 85,
  "tags": ["script_host", "lolbin"],
  "detections": [
    {
      "type": "PowerShell with -enc flag",
      "severity": "CRITICAL",
      "score": 100
    }
  ]
}
```

### Injection Event
```json
{
  "event_type": "injection_detected",
  "timestamp": "2024-01-15T10:35:22.6543210Z",
  "injection_type": "Classic Injection",
  "source_pid": 9999,
  "source_name": "malicious.exe",
  "target_pid": 1234,
  "target_name": "svchost.exe",
  "api_chain": ["VirtualAllocEx", "WriteProcessMemory", "CreateRemoteThread"],
  "risk_score": 95,
  "severity": "CRITICAL"
}
```

### Detection Event
```json
{
  "event_type": "detection",
  "timestamp": "2024-01-15T10:40:00.0000000Z",
  "detection_id": "detection-001",
  "rule_id": "office-ps-enc-rwx",
  "rule_name": "Office → PowerShell → RWX Memory Chain",
  "severity": "CRITICAL",
  "risk_score": 100,
  "process_chain": [
    {"pid": 1000, "name": "winword.exe"},
    {"pid": 5678, "name": "powershell.exe"}
  ],
  "matched_conditions": [
    "office_process",
    "spawned_powershell",
    "rwx_memory",
    "encoded_command"
  ]
}
```

---

## 🔗 Integration with Other Tools

### Piping to Python
```python
import json
import sys

for line in sys.stdin:
    event = json.loads(line)
    if event.get("risk_score", 0) > 70:
        print(f"ALERT: {event}")
        # Send to SIEM, etc.
```

**Usage:**
```cmd
TelemetryExplorer.exe --json | python alert.py
```

### Piping to Elasticsearch
```cmd
TelemetryExplorer.exe --json | curl -X POST http://localhost:9200/telemetry/_doc -d @-
```

### Piping to PowerShell
```powershell
TelemetryExplorer.exe --json | ForEach-Object { $_ | ConvertFrom-Json | Where-Object { $_.risk_score -gt 70 } | Format-Table }
```

### Piping to File
```cmd
# Save to file
TelemetryExplorer.exe --json > telemetry_$(date +%Y%m%d_%H%M%S).jsonl

# Rotate logs
TelemetryExplorer.exe --json >> telemetry.log 2>&1
```

---

## 📈 Advanced Usage

### Continuous Monitoring

Create a batch file `monitor.bat`:
```batch
@echo off
:loop
TelemetryExplorer.exe --json >> telemetry_%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%.log 2>&1
ping -n 60 127.0.0.1 > nul
goto loop
```

### Scheduled Scans

Use Windows Task Scheduler to run scans periodically:
```cmd
# Create task
schtasks /create /tn "TelemetryScan" /tr "C:\path\to\TelemetryExplorer.exe --scan-detect" /sc hourly /mo 1

# Start task
schtasks /start /tn "TelemetryScan"
```

### Remote Monitoring

```cmd
# On collector machine
TelemetryExplorer.exe --prod-start --log-output C:\logs

# On target machine (Phase 11)
# Configure agent to connect to collector
```

---

## 🎯 Best Practices

### For Security Analysts

1. **Start with `--list-processes`** to see what's running
2. **Check `--tree`** for suspicious process hierarchies
3. **Use `--cmdlines`** to see command line arguments
4. **Run `--scan-inject`** to check for code injection
5. **Check `--syscalls`** for suspicious syscalls
6. **Use `--detections`** for automated threat detection
7. **Take snapshots** of suspicious processes with `--memory-snapshot <pid>`

### For Incident Response

1. **Capture full system state:**
   ```cmd
   TelemetryExplorer.exe --json > incident_$(date +%s).jsonl
   ```

2. **Analyze suspicious process:**
   ```cmd
   TelemetryExplorer.exe --memory-snapshot <pid>
   TelemetryExplorer.exe --pe-reconstruct <pid>
   TelemetryExplorer.exe --ioc-extract <pid>
   ```

3. **Check for persistence:**
   ```cmd
   TelemetryExplorer.exe --list-processes | findstr "RUN"
   TelemetryExplorer.exe --list-sessions
   ```

4. **Check for lateral movement:**
   ```cmd
   TelemetryExplorer.exe --list-processes | findstr "PsExec\|WMI\|SMB"
   ```

### For Developers

1. **Extend detection rules:**
   - Add custom rules in `detection_intelligence/detection_engine.hpp`
   - Modify `EvaluateAllDetectionRules()` function

2. **Add new monitoring:**
   - Create new ETW monitors in appropriate engine
   - Add to `main.cpp` initialization

3. **Integrate with your tools:**
   - Use JSON output mode
   - Parse JSON events in your application
   - Use helper functions from `telemetry.hpp`

---

## 📊 Statistics and Metrics

### Engine Statistics

To get statistics for all engines:
```cmd
TelemetryExplorer.exe --list-processes  # Shows process count
TelemetryExplorer.exe --list-threads    # Shows thread count
TelemetryExplorer.exe --injection-dump # Shows injection stats
TelemetryExplorer.exe --syscall-dump   # Shows syscall stats
TelemetryExplorer.exe --detection-dump # Shows detection stats
TelemetryExplorer.exe --forensic-dump  # Shows forensic stats
TelemetryExplorer.exe --logs-dump      # Shows logging stats
```

### Performance Metrics

```cmd
TelemetryExplorer.exe --prod-status
```

Shows:
- Total log entries processed
- Rules evaluated and matched
- Messages processed
- Agents connected
- Processing throughput
- Memory usage

---

## 🔄 Update and Maintenance

### Checking for Updates

This is the complete implementation with all 11 phases. Future updates will be released as new versions.

### Clean Build

If you encounter issues:
```cmd
# Clean solution
delete /s /q TelemetryExplorer\x64\Release\*.*

# Rebuild
msbuild TelemetryExplorer.sln /p:Configuration=Release /p:Platform=x64
```

### Backup Configuration

Configuration is stored in:
- Project files (`.vcxproj`)
- Source files (`.hpp`, `.cpp`)

Backup the entire `TelemetryExplorer` directory.

---

## 📜 License

TelemetryExplorer is provided for educational and security research purposes.

**Important:**
- Only use on systems you own or have permission to monitor
- Do not use for malicious purposes
- Respect privacy and legal boundaries
- Compliance with local laws and regulations is your responsibility

---

## 🙏 Support and Contributions

### Reporting Issues

If you encounter bugs or have questions:
1. Check this usage guide
2. Check the IMPLEMENTATION_SUMMARY.md for technical details
3. Review the README.md files in each engine directory

### Contributing

Contributions are welcome:
- Bug fixes
- New detection rules
- Performance improvements
- Documentation updates
- New features (Phase 12+)

### Architecture Notes

All engines follow the same pattern:
1. **Info** (.hpp/.cpp): Core structures and types
2. **Database** (.hpp/.cpp): Thread-safe data storage
3. **Monitor** (.hpp/.cpp): ETW-based event monitoring
4. **Detector/Analyzer** (.hpp/.cpp): Detection logic (where applicable)
5. **README.md**: Engine-specific documentation

---

## 🎉 Conclusion

TelemetryExplorer provides comprehensive system visibility across 11 intelligence engines, making it a powerful tool for:
- **Threat Detection** - Identify malicious activity
- **Incident Response** - Investigate security incidents
- **Forensic Analysis** - Analyze compromised systems
- **Security Research** - Study attack techniques
- **System Monitoring** - Understand system behavior

**All 11 Phases Complete!** ✅

Start exploring with:
```cmd
TelemetryExplorer.exe --help
TelemetryExplorer.exe --list-processes
TelemetryExplorer.exe --tree
TelemetryExplorer.exe --scan-detect
```

Happy hunting! 🔍
