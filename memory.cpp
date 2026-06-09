#include <windows.h>
#include <iostream>
#include <winnt.h>

#include "telemetry.hpp"

void DumpMemoryBytes(
    HANDLE hProcess,
    LPVOID address)
{
    BYTE buffer[32];

    SIZE_T bytesRead;

    if (ReadProcessMemory(
        hProcess,
        address,
        buffer,
        sizeof(buffer),
        &bytesRead))
    {
        std::wcout
            << L"First Bytes : ";

        for (SIZE_T i = 0;
            i < bytesRead;
            i++)
        {
            printf("%02X ", buffer[i]);
        }

        std::wcout << std::endl;
    }
}

void ParsePEHeader(
    HANDLE hProcess,
    LPVOID baseAddress)
{
    IMAGE_DOS_HEADER dosHeader;

    SIZE_T bytesRead;

    if (!ReadProcessMemory(
        hProcess,
        baseAddress,
        &dosHeader,
        sizeof(dosHeader),
        &bytesRead))
    {
        return;
    }

    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        return;
    }

    std::wcout
        << L"[+] VALID DOS HEADER"
        << std::endl;

    IMAGE_NT_HEADERS ntHeaders;

    LPVOID ntHeaderAddress =
        (LPBYTE)baseAddress +
        dosHeader.e_lfanew;

    if (!ReadProcessMemory(
        hProcess,
        ntHeaderAddress,
        &ntHeaders,
        sizeof(ntHeaders),
        &bytesRead))
    {
        return;
    }

    if (ntHeaders.Signature != IMAGE_NT_SIGNATURE)
    {
        return;
    }

    std::wcout
        << L"[+] VALID PE HEADER"
        << std::endl;

    std::wcout
        << L"Sections     : "
        << ntHeaders.FileHeader.NumberOfSections
        << std::endl;
}

void DumpRegionToDisk(
    HANDLE hProcess,
    LPVOID baseAddress,
    SIZE_T regionSize)
{
    BYTE* buffer =
        new BYTE[regionSize];

    SIZE_T bytesRead;

    if (ReadProcessMemory(
        hProcess,
        baseAddress,
        buffer,
        regionSize,
        &bytesRead))
    {
        WCHAR filename[MAX_PATH];

        swprintf_s(
            filename,
            L"dumps\\dump_%p.bin",
            baseAddress);

        HANDLE hFile =
            CreateFileW(
                filename,
                GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

        if (hFile != INVALID_HANDLE_VALUE)
        {
            DWORD written;

            WriteFile(
                hFile,
                buffer,
                (DWORD)bytesRead,
                &written,
                NULL);

            CloseHandle(hFile);

            std::wcout
                << L"[+] Dumped -> "
                << filename
                << std::endl;
        }
    }

    delete[] buffer;
}

void EnumerateMemory(HANDLE hProcess)
{
    MEMORY_BASIC_INFORMATION mbi;

    unsigned char* addr = 0;

    while (VirtualQueryEx(
        hProcess,
        addr,
        &mbi,
        sizeof(mbi)))
    {
        if (!(mbi.Protect & PAGE_EXECUTE) &&
            !(mbi.Protect & PAGE_EXECUTE_READ) &&
            !(mbi.Protect & PAGE_EXECUTE_READWRITE))
        {
            addr += mbi.RegionSize;

            continue;
        }

        std::wcout
            << L"\n========== EXECUTABLE MEMORY =========="
            << std::endl;

        std::wcout
            << L"Base Address : "
            << mbi.BaseAddress
            << std::endl;

        std::wcout
            << L"Region Size  : "
            << mbi.RegionSize / 1024
            << L" KB"
            << std::endl;

        DumpMemoryBytes(
            hProcess,
            mbi.BaseAddress);

        ParsePEHeader(
            hProcess,
            mbi.BaseAddress);

        if (!IsMemoryBackedByModule(
            (uintptr_t)mbi.BaseAddress))
        {
            std::wcout
                << L"[!!!] EXECUTABLE MEMORY NOT BACKED BY MODULE"
                << std::endl;
        }

        if (mbi.Type == MEM_PRIVATE)
        {
            std::wcout
                << L"[!!!] PRIVATE EXECUTABLE MEMORY"
                << std::endl;

            DumpRegionToDisk(
                hProcess,
                mbi.BaseAddress,
                mbi.RegionSize);
        }

        addr += mbi.RegionSize;
    }
}
