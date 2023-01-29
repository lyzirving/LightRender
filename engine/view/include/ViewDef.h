#ifndef VIEW_DEF_H
#define VIEW_DEF_H

#include <stdint.h>

#define R_COMP(color)    (((color >> 24u) & 0xffu) * 1.f / 255.f)
#define G_COMP(color)    (((color >> 16u) & 0xffu) * 1.f / 255.f)
#define B_COMP(color)    (((color >> 8u) & 0xffu) * 1.f / 255.f)
#define A_COMP(color)    ((color & 0xffu) * 1.f / 255.f)

enum LayerType : uint8_t
{
    LAYER_BASIC,
    LAYER_CANVAS,
    LAYER_OBJ,
    LAYER_CNT
};

enum LayerOrder : uint8_t
{
    LOW,
    MID,
    MID_1,
    TOP
};

enum RenderType : uint8_t
{
    SCENE,
    RRT_CANVAS,
    RENDER_CNT
};

enum AdjFlag : uint8_t
{
    ADJ_CENTER = 0b00000001,
    ADJ_SCALE  = 0b00000010
};

#endif // !VIEW_DEF_H

