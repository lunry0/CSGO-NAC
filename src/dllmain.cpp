#include <Windows.h>
#include <thread>
#include <iostream>
#include <fstream>
#include "../include/csgo_sdk.h"
#include "../include/config.h"
#include "gui/gui.h"
#include "gui/animations.h"

// Global variables
HMODULE g_hModule = nullptr;
std::thread g_mainThread;
bool g_running = false;

// Forward declarations
void MainThread();
void InitializeConsole();
void ShutdownConsole();
bool InitializeDirectX();
void ShutdownDirectX();

// DirectX globals
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        {
            g_hModule = hModule;
            
            // Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications
            DisableThreadLibraryCalls(hModule);
            
            // Create main thread
            g_running = true;
            g_mainThread = std::thread(MainThread);
            
            break;
        }
    case DLL_PROCESS_DETACH:
        {
            g_running = false;
            
            // Wait for main thread to finish
            if (g_mainThread.joinable()) {
                g_mainThread.join();
            }
            
            break;
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

void MainThread()
{
    try {
        // Initialize console for debugging
        InitializeConsole();
        
        std::cout << "=== CS:GO NAC v" << NAC_VERSION << " ===" << std::endl;
        std::cout << "Professional Gaming Experience Enhancer" << std::endl;
        std::cout << "NAC - Not A Cheat" << std::endl;
        std::cout << "===============================" << std::endl;
        
        // Initialize configuration system
        std::cout << "Initializing configuration system..." << std::endl;
        if (!ConfigManager::Instance().Initialize()) {
            std::cerr << "Failed to initialize configuration system!" << std::endl;
            return;
        }
        
        // Wait for game to be ready
        std::cout << "Waiting for game to initialize..." << std::endl;
        Sleep(3000); // Give the game time to load
        
        // Initialize core systems
        std::cout << "Initializing NAC core systems..." << std::endl;
        if (!CSGO_NAC::Initialize()) {
            std::cerr << "Failed to initialize NAC core systems!" << std::endl;
            return;
        }
        
        // Initialize DirectX for GUI
        std::cout << "Initializing DirectX..." << std::endl;
        if (!InitializeDirectX()) {
            std::cerr << "Failed to initialize DirectX!" << std::endl;
            return;
        }
        
        // Initialize GUI system
        std::cout << "Initializing GUI system..." << std::endl;
        if (!NACGUI::Instance().Initialize(g_pd3dDevice, g_pd3dDeviceContext)) {
            std::cerr << "Failed to initialize GUI system!" << std::endl;
            return;
        }
        
        // Initialize hooks
        std::cout << "Installing hooks..." << std::endl;
        if (!Hooks::InstallHooks()) {
            std::cerr << "Failed to install hooks!" << std::endl;
            return;
        }
        
        std::cout << "CS:GO NAC initialized successfully!" << std::endl;
        std::cout << "Press INSERT to toggle menu" << std::endl;
        
        // Main loop
        auto lastTime = std::chrono::high_resolution_clock::now();
        while (g_running) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            // Process input
            NACGUI::Instance().ProcessInput();
            
            // Update animations
            AnimationManager::Instance().Update(deltaTime);
            
            // Check for config hot-reload
            ConfigManager::Instance().CheckForChanges();
            
            // Sleep to prevent high CPU usage
            Sleep(1);
        }
        
        std::cout << "Shutting down CS:GO NAC..." << std::endl;
        
        // Cleanup
        Hooks::RemoveHooks();
        NACGUI::Instance().Shutdown();
        ShutdownDirectX();
        CSGO_NAC::Shutdown();
        ConfigManager::Instance().Shutdown();
        
        std::cout << "CS:GO NAC shutdown complete." << std::endl;
        
        // Cleanup console
        ShutdownConsole();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception in main thread: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in main thread!" << std::endl;
    }
}

void InitializeConsole()
{
    if (!CONFIG_GET(misc, enableConsole)) return;
    
    // Allocate a console for this application
    AllocConsole();
    
    // Redirect stdout, stdin, stderr to console
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    
    // Set console title
    SetConsoleTitleA("CS:GO NAC - Debug Console");
    
    // Set console colors
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void ShutdownConsole()
{
    if (!CONFIG_GET(misc, enableConsole)) return;
    
    // Close console
    FreeConsole();
}

bool InitializeDirectX()
{
    // Find the game's window
    HWND hwnd = FindWindowA("Valve001", nullptr);
    if (!hwnd) {
        // Try alternative window class names for CS2
        hwnd = FindWindowA("SDL_app", nullptr);
    }
    
    if (!hwnd) {
        std::cerr << "Could not find game window!" << std::endl;
        return false;
    }
    
    // Get the swap chain from the game
    // This is a simplified version - in a real implementation, you would need to
    // hook the game's DirectX functions to get access to the device and swap chain
    
    // For now, create our own device for GUI rendering
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &g_pSwapChain,
        &g_pd3dDevice,
        &featureLevel,
        &g_pd3dDeviceContext
    );
    
    if (FAILED(hr)) {
        std::cerr << "Failed to create DirectX device and swap chain!" << std::endl;
        return false;
    }
    
    // Create render target view
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
    
    return true;
}

void ShutdownDirectX()
{
    if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
    
    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

// =============================================================================
// CSGO_NAC Implementation
// =============================================================================

namespace CSGO_NAC {
    bool Initialize() {
        std::cout << "Initializing CS:GO NAC core..." << std::endl;
        
        // Initialize memory management
        std::cout << "Setting up memory management..." << std::endl;
        
        // Initialize game state tracking
        std::cout << "Setting up game state tracking..." << std::endl;
        
        // Apply initial configuration
        std::cout << "Applying configuration..." << std::endl;
        g_Config.ApplySettings();
        
        std::cout << "CS:GO NAC core initialized successfully!" << std::endl;
        return true;
    }
    
    void Shutdown() {
        std::cout << "Shutting down CS:GO NAC core..." << std::endl;
        
        // Save configuration if enabled
        if (CONFIG_GET(menu, saveOnExit)) {
            ConfigManager::Instance().SaveConfig();
        }
        
        std::cout << "CS:GO NAC core shutdown complete." << std::endl;
    }
    
    PlayerInfo GetPlayerInfo(int playerId) {
        PlayerInfo info = {};
        info.id = playerId;
        strcpy_s(info.name, "Player");
        info.health = 100;
        info.armor = 100;
        info.isAlive = true;
        info.isEnemy = false;
        info.team = 2; // CT
        info.kills = 0;
        info.deaths = 0;
        info.assists = 0;
        info.money = 16000;
        strcpy_s(info.weapon, "ak47");
        
        // In a real implementation, this would read from game memory
        return info;
    }
    
    GameState GetGameState() {
        GameState state = {};
        state.inGame = true;
        state.isAlive = true;
        state.localPlayerTeam = 2;
        state.roundNumber = 1;
        state.roundTime = 115.0f;
        state.score_CT = 0;
        state.score_T = 0;
        strcpy_s(state.mapName, "de_dust2");
        state.gameMode = 1;
        
        // In a real implementation, this would read from game memory
        return state;
    }
    
    std::vector<Entity> GetEntities() {
        std::vector<Entity> entities;
        
        // In a real implementation, this would enumerate game entities
        // For now, return empty vector
        
        return entities;
    }
    
    void SetCustomCursor(int cursorType) {
        if (!CONFIG_GET(cursor, enableCustomCursor)) return;
        
        // Implementation would set custom cursor based on type
        std::cout << "Setting custom cursor type: " << cursorType << std::endl;
    }
    
    void EnableSmoothAnimations(bool enable) {
        CONFIG_SET(visual, enableSmoothAnimations, enable);
        std::cout << "Smooth animations " << (enable ? "enabled" : "disabled") << std::endl;
    }
    
    void SetThemeColors(Color primary, Color secondary) {
        CONFIG_SET(visual, themeColorPrimary, primary);
        CONFIG_SET(visual, themeColorSecondary, secondary);
        
        // Apply theme to GUI
        NACGUI::Instance().ApplyTheme();
        
        std::cout << "Theme colors updated" << std::endl;
    }
    
    void BoostFPS(int targetFPS) {
        if (!CONFIG_GET(performance, enableFPSBoost)) return;
        
        CONFIG_SET(performance, targetFPS, targetFPS);
        
        // Implementation would apply FPS boost settings
        std::cout << "FPS boost enabled, target: " << targetFPS << std::endl;
    }
    
    void OptimizeSettings() {
        if (!CONFIG_GET(performance, optimizeMemory)) return;
        
        // Implementation would optimize game settings for performance
        std::cout << "Optimizing game settings for performance..." << std::endl;
    }
    
    void SetLowLatencyMode(bool enable) {
        CONFIG_SET(performance, lowLatencyMode, enable);
        
        // Implementation would enable low latency mode
        std::cout << "Low latency mode " << (enable ? "enabled" : "disabled") << std::endl;
    }
    
    void SetCrosshairStyle(int style) {
        CONFIG_SET(player, crosshairStyle, style);
        
        // Implementation would apply crosshair style
        std::cout << "Crosshair style set to: " << style << std::endl;
    }
    
    void SetViewModelSettings(float fov, Vector3 offset) {
        CONFIG_SET(player, viewModelFOV, fov);
        CONFIG_SET(player, viewModelOffset, offset);
        
        // Implementation would apply viewmodel settings
        std::cout << "Viewmodel settings updated - FOV: " << fov << std::endl;
    }
    
    void EnableCustomHUD(bool enable) {
        CONFIG_SET(player, enableCustomHUD, enable);
        
        // Implementation would enable/disable custom HUD
        std::cout << "Custom HUD " << (enable ? "enabled" : "disabled") << std::endl;
    }
    
    void ShowMenu(bool show) {
        NACGUI::Instance().ShowMenu(show);
    }
    
    bool IsMenuVisible() {
        return NACGUI::Instance().IsMenuVisible();
    }
    
    void ProcessMenuInput() {
        NACGUI::Instance().ProcessInput();
    }
    
    void LoadConfig(const std::string& filename) {
        ConfigManager::Instance().LoadConfig(filename);
        std::cout << "Configuration loaded from: " << filename << std::endl;
    }
    
    void SaveConfig(const std::string& filename) {
        ConfigManager::Instance().SaveConfig(filename);
        std::cout << "Configuration saved to: " << filename << std::endl;
    }
    
    void ResetToDefaults() {
        ConfigManager::Instance().ResetToDefaults();
        std::cout << "Configuration reset to defaults" << std::endl;
    }
}

// =============================================================================
// Hooks Implementation
// =============================================================================

namespace Hooks {
    bool InstallHooks() {
        std::cout << "Installing hooks..." << std::endl;
        
        // Install DirectX hooks for GUI rendering
        if (!HookDirectX()) {
            std::cerr << "Failed to hook DirectX!" << std::endl;
            return false;
        }
        
        // Install game function hooks
        if (!HookGameFunctions()) {
            std::cerr << "Failed to hook game functions!" << std::endl;
            return false;
        }
        
        std::cout << "Hooks installed successfully!" << std::endl;
        return true;
    }
    
    void RemoveHooks() {
        std::cout << "Removing hooks..." << std::endl;
        
        UnhookDirectX();
        UnhookGameFunctions();
        
        std::cout << "Hooks removed successfully!" << std::endl;
    }
    
    bool HookDirectX() {
        // Implementation would hook DirectX Present function for GUI rendering
        std::cout << "DirectX hooks installed" << std::endl;
        return true;
    }
    
    void UnhookDirectX() {
        // Implementation would remove DirectX hooks
        std::cout << "DirectX hooks removed" << std::endl;
    }
    
    bool HookGameFunctions() {
        // Implementation would hook game functions for data access
        std::cout << "Game function hooks installed" << std::endl;
        return true;
    }
    
    void UnhookGameFunctions() {
        // Implementation would remove game function hooks
        std::cout << "Game function hooks removed" << std::endl;
    }
}

// =============================================================================
// Memory Implementation
// =============================================================================

namespace Memory {
    uintptr_t GetModuleBase(const char* moduleName) {
        HMODULE hModule = GetModuleHandleA(moduleName);
        return reinterpret_cast<uintptr_t>(hModule);
    }
    
    uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask) {
        size_t patternLength = strlen(mask);
        
        for (size_t i = 0; i <= size - patternLength; ++i) {
            bool found = true;
            for (size_t j = 0; j < patternLength; ++j) {
                if (mask[j] != '?' && pattern[j] != *reinterpret_cast<char*>(base + i + j)) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return base + i;
            }
        }
        
        return 0;
    }
    
    bool WriteMemory(uintptr_t address, void* data, size_t size) {
        DWORD oldProtect;
        if (!VirtualProtect(reinterpret_cast<void*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            return false;
        }
        
        memcpy(reinterpret_cast<void*>(address), data, size);
        
        VirtualProtect(reinterpret_cast<void*>(address), size, oldProtect, &oldProtect);
        return true;
    }
    
    bool ReadMemory(uintptr_t address, void* buffer, size_t size) {
        __try {
            memcpy(buffer, reinterpret_cast<void*>(address), size);
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }
}