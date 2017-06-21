#ifndef USDAI_RAY_TYPES5_H
#define USDAI_RAY_TYPES5_H

#include <stdint.h>

// Ray types from Arnold-5.x
namespace arnold5 {
    static constexpr uint8_t AI_RAY_UNDEFINED = 0x00;
    static constexpr uint8_t AI_RAY_CAMERA = 0x01;
    static constexpr uint8_t AI_RAY_SHADOW = 0x02;
    static constexpr uint8_t AI_RAY_DIFFUSE_TRANSMIT = 0x04;
    static constexpr uint8_t AI_RAY_SPECULAR_TRANSMIT = 0x08;
    static constexpr uint8_t AI_RAY_VOLUME = 0x10;
    static constexpr uint8_t AI_RAY_DIFFUSE_REFLECT = 0x20;
    static constexpr uint8_t AI_RAY_SPECULAR_REFLECT = 0x40;
    static constexpr uint8_t AI_RAY_SUBSURFACE = 0x80;

    static constexpr uint8_t AI_RAY_ALL_DIFFUSE = (AI_RAY_DIFFUSE_TRANSMIT | AI_RAY_DIFFUSE_REFLECT);
    static constexpr uint8_t AI_RAY_ALL_SPECULAR = (AI_RAY_SPECULAR_TRANSMIT| AI_RAY_SPECULAR_REFLECT);
    static constexpr uint8_t AI_RAY_ALL_REFLECT = (AI_RAY_DIFFUSE_REFLECT  | AI_RAY_SPECULAR_REFLECT);
    static constexpr uint8_t AI_RAY_ALL_TRANSMIT = (AI_RAY_DIFFUSE_TRANSMIT | AI_RAY_SPECULAR_TRANSMIT);
    static constexpr uint8_t AI_RAY_ALL = uint8_t(-1);
}

#define USD_ARNOLD_IMPORT_ARNOLD5 using namespace arnold5;

#endif
