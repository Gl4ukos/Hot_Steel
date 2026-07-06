#include "Entities.h"



//**************************
//  KAELEN VOSS
//  ***********************/
Kaelen_Voss::Kaelen_Voss(){
    mass = 10;
    elasticity_factor = 0.0;
    vertical_speed_cap = 4.0;
    horizontal_speed_cap = 2.5;
    vertical_acc = 30;
    horizontal_acc = 30;
    jump_boost = 3.2;
    state = IDLE;
    mesh.transform.scale = glm::vec3(0.3f, 0.35f, 1.0f);    
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

void Kaelen_Voss::update_state(Player_state new_state){
    state = new_state;
}

void Kaelen_Voss::update_hitbox(){
    mesh.update_hitbox();
}

Hitbox Kaelen_Voss::get_hitbox(){
    return mesh.hitbox;
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