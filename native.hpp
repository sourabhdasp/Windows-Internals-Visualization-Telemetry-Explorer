#pragma once

#include <windows.h>



typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;

} UNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;

    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;

    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;

} LDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA
{
    ULONG Length;

    BOOLEAN Initialized;

    HANDLE SsHandle;

    LIST_ENTRY InLoadOrderModuleList;

} PEB_LDR_DATA;

typedef struct _PEB
{
    BYTE Reserved1[2];

    BYTE BeingDebugged;

    BYTE Reserved2[1];

    PVOID Reserved3[2];

    PEB_LDR_DATA* Ldr;

} PEB;

typedef struct _PROCESS_BASIC_INFORMATION
{
    PVOID Reserved1;

    PVOID PebBaseAddress;

    PVOID Reserved2[2];

    ULONG_PTR UniqueProcessId;

    PVOID Reserved3;

} PROCESS_BASIC_INFORMATION;

typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)
(
    HANDLE,
    ULONG,
    PVOID,
    ULONG,
    PULONG
    );

typedef NTSTATUS(NTAPI* pNtQueryInformationThread)
(
    HANDLE,
    LONG,
    PVOID,
    ULONG,
    PULONG
    );
