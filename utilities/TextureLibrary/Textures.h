#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h> //GLAD must be included before GLFW, because GLFW can include OpenGL headers internally
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



enum Texture_type {
    BACKGROUND,
    PLATFORM,
    WALL,    

    PLAYER_RIGHT1,
    PLAYER_RIGHT2,
    PLAYER_RIGHT3,
    PLAYER_RIGHT4,
    PLAYER_RIGHT5,
    PLAYER_RIGHT6,
    PLAYER_RIGHT7,
    PLAYER_RIGHT8,
 
    PLAYER_LEFT1,
    PLAYER_LEFT2 ,
    PLAYER_LEFT3 ,
    PLAYER_LEFT4 ,
    PLAYER_LEFT5 ,
    PLAYER_LEFT6 ,
    PLAYER_LEFT7 ,
    PLAYER_LEFT8 ,

    PLAYER_IDLE,
    TRACKER_IDLE,
    SNIPER_LEFT,
    SNIPER_RIGHT,
    SNIPER_BEAM,
    TEXTURE_COUNT
}; 

class Texture{
public:
    GLuint id;

    bool load(const std::string& path, int stretch, int flip_horizontal);
    void bind(GLuint slot = 0);
};


class Texture_Library{
public:
    Texture textures[TEXTURE_COUNT];
    Texture_Library();
};


#endif