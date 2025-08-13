// This header file defines various types and constants used throughout the project.

#ifndef TYPES_INDEX_H
#define TYPES_INDEX_H

#include <cstdint>

// Define some common types used in the project
using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;

// Define constants for various settings
constexpr int MAX_FPS = 300;
constexpr int MIN_FPS = 30;

// Define a struct for configuration settings
struct Config {
    bool lowDetailsEnabled;
    bool fpsBoostEnabled;
    int targetFPS;
};

#endif // TYPES_INDEX_H