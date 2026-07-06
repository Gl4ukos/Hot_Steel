#ifndef ENTITIES_H
#define ENTITIES_H

#include "Meshes.h"
#include "TextureLibrary/Textures.h"


enum Player_state{
    RUN_LEFT = 0,
    RUN_RIGHT = 1,
    IDLE = 2
};

class Kaelen_Voss{
public:
    Kaelen_Voss();
    void draw(Shader& shader);
    void update_hitbox();
    Hitbox get_hitbox();
    void update_state(Player_state new_state);
    
    Texture* textures[3];
    Rectangle mesh;
    int stretch_texture =0;
    Player_state state;
    float mass;
    float elasticity_factor;
    float vertical_speed_cap;
    float horizontal_speed_cap;
    float vertical_acc;
    float horizontal_acc;
    float jump_boost;
};

class Platform{
public:
    Platform();
    void draw(Shader& shader);
    void update_hitbox();
    Hitbox get_hitbox();

    Texture* texture;
    int stretch_texture =1;
    Rectangle mesh;
};


class Background{
public:
    float colour_increment[3] = {0.01f, 0.00f, 0.003f};
    Rectangle mesh;
    Texture* texture;
    int stretch_texture =0;

    Background();
    void draw(Shader& shader);
    void cycle_colour();
};


#endif