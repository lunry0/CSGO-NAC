#include "dllmain.h"
#include "features/low_details.h"
#include "features/fps_boost.h"
#include "gui/gui.h"
#include "utils/logger.h"

HINSTANCE hInstance;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            hInstance = hModule;
            Logger::Init(); // Initialize the logger
            SetupHooks(); // Set up hooks for game improvements
            GUI::Initialize(); // Initialize the GUI
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            GUI::Cleanup(); // Clean up GUI resources
            CleanupHooks(); // Remove hooks and clean up
            Logger::Shutdown(); // Shutdown the logger
            break;
    }
    return TRUE;
}