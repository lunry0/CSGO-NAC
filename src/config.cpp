#include "../include/config.h"
#include <fstream>
#include <json/json.h>
#include <iostream>

// Global configuration instance
NACConfig g_Config;

// =============================================================================
// NACConfig Implementation
// =============================================================================

void NACConfig::LoadDefaults() {
    // Player settings defaults
    player.enableCustomCrosshair = true;
    player.crosshairStyle = 1;
    player.crosshairColor = Colors::Purple;
    player.crosshairSize = 5.0f;
    player.crosshairThickness = 1.0f;
    player.crosshairOutline = true;
    player.viewModelFOV = 68.0f;
    player.viewModelOffset = Vector3(2.0f, 0.0f, -2.0f);
    player.enableCustomHUD = true;
    player.hudScale = 1.0f;
    player.showPlayerInfo = true;
    player.showWeaponInfo = true;

    // Enemy settings defaults (NAC - information only)
    enemy.showEnemyInfo = false;
    enemy.showEnemyHealth = false;
    enemy.showEnemyDistance = false;
    enemy.showEnemyWeapon = false;
    enemy.enemyInfoColor = Colors::Red;
    enemy.enemyInfoDistance = 100.0f;
    enemy.highlightEnemies = false;
    enemy.enemyHighlightColor = Colors::Red;

    // Visual settings defaults
    visual.enableSmoothAnimations = true;
    visual.animationSpeed = 1.0f;
    visual.enableParticleEffects = true;
    visual.enhancedLighting = true;
    visual.brightness = 1.0f;
    visual.contrast = 1.0f;
    visual.saturation = 1.0f;
    visual.enableBloom = true;
    visual.enableMotionBlur = false;
    visual.enableAntiAliasing = true;
    visual.antiAliasingLevel = 4;
    visual.enableVSync = false;
    visual.fullscreenOptimization = true;
    visual.themeColorPrimary = Colors::Purple;
    visual.themeColorSecondary = Colors::DarkPurple;

    // Performance settings defaults
    performance.enableFPSBoost = true;
    performance.targetFPS = 300;
    performance.lowLatencyMode = true;
    performance.optimizeMemory = true;
    performance.reduceCPUUsage = true;
    performance.prioritizeGPU = true;
    performance.textureQuality = 2;
    performance.shadowQuality = 1;
    performance.effectsQuality = 2;
    performance.enableMulticore = true;

    // Misc settings defaults
    misc.enableConsole = true;
    misc.showFPS = true;
    misc.showPing = true;
    misc.showPacketLoss = false;
    misc.autoRecord = false;
    misc.enhancedAudio = true;
    misc.masterVolume = 1.0f;
    misc.enableDiscordRPC = true;

    // Menu settings defaults
    menu.enableMenu = true;
    menu.menuKey = VK_INSERT;
    menu.menuOpacity = 0.95f;
    menu.menuAnimations = true;
    menu.menuScale = 1.0f;
    menu.menuSounds = true;
    menu.saveOnExit = true;

    // Cursor settings defaults
    cursor.enableCustomCursor = true;
    cursor.cursorStyle = 1;
    cursor.cursorColor = Colors::Purple;
    cursor.cursorSize = 1.0f;
    cursor.cursorTrail = false;
    cursor.cursorGlow = true;

    // Animation settings defaults
    animation.smoothTransitions = true;
    animation.transitionSpeed = 0.3f;
    animation.easeInOut = true;
    animation.bounceEffects = false;
}

bool NACConfig::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        LoadDefaults();
        return false;
    }

    Json::Value root;
    Json::Reader reader;
    
    if (!reader.parse(file, root)) {
        LoadDefaults();
        return false;
    }

    try {
        // Load player settings
        if (root.isMember("player")) {
            const Json::Value& p = root["player"];
            player.enableCustomCrosshair = p.get("enableCustomCrosshair", true).asBool();
            player.crosshairStyle = p.get("crosshairStyle", 1).asInt();
            player.crosshairSize = p.get("crosshairSize", 5.0f).asFloat();
            player.crosshairThickness = p.get("crosshairThickness", 1.0f).asFloat();
            player.crosshairOutline = p.get("crosshairOutline", true).asBool();
            player.viewModelFOV = p.get("viewModelFOV", 68.0f).asFloat();
            player.enableCustomHUD = p.get("enableCustomHUD", true).asBool();
            player.hudScale = p.get("hudScale", 1.0f).asFloat();
            player.showPlayerInfo = p.get("showPlayerInfo", true).asBool();
            player.showWeaponInfo = p.get("showWeaponInfo", true).asBool();
            
            // Load crosshair color
            if (p.isMember("crosshairColor")) {
                const Json::Value& color = p["crosshairColor"];
                player.crosshairColor = Color(
                    color.get("r", 0.6f).asFloat(),
                    color.get("g", 0.2f).asFloat(),
                    color.get("b", 0.8f).asFloat(),
                    color.get("a", 1.0f).asFloat()
                );
            }
            
            // Load viewmodel offset
            if (p.isMember("viewModelOffset")) {
                const Json::Value& offset = p["viewModelOffset"];
                player.viewModelOffset = Vector3(
                    offset.get("x", 2.0f).asFloat(),
                    offset.get("y", 0.0f).asFloat(),
                    offset.get("z", -2.0f).asFloat()
                );
            }
        }

        // Load enemy settings
        if (root.isMember("enemy")) {
            const Json::Value& e = root["enemy"];
            enemy.showEnemyInfo = e.get("showEnemyInfo", false).asBool();
            enemy.showEnemyHealth = e.get("showEnemyHealth", false).asBool();
            enemy.showEnemyDistance = e.get("showEnemyDistance", false).asBool();
            enemy.showEnemyWeapon = e.get("showEnemyWeapon", false).asBool();
            enemy.enemyInfoDistance = e.get("enemyInfoDistance", 100.0f).asFloat();
            enemy.highlightEnemies = e.get("highlightEnemies", false).asBool();
            
            // Load enemy colors
            if (e.isMember("enemyInfoColor")) {
                const Json::Value& color = e["enemyInfoColor"];
                enemy.enemyInfoColor = Color(
                    color.get("r", 1.0f).asFloat(),
                    color.get("g", 0.0f).asFloat(),
                    color.get("b", 0.0f).asFloat(),
                    color.get("a", 1.0f).asFloat()
                );
            }
            
            if (e.isMember("enemyHighlightColor")) {
                const Json::Value& color = e["enemyHighlightColor"];
                enemy.enemyHighlightColor = Color(
                    color.get("r", 1.0f).asFloat(),
                    color.get("g", 0.0f).asFloat(),
                    color.get("b", 0.0f).asFloat(),
                    color.get("a", 1.0f).asFloat()
                );
            }
        }

        // Load visual settings
        if (root.isMember("visual")) {
            const Json::Value& v = root["visual"];
            visual.enableSmoothAnimations = v.get("enableSmoothAnimations", true).asBool();
            visual.animationSpeed = v.get("animationSpeed", 1.0f).asFloat();
            visual.enableParticleEffects = v.get("enableParticleEffects", true).asBool();
            visual.enhancedLighting = v.get("enhancedLighting", true).asBool();
            visual.brightness = v.get("brightness", 1.0f).asFloat();
            visual.contrast = v.get("contrast", 1.0f).asFloat();
            visual.saturation = v.get("saturation", 1.0f).asFloat();
            visual.enableBloom = v.get("enableBloom", true).asBool();
            visual.enableMotionBlur = v.get("enableMotionBlur", false).asBool();
            visual.enableAntiAliasing = v.get("enableAntiAliasing", true).asBool();
            visual.antiAliasingLevel = v.get("antiAliasingLevel", 4).asInt();
            visual.enableVSync = v.get("enableVSync", false).asBool();
            visual.fullscreenOptimization = v.get("fullscreenOptimization", true).asBool();
            
            // Load theme colors
            if (v.isMember("themeColorPrimary")) {
                const Json::Value& color = v["themeColorPrimary"];
                visual.themeColorPrimary = Color(
                    color.get("r", 0.6f).asFloat(),
                    color.get("g", 0.2f).asFloat(),
                    color.get("b", 0.8f).asFloat(),
                    color.get("a", 1.0f).asFloat()
                );
            }
            
            if (v.isMember("themeColorSecondary")) {
                const Json::Value& color = v["themeColorSecondary"];
                visual.themeColorSecondary = Color(
                    color.get("r", 0.3f).asFloat(),
                    color.get("g", 0.1f).asFloat(),
                    color.get("b", 0.4f).asFloat(),
                    color.get("a", 1.0f).asFloat()
                );
            }
        }

        // Load performance settings
        if (root.isMember("performance")) {
            const Json::Value& perf = root["performance"];
            performance.enableFPSBoost = perf.get("enableFPSBoost", true).asBool();
            performance.targetFPS = perf.get("targetFPS", 300).asInt();
            performance.lowLatencyMode = perf.get("lowLatencyMode", true).asBool();
            performance.optimizeMemory = perf.get("optimizeMemory", true).asBool();
            performance.reduceCPUUsage = perf.get("reduceCPUUsage", true).asBool();
            performance.prioritizeGPU = perf.get("prioritizeGPU", true).asBool();
            performance.textureQuality = perf.get("textureQuality", 2).asInt();
            performance.shadowQuality = perf.get("shadowQuality", 1).asInt();
            performance.effectsQuality = perf.get("effectsQuality", 2).asInt();
            performance.enableMulticore = perf.get("enableMulticore", true).asBool();
        }

        // Load misc settings
        if (root.isMember("misc")) {
            const Json::Value& m = root["misc"];
            misc.enableConsole = m.get("enableConsole", true).asBool();
            misc.showFPS = m.get("showFPS", true).asBool();
            misc.showPing = m.get("showPing", true).asBool();
            misc.showPacketLoss = m.get("showPacketLoss", false).asBool();
            misc.autoRecord = m.get("autoRecord", false).asBool();
            misc.enhancedAudio = m.get("enhancedAudio", true).asBool();
            misc.masterVolume = m.get("masterVolume", 1.0f).asFloat();
            misc.enableDiscordRPC = m.get("enableDiscordRPC", true).asBool();
        }

        // Load menu settings
        if (root.isMember("menu")) {
            const Json::Value& men = root["menu"];
            menu.enableMenu = men.get("enableMenu", true).asBool();
            menu.menuKey = men.get("menuKey", VK_INSERT).asInt();
            menu.menuOpacity = men.get("menuOpacity", 0.95f).asFloat();
            menu.menuAnimations = men.get("menuAnimations", true).asBool();
            menu.menuScale = men.get("menuScale", 1.0f).asFloat();
            menu.menuSounds = men.get("menuSounds", true).asBool();
            menu.saveOnExit = men.get("saveOnExit", true).asBool();
        }

        // Load cursor settings
        if (root.isMember("cursor")) {
            const Json::Value& c = root["cursor"];
            cursor.enableCustomCursor = c.get("enableCustomCursor", true).asBool();
            cursor.cursorStyle = c.get("cursorStyle", 1).asInt();
            cursor.cursorSize = c.get("cursorSize", 1.0f).asFloat();
            cursor.cursorTrail = c.get("cursorTrail", false).asBool();
            cursor.cursorGlow = c.get("cursorGlow", true).asBool();
            
            if (c.isMember("cursorColor")) {
                const Json::Value& color = c["cursorColor"];
                cursor.cursorColor = Color(
                    color.get("r", 0.6f).asFloat(),
                    color.get("g", 0.2f).asFloat(),
                    color.get("b", 0.8f).asFloat(),
                    color.get("a", 1.0f).asFloat()
                );
            }
        }

        // Load animation settings
        if (root.isMember("animation")) {
            const Json::Value& a = root["animation"];
            animation.smoothTransitions = a.get("smoothTransitions", true).asBool();
            animation.transitionSpeed = a.get("transitionSpeed", 0.3f).asFloat();
            animation.easeInOut = a.get("easeInOut", true).asBool();
            animation.bounceEffects = a.get("bounceEffects", false).asBool();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        LoadDefaults();
        return false;
    }

    return true;
}

bool NACConfig::SaveToFile(const std::string& filename) {
    Json::Value root;

    // Save player settings
    Json::Value playerJson;
    playerJson["enableCustomCrosshair"] = player.enableCustomCrosshair;
    playerJson["crosshairStyle"] = player.crosshairStyle;
    playerJson["crosshairSize"] = player.crosshairSize;
    playerJson["crosshairThickness"] = player.crosshairThickness;
    playerJson["crosshairOutline"] = player.crosshairOutline;
    playerJson["viewModelFOV"] = player.viewModelFOV;
    playerJson["enableCustomHUD"] = player.enableCustomHUD;
    playerJson["hudScale"] = player.hudScale;
    playerJson["showPlayerInfo"] = player.showPlayerInfo;
    playerJson["showWeaponInfo"] = player.showWeaponInfo;
    
    // Save crosshair color
    Json::Value crosshairColor;
    crosshairColor["r"] = player.crosshairColor.r;
    crosshairColor["g"] = player.crosshairColor.g;
    crosshairColor["b"] = player.crosshairColor.b;
    crosshairColor["a"] = player.crosshairColor.a;
    playerJson["crosshairColor"] = crosshairColor;
    
    // Save viewmodel offset
    Json::Value viewModelOffset;
    viewModelOffset["x"] = player.viewModelOffset.x;
    viewModelOffset["y"] = player.viewModelOffset.y;
    viewModelOffset["z"] = player.viewModelOffset.z;
    playerJson["viewModelOffset"] = viewModelOffset;
    
    root["player"] = playerJson;

    // Save enemy settings
    Json::Value enemyJson;
    enemyJson["showEnemyInfo"] = enemy.showEnemyInfo;
    enemyJson["showEnemyHealth"] = enemy.showEnemyHealth;
    enemyJson["showEnemyDistance"] = enemy.showEnemyDistance;
    enemyJson["showEnemyWeapon"] = enemy.showEnemyWeapon;
    enemyJson["enemyInfoDistance"] = enemy.enemyInfoDistance;
    enemyJson["highlightEnemies"] = enemy.highlightEnemies;
    
    Json::Value enemyInfoColor;
    enemyInfoColor["r"] = enemy.enemyInfoColor.r;
    enemyInfoColor["g"] = enemy.enemyInfoColor.g;
    enemyInfoColor["b"] = enemy.enemyInfoColor.b;
    enemyInfoColor["a"] = enemy.enemyInfoColor.a;
    enemyJson["enemyInfoColor"] = enemyInfoColor;
    
    Json::Value enemyHighlightColor;
    enemyHighlightColor["r"] = enemy.enemyHighlightColor.r;
    enemyHighlightColor["g"] = enemy.enemyHighlightColor.g;
    enemyHighlightColor["b"] = enemy.enemyHighlightColor.b;
    enemyHighlightColor["a"] = enemy.enemyHighlightColor.a;
    enemyJson["enemyHighlightColor"] = enemyHighlightColor;
    
    root["enemy"] = enemyJson;

    // Save visual settings
    Json::Value visualJson;
    visualJson["enableSmoothAnimations"] = visual.enableSmoothAnimations;
    visualJson["animationSpeed"] = visual.animationSpeed;
    visualJson["enableParticleEffects"] = visual.enableParticleEffects;
    visualJson["enhancedLighting"] = visual.enhancedLighting;
    visualJson["brightness"] = visual.brightness;
    visualJson["contrast"] = visual.contrast;
    visualJson["saturation"] = visual.saturation;
    visualJson["enableBloom"] = visual.enableBloom;
    visualJson["enableMotionBlur"] = visual.enableMotionBlur;
    visualJson["enableAntiAliasing"] = visual.enableAntiAliasing;
    visualJson["antiAliasingLevel"] = visual.antiAliasingLevel;
    visualJson["enableVSync"] = visual.enableVSync;
    visualJson["fullscreenOptimization"] = visual.fullscreenOptimization;
    
    Json::Value primaryColor;
    primaryColor["r"] = visual.themeColorPrimary.r;
    primaryColor["g"] = visual.themeColorPrimary.g;
    primaryColor["b"] = visual.themeColorPrimary.b;
    primaryColor["a"] = visual.themeColorPrimary.a;
    visualJson["themeColorPrimary"] = primaryColor;
    
    Json::Value secondaryColor;
    secondaryColor["r"] = visual.themeColorSecondary.r;
    secondaryColor["g"] = visual.themeColorSecondary.g;
    secondaryColor["b"] = visual.themeColorSecondary.b;
    secondaryColor["a"] = visual.themeColorSecondary.a;
    visualJson["themeColorSecondary"] = secondaryColor;
    
    root["visual"] = visualJson;

    // Save performance settings
    Json::Value performanceJson;
    performanceJson["enableFPSBoost"] = performance.enableFPSBoost;
    performanceJson["targetFPS"] = performance.targetFPS;
    performanceJson["lowLatencyMode"] = performance.lowLatencyMode;
    performanceJson["optimizeMemory"] = performance.optimizeMemory;
    performanceJson["reduceCPUUsage"] = performance.reduceCPUUsage;
    performanceJson["prioritizeGPU"] = performance.prioritizeGPU;
    performanceJson["textureQuality"] = performance.textureQuality;
    performanceJson["shadowQuality"] = performance.shadowQuality;
    performanceJson["effectsQuality"] = performance.effectsQuality;
    performanceJson["enableMulticore"] = performance.enableMulticore;
    root["performance"] = performanceJson;

    // Save misc settings
    Json::Value miscJson;
    miscJson["enableConsole"] = misc.enableConsole;
    miscJson["showFPS"] = misc.showFPS;
    miscJson["showPing"] = misc.showPing;
    miscJson["showPacketLoss"] = misc.showPacketLoss;
    miscJson["autoRecord"] = misc.autoRecord;
    miscJson["enhancedAudio"] = misc.enhancedAudio;
    miscJson["masterVolume"] = misc.masterVolume;
    miscJson["enableDiscordRPC"] = misc.enableDiscordRPC;
    root["misc"] = miscJson;

    // Save menu settings
    Json::Value menuJson;
    menuJson["enableMenu"] = menu.enableMenu;
    menuJson["menuKey"] = menu.menuKey;
    menuJson["menuOpacity"] = menu.menuOpacity;
    menuJson["menuAnimations"] = menu.menuAnimations;
    menuJson["menuScale"] = menu.menuScale;
    menuJson["menuSounds"] = menu.menuSounds;
    menuJson["saveOnExit"] = menu.saveOnExit;
    root["menu"] = menuJson;

    // Save cursor settings
    Json::Value cursorJson;
    cursorJson["enableCustomCursor"] = cursor.enableCustomCursor;
    cursorJson["cursorStyle"] = cursor.cursorStyle;
    cursorJson["cursorSize"] = cursor.cursorSize;
    cursorJson["cursorTrail"] = cursor.cursorTrail;
    cursorJson["cursorGlow"] = cursor.cursorGlow;
    
    Json::Value cursorColorJson;
    cursorColorJson["r"] = cursor.cursorColor.r;
    cursorColorJson["g"] = cursor.cursorColor.g;
    cursorColorJson["b"] = cursor.cursorColor.b;
    cursorColorJson["a"] = cursor.cursorColor.a;
    cursorJson["cursorColor"] = cursorColorJson;
    
    root["cursor"] = cursorJson;

    // Save animation settings
    Json::Value animationJson;
    animationJson["smoothTransitions"] = animation.smoothTransitions;
    animationJson["transitionSpeed"] = animation.transitionSpeed;
    animationJson["easeInOut"] = animation.easeInOut;
    animationJson["bounceEffects"] = animation.bounceEffects;
    root["animation"] = animationJson;

    // Write to file
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &file);

    return true;
}

void NACConfig::ApplySettings() {
    // Apply all settings to the game/system
    // This would contain the actual implementation to apply settings
    
    // Example implementations:
    if (performance.enableFPSBoost) {
        // Apply FPS boost settings
    }
    
    if (visual.enableSmoothAnimations) {
        // Enable smooth animations
    }
    
    // Apply other settings...
}

bool NACConfig::ValidateSettings() {
    ClampValues();
    return true;
}

void NACConfig::ClampValues() {
    // Clamp all values to valid ranges
    player.crosshairStyle = std::max(0, std::min(10, player.crosshairStyle));
    player.crosshairSize = std::max(1.0f, std::min(20.0f, player.crosshairSize));
    player.crosshairThickness = std::max(0.5f, std::min(5.0f, player.crosshairThickness));
    player.viewModelFOV = std::max(54.0f, std::min(90.0f, player.viewModelFOV));
    player.hudScale = std::max(0.5f, std::min(2.0f, player.hudScale));
    
    enemy.enemyInfoDistance = std::max(50.0f, std::min(500.0f, enemy.enemyInfoDistance));
    
    visual.animationSpeed = std::max(0.1f, std::min(3.0f, visual.animationSpeed));
    visual.brightness = std::max(0.5f, std::min(2.0f, visual.brightness));
    visual.contrast = std::max(0.5f, std::min(2.0f, visual.contrast));
    visual.saturation = std::max(0.0f, std::min(2.0f, visual.saturation));
    visual.antiAliasingLevel = std::max(2, std::min(16, visual.antiAliasingLevel));
    
    performance.targetFPS = std::max(60, std::min(500, performance.targetFPS));
    performance.textureQuality = std::max(0, std::min(2, performance.textureQuality));
    performance.shadowQuality = std::max(0, std::min(2, performance.shadowQuality));
    performance.effectsQuality = std::max(0, std::min(2, performance.effectsQuality));
    
    misc.masterVolume = std::max(0.0f, std::min(2.0f, misc.masterVolume));
    
    menu.menuOpacity = std::max(0.1f, std::min(1.0f, menu.menuOpacity));
    menu.menuScale = std::max(0.5f, std::min(2.0f, menu.menuScale));
    
    cursor.cursorStyle = std::max(0, std::min(10, cursor.cursorStyle));
    cursor.cursorSize = std::max(0.5f, std::min(3.0f, cursor.cursorSize));
    
    animation.transitionSpeed = std::max(0.1f, std::min(1.0f, animation.transitionSpeed));
}

ConfigValue NACConfig::GetValue(const std::string& category, const std::string& key) {
    // Implementation for getting config values by string keys
    // This would be used for dynamic config access
    return ConfigValue(false); // Placeholder
}

void NACConfig::SetValue(const std::string& category, const std::string& key, const ConfigValue& value) {
    // Implementation for setting config values by string keys
    // This would be used for dynamic config modification
}

std::vector<std::string> NACConfig::GetCategories() {
    return {
        "player", "enemy", "visual", "performance", 
        "misc", "menu", "cursor", "animation"
    };
}

std::vector<std::string> NACConfig::GetKeys(const std::string& category) {
    // Return keys for the specified category
    if (category == "player") {
        return {
            "enableCustomCrosshair", "crosshairStyle", "crosshairColor",
            "crosshairSize", "crosshairThickness", "crosshairOutline",
            "viewModelFOV", "viewModelOffset", "enableCustomHUD",
            "hudScale", "showPlayerInfo", "showWeaponInfo"
        };
    }
    // Add other categories...
    return {};
}

// =============================================================================
// ConfigManager Implementation
// =============================================================================

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::Initialize() {
    if (m_initialized) return true;
    
    m_configPath = "nac_config.json";
    LoadConfig();
    
    m_initialized = true;
    return true;
}

void ConfigManager::Shutdown() {
    if (!m_initialized) return;
    
    if (CONFIG_GET(menu, saveOnExit)) {
        SaveConfig();
    }
    
    m_initialized = false;
}

void ConfigManager::LoadConfig(const std::string& filename) {
    m_configPath = filename;
    
    if (!g_Config.LoadFromFile(filename)) {
        g_Config.LoadDefaults();
        SaveConfig(filename); // Save defaults
    }
    
    g_Config.ValidateSettings();
    g_Config.ApplySettings();
}

void ConfigManager::SaveConfig(const std::string& filename) {
    g_Config.ValidateSettings();
    g_Config.SaveToFile(filename.empty() ? m_configPath : filename);
}

void ConfigManager::ResetToDefaults() {
    g_Config.LoadDefaults();
    g_Config.ApplySettings();
    SaveConfig();
}

void ConfigManager::EnableHotReload(bool enable) {
    m_hotReloadEnabled = enable;
}

void ConfigManager::CheckForChanges() {
    if (!m_hotReloadEnabled) return;
    
    FILETIME currentTime;
    if (GetFileTime(m_configPath, currentTime)) {
        if (CompareFileTime(&currentTime, &m_lastWriteTime) > 0) {
            LoadConfig();
            m_lastWriteTime = currentTime;
        }
    }
}

bool ConfigManager::ValidateConfig() {
    return g_Config.ValidateSettings();
}

void ConfigManager::FixInvalidValues() {
    g_Config.ClampValues();
}

void ConfigManager::RegisterChangeCallback(const std::string& category, ConfigChangeCallback callback) {
    m_callbacks[category] = callback;
}

void ConfigManager::UnregisterChangeCallback(const std::string& category) {
    m_callbacks.erase(category);
}

void ConfigManager::NotifyChange(const std::string& category, const std::string& key) {
    auto it = m_callbacks.find(category);
    if (it != m_callbacks.end()) {
        it->second(category, key);
    }
}

bool ConfigManager::GetFileTime(const std::string& filename, FILETIME& fileTime) {
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, 
                              nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    bool result = GetFileTime(hFile, nullptr, nullptr, &fileTime) != 0;
    CloseHandle(hFile);
    
    return result;
}

// =============================================================================
// Configuration Presets
// =============================================================================

namespace ConfigPresets {
    void LoadPerformancePreset() {
        g_Config.LoadDefaults();
        
        // Optimize for maximum performance
        g_Config.performance.enableFPSBoost = true;
        g_Config.performance.targetFPS = 500;
        g_Config.performance.lowLatencyMode = true;
        g_Config.performance.optimizeMemory = true;
        g_Config.performance.reduceCPUUsage = true;
        g_Config.performance.prioritizeGPU = true;
        g_Config.performance.textureQuality = 0; // Low
        g_Config.performance.shadowQuality = 0;  // Low
        g_Config.performance.effectsQuality = 0; // Low
        g_Config.performance.enableMulticore = true;
        
        // Disable visual effects for performance
        g_Config.visual.enableParticleEffects = false;
        g_Config.visual.enhancedLighting = false;
        g_Config.visual.enableBloom = false;
        g_Config.visual.enableMotionBlur = false;
        g_Config.visual.enableAntiAliasing = false;
        g_Config.visual.enableVSync = false;
        
        g_Config.ApplySettings();
    }
    
    void LoadQualityPreset() {
        g_Config.LoadDefaults();
        
        // Balanced settings
        g_Config.performance.targetFPS = 300;
        g_Config.performance.textureQuality = 1; // Medium
        g_Config.performance.shadowQuality = 1;  // Medium
        g_Config.performance.effectsQuality = 1; // Medium
        
        g_Config.visual.enableAntiAliasing = true;
        g_Config.visual.antiAliasingLevel = 4;
        g_Config.visual.enableBloom = true;
        g_Config.visual.enableMotionBlur = false;
        
        g_Config.ApplySettings();
    }
    
    void LoadCompetitivePreset() {
        g_Config.LoadDefaults();
        
        // Optimized for competitive play
        g_Config.performance.enableFPSBoost = true;
        g_Config.performance.targetFPS = 400;
        g_Config.performance.lowLatencyMode = true;
        g_Config.performance.textureQuality = 1; // Medium for visibility
        g_Config.performance.shadowQuality = 0;  // Low for performance
        g_Config.performance.effectsQuality = 1; // Medium
        
        g_Config.visual.enableMotionBlur = false;
        g_Config.visual.enableVSync = false;
        g_Config.visual.brightness = 1.2f; // Slightly brighter
        g_Config.visual.contrast = 1.1f;   // Slightly higher contrast
        
        // Enhanced crosshair for competitive
        g_Config.player.enableCustomCrosshair = true;
        g_Config.player.crosshairStyle = 2;
        g_Config.player.crosshairSize = 4.0f;
        g_Config.player.crosshairThickness = 1.5f;
        g_Config.player.crosshairOutline = true;
        
        g_Config.ApplySettings();
    }
    
    void LoadCasualPreset() {
        g_Config.LoadDefaults();
        
        // Enhanced visuals for casual play
        g_Config.performance.targetFPS = 200;
        g_Config.performance.textureQuality = 2; // High
        g_Config.performance.shadowQuality = 2;  // High
        g_Config.performance.effectsQuality = 2; // High
        
        g_Config.visual.enableParticleEffects = true;
        g_Config.visual.enhancedLighting = true;
        g_Config.visual.enableBloom = true;
        g_Config.visual.enableAntiAliasing = true;
        g_Config.visual.antiAliasingLevel = 8;
        
        g_Config.ApplySettings();
    }
}