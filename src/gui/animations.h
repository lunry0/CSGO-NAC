// CS:GO NAC Animation System
// Smooth, professional animations for GUI elements

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "../../include/csgo_sdk.h"
#include <functional>
#include <vector>
#include <memory>
#include <chrono>

// Animation easing types
enum class EasingType {
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT,
    BOUNCE_IN,
    BOUNCE_OUT,
    BOUNCE_IN_OUT,
    ELASTIC_IN,
    ELASTIC_OUT,
    ELASTIC_IN_OUT,
    BACK_IN,
    BACK_OUT,
    BACK_IN_OUT
};

// Animation state
enum class AnimationState {
    STOPPED,
    PLAYING,
    PAUSED,
    COMPLETED
};

// Base animation class
class Animation {
public:
    Animation(float duration, EasingType easing = EasingType::EASE_IN_OUT);
    virtual ~Animation() = default;

    // Control methods
    virtual void Start();
    virtual void Stop();
    virtual void Pause();
    virtual void Resume();
    virtual void Reset();
    
    // Update method - called every frame
    virtual void Update(float deltaTime);
    
    // State queries
    bool IsRunning() const { return m_state == AnimationState::PLAYING; }
    bool IsCompleted() const { return m_state == AnimationState::COMPLETED; }
    bool IsPaused() const { return m_state == AnimationState::PAUSED; }
    
    // Progress and timing
    float GetProgress() const { return m_progress; }
    float GetEasedProgress() const;
    float GetDuration() const { return m_duration; }
    float GetElapsedTime() const { return m_elapsedTime; }
    
    // Configuration
    void SetDuration(float duration) { m_duration = duration; }
    void SetEasing(EasingType easing) { m_easing = easing; }
    void SetLoop(bool loop) { m_loop = loop; }
    void SetReverse(bool reverse) { m_reverse = reverse; }
    
    // Callbacks
    void SetOnComplete(std::function<void()> callback) { m_onComplete = callback; }
    void SetOnUpdate(std::function<void(float)> callback) { m_onUpdate = callback; }
    
protected:
    float m_duration;
    float m_elapsedTime;
    float m_progress;
    EasingType m_easing;
    AnimationState m_state;
    bool m_loop;
    bool m_reverse;
    
    std::function<void()> m_onComplete;
    std::function<void(float)> m_onUpdate;
    
    // Easing functions
    float ApplyEasing(float t) const;
    
private:
    // Individual easing function implementations
    float EaseLinear(float t) const;
    float EaseInQuad(float t) const;
    float EaseOutQuad(float t) const;
    float EaseInOutQuad(float t) const;
    float EaseBounceIn(float t) const;
    float EaseBounceOut(float t) const;
    float EaseBounceInOut(float t) const;
    float EaseElasticIn(float t) const;
    float EaseElasticOut(float t) const;
    float EaseElasticInOut(float t) const;
    float EaseBackIn(float t) const;
    float EaseBackOut(float t) const;
    float EaseBackInOut(float t) const;
};

// Specific animation types
class FadeAnimation : public Animation {
public:
    FadeAnimation(float duration, float startAlpha = 0.0f, float endAlpha = 1.0f, 
                  EasingType easing = EasingType::EASE_IN_OUT);
    
    void Update(float deltaTime) override;
    
    float GetCurrentAlpha() const { return m_currentAlpha; }
    void SetAlphaRange(float start, float end) { m_startAlpha = start; m_endAlpha = end; }
    
private:
    float m_startAlpha;
    float m_endAlpha;
    float m_currentAlpha;
};

class SlideAnimation : public Animation {
public:
    SlideAnimation(float duration, Vector2 startPos, Vector2 endPos, 
                   EasingType easing = EasingType::EASE_IN_OUT);
    
    void Update(float deltaTime) override;
    
    Vector2 GetCurrentPosition() const { return m_currentPosition; }
    void SetPositionRange(Vector2 start, Vector2 end) { m_startPosition = start; m_endPosition = end; }
    
private:
    Vector2 m_startPosition;
    Vector2 m_endPosition;
    Vector2 m_currentPosition;
};

class ScaleAnimation : public Animation {
public:
    ScaleAnimation(float duration, float startScale = 0.0f, float endScale = 1.0f, 
                   EasingType easing = EasingType::BOUNCE_OUT);
    
    void Update(float deltaTime) override;
    
    float GetCurrentScale() const { return m_currentScale; }
    void SetScaleRange(float start, float end) { m_startScale = start; m_endScale = end; }
    
private:
    float m_startScale;
    float m_endScale;
    float m_currentScale;
};

class ColorAnimation : public Animation {
public:
    ColorAnimation(float duration, Color startColor, Color endColor, 
                   EasingType easing = EasingType::EASE_IN_OUT);
    
    void Update(float deltaTime) override;
    
    Color GetCurrentColor() const { return m_currentColor; }
    void SetColorRange(Color start, Color end) { m_startColor = start; m_endColor = end; }
    
private:
    Color m_startColor;
    Color m_endColor;
    Color m_currentColor;
};

class RotationAnimation : public Animation {
public:
    RotationAnimation(float duration, float startAngle = 0.0f, float endAngle = 360.0f, 
                      EasingType easing = EasingType::LINEAR);
    
    void Update(float deltaTime) override;
    
    float GetCurrentAngle() const { return m_currentAngle; }
    void SetAngleRange(float start, float end) { m_startAngle = start; m_endAngle = end; }
    
private:
    float m_startAngle;
    float m_endAngle;
    float m_currentAngle;
};

// Animation sequence - chain multiple animations
class AnimationSequence {
public:
    AnimationSequence();
    ~AnimationSequence();
    
    // Add animations to the sequence
    void AddAnimation(std::shared_ptr<Animation> animation);
    void AddDelay(float delay);
    
    // Control methods
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Reset();
    
    // Update
    void Update(float deltaTime);
    
    // State
    bool IsRunning() const;
    bool IsCompleted() const;
    
    // Callbacks
    void SetOnComplete(std::function<void()> callback) { m_onComplete = callback; }
    
private:
    struct SequenceItem {
        std::shared_ptr<Animation> animation;
        float delay;
        bool isDelay;
        
        SequenceItem(std::shared_ptr<Animation> anim) : animation(anim), delay(0.0f), isDelay(false) {}
        SequenceItem(float d) : animation(nullptr), delay(d), isDelay(true) {}
    };
    
    std::vector<SequenceItem> m_items;
    size_t m_currentIndex;
    float m_delayTimer;
    bool m_isRunning;
    
    std::function<void()> m_onComplete;
};

// Animation group - run multiple animations simultaneously
class AnimationGroup {
public:
    AnimationGroup();
    ~AnimationGroup();
    
    // Add animations to the group
    void AddAnimation(std::shared_ptr<Animation> animation);
    
    // Control methods
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Reset();
    
    // Update
    void Update(float deltaTime);
    
    // State
    bool IsRunning() const;
    bool IsCompleted() const;
    
    // Callbacks
    void SetOnComplete(std::function<void()> callback) { m_onComplete = callback; }
    
private:
    std::vector<std::shared_ptr<Animation>> m_animations;
    std::function<void()> m_onComplete;
};

// Animation manager - global animation system
class AnimationManager {
public:
    static AnimationManager& Instance();
    
    // Animation management
    void AddAnimation(std::shared_ptr<Animation> animation);
    void RemoveAnimation(std::shared_ptr<Animation> animation);
    void ClearAllAnimations();
    
    // Update all animations
    void Update(float deltaTime);
    
    // Global animation controls
    void PauseAll();
    void ResumeAll();
    void StopAll();
    
    // Animation creation helpers
    std::shared_ptr<FadeAnimation> CreateFadeAnimation(float duration, float startAlpha = 0.0f, float endAlpha = 1.0f);
    std::shared_ptr<SlideAnimation> CreateSlideAnimation(float duration, Vector2 startPos, Vector2 endPos);
    std::shared_ptr<ScaleAnimation> CreateScaleAnimation(float duration, float startScale = 0.0f, float endScale = 1.0f);
    std::shared_ptr<ColorAnimation> CreateColorAnimation(float duration, Color startColor, Color endColor);
    std::shared_ptr<RotationAnimation> CreateRotationAnimation(float duration, float startAngle = 0.0f, float endAngle = 360.0f);
    
    // Sequence and group creation
    std::shared_ptr<AnimationSequence> CreateSequence();
    std::shared_ptr<AnimationGroup> CreateGroup();
    
    // Performance settings
    void SetMaxAnimations(size_t max) { m_maxAnimations = max; }
    size_t GetActiveAnimationCount() const { return m_animations.size(); }
    
private:
    AnimationManager() = default;
    ~AnimationManager() = default;
    
    std::vector<std::shared_ptr<Animation>> m_animations;
    size_t m_maxAnimations = 100;
    
    void CleanupCompletedAnimations();
};

// Utility functions for common animation patterns
namespace AnimationUtils {
    // Create common animation combinations
    std::shared_ptr<AnimationSequence> CreateFadeInSlideUp(float duration, Vector2 startPos, Vector2 endPos);
    std::shared_ptr<AnimationSequence> CreateScaleInFadeIn(float duration);
    std::shared_ptr<AnimationGroup> CreatePulseEffect(float duration, float minScale = 0.9f, float maxScale = 1.1f);
    std::shared_ptr<AnimationSequence> CreateShakeEffect(float duration, float intensity = 5.0f);
    
    // Interpolation helpers
    float Lerp(float a, float b, float t);
    Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
    Color Lerp(const Color& a, const Color& b, float t);
    
    // Timing utilities
    float GetTime(); // Get current time in seconds
    float GetDeltaTime(); // Get delta time since last frame
}

// Predefined animation presets
namespace AnimationPresets {
    // Menu animations
    extern const float MENU_FADE_DURATION;
    extern const float MENU_SLIDE_DURATION;
    extern const EasingType MENU_EASING;
    
    // Button animations
    extern const float BUTTON_HOVER_DURATION;
    extern const float BUTTON_CLICK_DURATION;
    extern const EasingType BUTTON_EASING;
    
    // Notification animations
    extern const float NOTIFICATION_DURATION;
    extern const EasingType NOTIFICATION_EASING;
    
    // Loading animations
    extern const float LOADING_ROTATION_DURATION;
    extern const float LOADING_PULSE_DURATION;
}

#endif // ANIMATIONS_H