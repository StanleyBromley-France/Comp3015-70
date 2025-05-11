#include "car_colours.h"

#include <array>
#include <stdexcept>
#include <iostream>

#include "../../../helper/texture.h"

namespace CarColours
{
    static std::array<GLuint, COUNT> textures;
    static bool s_initialized = false;

    static const std::array<std::string, COUNT> filenames = {
    "diffuse-orange.png",
    "diffuse-black.png",
    "diffuse-blue.png",
    "diffuse-darkblue.png",
    "diffuse-darkgrey.png",
    "diffuse-grey.png",
    "diffuse-red.png"
    };


    GLuint load(Index idx)
    {
        if (idx < 0 || idx >= COUNT)
            std::cout << "CarColours::load() : invalid index" << "\n";

        const std::string path = "media/texture/" + filenames[idx];

        return Texture::loadTexture(path);
    }
}
