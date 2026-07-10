#include "Entities.h"



glm::vec3 get_collision_displacement(const Hitbox& a, const Hitbox& b)
{
    glm::vec3 displacement(0.0f, 0.0f, 0.0f);
    bool in_x = false;

    if(a.min.x <= b.max.x && a.max.x >= b.min.x){
        float right_dis = b.max.x - a.min.x;
        float left_dis = b.min.x - a.max.x;

        if(std::abs(right_dis) > std::abs(left_dis)){
            displacement.x = left_dis;
        }else{
            displacement.x = right_dis;
        }

        if(a.min.y <= b.max.y && a.max.y >= b.min.y){
            right_dis = b.max.y - a.min.y;
            left_dis = b.min.y - a.max.y;

            if(std::abs(right_dis) > std::abs(left_dis)){
                displacement.y = left_dis;
            }else{
                displacement.y = right_dis;
            }

            // COLLISION DATECTED
            // we keep only the least displacement to return
            if(std::abs(displacement.x) > std::abs(displacement.y)){
                displacement.x = 0.0f; 
            }else{
                displacement.y = 0.0f;
            }
        }else{
            displacement.x = 0.0f;
        }
    }
    return displacement;
}

//**************************
//  ENTITY
//  ***********************/
Entity::Entity(){
    mass = 10;
    elasticity_factor = 0.0;
    vertical_speed_cap = 4.0;
    horizontal_speed_cap = 1.0;
    vertical_acc = 30;
    horizontal_acc = 30;
    jump_boost = 3.2;
    gravity = -15.0;
    friction = 10.0;
    jumpsLeft = 1;
    mesh.transform.scale = glm::vec3(0.35f, 0.4f, 1.0f);  

}

void Entity::draw(Shader& shader){
    std::cout<<"DRAW FUNCTION NOT ASSIGNED\n";
}

void Entity::update_hitbox(){
    mesh.update_hitbox();
}


Hitbox Entity::get_hitbox(){
    return mesh.hitbox;
}





//**************************
//  KAELEN VOSS
//  ***********************/
Kaelen_Voss::Kaelen_Voss(Texture_Library* tex_lib){
    mass = 10;
    elasticity_factor = 0.0;
    vertical_speed_cap = 4.0;
    horizontal_speed_cap = 2.0;
    vertical_acc = 30;
    horizontal_acc = 20;
    jump_boost = 3.2;
    facing_direction = RIGHT;
    prev_facing_direction = RIGHT;
    state_type = IDLE;
    prev_state_type = IDLE;
    weapon_state = READY;
    prev_weapon_state = READY;
    friction = 15;
    jumpsLeft=3;
    mesh.hitbox.offset_min = glm::vec2(0.14f, 0.0f); //percentage
    mesh.hitbox.offset_max = glm::vec2(0.12f, 0.03f); //percentage
    mesh.transform.scale = glm::vec3(0.55f, 0.7f, 1.0f);   
    weapon_mesh.transform.scale = glm::vec3(0.55f, 0.55f, 1.0f);
    weapon_mesh.transform.position = mesh.transform.position;

    tex_run_right[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_run_right[1] = &tex_lib->textures[PLAYER_RIGHT2];
    tex_run_right[2] = &tex_lib->textures[PLAYER_RIGHT3];
    tex_run_right[3] = &tex_lib->textures[PLAYER_RIGHT4];
    tex_run_right[4] = &tex_lib->textures[PLAYER_RIGHT5];
    tex_run_right[5] = &tex_lib->textures[PLAYER_RIGHT6];
    tex_run_right[6] = &tex_lib->textures[PLAYER_RIGHT7];
    tex_run_right[7] = &tex_lib->textures[PLAYER_RIGHT8];

    tex_run_left[0] = &tex_lib->textures[PLAYER_LEFT1];
    tex_run_left[1] = &tex_lib->textures[PLAYER_LEFT2];
    tex_run_left[2] = &tex_lib->textures[PLAYER_LEFT3];
    tex_run_left[3] = &tex_lib->textures[PLAYER_LEFT4];
    tex_run_left[4] = &tex_lib->textures[PLAYER_LEFT5];
    tex_run_left[5] = &tex_lib->textures[PLAYER_LEFT6];
    tex_run_left[6] = &tex_lib->textures[PLAYER_LEFT7];
    tex_run_left[7] = &tex_lib->textures[PLAYER_LEFT8];

    tex_idle[0] = &tex_lib->textures[PLAYER_IDLE];
    tex_idle[1] = &tex_lib->textures[PLAYER_IDLE];
    tex_idle[2] = &tex_lib->textures[PLAYER_IDLE];
    tex_idle[3] = &tex_lib->textures[PLAYER_IDLE];

    tex_ascend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_ascend[1] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_ascend[2] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_ascend[3] = &tex_lib->textures[PLAYER_RIGHT1];

    tex_descend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_descend[1] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_descend[2] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_descend[3] = &tex_lib->textures[PLAYER_RIGHT1];

    sniper_tex_left[0] = &tex_lib->textures[SNIPER_LEFT];
    sniper_tex_left[1] = &tex_lib->textures[SNIPER_LEFT];
    sniper_tex_left[2] = &tex_lib->textures[SNIPER_LEFT];
    sniper_tex_left[3] = &tex_lib->textures[SNIPER_LEFT];

    sniper_tex_right[0] = &tex_lib->textures[SNIPER_RIGHT];
    sniper_tex_right[1] = &tex_lib->textures[SNIPER_RIGHT];
    sniper_tex_right[2] = &tex_lib->textures[SNIPER_RIGHT];
    sniper_tex_right[3] = &tex_lib->textures[SNIPER_RIGHT];



    current_tex = tex_idle[0];
    current_weapon_tex= sniper_tex_right[0];
}

void Kaelen_Voss::update_movement_state(Movement_Control_Input input, float frameTime){
    mesh.acceleration = glm::vec3(0.0f, gravity, 0.0f);

    if(input.left){
        if(mesh.velocity.x>0){
            mesh.velocity.x = 0;
        }
        mesh.acceleration.x = -horizontal_acc;
    }

    if(input.right){
        if(mesh.velocity.x<0){
            mesh.velocity.x = 0;
        }
        mesh.acceleration.x = horizontal_acc;
    }

    if(input.up){
        if((jumpTimer > jump_cooldown) && jumpsLeft>0){
            mesh.velocity.y = jump_boost;
            jumpsLeft -=1;
            jumpTimer = 0.0f;
        }
    }
    jumpTimer += frameTime;

    if(input.down){
        mesh.velocity.y = -jump_boost;
    }

    if(mesh.acceleration.x == 0){
        mesh.acceleration.x -= (mesh.velocity.x * friction);
    }

    if(mesh.transform.position.y <= -0.99){
        jumpsLeft = 3;
        mesh.velocity.y = std::max(0.0f, mesh.velocity.y);
    }
    mesh.transform.position += mesh.velocity * frameTime;
    mesh.velocity += mesh.acceleration * frameTime;
    mesh.velocity.x = std::max(std::min(mesh.velocity.x, horizontal_speed_cap), -horizontal_speed_cap);

}

void Kaelen_Voss::update_texture(){
    prev_state_type = state_type;
    prev_facing_direction = facing_direction;
    if(mesh.velocity.x > 0.2){
        facing_direction = RIGHT;
        state_type = RUNNING;
    }else if(mesh.velocity.x < -0.2){
        facing_direction = LEFT;
        state_type = RUNNING;
    }else{
        state_type = IDLE;
    }

    if(state_type == RUNNING){
        if(facing_direction == RIGHT){
            if(prev_facing_direction == RIGHT){
                texture_duration +=1;
                if(texture_duration >= max_texture_duration){
                    current_tex_index +=1;
                    texture_duration =0;
                    if(current_tex_index >= sizeof(tex_run_right)/sizeof(Texture*)){
                        current_tex_index =0;
                    }
                }
                current_tex = tex_run_right[current_tex_index];
            }else{
                texture_duration=0;
                current_tex_index=0;
            }
            current_weapon_tex = sniper_tex_right[0];
            weapon_mesh.transform.position = mesh.transform.position + weapon_right_position_offset;
        }else{
            if(prev_facing_direction == LEFT){
                texture_duration +=1;
                if(texture_duration >= max_texture_duration){
                    current_tex_index +=1;
                    texture_duration =0;
                    if(current_tex_index >= sizeof(tex_run_left)/sizeof(Texture*)){
                        current_tex_index =0;
                    }
                }
                current_tex = tex_run_left[current_tex_index];
            }else{
                texture_duration=0;
                current_tex_index=0;
            }
            current_weapon_tex = sniper_tex_left[0];
            weapon_mesh.transform.position = mesh.transform.position + weapon_left_position_offset;
        }
    }else{
        if(facing_direction == LEFT){
            current_weapon_tex = sniper_tex_left[0];
            weapon_mesh.transform.position = mesh.transform.position + weapon_left_position_offset;

        }else if(facing_direction == RIGHT){
            current_weapon_tex = sniper_tex_right[0];
            weapon_mesh.transform.position = mesh.transform.position + weapon_right_position_offset;
        }
    }

}

void Kaelen_Voss::draw(Shader& shader){
    update_texture();
    shader.set_float("fragment_opacity", opacity);
    shader.set_int("use_texture", current_tex != nullptr ? 1 : 0);
    if(current_tex){
        current_tex->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);

    shader.set_int("use_texture", current_weapon_tex != nullptr? 1:0);
    if(current_weapon_tex){
        current_weapon_tex->bind(0);
        shader.set_int("tex", 0);
    }
    weapon_mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);
}


Beam::Beam(Texture_Library* tex_lib, glm::vec3 origin, float angle)
{
    lifetime = 2.0f;
    age = 0.0f;

    width = 0.7f;

    start = origin;

    const float maxLength = 1.0f;

    end = origin + glm::vec3(
        cos(angle),
        sin(angle),
        0.0f
    ) * maxLength;


    texture = &tex_lib->textures[SNIPER_BEAM];

    mesh.transform.position = start;
    mesh.transform.rotation.z = angle;
    mesh.transform.scale = glm::vec3(maxLength, width, 1.0f);

    mesh.additional_colour = glm::vec4(0.0f);
}

void Beam::draw(Shader& shader)
{
    shader.set_int("use_texture", texture != nullptr);
    shader.set_float("fragment_opacity", opacity);

    if(texture)
    {
        texture->bind(0);
        shader.set_int("tex", 0);
    }

    mesh.draw(shader, false);

    shader.set_int("use_texture", 0);
}

void Beam::update_texture(float dt)
{
    age += dt;

    float t = age / lifetime;
    t = glm::clamp(t, 0.0f, 1.0f);

    // Fade
    opacity = 1.0f - t;
}



//**************************
//  Tracker_robot
//  ***********************/
Tracker_robot::Tracker_robot(Texture_Library* tex_lib) : Entity(){
    mass = 10;
    elasticity_factor = 0.0;
    vertical_speed_cap = 4.0;
    horizontal_speed_cap = 1.2;
    vertical_acc = 30;
    horizontal_acc = 10;
    jump_boost = 3.3;
    state_type = IDLE;
    jumpsLeft=2;

    current_tex = &tex_lib->textures[TRACKER_IDLE];
    state_type = IDLE;

    mesh.transform.scale = glm::vec3(0.4f, 0.4f, 1.0f);    
    mesh.transform.position = glm::vec3(0.0f, -0.7f, 0.0f);
    mesh.hitbox.offset_min = glm::vec2(0.05f, 0.05f);
    mesh.hitbox.offset_max = glm::vec2(0.05f, 0.1f);
}


void Tracker_robot::update_movement_state(Movement_Control_Input input, float frameTime){
    mesh.acceleration = glm::vec3(0.0f, gravity, 0.0f);

    if(input.left){
        if(mesh.velocity.x>0){
            mesh.velocity.x = 0;
        }
        mesh.acceleration.x = -horizontal_acc;
    }

    if(input.right){
        if(mesh.velocity.x<0){
            mesh.velocity.x = 0;
        }
        mesh.acceleration.x = horizontal_acc;
    }


    if(input.up){
        if((jumpTimer > jump_cooldown) && jumpsLeft>0){
            mesh.velocity.y = jump_boost;
            jumpsLeft -=1;
            jumpTimer = 0.0f;
        }
    }
    jumpTimer += frameTime;

    if(input.down){
        mesh.velocity.y = -jump_boost;
    }

    if(mesh.acceleration.x == 0){
        mesh.acceleration.x -= (mesh.velocity.x * friction);
    }

    if(mesh.transform.position.y <= -0.99){
        jumpsLeft = 3;
        mesh.velocity.y = std::max(0.0f, mesh.velocity.y);
    }
    mesh.transform.position += mesh.velocity * frameTime;
    mesh.velocity += mesh.acceleration * frameTime;
    mesh.velocity.x = std::max(std::min(mesh.velocity.x, horizontal_speed_cap), -horizontal_speed_cap);
}

Movement_Control_Input Tracker_robot::think(float x_diff, float y_diff){
    Movement_Control_Input decision;
    decision.right = 0;
    decision.left = 0;
    decision.up = 0;
    decision.down = 0;


    if(x_diff > 0.04){
        decision.right = 1;
    }else if(x_diff < -0.04){
        decision.left = 1 ;
    }

    if(y_diff > 0.15){
        decision.up = 1;
    }

    return decision;
}

void Tracker_robot::draw (Shader& shader){
    shader.set_int("use_texture", current_tex != nullptr ? 1:0);
    shader.set_float("fragment_opacity", opacity);
    if(current_tex){
        current_tex->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);
}

//**************************
//  PLATFORM
//  ***********************/
Platform::Platform(){
    mesh.additional_colour = glm::vec4(0.0f);

}

void Platform::draw(Shader& shader){
    shader.set_int("use_texture", texture != nullptr ? 1 : 0);
    shader.set_float("fragment_opacity", opacity);
    if(texture){
        texture->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);
}

void Platform::update_hitbox(){
    mesh.update_hitbox();
}

Hitbox Platform::get_hitbox(){
    return mesh.hitbox;
}



// ******************************
// BACKGROUND
// ******************************
Background::Background(){
    mesh.transform.position = glm::vec3(-1.0f, -1.0f, 0.8f);
    mesh.transform.scale = glm::vec3(5.0f, 5.0f, 1.0f);
    mesh.additional_colour = glm::vec4(0.2f, 0.2f, 0.2f, 0.2f);
}

void Background::cycle_colour(){
    for(int i=0; i<3; i++){
        mesh.additional_colour[i] += colour_increment[i];

        if(mesh.additional_colour[i] > 1.0f){
            colour_increment[i] *= -1.0f;
        }else if(mesh.additional_colour[i] < 0.0f){
            colour_increment[i] *= -1.0f;
        }
    }
}

void Background::draw(Shader& shader){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader.set_int("use_texture", texture != nullptr ? 1 : 0);
    shader.set_float("fragment_opacity", opacity);
    if(texture){
        texture->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);

}


// *******************
// WORLD
// *******************
World::World(Texture_Library* tex_lib){

    background.texture = &tex_lib->textures[BACKGROUND];

    platforms[0].texture = &tex_lib->textures[PLATFORM];
    platforms[0].mesh.transform.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    platforms[0].mesh.transform.scale = glm::vec3(10.0f, 0.5f, 1.0f);

    platforms[1].texture = nullptr;
    platforms[1].mesh.transform.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    platforms[1].mesh.transform.scale = glm::vec3(1.0f, 0.2f, 1.0f);
    
    platforms[2].texture = nullptr;
    platforms[2].mesh.transform.position = glm::vec3(-0.0f, -0.0f, 0.0f);
    platforms[2].mesh.transform.scale = glm::vec3(1.0f, 0.2f, 1.0f);
    
    platforms[3].texture = nullptr;
    platforms[3].mesh.transform.position = glm::vec3(0.5f, 0.5f, 0.0f);
    platforms[3].mesh.transform.scale = glm::vec3(1.0f, 0.2f, 1.0f);
    
    platforms[4].texture = &tex_lib->textures[PLATFORM];
    platforms[4].mesh.transform.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    platforms[4].mesh.transform.scale = glm::vec3(0.2f, 2.0f, 1.0f);
    
    platforms[5].texture = &tex_lib->textures[PLATFORM];
    platforms[5].mesh.transform.position = glm::vec3(0.95f, -1.0f, 0.0f);
    platforms[5].mesh.transform.scale = glm::vec3(0.2f, 2.0f, 1.0f);

    for(int i = 0; i < platform_count; i++){
        platforms[i].update_hitbox();
    }

}

glm::vec4 World::get_ambient_colour(){
    return background.mesh.additional_colour;
}

void World::draw(Shader& shader){
    background.cycle_colour();
    background.draw(shader);

    for(int i=0; i<platform_count; i++){
        platforms[i].mesh.additional_colour = background.mesh.additional_colour;
        platforms[i].draw(shader);
    }
}




glm::vec3 World::get_total_collision_displacement(const Hitbox& a){
    glm::vec3 displacement(0.0f, 0.0f, 0.0f);
    Hitbox b;

    for(int i=0; i<platform_count; i++){
        b = platforms[i].mesh.hitbox;
        displacement += get_collision_displacement(a, b);
    }
    return displacement;
}
 
