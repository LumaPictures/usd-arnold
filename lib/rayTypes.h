#ifndef USDAI_RAY_TYPES_H
#define USDAI_RAY_TYPES_H

// Copied from ai_ray.h
#define AI_RAY_UNDEFINED   0x00        // undefined type
#define AI_RAY_CAMERA      0x01        // ray originating at the camera
#define AI_RAY_SHADOW      0x02        // shadow ray towards a light source
#define AI_RAY_REFLECTED   0x04        // mirror reflection ray
#define AI_RAY_REFRACTED   0x08        // mirror refraction ray
#define AI_RAY_SUBSURFACE  0x10        // subsurface scattering probe ray
#define AI_RAY_DIFFUSE     0x20        // indirect diffuse (also known as diffuse GI) ray
#define AI_RAY_GLOSSY      0x40        // glossy/blurred reflection ray
#define AI_RAY_ALL         0xFF        // mask for all ray types
#define AI_RAY_GENERIC     AI_RAY_ALL  // mask for all ray types


#endif
