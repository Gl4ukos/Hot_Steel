#include "Entities.h"



//**************************
//  KAELEN VOSS
//  ***********************/
Kaelen_Voss::Kaelen_Voss(){

}

void Kaelen_Voss::draw(Shader& shader){
    shader.set_int("use_texture", texture != nullptr ? 1 : 0);
    if(texture){
        texture->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, stretch_texture);
    shader.set_int("use_texture", 0);

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