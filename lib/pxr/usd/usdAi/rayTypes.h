// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef USDAI_RAY_TYPES5_H
#define USDAI_RAY_TYPES5_H

#include <stdint.h>

// Ray types from Arnold-5.x
namespace arnold {
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

#endif
