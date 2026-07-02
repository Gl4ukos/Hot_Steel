#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader{
public:
    unsigned int ID; //program ID

    Shader(const char* vertex_filename, const char* fragment_filename);

    void use();
    void die();

    //utilities with uniforms
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void set_vec4(const std::string name, float a, float b, float c, float d);
    void set_vec3(const std::string name, float a, float b, float c);
    
};

#endif