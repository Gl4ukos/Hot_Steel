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
    PLAYER =3
}; 

class Texture{
public:
    GLuint id;

    bool load(const std::string& path, int stretch);
    void bind(GLuint slot = 0);
};


class Texture_Library{
public:
    Texture textures[4];
    Texture_Library();
};


#endif