#include "Textures.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"


void flip_image_horizontal(unsigned char* data, int width, int height, int channels)
{
    for (int y = 0; y < height; y++){
        unsigned char* row = data + y * width * channels;
        for (int x = 0; x < width / 2; x++){
            int left = x * channels;
            int right = (width - 1 - x) * channels;

            for (int c = 0; c < channels; c++){
                std::swap(row[left + c], row[right + c]);
            }
        }
    }
}


// **************************
// TEXTURE LIBRARY
// **************************
Texture_Library::Texture_Library(){
    if (!textures[BACKGROUND].load("utilities/TextureLibrary/textures/simulation_background.jpg", 0,0))
    {
        std::cout << "Couldn't load background texture\n";
    }

    if (!textures[PLATFORM].load("utilities/TextureLibrary/textures/platform.png", 0,0))
    {
        std::cout << "Couldn't load platform texture\n";
    }

    if (!textures[WALL].load("utilities/TextureLibrary/textures/wall.jpg", 0,0))
    {
        std::cout << "Couldn't load wall texture\n";
    }
    stbi_set_flip_vertically_on_load(true); // inversion for png loading

    if (!textures[PLAYER_RIGHT1].load("utilities/TextureLibrary/textures/kaelen_run1.png", 0,0))
    {
        std::cout << "Couldn't load player texture\n";
    }
    if (!textures[PLAYER_LEFT1].load("utilities/TextureLibrary/textures/kaelen_run1.png", 0,1))
    {
        std::cout << "Couldn't load player texture\n";
    }


    if (!textures[PLAYER_RIGHT2].load("utilities/TextureLibrary/textures/kaelen_run2.png", 0,0))
    {
        std::cout << "Couldn't load player texture\n";
    }
    if (!textures[PLAYER_LEFT2].load("utilities/TextureLibrary/textures/kaelen_run2.png", 0,1))
    {
        std::cout << "Couldn't load player texture\n";
    }



    if (!textures[PLAYER_RIGHT3].load("utilities/TextureLibrary/textures/kaelen_run3.png", 0,0))
    {
        std::cout << "Couldn't load player texture\n";
    }
    if (!textures[PLAYER_LEFT3].load("utilities/TextureLibrary/textures/kaelen_run3.png", 0,1))
    {
        std::cout << "Couldn't load player texture\n";
    }

    if (!textures[PLAYER_RIGHT4].load("utilities/TextureLibrary/textures/kaelen_run4.png", 0,0))
    {
        std::cout << "Couldn't load player texture\n";
    }
    if (!textures[PLAYER_LEFT4].load("utilities/TextureLibrary/textures/kaelen_run4.png", 0,1))
    {
        std::cout << "Couldn't load player texture\n";
    }



    if (!textures[PLAYER_IDLE].load("utilities/TextureLibrary/textures/kat.png", 0,0))
    {
        std::cout << "Couldn't load player texture\n";
    }
    stbi_set_flip_vertically_on_load(false);

    if (!textures[KIKE].load("utilities/TextureLibrary/textures/kike.png", 0,0))
    {
        std::cout << "Couldn't load player texture\n";
    }

}


// **************************
// TEXTURE
// **************************

bool Texture::load(const std::string& path, int stretch, int flip_horizontal){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data) return false;

    if (flip_horizontal)
    {
        flip_image_horizontal(data, w, h, channels);
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 w, h, 0,
                 format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

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