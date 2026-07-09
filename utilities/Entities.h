#ifndef ENTITIES_H
#define ENTITIES_H

#include "Meshes.h"
#include "TextureLibrary/Textures.h"



class Movement_Control_Input{
public:
    int left;
    int right;
    int up;
    int down;

    Movement_Control_Input(){
        left = 0;
        right = 0;
        up = 0;
        down = 0;
    }
};

glm::vec3 get_collision_displacement(const Hitbox& a, const Hitbox& b);


class Entity{
public:
    Entity();
    void draw(Shader& shader);
    void update_hitbox();
    Hitbox get_hitbox();


    Rectangle mesh;
    int stretch_texture= 0;
    float jump_cooldown = 0.2f;
    float jumpTimer = 0.0f;
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
    Texture* current_tex;
    Texture* tex_run_right[8];
    Texture* tex_run_left[8];
    Texture* tex_ascend[4];
    Texture* tex_descend[4];
    Texture* tex_idle[4];

    int texture_duration = 0;
    const int max_texture_duration = 4;
    int current_tex_index = 0;
    glm::vec3 control_input = glm::vec3(0.0f);

    enum Entity_state{
        RUN_LEFT = 0,
        RUN_RIGHT = 1,
        ASCENDING = 2,
        DESCENDING = 3,
        IDLE = 4
    }state_type, prev_state_type;

    Kaelen_Voss(Texture_Library* tex_lib);
    void update_movement_state(Movement_Control_Input input, float frameTime);
    void move();
    void draw(Shader& shader);
private:
    void update_texture();
};

class Tracker_robot : public Entity{
public:
    Texture* current_tex;

    enum Entity_state{
        RUN_LEFT = 0,
        RUN_RIGHT = 1,
        IDLE = 2
    }state_type, prev_state_type;

    Tracker_robot(Texture_Library* tex_lib);
    Movement_Control_Input think(float x_diff, float y_diff);
    void update_movement_state(Movement_Control_Input input, float frameTime);
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