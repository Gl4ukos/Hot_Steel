#include "Entities.h"

Kaelen_Voss::Kaelen_Voss(){

}

void Kaelen_Voss::draw(Shader& shader){
    shader.set_int("use_texture", texture != nullptr ? 1 : 0);
    if(texture){
        texture->bind(0);
        shader.set_int("tex", 0);
    }
    mesh.draw(shader, 0);
    shader.set_int("use_texture", 0);

}

void Kaelen_Voss::update_hitbox(){
}

void Kaelen_Voss::update_texture(){

}