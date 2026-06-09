@echo off
REM TelemetryExplorer Build Script for cl.exe
REM This script compiles the project with proper C++17 flags and Windows SDK

SETLOCAL

REM Set compiler flags
set CL_FLAGS=/std:c++17 /EHsc /W4 /WX /Zi /nologo /c /DWIN32 /D_WINDOWS /D_DEBUG /D_CONSOLE /DUNICODE /D_UNICODE

REM Set include directories
set INCLUDES=/I." /I"TelemetryExplorer" /I"%%WindowsSdkDir%%Include\%%WindowsSDKVersion%%\ucrt" /I"%%WindowsSdkDir%%Include\%%WindowsSDKVersion%%\um" /I"%%WindowsSdkDir%%Include\%%WindowsSDKVersion%%\winrt" /I"%%WindowsSdkDir%%Include\%%WindowsSDKVersion%%\cppwinrt"

REM Set linker flags
set LINK_FLAGS=/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup /DEBUG /OUT:TelemetryExplorer.exe

REM Set libraries
set LIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

REM Create build directory
if not exist "build" mkdir build

REM Compile all source files
echo Compiling TelemetryExplorer...

REM Main files
cl %CL_FLAGS% %INCLUDES% /Fobuild\main.obj main.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\detection.obj detection.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\etw.obj etw.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\gui.obj gui.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\memory.obj memory.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\modules.obj modules.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\peb.obj peb.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\process.obj process.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\threades.obj threades.cpp

REM Process Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\process_intelligence\process_info.obj process_intelligence\process_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\process_intelligence\process_database.obj process_intelligence\process_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\process_intelligence\lineage_detector.obj process_intelligence\lineage_detector.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\process_intelligence\session_tracker.obj process_intelligence\session_tracker.cpp

REM Thread Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\thread_intelligence\thread_info.obj thread_intelligence\thread_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\thread_intelligence\thread_database.obj thread_intelligence\thread_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\thread_intelligence\thread_monitor.obj thread_intelligence\thread_monitor.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\thread_intelligence\stack_walker.obj thread_intelligence\stack_walker.cpp

REM Memory Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\memory_intelligence\memory_info.obj memory_intelligence\memory_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\memory_intelligence\memory_database.obj memory_intelligence\memory_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\memory_intelligence\memory_monitor.obj memory_intelligence\memory_monitor.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\memory_intelligence\entropy_analysis.obj memory_intelligence\entropy_analysis.cpp

REM DLL Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\dll_intelligence\dll_info.obj dll_intelligence\dll_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\dll_intelligence\dll_database.obj dll_intelligence\dll_database.cpp

REM Command Line Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\commandline_intelligence\commandline_info.obj commandline_intelligence\commandline_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\commandline_intelligence\commandline_database.obj commandline_intelligence\commandline_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\commandline_intelligence\commandline_analyzer.obj commandline_intelligence\commandline_analyzer.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\commandline_intelligence\commandline_monitor.obj commandline_intelligence\commandline_monitor.cpp

REM Injection Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\injection_intelligence\injection_info.obj injection_intelligence\injection_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\injection_intelligence\injection_database.obj injection_intelligence\injection_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\injection_intelligence\injection_detector.obj injection_intelligence\injection_detector.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\injection_intelligence\injection_monitor.obj injection_intelligence\injection_monitor.cpp

REM Syscall Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\syscall_intelligence\syscall_info.obj syscall_intelligence\syscall_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\syscall_intelligence\syscall_database.obj syscall_intelligence\syscall_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\syscall_intelligence\syscall_monitor.obj syscall_intelligence\syscall_monitor.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\syscall_intelligence\syscall_detector.obj syscall_intelligence\syscall_detector.cpp

REM Kernel Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\kernel_intelligence\kernel_info.obj kernel_intelligence\kernel_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\kernel_intelligence\kernel_database.obj kernel_intelligence\kernel_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\kernel_intelligence\kernel_monitor.obj kernel_intelligence\kernel_monitor.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\kernel_intelligence\kernel_detector.obj kernel_intelligence\kernel_detector.cpp

REM Detection Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\detection_intelligence\detection_info.obj detection_intelligence\detection_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\detection_intelligence\detection_database.obj detection_intelligence\detection_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\detection_intelligence\detection_engine.obj detection_intelligence\detection_engine.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\detection_intelligence\detection_correlator.obj detection_intelligence\detection_correlator.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\detection_intelligence\detection_graph.obj detection_intelligence\detection_graph.cpp

REM Forensics Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\forensics_intelligence\forensics_info.obj forensics_intelligence\forensics_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\forensics_intelligence\forensics_database.obj forensics_intelligence\forensics_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\forensics_intelligence\forensics_monitor.obj forensics_intelligence\forensics_monitor.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\forensics_intelligence\forensics_extractor.obj forensics_intelligence\forensics_extractor.cpp

REM Production Intelligence
cl %CL_FLAGS% %INCLUDES% /Fobuild\production_intelligence\production_info.obj production_intelligence\production_info.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\production_intelligence\production_database.obj production_intelligence\production_database.cpp
cl %CL_FLAGS% %INCLUDES% /Fobuild\production_intelligence\production_monitor.obj production_intelligence\production_monitor.cpp

REM Link all object files
echo Linking...
link /SUBSYSTEM:CONSOLE /DEBUG /OUT:TelemetryExplorer.exe \
  build\*.obj \
  %LIBS%

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    TelemetryExplorer.exe
) else (
    echo Build failed with error %ERRORLEVEL%
)
