#include<Windows.h>
#include"MyCheatWindow.h"
#include<memory>
#include"MemoryManger.h"
#include<cstdint>
#include"Offset.h"





int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*DWORD pid = 0;
	HWND hwnd = FindWindowA("UnrealWindow", nullptr);
	if (!hwnd)
	{
		OutputDebugString(L"Find Window failed\n");
		return 0x00;
	}

	GetWindowThreadProcessId(hwnd, &pid);

	if (!pid)
	{
		OutputDebugString(L"Get process id failed\n");
		return -2;
	}
	MemoryManger::getInstance()->initialize(pid);	*/

	std::unique_ptr<MyCheatWindow> window = std::make_unique<MyCheatWindow>();

	window->Run();
	return 0;
}
