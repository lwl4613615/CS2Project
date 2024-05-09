#include"MyCheatWindow.h"
#include<windows.h>
#include <string>
#include<chrono>
MyCheatWindow::MyCheatWindow()
{
	
}
void MyCheatWindow::RenderContents()
{
	DrawMenu();
}
void inputHandler() {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	for (int i = 1; i < 5; i++) {
		io.MouseDown[i] = false;
	}
}

void MyCheatWindow::DrawMenu()
{
	
	ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(),20.0f,{ 0,0 }, IM_COL32(255, 0, 0, 255), "lwl's gift");
	ImGuiWindowClass noAutoMerge;
	noAutoMerge.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoAutoMerge;
	ImGui::SetNextWindowClass(&noAutoMerge);//自动脱离渲染窗口
	
	std::u8string title = u8"战神龙辅助";
	// 设置窗口置于最顶层	
	ImGui::Begin(reinterpret_cast<const char*>(title.c_str()));
	static bool v=false;
	ImGui::Checkbox("dasdasda", &v);
	ImGui::Text("Hello, world!");
	ImGui::End();
	
}
