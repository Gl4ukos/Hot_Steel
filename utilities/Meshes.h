#ifndef MESHES_H
#define MESHES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h> //GLAD must be included before GLFW, because GLFW can include OpenGL headers internally
#include "Shader.h"

class Pyramid{

private:
    float vertices[24] = {
        // positions         // colors
        0.135f, 0.315f, 0.07f,  1.0f, 0.5f, 0.5f,   // top right front
        -0.135f, 0.315f, 0.07f,  0.5f, 1.0f, 0.5f,  // top left front
        0.0f,  -0.05f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom mid
        0.0f, 0.315f, -0.15f,  0.5f, 0.5f, 1.0f   // top mid back
    };

    //indices for pyramid faces
    unsigned int indices[12] = {
        0, 1, 2, // front
        1, 3, 2, // left
        3, 0, 2, // right
        0, 3, 1  // back
    };

    unsigned int VBO; // a Vertex Buffer Object (stores all raw vertex data (coordinates, colors, etc))
    unsigned int VAO; // a Vertex Array Object (stores all info about how the raw vertex data will be interpreted)
    unsigned int EBO; // element buffer object (these will be the indices telling OpenGL which vertices form triangles)

public:

    struct Transform{
        float x_rotation = -0.3;
        float y_rotation = 0.0;
        float z_rotation = 0.0;
        float y_offset = 0.0;
    }transform;

    struct Colour{
        float r = 0.0;
        float g = 0.0;
        float b = 0.0;
        float a = 0.0;
    }colour;


    Pyramid();
    void draw(Shader& shader);
    unsigned int get_vao();
    void destroy(); // optional: de-allocate all resources once they've outlived their purpose:
};

class Rectangle{
private:
    float vertices[24] = {
        //positions         //colors
        -0.005f, -0.3f, 0.0f,  0.64f, 0.91f, 0.91f,     // top left
        0.005f, -0.3f, 0.0f,   0.64f, 0.91f, 0.91f,     // top right
        -0.95f, -1.0f, 0.0f,  0.03f, 0.1f, 0.1f,     // bottom left
        0.95f, -1.0f, 0.0f,   0.03f, 0.1f, 0.1f     // bottom right
    };

    //indices for rectangle
    unsigned int indices[6] = {
        0, 1, 2, // left triangle
        1, 2, 3 // right triangle
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:
    struct Transform{
        float x_rotation = 0.0;
        float y_rotation = 0.0;
        float z_rotation = 0.0;
        float y_offset = 0.0;
    }transform;

    struct Colour{
        float r = 0.0;
        float g = 0.0;
        float b = 0.0;
        float a = 0.0;
    }colour;

    Rectangle();
    void draw(Shader& shader);
    unsigned int get_vao();
    void destroy();
};




#endif