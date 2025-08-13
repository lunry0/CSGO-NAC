// CS:GO NAC Configuration System
// Over 50+ customizable options for professional gaming

#ifndef CONFIG_H
#define CONFIG_H

#include "csgo_sdk.h"
#include <string>
#include <unordered_map>
#include <variant>

// Configuration value types
using ConfigValue = std::variant<bool, int, float, std::string, Color>;

// Configuration structure with 50+ options
struct NACConfig {
    // === PLAYER SETTINGS (12 options) ===
    struct PlayerSettings {
        bool enableCustomCrosshair = true;
        int crosshairStyle = 1;                    // 0-10 different styles
        Color crosshairColor = Colors::Purple;
        float crosshairSize = 5.0f;
        float crosshairThickness = 1.0f;
        bool crosshairOutline = true;
        float viewModelFOV = 68.0f;
        Vector3 viewModelOffset = Vector3(2.0f, 0.0f, -2.0f);
        bool enableCustomHUD = true;
        float hudScale = 1.0f;
        bool showPlayerInfo = true;
        bool showWeaponInfo = true;
    } player;

    // === ENEMY SETTINGS (8 options) ===
    struct EnemySettings {
        bool showEnemyInfo = false;                // NAC - information only
        bool showEnemyHealth = false;
        bool showEnemyDistance = false;
        bool showEnemyWeapon = false;
        Color enemyInfoColor = Colors::Red;
        float enemyInfoDistance = 100.0f;
        bool highlightEnemies = false;
        Color enemyHighlightColor = Colors::Red;
    } enemy;

    // === VISUAL SETTINGS (15 options) ===
    struct VisualSettings {
        bool enableSmoothAnimations = true;
        float animationSpeed = 1.0f;
        bool enableParticleEffects = true;
        bool enhancedLighting = true;
        float brightness = 1.0f;
        float contrast = 1.0f;
        float saturation = 1.0f;
        bool enableBloom = true;
        bool enableMotionBlur = false;
        bool enableAntiAliasing = true;
        int antiAliasingLevel = 4;                 // 2x, 4x, 8x, 16x
        bool enableVSync = false;
        bool fullscreenOptimization = true;
        Color themeColorPrimary = Colors::Purple;
        Color themeColorSecondary = Colors::DarkPurple;
    } visual;

    // === PERFORMANCE SETTINGS (10 options) ===
    struct PerformanceSettings {
        bool enableFPSBoost = true;
        int targetFPS = 300;
        bool lowLatencyMode = true;
        bool optimizeMemory = true;
        bool reduceCPUUsage = true;
        bool prioritizeGPU = true;
        int textureQuality = 2;                    // 0=Low, 1=Medium, 2=High
        int shadowQuality = 1;
        int effectsQuality = 2;
        bool enableMulticore = true;
    } performance;

    // === MISC SETTINGS (8 options) ===
    struct MiscSettings {
        bool enableConsole = true;
        bool showFPS = true;
        bool showPing = true;
        bool showPacketLoss = false;
        bool autoRecord = false;
        bool enhancedAudio = true;
        float masterVolume = 1.0f;
        bool enableDiscordRPC = true;
    } misc;

    // === MENU SETTINGS (7 options) ===
    struct MenuSettings {
        bool enableMenu = true;
        int menuKey = VK_INSERT;                   // Default toggle key
        float menuOpacity = 0.95f;
        bool menuAnimations = true;
        float menuScale = 1.0f;
        bool menuSounds = true;
        bool saveOnExit = true;
    } menu;

    // === CURSOR SETTINGS (6 options) ===
    struct CursorSettings {
        bool enableCustomCursor = true;
        int cursorStyle = 1;                       // 0-10 different cursor styles
        Color cursorColor = Colors::Purple;
        float cursorSize = 1.0f;
        bool cursorTrail = false;
        bool cursorGlow = true;
    } cursor;

    // === ANIMATION SETTINGS (4 options) ===
    struct AnimationSettings {
        bool smoothTransitions = true;
        float transitionSpeed = 0.3f;
        bool easeInOut = true;
        bool bounceEffects = false;
    } animation;

    // Configuration management
    void LoadDefaults();
    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename);
    void ApplySettings();
    
    // Validation
    bool ValidateSettings();
    void ClampValues();
    
    // Utility functions
    ConfigValue GetValue(const std::string& category, const std::string& key);
    void SetValue(const std::string& category, const std::string& key, const ConfigValue& value);
    std::vector<std::string> GetCategories();
    std::vector<std::string> GetKeys(const std::string& category);
};

// Global configuration instance
extern NACConfig g_Config;

// Configuration manager class
class ConfigManager {
public:
    static ConfigManager& Instance();
    
    bool Initialize();
    void Shutdown();
    
    void LoadConfig(const std::string& filename = "nac_config.json");
    void SaveConfig(const std::string& filename = "nac_config.json");
    void ResetToDefaults();
    
    // Hot-reload support
    void EnableHotReload(bool enable);
    void CheckForChanges();
    
    // Configuration validation
    bool ValidateConfig();
    void FixInvalidValues();
    
    // Event system for config changes
    using ConfigChangeCallback = std::function<void(const std::string&, const std::string&)>;
    void RegisterChangeCallback(const std::string& category, ConfigChangeCallback callback);
    void UnregisterChangeCallback(const std::string& category);
    
private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    
    bool m_initialized = false;
    bool m_hotReloadEnabled = false;
    FILETIME m_lastWriteTime = {};
    std::string m_configPath;
    
    std::unordered_map<std::string, ConfigChangeCallback> m_callbacks;
    
    void NotifyChange(const std::string& category, const std::string& key);
    bool GetFileTime(const std::string& filename, FILETIME& fileTime);
};

// Macros for easy config access
#define CONFIG_GET(category, key) g_Config.category.key
#define CONFIG_SET(category, key, value) g_Config.category.key = value

// Configuration presets
namespace ConfigPresets {
    void LoadPerformancePreset();      // Maximum FPS, minimal visual effects
    void LoadQualityPreset();          // Balanced performance and quality
    void LoadCompetitivePreset();      // Optimized for competitive play
    void LoadCasualPreset();           // Enhanced visuals for casual play
}

#endif // CONFIG_H