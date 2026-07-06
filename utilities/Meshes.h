#ifndef MESHES_H
#define MESHES_H

#include "Shader.h"


struct Hitbox {
    glm::vec3 min = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 max = glm::vec3(0.0f, 0.0f, 0.0f);
};


class Mesh{

public:
    struct Transform {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
    }transform;

    glm::vec4 additional_colour = glm::vec4(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f);

    Hitbox hitbox;

    float mass;
    float elasticity_factor;
    float vertical_speed_cap;
    float horizontal_speed_cap;
    float vertical_acc;
    float horizontal_acc;
    float jump_boost;

    void draw(Shader& shader, int stretch);
    void update_hitbox();
    void set_colour();
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
    void draw(Shader& shader, int stretch);
    void update_hitbox();
    float cycle_rads_wrap(float &value, float &increment);
    float cycle_value(float value, float increment);
};




class Rectangle : public Mesh{
private:
    float vertices[48] = {
        //positions         //colors            //texture coords
        -0.0f, +0.4f, 0.0f,   0.84f, 1.0f, 1.0f,     0.0f, 1.0f,       //top left
         0.4f, +0.4f, 0.0f,   0.84f, 1.0f, 1.0f,     1.0f, 1.0f,       //top right
        -0.0f, -0.0f, 0.0f,  0.9f, 0.9f, 0.9f,      0.0f, 0.0f,      //bot left
        +0.4f, -0.0f, 0.0f,  0.9f, 0.9f, 0.9f,      1.0f, 0.0f     //bot right  
    };

    //indices for rectangle<
    unsigned int indices[6] = {
        0, 1, 2,
        1, 2, 3
    };

public:

    Rectangle();
    void update_hitbox();
    void draw(Shader& shader, int stretch);
};




#endif