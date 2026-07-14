#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader{
public:
    unsigned int ID; //program ID

    Shader(const char* vertex_filename, const char* fragment_filename);

    void use();
    void die();

    //utilities with uniforms
    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    
    void set_vec4(const std::string name, float a, float b, float c, float d);
    void set_vec4(const std::string &name, const glm::vec4& vec);
    
    void set_vec3(const std::string name, float a, float b, float c);
    void set_vec3(const std::string& name, const glm::vec3& vec);
    

    void set_vec2(const std::string& name, float a, float b);
    
    void set_mat4(const std::string name, glm::mat4 mat);
    
};

#endif