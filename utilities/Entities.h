#ifndef ENTITIES_H
#define ENTITIES_H

#include "Meshes.h"
#include "TextureLibrary/Textures.h"

class Kaelen_Voss{
public:
    Kaelen_Voss();
    void draw(Shader& shader);
    void update_hitbox();
    void update_texture();

    
    Texture* texture;
    Rectangle mesh;
    float mass;
    float elasticity_factor;
    float vertical_speed_cap;
    float horizontal_speed_cap;
    float vertical_acc;
    float horizontal_acc;
    float jump_boost;
};

class Platform{

};

#endif