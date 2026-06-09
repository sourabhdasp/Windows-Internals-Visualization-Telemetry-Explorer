#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

#include "telemetry.hpp"
#include "native.hpp"

void EnumerateThreads(DWORD pid)
{
    HANDLE snapshot =
        CreateToolhelp32Snapshot(
            TH32CS_SNAPTHREAD,
            0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    THREADENTRY32 te;

    te.dwSize = sizeof(THREADENTRY32);

    HMODULE hNtdll =
        GetModuleHandleW(L"ntdll.dll");

    if (!hNtdll)
    {
        CloseHandle(snapshot);

        return;
    }

    pNtQueryInformationThread
        NtQueryInformationThread =
        (pNtQueryInformationThread)
        GetProcAddress(
            hNtdll,
            "NtQueryInformationThread");

    if (!NtQueryInformationThread)
    {
        CloseHandle(snapshot);

        return;
    }

    std::wcout
        << L"\n========== THREAD ANALYSIS =========="
        << std::endl;

    if (Thread32First(snapshot, &te))
    {
        do
        {
            if (te.th32OwnerProcessID != pid)
            {
                continue;
            }

            HANDLE hThread =
                OpenThread(
                    THREAD_QUERY_INFORMATION,
                    FALSE,
                    te.th32ThreadID);

            if (!hThread)
            {
                continue;
            }

            PVOID startAddress = nullptr;

            NTSTATUS status =
                NtQueryInformationThread(
                    hThread,
                    9,
                    &startAddress,
                    sizeof(startAddress),
                    NULL);

            if (status == 0)
            {
                std::wcout
                    << L"\nThread ID    : "
                    << te.th32ThreadID
                    << std::endl;

                std::wcout
                    << L"StartAddress : "
                    << startAddress
                    << std::endl;

                if (!IsMemoryBackedByModule(
                    (uintptr_t)startAddress))
                {
                    std::wcout
                        << L"[!!!] THREAD EXECUTING OUTSIDE MODULE"
                        << std::endl;
                }
            }

            CloseHandle(hThread);

        } while (Thread32Next(snapshot, &te));
    }

    CloseHandle(snapshot);
}
