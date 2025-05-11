#pragma once
#include "../../../helper/texture.h"

namespace CarColours
{
    enum Index {
        Orange,
        Black,
        Blue,
        DarkBlue,
        DarkGrey,
        Grey,
        Red,
        COUNT
    };

    GLuint load(Index idx);
}
