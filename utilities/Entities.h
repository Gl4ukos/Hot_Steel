#ifndef ENTITIES_H
#define ENTITIES_H

#include "Meshes.h"
#include "TextureLibrary/Textures.h"
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>


class Movement_Control_Input{
public:
    int left;
    int right;
    int up;
    int down;
    int shoot;

    Movement_Control_Input(){
        left = 0;
        right = 0;
        up = 0;
        down = 0;
        shoot = 0;
    }
};

glm::vec3 get_collision_displacement(const Hitbox& a, const Hitbox& b);


class World;
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
    float shoot_cooldown = 0.2f;
    float shootTimer = 0.0f;
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

    Texture* current_weapon_tex;
    Texture* sniper_tex_left[4];
    Texture* sniper_tex_right[5];
    Rectangle weapon_mesh;
    glm::vec3 weapon_right_position_offset = glm::vec3(0.12f, 0.08f, 0.0f);
    glm::vec3 weapon_left_position_offset = glm::vec3(-0.12f, 0.08f, 0.0f);

    glm::vec3 sniper_beam_offset_right = glm::vec3(1.25, 0.07f, 0.5f);
    glm::vec3 sniper_beam_offset_left = glm::vec3(-1.25, 0.08f, 0.5f);


    float opacity = 1.0f;
    int texture_duration = 0;
    const int max_texture_duration = 4;
    int current_tex_index = 0;
    glm::vec3 control_input = glm::vec3(0.0f);

    enum Facing_direction{
        LEFT,
        RIGHT
    }facing_direction, prev_facing_direction;

    enum Entity_state{
        RUNNING, 
        ASCENDING,
        DESCENDING,
        DAMAGED,
        IDLE
    }state_type, prev_state_type;

    enum Weapon_state{
        INACTIVE,
        READY,
        SHOOTING,
        RELOADING
    }weapon_state, prev_weapon_state;

    Kaelen_Voss(Texture_Library* tex_lib);
    void update_movement_state(Movement_Control_Input input, float frameTime);
    void move();
    void spawn_projectiles(World* world, Texture_Library* tex_lib);
    void draw(Shader& shader);
private:
    void update_texture();
};

class Tracker_robot : public Entity{
public:
    Texture* current_tex;
    float opacity = 1.0f;

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


class Beam{
public:
    glm::vec3 start;
    float angle;
    glm::vec3 end;

    float width;
    float age;
    float lifetime;
    float damage_lifetime;
    int is_active = 1;

    Texture* texture;
    float opacity = 1.0f;

        
    Rectangle mesh;

    Beam(Texture_Library* tex_lib, glm::vec3 origin, float angle);
    void update_texture(float dt);
    void draw(Shader& shader);
    bool is_dead();
};

class Platform{
public:
    Platform();
    void draw(Shader& shader);
    void update_hitbox();
    Hitbox get_hitbox();

    Texture* texture;
    int stretch_texture =1;
    float opacity = 1.0f;
    Rectangle mesh;
};


class Background{
public:
    float colour_increment[3] = {0.01f, 0.00f, 0.003f};
    Rectangle mesh;
    Texture* texture;
    int stretch_texture =0;
    float opacity = 1.0f;

    Background();
    void draw(Shader& shader);
    void cycle_colour();
};

class World{
public:
    World(Texture_Library* tex_lib);
    void draw(Shader& shader, glm::vec3 camera_pos);
    void update_projectiles(float frameTime);
    glm::vec3 get_total_collision_displacement(const Hitbox& a);
    bool is_entity_shot(const Hitbox& hitbox);
    glm::vec4 get_ambient_colour();

    Background background;
    Platform platforms[5];  
    int platform_count = (sizeof(platforms)/sizeof(Platform));


    std::vector<Beam> spawned_beams;
    float hitStop = 0.0f;
    int background_static =1;
};

#endif