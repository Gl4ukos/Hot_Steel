#ifndef MESHES_H
#define MESHES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h> //GLAD must be included before GLFW, because GLFW can include OpenGL headers internally



class Pyramid{

private:
    float vertices[24] = {
        // positions         // colors
        0.27f, 0.43f, 0.15f,  1.0f, 0.0f, 0.0f,   // top right front
        -0.27f, 0.43f, 0.15f,  0.0f, 1.0f, 0.0f,  // top left front
        0.0f,  -0.3f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom mid
        0.0f, 0.43f, -0.31f,  0.0f, 0.0f, 1.0f   // top mid back
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
    Pyramid();
    unsigned int get_vao();
    void destroy(); // optional: de-allocate all resources once they've outlived their purpose:
};



#endif