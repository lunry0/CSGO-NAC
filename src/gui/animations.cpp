// CS:GO NAC Animation System Implementation
// Smooth, professional animations for GUI elements

#include "animations.h"
#include "../../include/config.h"
#include <algorithm>
#include <cmath>

// =============================================================================
// Animation Base Class Implementation
// =============================================================================

Animation::Animation(float duration, EasingType easing)
    : m_duration(duration)
    , m_elapsedTime(0.0f)
    , m_progress(0.0f)
    , m_easing(easing)
    , m_state(AnimationState::STOPPED)
    , m_loop(false)
    , m_reverse(false)
{
}

void Animation::Start() {
    m_state = AnimationState::PLAYING;
    m_elapsedTime = 0.0f;
    m_progress = 0.0f;
}

void Animation::Stop() {
    m_state = AnimationState::STOPPED;
    m_elapsedTime = 0.0f;
    m_progress = 0.0f;
}

void Animation::Pause() {
    if (m_state == AnimationState::PLAYING) {
        m_state = AnimationState::PAUSED;
    }
}

void Animation::Resume() {
    if (m_state == AnimationState::PAUSED) {
        m_state = AnimationState::PLAYING;
    }
}

void Animation::Reset() {
    m_elapsedTime = 0.0f;
    m_progress = 0.0f;
    m_state = AnimationState::STOPPED;
}

void Animation::Update(float deltaTime) {
    if (m_state != AnimationState::PLAYING) return;
    
    m_elapsedTime += deltaTime;
    
    if (m_elapsedTime >= m_duration) {
        if (m_loop) {
            m_elapsedTime = 0.0f;
            m_progress = 0.0f;
        } else {
            m_elapsedTime = m_duration;
            m_progress = 1.0f;
            m_state = AnimationState::COMPLETED;
            
            if (m_onComplete) {
                m_onComplete();
            }
        }
    } else {
        m_progress = m_elapsedTime / m_duration;
    }
    
    if (m_reverse) {
        m_progress = 1.0f - m_progress;
    }
    
    if (m_onUpdate) {
        m_onUpdate(GetEasedProgress());
    }
}

float Animation::GetEasedProgress() const {
    return ApplyEasing(m_progress);
}

float Animation::ApplyEasing(float t) const {
    switch (m_easing) {
        case EasingType::LINEAR:
            return EaseLinear(t);
        case EasingType::EASE_IN:
            return EaseInQuad(t);
        case EasingType::EASE_OUT:
            return EaseOutQuad(t);
        case EasingType::EASE_IN_OUT:
            return EaseInOutQuad(t);
        case EasingType::BOUNCE_IN:
            return EaseBounceIn(t);
        case EasingType::BOUNCE_OUT:
            return EaseBounceOut(t);
        case EasingType::BOUNCE_IN_OUT:
            return EaseBounceInOut(t);
        case EasingType::ELASTIC_IN:
            return EaseElasticIn(t);
        case EasingType::ELASTIC_OUT:
            return EaseElasticOut(t);
        case EasingType::ELASTIC_IN_OUT:
            return EaseElasticInOut(t);
        case EasingType::BACK_IN:
            return EaseBackIn(t);
        case EasingType::BACK_OUT:
            return EaseBackOut(t);
        case EasingType::BACK_IN_OUT:
            return EaseBackInOut(t);
        default:
            return EaseLinear(t);
    }
}

// Easing function implementations
float Animation::EaseLinear(float t) const {
    return t;
}

float Animation::EaseInQuad(float t) const {
    return t * t;
}

float Animation::EaseOutQuad(float t) const {
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float Animation::EaseInOutQuad(float t) const {
    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float Animation::EaseBounceIn(float t) const {
    return 1.0f - EaseBounceOut(1.0f - t);
}

float Animation::EaseBounceOut(float t) const {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;
    
    if (t < 1.0f / d1) {
        return n1 * t * t;
    } else if (t < 2.0f / d1) {
        t -= 1.5f / d1;
        return n1 * t * t + 0.75f;
    } else if (t < 2.5f / d1) {
        t -= 2.25f / d1;
        return n1 * t * t + 0.9375f;
    } else {
        t -= 2.625f / d1;
        return n1 * t * t + 0.984375f;
    }
}

float Animation::EaseBounceInOut(float t) const {
    return t < 0.5f
        ? (1.0f - EaseBounceOut(1.0f - 2.0f * t)) / 2.0f
        : (1.0f + EaseBounceOut(2.0f * t - 1.0f)) / 2.0f;
}

float Animation::EaseElasticIn(float t) const {
    const float c4 = (2.0f * 3.14159265359f) / 3.0f;
    
    return t == 0.0f ? 0.0f
         : t == 1.0f ? 1.0f
         : -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
}

float Animation::EaseElasticOut(float t) const {
    const float c4 = (2.0f * 3.14159265359f) / 3.0f;
    
    return t == 0.0f ? 0.0f
         : t == 1.0f ? 1.0f
         : std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float Animation::EaseElasticInOut(float t) const {
    const float c5 = (2.0f * 3.14159265359f) / 4.5f;
    
    return t == 0.0f ? 0.0f
         : t == 1.0f ? 1.0f
         : t < 0.5f
         ? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
         : (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

float Animation::EaseBackIn(float t) const {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    
    return c3 * t * t * t - c1 * t * t;
}

float Animation::EaseBackOut(float t) const {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    
    return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

float Animation::EaseBackInOut(float t) const {
    const float c1 = 1.70158f;
    const float c2 = c1 * 1.525f;
    
    return t < 0.5f
        ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
        : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}

// =============================================================================
// Specific Animation Types Implementation
// =============================================================================

// FadeAnimation
FadeAnimation::FadeAnimation(float duration, float startAlpha, float endAlpha, EasingType easing)
    : Animation(duration, easing)
    , m_startAlpha(startAlpha)
    , m_endAlpha(endAlpha)
    , m_currentAlpha(startAlpha)
{
}

void FadeAnimation::Update(float deltaTime) {
    Animation::Update(deltaTime);
    
    float easedProgress = GetEasedProgress();
    m_currentAlpha = AnimationUtils::Lerp(m_startAlpha, m_endAlpha, easedProgress);
}

// SlideAnimation
SlideAnimation::SlideAnimation(float duration, Vector2 startPos, Vector2 endPos, EasingType easing)
    : Animation(duration, easing)
    , m_startPosition(startPos)
    , m_endPosition(endPos)
    , m_currentPosition(startPos)
{
}

void SlideAnimation::Update(float deltaTime) {
    Animation::Update(deltaTime);
    
    float easedProgress = GetEasedProgress();
    m_currentPosition = AnimationUtils::Lerp(m_startPosition, m_endPosition, easedProgress);
}

// ScaleAnimation
ScaleAnimation::ScaleAnimation(float duration, float startScale, float endScale, EasingType easing)
    : Animation(duration, easing)
    , m_startScale(startScale)
    , m_endScale(endScale)
    , m_currentScale(startScale)
{
}

void ScaleAnimation::Update(float deltaTime) {
    Animation::Update(deltaTime);
    
    float easedProgress = GetEasedProgress();
    m_currentScale = AnimationUtils::Lerp(m_startScale, m_endScale, easedProgress);
}

// ColorAnimation
ColorAnimation::ColorAnimation(float duration, Color startColor, Color endColor, EasingType easing)
    : Animation(duration, easing)
    , m_startColor(startColor)
    , m_endColor(endColor)
    , m_currentColor(startColor)
{
}

void ColorAnimation::Update(float deltaTime) {
    Animation::Update(deltaTime);
    
    float easedProgress = GetEasedProgress();
    m_currentColor = AnimationUtils::Lerp(m_startColor, m_endColor, easedProgress);
}

// RotationAnimation
RotationAnimation::RotationAnimation(float duration, float startAngle, float endAngle, EasingType easing)
    : Animation(duration, easing)
    , m_startAngle(startAngle)
    , m_endAngle(endAngle)
    , m_currentAngle(startAngle)
{
}

void RotationAnimation::Update(float deltaTime) {
    Animation::Update(deltaTime);
    
    float easedProgress = GetEasedProgress();
    m_currentAngle = AnimationUtils::Lerp(m_startAngle, m_endAngle, easedProgress);
}

// =============================================================================
// Animation Manager Implementation
// =============================================================================

AnimationManager& AnimationManager::Instance() {
    static AnimationManager instance;
    return instance;
}

void AnimationManager::AddAnimation(std::shared_ptr<Animation> animation) {
    if (m_animations.size() >= m_maxAnimations) {
        CleanupCompletedAnimations();
    }
    
    m_animations.push_back(animation);
}

void AnimationManager::RemoveAnimation(std::shared_ptr<Animation> animation) {
    m_animations.erase(
        std::remove(m_animations.begin(), m_animations.end(), animation),
        m_animations.end()
    );
}

void AnimationManager::ClearAllAnimations() {
    m_animations.clear();
}

void AnimationManager::Update(float deltaTime) {
    if (!CONFIG_GET(visual, enableSmoothAnimations)) return;
    
    // Apply animation speed multiplier
    float adjustedDeltaTime = deltaTime * CONFIG_GET(visual, animationSpeed);
    
    for (auto& animation : m_animations) {
        animation->Update(adjustedDeltaTime);
    }
    
    CleanupCompletedAnimations();
}

void AnimationManager::PauseAll() {
    for (auto& animation : m_animations) {
        animation->Pause();
    }
}

void AnimationManager::ResumeAll() {
    for (auto& animation : m_animations) {
        animation->Resume();
    }
}

void AnimationManager::StopAll() {
    for (auto& animation : m_animations) {
        animation->Stop();
    }
}

std::shared_ptr<FadeAnimation> AnimationManager::CreateFadeAnimation(float duration, float startAlpha, float endAlpha) {
    auto animation = std::make_shared<FadeAnimation>(duration, startAlpha, endAlpha);
    AddAnimation(animation);
    return animation;
}

std::shared_ptr<SlideAnimation> AnimationManager::CreateSlideAnimation(float duration, Vector2 startPos, Vector2 endPos) {
    auto animation = std::make_shared<SlideAnimation>(duration, startPos, endPos);
    AddAnimation(animation);
    return animation;
}

std::shared_ptr<ScaleAnimation> AnimationManager::CreateScaleAnimation(float duration, float startScale, float endScale) {
    auto animation = std::make_shared<ScaleAnimation>(duration, startScale, endScale);
    AddAnimation(animation);
    return animation;
}

std::shared_ptr<ColorAnimation> AnimationManager::CreateColorAnimation(float duration, Color startColor, Color endColor) {
    auto animation = std::make_shared<ColorAnimation>(duration, startColor, endColor);
    AddAnimation(animation);
    return animation;
}

std::shared_ptr<RotationAnimation> AnimationManager::CreateRotationAnimation(float duration, float startAngle, float endAngle) {
    auto animation = std::make_shared<RotationAnimation>(duration, startAngle, endAngle);
    AddAnimation(animation);
    return animation;
}

void AnimationManager::CleanupCompletedAnimations() {
    m_animations.erase(
        std::remove_if(m_animations.begin(), m_animations.end(),
            [](const std::shared_ptr<Animation>& animation) {
                return animation->IsCompleted();
            }),
        m_animations.end()
    );
}

// =============================================================================
// Animation Utilities Implementation
// =============================================================================

namespace AnimationUtils {
    float Lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
    
    Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
        return Vector2(
            Lerp(a.x, b.x, t),
            Lerp(a.y, b.y, t)
        );
    }
    
    Color Lerp(const Color& a, const Color& b, float t) {
        return Color(
            Lerp(a.r, b.r, t),
            Lerp(a.g, b.g, t),
            Lerp(a.b, b.b, t),
            Lerp(a.a, b.a, t)
        );
    }
    
    float GetTime() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float>(currentTime - startTime).count();
    }
    
    float GetDeltaTime() {
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        return deltaTime;
    }
}

// =============================================================================
// Animation Presets
// =============================================================================

namespace AnimationPresets {
    // Menu animations
    const float MENU_FADE_DURATION = 0.3f;
    const float MENU_SLIDE_DURATION = 0.4f;
    const EasingType MENU_EASING = EasingType::EASE_OUT;
    
    // Button animations
    const float BUTTON_HOVER_DURATION = 0.15f;
    const float BUTTON_CLICK_DURATION = 0.1f;
    const EasingType BUTTON_EASING = EasingType::EASE_IN_OUT;
    
    // Notification animations
    const float NOTIFICATION_DURATION = 0.5f;
    const EasingType NOTIFICATION_EASING = EasingType::BOUNCE_OUT;
    
    // Loading animations
    const float LOADING_ROTATION_DURATION = 1.0f;
    const float LOADING_PULSE_DURATION = 1.5f;
}