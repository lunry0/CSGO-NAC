#include "gui.h"
#include "animations.h"
#include "../../include/config.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <d3d11.h>
#include <tchar.h>
#include <chrono>
#include <algorithm>

// Global variables
static auto g_lastFrameTime = std::chrono::high_resolution_clock::now();

// =============================================================================
// GUIElement Implementation
// =============================================================================

GUIElement::GUIElement(const std::string& id, ElementType type) 
    : m_id(id), m_type(type) {
}

void GUIElement::Update(float deltaTime) {
    UpdateAnimation(deltaTime);
}

bool GUIElement::HandleInput() {
    return false; // Base implementation
}

void GUIElement::SetAnimation(AnimationState state) {
    if (m_targetState != state) {
        m_targetState = state;
    }
}

Color GUIElement::GetCurrentColor() const {
    Color baseColor;
    switch (m_currentState) {
        case AnimationState::IDLE: baseColor = m_colorIdle; break;
        case AnimationState::HOVER: baseColor = m_colorHover; break;
        case AnimationState::ACTIVE: baseColor = m_colorActive; break;
        case AnimationState::DISABLED: baseColor = m_colorDisabled; break;
    }
    
    // Apply animation progress
    if (m_currentState != m_targetState) {
        Color targetColor;
        switch (m_targetState) {
            case AnimationState::IDLE: targetColor = m_colorIdle; break;
            case AnimationState::HOVER: targetColor = m_colorHover; break;
            case AnimationState::ACTIVE: targetColor = m_colorActive; break;
            case AnimationState::DISABLED: targetColor = m_colorDisabled; break;
        }
        
        float t = m_animationProgress;
        return Color(
            baseColor.r + (targetColor.r - baseColor.r) * t,
            baseColor.g + (targetColor.g - baseColor.g) * t,
            baseColor.b + (targetColor.b - baseColor.b) * t,
            baseColor.a + (targetColor.a - baseColor.a) * t
        );
    }
    
    return baseColor;
}

void GUIElement::UpdateAnimation(float deltaTime) {
    if (m_currentState != m_targetState) {
        m_animationProgress += deltaTime * m_animationSpeed;
        if (m_animationProgress >= 1.0f) {
            m_animationProgress = 1.0f;
            m_currentState = m_targetState;
        }
    } else {
        m_animationProgress = 0.0f;
    }
}

// =============================================================================
// Button Implementation
// =============================================================================

Button::Button(const std::string& id, const std::string& text, std::function<void()> callback)
    : GUIElement(id, ElementType::BUTTON), m_text(text), m_callback(callback) {
}

void Button::Render() {
    if (!m_visible) return;
    
    ImGui::PushID(m_id.c_str());
    
    Color currentColor = GetCurrentColor();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(currentColor.r, currentColor.g, currentColor.b, currentColor.a));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(m_colorHover.r, m_colorHover.g, m_colorHover.b, m_colorHover.a));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(m_colorActive.r, m_colorActive.g, m_colorActive.b, m_colorActive.a));
    
    if (ImGui::Button(m_text.c_str(), ImVec2(m_size.x, m_size.y))) {
        if (m_callback) {
            m_callback();
        }
    }
    
    // Handle animation states
    if (ImGui::IsItemHovered()) {
        SetAnimation(AnimationState::HOVER);
    } else if (ImGui::IsItemActive()) {
        SetAnimation(AnimationState::ACTIVE);
    } else {
        SetAnimation(AnimationState::IDLE);
    }
    
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

bool Button::HandleInput() {
    return ImGui::IsItemClicked();
}

// =============================================================================
// Checkbox Implementation
// =============================================================================

Checkbox::Checkbox(const std::string& id, const std::string& label, bool* value)
    : GUIElement(id, ElementType::CHECKBOX), m_label(label), m_value(value) {
}

void Checkbox::Render() {
    if (!m_visible) return;
    
    ImGui::PushID(m_id.c_str());
    
    Color currentColor = GetCurrentColor();
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(currentColor.r, currentColor.g, currentColor.b, currentColor.a));
    
    ImGui::Checkbox(m_label.c_str(), m_value);
    
    // Handle animation states
    if (ImGui::IsItemHovered()) {
        SetAnimation(AnimationState::HOVER);
    } else {
        SetAnimation(AnimationState::IDLE);
    }
    
    ImGui::PopStyleColor();
    ImGui::PopID();
}

bool Checkbox::HandleInput() {
    return ImGui::IsItemClicked();
}

// =============================================================================
// SliderInt Implementation
// =============================================================================

SliderInt::SliderInt(const std::string& id, const std::string& label, int* value, int min, int max)
    : GUIElement(id, ElementType::SLIDER_INT), m_label(label), m_value(value), m_min(min), m_max(max) {
}

void SliderInt::Render() {
    if (!m_visible) return;
    
    ImGui::PushID(m_id.c_str());
    
    Color currentColor = GetCurrentColor();
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(currentColor.r, currentColor.g, currentColor.b, currentColor.a));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(m_colorActive.r, m_colorActive.g, m_colorActive.b, m_colorActive.a));
    
    ImGui::SliderInt(m_label.c_str(), m_value, m_min, m_max);
    
    // Handle animation states
    if (ImGui::IsItemHovered()) {
        SetAnimation(AnimationState::HOVER);
    } else if (ImGui::IsItemActive()) {
        SetAnimation(AnimationState::ACTIVE);
    } else {
        SetAnimation(AnimationState::IDLE);
    }
    
    ImGui::PopStyleColor(2);
    ImGui::PopID();
}

bool SliderInt::HandleInput() {
    return ImGui::IsItemActive();
}

// =============================================================================
// SliderFloat Implementation
// =============================================================================

SliderFloat::SliderFloat(const std::string& id, const std::string& label, float* value, float min, float max)
    : GUIElement(id, ElementType::SLIDER_FLOAT), m_label(label), m_value(value), m_min(min), m_max(max) {
}

void SliderFloat::Render() {
    if (!m_visible) return;
    
    ImGui::PushID(m_id.c_str());
    
    Color currentColor = GetCurrentColor();
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(currentColor.r, currentColor.g, currentColor.b, currentColor.a));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(m_colorActive.r, m_colorActive.g, m_colorActive.b, m_colorActive.a));
    
    ImGui::SliderFloat(m_label.c_str(), m_value, m_min, m_max, "%.2f");
    
    // Handle animation states
    if (ImGui::IsItemHovered()) {
        SetAnimation(AnimationState::HOVER);
    } else if (ImGui::IsItemActive()) {
        SetAnimation(AnimationState::ACTIVE);
    } else {
        SetAnimation(AnimationState::IDLE);
    }
    
    ImGui::PopStyleColor(2);
    ImGui::PopID();
}

bool SliderFloat::HandleInput() {
    return ImGui::IsItemActive();
}

// =============================================================================
// ColorPicker Implementation
// =============================================================================

ColorPicker::ColorPicker(const std::string& id, const std::string& label, Color* color)
    : GUIElement(id, ElementType::COLOR_PICKER), m_label(label), m_color(color) {
}

void ColorPicker::Render() {
    if (!m_visible) return;
    
    ImGui::PushID(m_id.c_str());
    
    float colorArray[4] = { m_color->r, m_color->g, m_color->b, m_color->a };
    
    if (ImGui::ColorEdit4(m_label.c_str(), colorArray, ImGuiColorEditFlags_AlphaPreview)) {
        m_color->r = colorArray[0];
        m_color->g = colorArray[1];
        m_color->b = colorArray[2];
        m_color->a = colorArray[3];
    }
    
    // Handle animation states
    if (ImGui::IsItemHovered()) {
        SetAnimation(AnimationState::HOVER);
    } else {
        SetAnimation(AnimationState::IDLE);
    }
    
    ImGui::PopID();
}

bool ColorPicker::HandleInput() {
    return ImGui::IsItemActive();
}

// =============================================================================
// TabManager Implementation
// =============================================================================

void TabManager::AddTab(const std::string& name, std::function<void()> renderFunc) {
    Tab tab;
    tab.name = name;
    tab.renderFunc = renderFunc;
    tab.active = m_tabs.empty(); // First tab is active by default
    m_tabs.push_back(tab);
}

void TabManager::RemoveTab(const std::string& name) {
    m_tabs.erase(std::remove_if(m_tabs.begin(), m_tabs.end(),
        [&name](const Tab& tab) { return tab.name == name; }), m_tabs.end());
}

void TabManager::SetActiveTab(const std::string& name) {
    for (size_t i = 0; i < m_tabs.size(); ++i) {
        if (m_tabs[i].name == name) {
            m_activeTab = static_cast<int>(i);
            break;
        }
    }
}

void TabManager::Render() {
    if (m_tabs.empty()) return;
    
    // Render tab bar
    if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_Reorderable)) {
        for (size_t i = 0; i < m_tabs.size(); ++i) {
            ImGuiTabItemFlags flags = 0;
            if (static_cast<int>(i) == m_activeTab) {
                flags |= ImGuiTabItemFlags_SetSelected;
            }
            
            if (ImGui::BeginTabItem(m_tabs[i].name.c_str(), nullptr, flags)) {
                m_activeTab = static_cast<int>(i);
                m_tabs[i].renderFunc();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

// =============================================================================
// NACGUI Implementation
// =============================================================================

NACGUI& NACGUI::Instance() {
    static NACGUI instance;
    return instance;
}

bool NACGUI::Initialize(ID3D11Device* device, ID3D11DeviceContext* context) {
    if (m_initialized) return true;
    
    m_device = device;
    m_context = context;
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    
    // Setup Dear ImGui style
    ApplyTheme();
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplDX11_Init(device, context)) {
        return false;
    }
    
    // Setup tabs
    m_tabManager.AddTab("Player", [this]() { RenderPlayerTab(); });
    m_tabManager.AddTab("Enemy", [this]() { RenderEnemyTab(); });
    m_tabManager.AddTab("Visual", [this]() { RenderVisualTab(); });
    m_tabManager.AddTab("Performance", [this]() { RenderPerformanceTab(); });
    m_tabManager.AddTab("Misc", [this]() { RenderMiscTab(); });
    m_tabManager.AddTab("Menu", [this]() { RenderMenuTab(); });
    m_tabManager.AddTab("Cursor", [this]() { RenderCursorTab(); });
    m_tabManager.AddTab("Animation", [this]() { RenderAnimationTab(); });
    
    m_initialized = true;
    return true;
}

void NACGUI::Shutdown() {
    if (!m_initialized) return;
    
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();
    
    m_initialized = false;
}

void NACGUI::BeginFrame() {
    if (!m_initialized) return;
    
    // Calculate delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - g_lastFrameTime).count();
    g_lastFrameTime = currentTime;
    
    // Update animations
    UpdateAnimations(deltaTime);
    
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
}

void NACGUI::EndFrame() {
    if (!m_initialized) return;
    
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void NACGUI::Render() {
    if (!m_initialized || !m_showMenu) return;
    
    // Update menu alpha animation
    m_targetAlpha = m_showMenu ? CONFIG_GET(menu, menuOpacity) : 0.0f;
    float alphaSpeed = 5.0f;
    m_menuAlpha += (m_targetAlpha - m_menuAlpha) * alphaSpeed * ImGui::GetIO().DeltaTime;
    
    if (m_menuAlpha < 0.01f && !m_showMenu) return;
    
    // Set window alpha
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_menuAlpha);
    
    // Main menu window
    ImGui::SetNextWindowSize(ImVec2(m_menuSize.x, m_menuSize.y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(m_menuPosition.x, m_menuPosition.y), ImGuiCond_FirstUseEver);
    
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
    if (CONFIG_GET(menu, menuAnimations)) {
        windowFlags |= ImGuiWindowFlags_NoResize;
    }
    
    if (ImGui::Begin("CS:GO NAC - Professional Gaming Enhancement", &m_showMenu, windowFlags)) {
        RenderHeader();
        
        ImGui::Separator();
        
        // Render tabs
        m_tabManager.Render();
        
        ImGui::Separator();
        
        RenderFooter();
    }
    ImGui::End();
    
    ImGui::PopStyleVar();
}

void NACGUI::ProcessInput() {
    // Handle menu toggle
    if (GetAsyncKeyState(CONFIG_GET(menu, menuKey)) & 0x8000) {
        static bool keyPressed = false;
        if (!keyPressed) {
            ToggleMenu();
            keyPressed = true;
        }
    } else {
        static bool keyPressed = false;
        keyPressed = false;
    }
}

bool NACGUI::WantCaptureMouse() const {
    return m_initialized && m_showMenu && ImGui::GetIO().WantCaptureMouse;
}

bool NACGUI::WantCaptureKeyboard() const {
    return m_initialized && m_showMenu && ImGui::GetIO().WantCaptureKeyboard;
}

void NACGUI::ApplyTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Colors
    ImVec4* colors = style.Colors;
    
    // Purple-black theme
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.60f, 0.20f, 0.80f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.30f, 0.10f, 0.40f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.60f, 0.20f, 0.80f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.60f, 0.20f, 0.80f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.30f, 0.10f, 0.40f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.80f, 0.40f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.30f, 0.10f, 0.40f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.80f, 0.40f, 1.00f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.60f, 0.20f, 0.80f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.60f, 0.20f, 0.80f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.60f, 0.20f, 0.80f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.60f, 0.20f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.60f, 0.20f, 0.80f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.60f, 0.20f, 0.80f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.60f, 0.20f, 0.80f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.30f, 0.10f, 0.40f, 0.86f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.60f, 0.20f, 0.80f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.60f, 0.20f, 0.80f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.15f, 0.05f, 0.20f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.30f, 0.10f, 0.40f, 1.00f);
    
    // Style
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.TabRounding = 3.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;
}

void NACGUI::SetCustomColors() {
    ApplyTheme();
}

void NACGUI::UpdateAnimations(float deltaTime) {
    // Update menu alpha animation
    if (m_showMenu && m_menuAlpha < m_targetAlpha) {
        m_menuAlpha = std::min(m_menuAlpha + deltaTime * 3.0f, m_targetAlpha);
    } else if (!m_showMenu && m_menuAlpha > 0.0f) {
        m_menuAlpha = std::max(m_menuAlpha - deltaTime * 3.0f, 0.0f);
    }
}

// Tab rendering functions will be implemented in the next part...
void NACGUI::RenderPlayerTab() {
    ImGui::Text("Player Settings");
    ImGui::Separator();
    
    // Player settings implementation
    ImGui::Checkbox("Enable Custom Crosshair", &CONFIG_GET(player, enableCustomCrosshair));
    ImGui::SliderInt("Crosshair Style", &CONFIG_GET(player, crosshairStyle), 0, 10);
    
    float crosshairColor[4] = { 
        CONFIG_GET(player, crosshairColor).r, 
        CONFIG_GET(player, crosshairColor).g, 
        CONFIG_GET(player, crosshairColor).b, 
        CONFIG_GET(player, crosshairColor).a 
    };
    if (ImGui::ColorEdit4("Crosshair Color", crosshairColor)) {
        CONFIG_SET(player, crosshairColor, Color(crosshairColor[0], crosshairColor[1], crosshairColor[2], crosshairColor[3]));
    }
    
    ImGui::SliderFloat("Crosshair Size", &CONFIG_GET(player, crosshairSize), 1.0f, 20.0f);
    ImGui::SliderFloat("Crosshair Thickness", &CONFIG_GET(player, crosshairThickness), 0.5f, 5.0f);
    ImGui::Checkbox("Crosshair Outline", &CONFIG_GET(player, crosshairOutline));
    
    ImGui::Separator();
    
    ImGui::SliderFloat("Viewmodel FOV", &CONFIG_GET(player, viewModelFOV), 54.0f, 90.0f);
    ImGui::Checkbox("Enable Custom HUD", &CONFIG_GET(player, enableCustomHUD));
    ImGui::SliderFloat("HUD Scale", &CONFIG_GET(player, hudScale), 0.5f, 2.0f);
    ImGui::Checkbox("Show Player Info", &CONFIG_GET(player, showPlayerInfo));
    ImGui::Checkbox("Show Weapon Info", &CONFIG_GET(player, showWeaponInfo));
}

void NACGUI::RenderEnemyTab() {
    ImGui::Text("Enemy Settings");
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "NAC - Information Only (Not A Cheat)");
    ImGui::Separator();
    
    ImGui::Checkbox("Show Enemy Info", &CONFIG_GET(enemy, showEnemyInfo));
    ImGui::Checkbox("Show Enemy Health", &CONFIG_GET(enemy, showEnemyHealth));
    ImGui::Checkbox("Show Enemy Distance", &CONFIG_GET(enemy, showEnemyDistance));
    ImGui::Checkbox("Show Enemy Weapon", &CONFIG_GET(enemy, showEnemyWeapon));
    
    float enemyInfoColor[4] = { 
        CONFIG_GET(enemy, enemyInfoColor).r, 
        CONFIG_GET(enemy, enemyInfoColor).g, 
        CONFIG_GET(enemy, enemyInfoColor).b, 
        CONFIG_GET(enemy, enemyInfoColor).a 
    };
    if (ImGui::ColorEdit4("Enemy Info Color", enemyInfoColor)) {
        CONFIG_SET(enemy, enemyInfoColor, Color(enemyInfoColor[0], enemyInfoColor[1], enemyInfoColor[2], enemyInfoColor[3]));
    }
    
    ImGui::SliderFloat("Info Distance", &CONFIG_GET(enemy, enemyInfoDistance), 50.0f, 500.0f);
    ImGui::Checkbox("Highlight Enemies", &CONFIG_GET(enemy, highlightEnemies));
    
    float enemyHighlightColor[4] = { 
        CONFIG_GET(enemy, enemyHighlightColor).r, 
        CONFIG_GET(enemy, enemyHighlightColor).g, 
        CONFIG_GET(enemy, enemyHighlightColor).b, 
        CONFIG_GET(enemy, enemyHighlightColor).a 
    };
    if (ImGui::ColorEdit4("Highlight Color", enemyHighlightColor)) {
        CONFIG_SET(enemy, enemyHighlightColor, Color(enemyHighlightColor[0], enemyHighlightColor[1], enemyHighlightColor[2], enemyHighlightColor[3]));
    }
}

void NACGUI::RenderVisualTab() {
    ImGui::Text("Visual Settings");
    ImGui::Separator();
    
    ImGui::Checkbox("Enable Smooth Animations", &CONFIG_GET(visual, enableSmoothAnimations));
    ImGui::SliderFloat("Animation Speed", &CONFIG_GET(visual, animationSpeed), 0.1f, 3.0f);
    ImGui::Checkbox("Enable Particle Effects", &CONFIG_GET(visual, enableParticleEffects));
    ImGui::Checkbox("Enhanced Lighting", &CONFIG_GET(visual, enhancedLighting));
    
    ImGui::Separator();
    
    ImGui::SliderFloat("Brightness", &CONFIG_GET(visual, brightness), 0.5f, 2.0f);
    ImGui::SliderFloat("Contrast", &CONFIG_GET(visual, contrast), 0.5f, 2.0f);
    ImGui::SliderFloat("Saturation", &CONFIG_GET(visual, saturation), 0.0f, 2.0f);
    
    ImGui::Separator();
    
    ImGui::Checkbox("Enable Bloom", &CONFIG_GET(visual, enableBloom));
    ImGui::Checkbox("Enable Motion Blur", &CONFIG_GET(visual, enableMotionBlur));
    ImGui::Checkbox("Enable Anti-Aliasing", &CONFIG_GET(visual, enableAntiAliasing));
    ImGui::SliderInt("AA Level", &CONFIG_GET(visual, antiAliasingLevel), 2, 16);
    ImGui::Checkbox("Enable VSync", &CONFIG_GET(visual, enableVSync));
    ImGui::Checkbox("Fullscreen Optimization", &CONFIG_GET(visual, fullscreenOptimization));
    
    ImGui::Separator();
    
    float primaryColor[4] = { 
        CONFIG_GET(visual, themeColorPrimary).r, 
        CONFIG_GET(visual, themeColorPrimary).g, 
        CONFIG_GET(visual, themeColorPrimary).b, 
        CONFIG_GET(visual, themeColorPrimary).a 
    };
    if (ImGui::ColorEdit4("Primary Theme Color", primaryColor)) {
        CONFIG_SET(visual, themeColorPrimary, Color(primaryColor[0], primaryColor[1], primaryColor[2], primaryColor[3]));
    }
    
    float secondaryColor[4] = { 
        CONFIG_GET(visual, themeColorSecondary).r, 
        CONFIG_GET(visual, themeColorSecondary).g, 
        CONFIG_GET(visual, themeColorSecondary).b, 
        CONFIG_GET(visual, themeColorSecondary).a 
    };
    if (ImGui::ColorEdit4("Secondary Theme Color", secondaryColor)) {
        CONFIG_SET(visual, themeColorSecondary, Color(secondaryColor[0], secondaryColor[1], secondaryColor[2], secondaryColor[3]));
    }
}

void NACGUI::RenderPerformanceTab() {
    ImGui::Text("Performance Settings");
    ImGui::Separator();
    
    ImGui::Checkbox("Enable FPS Boost", &CONFIG_GET(performance, enableFPSBoost));
    ImGui::SliderInt("Target FPS", &CONFIG_GET(performance, targetFPS), 60, 500);
    ImGui::Checkbox("Low Latency Mode", &CONFIG_GET(performance, lowLatencyMode));
    ImGui::Checkbox("Optimize Memory", &CONFIG_GET(performance, optimizeMemory));
    ImGui::Checkbox("Reduce CPU Usage", &CONFIG_GET(performance, reduceCPUUsage));
    ImGui::Checkbox("Prioritize GPU", &CONFIG_GET(performance, prioritizeGPU));
    
    ImGui::Separator();
    
    const char* qualityItems[] = { "Low", "Medium", "High" };
    ImGui::Combo("Texture Quality", &CONFIG_GET(performance, textureQuality), qualityItems, 3);
    ImGui::Combo("Shadow Quality", &CONFIG_GET(performance, shadowQuality), qualityItems, 3);
    ImGui::Combo("Effects Quality", &CONFIG_GET(performance, effectsQuality), qualityItems, 3);
    
    ImGui::Checkbox("Enable Multicore", &CONFIG_GET(performance, enableMulticore));
}

void NACGUI::RenderMiscTab() {
    ImGui::Text("Miscellaneous Settings");
    ImGui::Separator();
    
    ImGui::Checkbox("Enable Console", &CONFIG_GET(misc, enableConsole));
    ImGui::Checkbox("Show FPS", &CONFIG_GET(misc, showFPS));
    ImGui::Checkbox("Show Ping", &CONFIG_GET(misc, showPing));
    ImGui::Checkbox("Show Packet Loss", &CONFIG_GET(misc, showPacketLoss));
    ImGui::Checkbox("Auto Record", &CONFIG_GET(misc, autoRecord));
    ImGui::Checkbox("Enhanced Audio", &CONFIG_GET(misc, enhancedAudio));
    ImGui::SliderFloat("Master Volume", &CONFIG_GET(misc, masterVolume), 0.0f, 2.0f);
    ImGui::Checkbox("Enable Discord RPC", &CONFIG_GET(misc, enableDiscordRPC));
}

void NACGUI::RenderMenuTab() {
    ImGui::Text("Menu Settings");
    ImGui::Separator();
    
    ImGui::Checkbox("Enable Menu", &CONFIG_GET(menu, enableMenu));
    
    // Key binding for menu toggle
    ImGui::Text("Menu Toggle Key: INSERT");
    ImGui::SliderFloat("Menu Opacity", &CONFIG_GET(menu, menuOpacity), 0.1f, 1.0f);
    ImGui::Checkbox("Menu Animations", &CONFIG_GET(menu, menuAnimations));
    ImGui::SliderFloat("Menu Scale", &CONFIG_GET(menu, menuScale), 0.5f, 2.0f);
    ImGui::Checkbox("Menu Sounds", &CONFIG_GET(menu, menuSounds));
    ImGui::Checkbox("Save on Exit", &CONFIG_GET(menu, saveOnExit));
}

void NACGUI::RenderCursorTab() {
    ImGui::Text("Cursor Settings");
    ImGui::Separator();
    
    ImGui::Checkbox("Enable Custom Cursor", &CONFIG_GET(cursor, enableCustomCursor));
    ImGui::SliderInt("Cursor Style", &CONFIG_GET(cursor, cursorStyle), 0, 10);
    
    float cursorColor[4] = { 
        CONFIG_GET(cursor, cursorColor).r, 
        CONFIG_GET(cursor, cursorColor).g, 
        CONFIG_GET(cursor, cursorColor).b, 
        CONFIG_GET(cursor, cursorColor).a 
    };
    if (ImGui::ColorEdit4("Cursor Color", cursorColor)) {
        CONFIG_SET(cursor, cursorColor, Color(cursorColor[0], cursorColor[1], cursorColor[2], cursorColor[3]));
    }
    
    ImGui::SliderFloat("Cursor Size", &CONFIG_GET(cursor, cursorSize), 0.5f, 3.0f);
    ImGui::Checkbox("Cursor Trail", &CONFIG_GET(cursor, cursorTrail));
    ImGui::Checkbox("Cursor Glow", &CONFIG_GET(cursor, cursorGlow));
}

void NACGUI::RenderAnimationTab() {
    ImGui::Text("Animation Settings");
    ImGui::Separator();
    
    ImGui::Checkbox("Smooth Transitions", &CONFIG_GET(animation, smoothTransitions));
    ImGui::SliderFloat("Transition Speed", &CONFIG_GET(animation, transitionSpeed), 0.1f, 1.0f);
    ImGui::Checkbox("Ease In/Out", &CONFIG_GET(animation, easeInOut));
    ImGui::Checkbox("Bounce Effects", &CONFIG_GET(animation, bounceEffects));
}

void NACGUI::RenderHeader() {
    ImGui::TextColored(ImVec4(0.6f, 0.2f, 0.8f, 1.0f), "CS:GO NAC v%s", NAC_VERSION);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " - Not A Cheat");
    ImGui::Text("Professional Gaming Experience Enhancer");
}

void NACGUI::RenderFooter() {
    ImGui::Separator();
    
    if (ImGui::Button("Load Config")) {
        ConfigManager::Instance().LoadConfig();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Save Config")) {
        ConfigManager::Instance().SaveConfig();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Reset to Defaults")) {
        ConfigManager::Instance().ResetToDefaults();
    }
    
    ImGui::SameLine();
    ImGui::Text("| FPS: %.1f", ImGui::GetIO().Framerate);
}

float NACGUI::EaseInOut(float t) {
    return t * t * (3.0f - 2.0f * t);
}

float NACGUI::Bounce(float t) {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

Color NACGUI::LerpColor(const Color& a, const Color& b, float t) {
    return Color(
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t
    );
}

// =============================================================================
// GUIUtils Implementation
// =============================================================================

namespace GUIUtils {
    void DrawGradientRect(Vector2 pos, Vector2 size, Color colorTop, Color colorBottom) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 p1(pos.x, pos.y);
        ImVec2 p2(pos.x + size.x, pos.y + size.y);
        
        ImU32 col1 = ImGui::ColorConvertFloat4ToU32(ImVec4(colorTop.r, colorTop.g, colorTop.b, colorTop.a));
        ImU32 col2 = ImGui::ColorConvertFloat4ToU32(ImVec4(colorBottom.r, colorBottom.g, colorBottom.b, colorBottom.a));
        
        drawList->AddRectFilledMultiColor(p1, p2, col1, col1, col2, col2);
    }
    
    void DrawRoundedRect(Vector2 pos, Vector2 size, float rounding, Color color) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 p1(pos.x, pos.y);
        ImVec2 p2(pos.x + size.x, pos.y + size.y);
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color.r, color.g, color.b, color.a));
        
        drawList->AddRectFilled(p1, p2, col, rounding);
    }
    
    void DrawGlowEffect(Vector2 pos, Vector2 size, Color color, float intensity) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        for (int i = 0; i < 5; ++i) {
            float alpha = color.a * intensity * (1.0f - i * 0.2f);
            float expand = i * 2.0f;
            
            ImVec2 p1(pos.x - expand, pos.y - expand);
            ImVec2 p2(pos.x + size.x + expand, pos.y + size.y + expand);
            ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color.r, color.g, color.b, alpha));
            
            drawList->AddRectFilled(p1, p2, col, 3.0f + expand);
        }
    }
    
    void DrawAnimatedBorder(Vector2 pos, Vector2 size, Color color, float progress) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 p1(pos.x, pos.y);
        ImVec2 p2(pos.x + size.x, pos.y + size.y);
        
        float thickness = 2.0f;
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color.r, color.g, color.b, color.a));
        
        // Animate border drawing based on progress
        float perimeter = 2.0f * (size.x + size.y);
        float currentLength = perimeter * progress;
        
        // This is a simplified version - full implementation would draw partial borders
        drawList->AddRect(p1, p2, col, 0.0f, 0, thickness);
    }
    
    void DrawText(Vector2 pos, const std::string& text, Color color, float size) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 textPos(pos.x, pos.y);
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color.r, color.g, color.b, color.a));
        
        drawList->AddText(nullptr, size, textPos, col, text.c_str());
    }
    
    void DrawTextCentered(Vector2 pos, Vector2 bounds, const std::string& text, Color color, float size) {
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        Vector2 centeredPos(
            pos.x + (bounds.x - textSize.x) * 0.5f,
            pos.y + (bounds.y - textSize.y) * 0.5f
        );
        
        DrawText(centeredPos, text, color, size);
    }
    
    void DrawIcon(Vector2 pos, int iconType, Color color, float size) {
        // Icon rendering would be implemented based on icon font or texture atlas
        // For now, just draw a placeholder
        DrawText(pos, "I", color, size);
    }
    
    float SmoothStep(float edge0, float edge1, float x) {
        float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }
    
    float Pulse(float time, float frequency) {
        return (sin(time * frequency * 2.0f * 3.14159f) + 1.0f) * 0.5f;
    }
    
    Vector2 SpringAnimation(Vector2 current, Vector2 target, float& velocity, float deltaTime) {
        float springConstant = 200.0f;
        float dampening = 20.0f;
        
        Vector2 displacement = target - current;
        Vector2 springForce = displacement * springConstant;
        Vector2 dampingForce = Vector2(velocity, velocity) * -dampening;
        Vector2 force = springForce + dampingForce;
        
        velocity += force.x * deltaTime; // Simplified for 1D
        Vector2 newPos = current + Vector2(velocity * deltaTime, velocity * deltaTime);
        
        return newPos;
    }
}