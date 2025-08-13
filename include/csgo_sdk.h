// CS:GO NAC (Not A Cheat) - Enhancement DLL for CS:GO 2
// Professional Gaming Experience Enhancer

#ifndef CSGO_SDK_H
#define CSGO_SDK_H

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

// Game constants
#define GAME_NAME "Counter-Strike 2"
#define NAC_VERSION "1.0.0"
#define MAX_PLAYERS 64
#define MAX_ENTITIES 2048

// Color definitions for purple-black theme
struct Color {
    float r, g, b, a;
    Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f) 
        : r(red), g(green), b(blue), a(alpha) {}
};

namespace Colors {
    const Color Purple = Color(0.6f, 0.2f, 0.8f, 1.0f);
    const Color DarkPurple = Color(0.3f, 0.1f, 0.4f, 1.0f);
    const Color Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
    const Color White = Color(1.0f, 1.0f, 1.0f, 1.0f);
    const Color Gray = Color(0.5f, 0.5f, 0.5f, 1.0f);
    const Color Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
    const Color Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
    const Color Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
}

// Vector structures
struct Vector2 {
    float x, y;
    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
};

struct Vector3 {
    float x, y, z;
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    float Length() const { return sqrt(x * x + y * y + z * z); }
};

// Player information structure
struct PlayerInfo {
    int id;
    char name[64];
    int health;
    int armor;
    bool isAlive;
    bool isEnemy;
    Vector3 position;
    Vector3 viewAngles;
    int team;
    float distance;
    bool isVisible;
    int kills;
    int deaths;
    int assists;
    float money;
    char weapon[32];
};

// Entity structure
struct Entity {
    int id;
    Vector3 position;
    Vector3 velocity;
    int type;
    bool isValid;
    float health;
    int team;
};

// Game state structure
struct GameState {
    bool inGame;
    bool isAlive;
    int localPlayerTeam;
    int roundNumber;
    float roundTime;
    int score_CT;
    int score_T;
    char mapName[64];
    int gameMode;
};

// Configuration categories
enum class ConfigCategory {
    PLAYER_SETTINGS,
    ENEMY_SETTINGS,
    VISUAL_SETTINGS,
    PERFORMANCE_SETTINGS,
    MISC_SETTINGS,
    MENU_SETTINGS,
    CURSOR_SETTINGS,
    ANIMATION_SETTINGS
};

// Core function declarations
namespace CSGO_NAC {
    // Initialization
    bool Initialize();
    void Shutdown();
    
    // Game interaction
    PlayerInfo GetPlayerInfo(int playerId);
    GameState GetGameState();
    std::vector<Entity> GetEntities();
    
    // Visual enhancements
    void SetCustomCursor(int cursorType);
    void EnableSmoothAnimations(bool enable);
    void SetThemeColors(Color primary, Color secondary);
    
    // Performance
    void BoostFPS(int targetFPS);
    void OptimizeSettings();
    void SetLowLatencyMode(bool enable);
    
    // Player enhancements
    void SetCrosshairStyle(int style);
    void SetViewModelSettings(float fov, Vector3 offset);
    void EnableCustomHUD(bool enable);
    
    // Menu system
    void ShowMenu(bool show);
    bool IsMenuVisible();
    void ProcessMenuInput();
    
    // Configuration
    void LoadConfig(const std::string& filename);
    void SaveConfig(const std::string& filename);
    void ResetToDefaults();
}

// Hook declarations
namespace Hooks {
    bool InstallHooks();
    void RemoveHooks();
    
    // DirectX hooks
    bool HookDirectX();
    void UnhookDirectX();
    
    // Game hooks
    bool HookGameFunctions();
    void UnhookGameFunctions();
}

// Memory management
namespace Memory {
    uintptr_t GetModuleBase(const char* moduleName);
    uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask);
    bool WriteMemory(uintptr_t address, void* data, size_t size);
    bool ReadMemory(uintptr_t address, void* buffer, size_t size);
}

#endif // CSGO_SDK_H