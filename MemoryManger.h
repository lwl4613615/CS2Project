#pragma once
#include <mutex>
#include<windows.h>
#include<vector>
#include <tlhelp32.h>
#include <tchar.h>
class MemoryManger
{   
    private:
        static MemoryManger* instance;
        static std::once_flag initInstanceFlag;
        MemoryManger() = default;
		HANDLE process;
        DWORD m_pid;
       

    public:
        static MemoryManger* getInstance() {
            std::call_once(initInstanceFlag, []() {
                instance = new MemoryManger();
                });
            return instance;
        }
		template<typename T>
		T ReadMemory (ULONG64 address) {
			T value;
#ifndef _USE_DRIVER

            ReadProcessMemory(process, (LPVOID)address, &value, sizeof(T), 0);
#else
			Api::DdReadData(process, address, &value, sizeof(T));
#endif // !

			return value;
		}
        template<typename T>
        std::vector<T> ReadMemory(ULONG64 address, ULONG64 size)
        {
			std::vector<T> value(size);
#ifndef _USE_DRIVER
			ReadProcessMemory(process, (LPVOID)address, value.data(), sizeof(T) * size, 0);
#else
			Api::DdReadData(process, address, value.data(), sizeof(T) * size);
#endif // !
			return value;
        }
        template<typename T>
		void WriteMemory(ULONG64 address, T value) {
#ifndef _USE_DRIVER
			WriteProcessMemory(process, (LPVOID)address, &value, sizeof(T), 0);
#else
			Api::DdWriteData(process, address, &value, sizeof(T));
#endif // !
		}

		template<typename T>
        void WriteMemory(ULONG64 address, std::vector<T> value) {
#ifndef _USE_DRIVER
			WriteProcessMemory(process, (LPVOID)address, value.data(), sizeof(T) * value.size(), 0);
#else
			Api::DdWriteData(process, address, value.data(), sizeof(T) * value.size());
#endif // !
        }

		HMODULE GetModule(const char* moduleName,PULONG64 moduleSize) {
#ifndef _USE_DRIVER
            HMODULE base = 0;
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_pid);
            if (snapshot == INVALID_HANDLE_VALUE) {
                return 0;
            }
            MODULEENTRY32 moduleEntry;
            moduleEntry.dwSize = sizeof(moduleEntry);
            if (Module32First(snapshot, &moduleEntry)) {
                do {
                    char szModuleA[MAX_MODULE_NAME32 + 1];
                    WideCharToMultiByte(CP_ACP, 0, moduleEntry.szModule, -1, szModuleA, sizeof(szModuleA), NULL, NULL);
                    if (_stricmp(szModuleA, moduleName) == 0) {
                        base = (HMODULE)moduleEntry.modBaseAddr;
                        *moduleSize = moduleEntry.modBaseSize;
                        CloseHandle(snapshot);
                        return base;
                    }
                } while (Module32Next(snapshot, &moduleEntry));
        }

#else
            HMODULE  base =(HMODULE)Api::DdGetModuleDll(process, (char*)moduleName, moduleSize);
#endif // !
			return base;
		}



        void initialize(DWORD pid) {
            static bool initialized = false;
            if (!initialized) {
                m_pid = pid;
#ifndef _USE_DRIVER               
				process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
#else
				process = UlongToHandle(pid);
#endif // !
                initialized = true;
            }
        }



};

