#include <windows.h>
#include <psapi.h>
#include <iostream>

#include "telemetry.hpp"

#pragma comment(lib, "Psapi.lib")

std::vector<ModuleInfo> g_Modules;

void EnumerateModules(HANDLE hProcess)
{
    g_Modules.clear();

    HMODULE hMods[1024];

    DWORD cbNeeded;

    if (EnumProcessModules(
        hProcess,
        hMods,
        sizeof(hMods),
        &cbNeeded))
    {
        std::wcout
            << L"\n========== MODULES =========="
            << std::endl;

        for (unsigned int i = 0;
            i < (cbNeeded / sizeof(HMODULE));
            i++)
        {
            WCHAR moduleName[MAX_PATH];

            MODULEINFO modInfo;

            if (GetModuleBaseNameW(
                hProcess,
                hMods[i],
                moduleName,
                MAX_PATH))
            {
                GetModuleInformation(
                    hProcess,
                    hMods[i],
                    &modInfo,
                    sizeof(modInfo));

                std::wcout
                    << L"\nModule Name : "
                    << moduleName
                    << std::endl;

                std::wcout
                    << L"Base Address: "
                    << modInfo.lpBaseOfDll
                    << std::endl;

                std::wcout
                    << L"Image Size  : "
                    << modInfo.SizeOfImage / 1024
                    << L" KB"
                    << std::endl;

                ModuleInfo mod;

                mod.name = moduleName;

                mod.base =
                    (uintptr_t)modInfo.lpBaseOfDll;

                mod.size =
                    modInfo.SizeOfImage;

                g_Modules.push_back(mod);
            }
        }
    }
}

bool IsMemoryBackedByModule(
    uintptr_t address)
{
    for (auto& mod : g_Modules)
    {
        uintptr_t start = mod.base;

        uintptr_t end =
            mod.base + mod.size;

        if (address >= start &&
            address < end)
        {
            return true;
        }
    }

    return false;
}
