#pragma once
#include <Windows.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include <d3d11.h>
#include"Imgui/font.h"
#include<string>
#include <algorithm>
#include <random>
class ImGuiApp {
public:
    ImGuiApp() = default; 

    void Run() {
        // 初始化DirectX 11和窗口
        // ...
        do
        {
            bool result;
            result = CreateMainWindow();
            if (!result)
            {
                OutputDebugStringW(L"CreateMainWindow failed\n");
                return;
            }
            result = CreateDeviceD3D();
            if (!result)
            {
                OutputDebugStringW(L"CreateD3ddevice failed!\n");
                return;
            }
            // Show the window
            ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
            ::UpdateWindow(m_hwnd);

        } while (false);
        ImGui_ImplWin32_EnableAlphaCompositing(m_hwnd);

        LONG exStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
        exStyle |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
        SetWindowLong(m_hwnd, GWL_EXSTYLE, exStyle);//鼠标穿透

        // 初始化ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
       // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //ImGui::StyleColorsDark();
        SetStyle();
        io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 16.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        
        ImGui_ImplWin32_Init(m_hwnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
        ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        MSG msg{};
        // 渲染循环
        while (msg.message != WM_QUIT) {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }

            WindowResize();

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//设置优先级别最高
            RenderContents();

            ImGui::Render();
            g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
            g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            // Update and Render additional Platform Windows
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
            g_pSwapChain->Present(1, 0); // Present with vsync
        }

        // 清理
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        ::DestroyWindow(m_hwnd);
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
        
    }

protected:
    virtual void RenderContents() = 0;
    
    inline std::string generate_random_class_name(size_t length) {
        // 定义字符集
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        // 创建随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, charset.size() - 1);

        // 生成随机窗口类名
        std::string className;
        for (int i = 0; i < length; ++i) {
            className += charset[distr(gen)];
        }

        return className;
    }
private:
    bool CreateMainWindow();
    bool CreateDeviceD3D();
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    void WindowResize();
    void SetStyle();
    
private:
  
    //about DirectX 11
private:
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;

    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    HWND m_hwnd = nullptr;
    WNDCLASSEXA wc;
};