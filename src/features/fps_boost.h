// filepath: csgo-improvement-dll/src/features/fps_boost.h

#ifndef FPS_BOOST_H
#define FPS_BOOST_H

class FPSBoost {
public:
    FPSBoost();
    ~FPSBoost();

    void enable();
    void disable();
    bool isEnabled() const;

private:
    bool enabled;
    void applyBoost();
    void revertBoost();
};

#endif // FPS_BOOST_H