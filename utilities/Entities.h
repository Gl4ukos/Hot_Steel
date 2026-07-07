#ifndef ENTITIES_H
#define ENTITIES_H

#include "Meshes.h"
#include "TextureLibrary/Textures.h"


enum Entity_state{
    RUN_LEFT = 0,
    RUN_RIGHT = 1,
    IDLE = 2
};

struct Movement_Control_Input{
    int left;
    int right;
    int up;
    int down;
};

glm::vec3 get_collision_displacement(const Hitbox& a, const Hitbox& b);


class Entity{
public:
    Entity();
    void draw(Shader& shader);
    void update_hitbox();
    Hitbox get_hitbox();
    void update_movement_state(Movement_Control_Input input); //either 1.0 or 0.0, depending on keystroke


    Texture* textures[3];
    Rectangle mesh;
    int stretch_texture= 0;
    bool spaceWasDown;
    Entity_state state_type;
    float mass;
    float elasticity_factor;
    float friction;
    float gravity;
    float vertical_speed_cap;
    float horizontal_speed_cap;
    float vertical_acc;
    float horizontal_acc;
    float jump_boost;
    int jumpsLeft;
};

class Kaelen_Voss : public Entity{
public:
    Kaelen_Voss(Texture_Library* tex_lib);
    void move();
    void draw(Shader& shader);

    glm::vec3 control_input = glm::vec3(0.0f);
};

class Kike : public Entity{
public:
    Kike(Texture_Library* tex_lib);
    void move();
    void draw(Shader& shader);
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

class World{
public:
    World(Texture_Library* tex_lib);
    void draw(Shader& shader);
    glm::vec3 get_total_collision_displacement(const Hitbox& a);
    glm::vec4 get_ambient_colour();

    Background background;
    int platform_count = 6;
    Platform platforms[6];  
};

#endif