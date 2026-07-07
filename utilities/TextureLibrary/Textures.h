#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h> //GLAD must be included before GLFW, because GLFW can include OpenGL headers internally
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



enum Texture_type {
    BACKGROUND =0,
    PLATFORM = 1,
    WALL = 2,     
    PLAYER_RIGHT1 = 3,
    PLAYER_RIGHT2 = 4,
    PLAYER_RIGHT3 = 5,
    PLAYER_RIGHT4 = 6,
    PLAYER_LEFT1 = 7,
    PLAYER_LEFT2 = 8,
    PLAYER_LEFT3 = 9,
    PLAYER_LEFT4 = 10,
    PLAYER_IDLE = 11,
    KIKE = 12
}; 

class Texture{
public:
    GLuint id;

    bool load(const std::string& path, int stretch, int flip_horizontal);
    void bind(GLuint slot = 0);
};


class Texture_Library{
public:
    Texture textures[13];
    Texture_Library();
};


#endif