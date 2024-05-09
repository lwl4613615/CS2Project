#pragma once
#include"ImguiWindow.h"

class MyCheatWindow : public ImGuiApp
{
public:
	MyCheatWindow();
	
protected:
	void RenderContents() override;
private :
	void DrawMenu();
	
};