#ifndef USDAI_RAY_TYPES4_H
#define USDAI_RAY_TYPES4_H

#include <stdint.h>

// Ray types from Arnold-4.x
namespace arnold4 {
    static constexpr uint8_t AI_RAY_UNDEFINED = 0x00;
    static constexpr uint8_t AI_RAY_CAMERA = 0x01;
    static constexpr uint8_t AI_RAY_SHADOW = 0x02;
    static constexpr uint8_t AI_RAY_REFLECTED = 0x04;
    static constexpr uint8_t AI_RAY_REFRACTED = 0x08;
    static constexpr uint8_t AI_RAY_SUBSURFACE = 0x10;
    static constexpr uint8_t AI_RAY_DIFFUSE = 0x20;
    static constexpr uint8_t AI_RAY_GLOSSY = 0x40;
    static constexpr uint8_t AI_RAY_ALL = 0xFF;
    static constexpr uint8_t AI_RAY_GENERIC = AI_RAY_ALL;
}

#define USD_ARNOLD_IMPORT_ARNOLD4 using namespace arnold4;

#endif
