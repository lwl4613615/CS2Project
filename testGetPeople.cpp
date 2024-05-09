// testGetPeople.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<windows.h>
#include <psapi.h>
DWORD pid = 0;

ULONG_PTR GetModuleBaseAddress(DWORD pid, const wchar_t* modName)
{
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProc != NULL)
    {
        HMODULE hMods[1024];
        DWORD cbNeeded;
        if (EnumProcessModules(hProc, hMods, sizeof(hMods), &cbNeeded))
        {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];
                if (GetModuleBaseName(hProc, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    if (_wcsicmp(modName, szModName) == 0)
                    {
                        CloseHandle(hProc);
                        return (ULONG_PTR)hMods[i];
                    }
                }
            }
        }
    }
    CloseHandle(hProc);
    return 0;
}
uint32_t offset = 0x18C7F88;
int main()
{
    HWND hWindow = NULL;
    hWindow = FindWindow(L"SDL_app", L"Counter-Strike 2");

    GetWindowThreadProcessId(hWindow, &pid);
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    auto client = GetModuleBaseAddress(pid, L"client.dll");

   
    uint64_t base = 0;
     DWORD PAWNOFFSET = 0x7e4;
     for (uint64_t i = 0; i < 64; i++)
     {

        
         ReadProcessMemory(process, (LPVOID)(client + offset), &base, sizeof(base), nullptr);
         if (!base) break;
         uint64_t list_entry=0;
         ReadProcessMemory(process, LPVOID(base + 0x8 * ((i & 0x7FFF) >> 9) + 16), &list_entry, sizeof(list_entry), nullptr);
         if (!list_entry) continue;
         uint64_t entity=0;
         ReadProcessMemory(process, LPVOID(list_entry+0x78*(i&0x1ff)), &entity, sizeof(entity), nullptr);
         if (!entity) continue;
         uint32_t teamNum;
         ReadProcessMemory(process, LPVOID(entity+0x3cb), &teamNum, sizeof(teamNum), nullptr);
         uint32_t playerPawn=0;
         ReadProcessMemory(process, LPVOID(entity + PAWNOFFSET), &playerPawn, sizeof(playerPawn), nullptr);
         if (!playerPawn) continue;
         uint64_t list_entry2=0; 
         ReadProcessMemory(process, LPVOID(base + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16), &list_entry2, sizeof(list_entry2), nullptr);
         if (!list_entry2) continue;

         uint64_t pCSPlayerPawn = 0;
         ReadProcessMemory(process, LPVOID(list_entry2 + 0x78 * (playerPawn & 0x1ff)), &pCSPlayerPawn, sizeof(pCSPlayerPawn), nullptr);
         if (!pCSPlayerPawn)continue;
         DWORD health = 0;

         ReadProcessMemory(process, LPVOID(pCSPlayerPawn + 0x334), &health, sizeof(health), nullptr);

         printf("%d player objaddress: %llx controller address:%llx health:%d team:%d\n", i, pCSPlayerPawn, entity, health, teamNum);
     }

    return 0;

}
