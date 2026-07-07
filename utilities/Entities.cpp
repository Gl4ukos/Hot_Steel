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
    horizontal_speed_cap = 2.5;
    vertical_acc = 30;
    horizontal_acc = 30;
    jump_boost = 3.2;
    gravity = -15.0;
    friction = 10.0;
    jumpsLeft = 1;
    mesh.transform.scale = glm::vec3(0.3f, 0.35f, 1.0f);  

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
    horizontal_speed_cap = 0.5;
    vertical_acc = 30;
    horizontal_acc = 30;
    jump_boost = 3.2;
    state_type = IDLE;
    jumpsLeft=3;
    spaceWasDown = 0;
    mesh.transform.scale = glm::vec3(0.5f, 0.5f, 1.0f);    

    current_tex = &tex_lib->textures[PLAYER_RIGHT1];
    tex_run_right[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_run_right[1] = &tex_lib->textures[PLAYER_RIGHT2];
    tex_run_right[2] = &tex_lib->textures[PLAYER_RIGHT3];
    tex_run_right[3] = &tex_lib->textures[PLAYER_RIGHT4];

    tex_run_left[0] = &tex_lib->textures[PLAYER_LEFT1];
    tex_run_left[1] = &tex_lib->textures[PLAYER_LEFT2];
    tex_run_left[2] = &tex_lib->textures[PLAYER_LEFT3];
    tex_run_left[3] = &tex_lib->textures[PLAYER_LEFT4];

    tex_idle[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_idle[1] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_idle[2] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_idle[3] = &tex_lib->textures[PLAYER_RIGHT1];

    tex_ascend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_ascend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_ascend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_ascend[0] = &tex_lib->textures[PLAYER_RIGHT1];

    tex_descend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_descend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_descend[0] = &tex_lib->textures[PLAYER_RIGHT1];
    tex_descend[0] = &tex_lib->textures[PLAYER_RIGHT1];
}


void Kaelen_Voss::update_movement_state(Movement_Control_Input input, float frame_duration){
    mesh.acceleration = glm::vec3(0.0f, gravity, 0.0f);
    prev_state_type = state_type;

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

    bool spaceDown = input.up;
    bool spacePressed = spaceDown && !spaceWasDown;
    spaceWasDown = spaceDown;

    if(spacePressed && jumpsLeft>0){
        mesh.velocity.y = jump_boost;
        jumpsLeft -=1;
    }

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
}

void Kaelen_Voss::update_texture(){

    if(mesh.velocity.x > 0.2){
        state_type = RUN_RIGHT;
    }else if(mesh.velocity.x < -0.2){
        state_type = RUN_LEFT;
    }else{
        state_type = IDLE;
    }

    if(state_type == RUN_RIGHT){
        if(prev_state_type == RUN_RIGHT){
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
        }
    }else if(state_type == RUN_LEFT){
        if(prev_state_type == RUN_LEFT){
            texture_duration +=1;
            if(texture_duration >= max_texture_duration){
                current_tex_index +=1;
                texture_duration =0;
                if(current_tex_index >= sizeof(tex_run_right)/sizeof(Texture*)){
                    current_tex_index =0;
                }
            }
            current_tex = tex_run_left[current_tex_index];
        }else{
            texture_duration=0;
        }
    }else{
        current_tex = tex_idle[0];
    }

}

void Kaelen_Voss::draw(Shader& shader){
    update_texture();
    shader.set_int("use_texture", current_tex != nullptr ? 1 : 0);
    if(current_tex){
        current_tex->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);
}


//**************************
//  KIKE
//  ***********************/
Kike::Kike(Texture_Library* tex_lib) : Entity(){

    current_tex = &tex_lib->textures[KIKE];
    state_type = IDLE;

    mesh.transform.scale = glm::vec3(0.3f, 0.35f, 1.0f);    
    mesh.transform.position = glm::vec3(0.0f, -0.7f, 0.0f);
}


void Kike::update_movement_state(Movement_Control_Input input, float frame_duration){
    mesh.acceleration = glm::vec3(0.0f, gravity, 0.0f);
    prev_state_type = state_type;

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

    bool spaceDown = input.up;
    bool spacePressed = spaceDown && !spaceWasDown;
    spaceWasDown = spaceDown;

    if(spacePressed && jumpsLeft>0){
        mesh.velocity.y = jump_boost;
        jumpsLeft -=1;
    }

    if(input.down){
        mesh.velocity.y = -jump_boost;
    }

    if(mesh.acceleration.x == 0){
        mesh.acceleration.x -= (mesh.velocity.x * friction);
    }

    if(mesh.transform.position.y <= -0.99){
        jumpsLeft = 3;
        mesh.velocity.y = std::max(0.0f, mesh.velocity.y);
        state_type = IDLE;
    }

}

void Kike::draw (Shader& shader){
    shader.set_int("use_texture", current_tex != nullptr ? 1:0);
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
    // glClearColor(additional_colour[0], additional_colour[1], additional_colour[2], additional_colour[3]); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader.set_int("use_texture", texture != nullptr ? 1 : 0);
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
    platforms[1].mesh.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);
    
    platforms[2].texture = nullptr;
    platforms[2].mesh.transform.position = glm::vec3(-0.0f, -0.0f, 0.0f);
    platforms[2].mesh.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);
    
    platforms[3].texture = nullptr;
    platforms[3].mesh.transform.position = glm::vec3(0.5f, 0.5f, 0.0f);
    platforms[3].mesh.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);
    
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
 
