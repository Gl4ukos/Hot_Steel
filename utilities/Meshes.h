#ifndef MESHES_H
#define MESHES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h> //GLAD must be included before GLFW, because GLFW can include OpenGL headers internally
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



struct Hitbox {
    glm::vec3 min;
    glm::vec3 max;
};

class Mesh{

public:
    struct Transform {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
    }transform;

    struct Colour{
        float r = 0.0;
        float g = 0.0;
        float b = 0.0;
        float a = 0.0;
    }colour;

    void draw(Shader& shader);
    Hitbox get_hitbox() const;
    unsigned int get_vao();
    void destroy(); // optional: de-allocate all resources once they've outlived their purpose


protected:

    unsigned int VBO; // a Vertex Buffer Object (stores all raw vertex data (coordinates, colors, etc))
    unsigned int VAO; // a Vertex Array Object (stores all info about how the raw vertex data will be interpreted)
    unsigned int EBO; // element buffer object (these will be the indices telling OpenGL which vertices form triangles)

    glm::mat4 getModelMatrix();
};



class Pyramid : public Mesh{

private:
    float vertices[24] = {
        // positions         // colors
        0.135f, 0.32f, 0.07f,  1.0f, 0.5f, 0.5f,   // top right front
        -0.135f, 0.32f, 0.07f,  0.5f, 1.0f, 0.5f,  // top left front
        0.0f,  0.00f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom mid
        0.0f, 0.32f, -0.15f,  0.5f, 0.5f, 1.0f   // top mid back
    };

    //indices for pyramid faces
    unsigned int indices[12] = {
        0, 1, 2, // front
        1, 3, 2, // left
        3, 0, 2, // right
        0, 3, 1  // back
    };  

public:
    Pyramid(); 
    Hitbox get_hitbox()const;
    void draw(Shader& shader);
};

class Rectangle : public Mesh{
private:
    float vertices[48] = {
        //positions         //colors            //texture coords
        -0.5f, -0.5f, 0.2f,   0.6f, 0.6f, 0.9f,       //top left
         0.5f, -0.5f, 0.2f,   0.9f, 0.6f, 0.6f ,       //top right
        -0.5f, -0.7f, 0.2f,  0.0f, 0.0f, 0.0f   ,      //bot left
        +0.5f, -0.7f, 0.2f,  0.0f, 0.0f, 0.0f     //bot right  
    };

    //indices for rectangle
    unsigned int indices[6] = {
        0, 1, 2,
        1, 2, 3
    };

public:
    Rectangle();
    Hitbox get_hitbox()const;
    void draw(Shader& shader);
};


class Kuv : public Mesh{
private:
    float vertices[48] = {
        //positions         //colors            //texture coords
        -0.3f, -0.6f, -0.2f,   0.0f, 0.0f, 0.0f,       //top left
         0.3f, -0.6f, -0.2f,   0.0f, 0.0f, 0.0f ,       //top right
        -0.3f, -0.7f, -0.2f,  0.0f, 0.0f, 0.0f   ,      //bot left
        +0.3f, -0.7f, -0.2f,  0.0f, 0.0f, 0.0f,     //bot right


        -0.3f, -0.6f, 0.2f,   0.5f, 0.5f, 0.5f ,        //top left
         0.3f, -0.6f, 0.2f,   0.5f, 0.5f, 0.5f,         //top right
        -0.3f, -0.7f, 0.2f,  0.5f, 0.5f, 0.5f ,      //bot left
        +0.3f, -0.7f, 0.2f,  0.5f, 0.5f, 0.5f         //bot right     
    };

    //indices for rectangle
    unsigned int indices[36] = {
        // Front
        0, 1, 2,
        1, 3, 2,

        // Back
        4, 6, 5,
        5, 6, 7,

        // Top
        4, 5, 0,
        5, 1, 0,

        // Bottom
        2, 3, 6,
        3, 7, 6,

        // Left
        4, 0, 6,
        0, 2, 6,

        // Right
        1, 5, 3,
        5, 7, 3
    };

public:
    Kuv();
    void draw(Shader& shader);
};

#endif