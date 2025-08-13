// CS:GO NAC GUI System
// Modern, animated GUI with purple-black theme

#ifndef GUI_H
#define GUI_H

#include "../../include/csgo_sdk.h"
#include "../../include/config.h"
#include <d3d11.h>
#include <vector>
#include <string>
#include <functional>
#include <memory>

// Forward declarations
struct ImDrawList;
struct ImGuiContext;

// GUI Element types
enum class ElementType {
    WINDOW,
    BUTTON,
    CHECKBOX,
    SLIDER_INT,
    SLIDER_FLOAT,
    COLOR_PICKER,
    COMBO,
    TEXT,
    SEPARATOR,
    TAB,
    GROUP
};

// Animation states
enum class AnimationState {
    IDLE,
    HOVER,
    ACTIVE,
    DISABLED
};

// GUI Element base class
class GUIElement {
public:
    GUIElement(const std::string& id, ElementType type);
    virtual ~GUIElement() = default;
    
    virtual void Render() = 0;
    virtual void Update(float deltaTime);
    virtual bool HandleInput();
    
    // Animation system
    void SetAnimation(AnimationState state);
    float GetAnimationProgress() const { return m_animationProgress; }
    
    // Properties
    void SetPosition(Vector2 pos) { m_position = pos; }
    void SetSize(Vector2 size) { m_size = size; }
    void SetVisible(bool visible) { m_visible = visible; }
    void SetEnabled(bool enabled) { m_enabled = enabled; }
    
    Vector2 GetPosition() const { return m_position; }
    Vector2 GetSize() const { return m_size; }
    bool IsVisible() const { return m_visible; }
    bool IsEnabled() const { return m_enabled; }
    
protected:
    std::string m_id;
    ElementType m_type;
    Vector2 m_position;
    Vector2 m_size;
    bool m_visible = true;
    bool m_enabled = true;
    
    // Animation properties
    AnimationState m_currentState = AnimationState::IDLE;
    AnimationState m_targetState = AnimationState::IDLE;
    float m_animationProgress = 0.0f;
    float m_animationSpeed = 5.0f;
    
    // Colors for different states
    Color m_colorIdle = Colors::DarkPurple;
    Color m_colorHover = Colors::Purple;
    Color m_colorActive = Colors::White;
    Color m_colorDisabled = Colors::Gray;
    
    Color GetCurrentColor() const;
    void UpdateAnimation(float deltaTime);
};

// Specific GUI elements
class Button : public GUIElement {
public:
    Button(const std::string& id, const std::string& text, std::function<void()> callback = nullptr);
    void Render() override;
    bool HandleInput() override;
    
    void SetText(const std::string& text) { m_text = text; }
    void SetCallback(std::function<void()> callback) { m_callback = callback; }
    
private:
    std::string m_text;
    std::function<void()> m_callback;
};

class Checkbox : public GUIElement {
public:
    Checkbox(const std::string& id, const std::string& label, bool* value);
    void Render() override;
    bool HandleInput() override;
    
private:
    std::string m_label;
    bool* m_value;
};

class SliderInt : public GUIElement {
public:
    SliderInt(const std::string& id, const std::string& label, int* value, int min, int max);
    void Render() override;
    bool HandleInput() override;
    
private:
    std::string m_label;
    int* m_value;
    int m_min, m_max;
};

class SliderFloat : public GUIElement {
public:
    SliderFloat(const std::string& id, const std::string& label, float* value, float min, float max);
    void Render() override;
    bool HandleInput() override;
    
private:
    std::string m_label;
    float* m_value;
    float m_min, m_max;
};

class ColorPicker : public GUIElement {
public:
    ColorPicker(const std::string& id, const std::string& label, Color* color);
    void Render() override;
    bool HandleInput() override;
    
private:
    std::string m_label;
    Color* m_color;
};

// Tab system
class TabManager {
public:
    void AddTab(const std::string& name, std::function<void()> renderFunc);
    void RemoveTab(const std::string& name);
    void SetActiveTab(const std::string& name);
    void Render();
    
private:
    struct Tab {
        std::string name;
        std::function<void()> renderFunc;
        bool active = false;
        float animationProgress = 0.0f;
    };
    
    std::vector<Tab> m_tabs;
    int m_activeTab = 0;
};

// Main GUI class
class NACGUI {
public:
    static NACGUI& Instance();
    
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    void Render();
    
    // Menu management
    void ShowMenu(bool show) { m_showMenu = show; }
    bool IsMenuVisible() const { return m_showMenu; }
    void ToggleMenu() { m_showMenu = !m_showMenu; }
    
    // Input handling
    void ProcessInput();
    bool WantCaptureMouse() const;
    bool WantCaptureKeyboard() const;
    
    // Theme management
    void ApplyTheme();
    void SetCustomColors();
    
    // Animation system
    void UpdateAnimations(float deltaTime);
    
private:
    NACGUI() = default;
    ~NACGUI() = default;
    
    bool m_initialized = false;
    bool m_showMenu = false;
    
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    
    // GUI state
    Vector2 m_menuSize = Vector2(800, 600);
    Vector2 m_menuPosition = Vector2(100, 100);
    float m_menuAlpha = 0.0f;
    float m_targetAlpha = 0.0f;
    
    // Tab system
    TabManager m_tabManager;
    
    // Render functions for each tab
    void RenderPlayerTab();
    void RenderEnemyTab();
    void RenderVisualTab();
    void RenderPerformanceTab();
    void RenderMiscTab();
    void RenderMenuTab();
    void RenderCursorTab();
    void RenderAnimationTab();
    
    // Helper functions
    void RenderHeader();
    void RenderFooter();
    void RenderBackground();
    
    // Style helpers
    void PushStyleColors();
    void PopStyleColors();
    
    // Animation helpers
    float EaseInOut(float t);
    float Bounce(float t);
    Color LerpColor(const Color& a, const Color& b, float t);
};

// Utility functions for custom rendering
namespace GUIUtils {
    void DrawGradientRect(Vector2 pos, Vector2 size, Color colorTop, Color colorBottom);
    void DrawRoundedRect(Vector2 pos, Vector2 size, float rounding, Color color);
    void DrawGlowEffect(Vector2 pos, Vector2 size, Color color, float intensity);
    void DrawAnimatedBorder(Vector2 pos, Vector2 size, Color color, float progress);
    
    // Text rendering
    void DrawText(Vector2 pos, const std::string& text, Color color, float size = 16.0f);
    void DrawTextCentered(Vector2 pos, Vector2 bounds, const std::string& text, Color color, float size = 16.0f);
    
    // Icon rendering
    void DrawIcon(Vector2 pos, int iconType, Color color, float size = 16.0f);
    
    // Animation helpers
    float SmoothStep(float edge0, float edge1, float x);
    float Pulse(float time, float frequency = 1.0f);
    Vector2 SpringAnimation(Vector2 current, Vector2 target, float& velocity, float deltaTime);
}

#endif // GUI_H