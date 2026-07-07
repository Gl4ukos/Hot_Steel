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
    state = IDLE;
    mesh.transform.scale = glm::vec3(0.3f, 0.35f, 1.0f);  

    textures[0] = nullptr;
    textures[1] = nullptr;
    textures[2] = nullptr;
}

void Entity::draw(Shader& shader){
    std::cout<<"DRAW FUNCTION NOT ASSIGNED\n";
}

void Entity::update_hitbox(){
    mesh.update_hitbox();
}

void Entity::update_state(Entity_state new_state){
    state = new_state;
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
    horizontal_speed_cap = 2.5;
    vertical_acc = 30;
    horizontal_acc = 30;
    jump_boost = 3.2;
    state = IDLE;
    mesh.transform.scale = glm::vec3(0.3f, 0.35f, 1.0f);    

    textures[0] = &tex_lib->textures[PLAYER_LEFT];
    textures[1] = &tex_lib->textures[PLAYER_RIGHT];
    textures[2] = &tex_lib->textures[PLAYER_IDLE];
}

void Kaelen_Voss::draw(Shader& shader){
    shader.set_int("use_texture", textures[state] != nullptr ? 1 : 0);
    if(textures[state]){
        textures[state]->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);

}

//**************************
//  KIKE
//  ***********************/
Kike::Kike(Texture_Library* tex_lib) : Entity(){

    textures[0] = &tex_lib->textures[KIKE];
    textures[1] = &tex_lib->textures[KIKE];
    textures[2] = &tex_lib->textures[KIKE];

    mesh.transform.scale = glm::vec3(0.3f, 0.35f, 1.0f);    
    mesh.transform.position = glm::vec3(0.0f, -0.7f, 0.0f);
}

void Kike::draw (Shader& shader){
    shader.set_int("use_texture", textures[state] != nullptr ? 1:0);
    if(textures[state]){
        textures[state]->bind(0);
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
 
