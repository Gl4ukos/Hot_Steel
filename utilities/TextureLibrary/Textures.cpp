#include "Textures.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

// **************************
// TEXTURE LIBRARY
// **************************
Texture_Library::Texture_Library(){
    if (!textures[BACKGROUND].load("utilities/TextureLibrary/textures/simulation_background.jpg", 0))
    {
        std::cout << "Couldn't load background texture\n";
    }

    if (!textures[PLATFORM].load("utilities/TextureLibrary/textures/platform.png", 0))
    {
        std::cout << "Couldn't load platform texture\n";
    }

    if (!textures[WALL].load("utilities/TextureLibrary/textures/wall.jpg", 0))
    {
        std::cout << "Couldn't load wall texture\n";
    }

    if (!textures[PLAYER].load("utilities/TextureLibrary/textures/side-eye-what.gif", 0))
    {
        std::cout << "Couldn't load player texture\n";
    }
}


// **************************
// TEXTURE
// **************************

bool Texture::load(const std::string& path, int stretch)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data) return false;

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 w, h, 0,
                 format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // IMPORTANT default settings
    if(stretch == 1){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::bind(GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}