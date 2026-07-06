#version 330 core
out vec4 frag_colour;

in vec3 outColor;
in vec2 TexCoord;  

uniform vec4 another_colour;
uniform sampler2D tex;
uniform int use_texture;
uniform vec2 uvScale;

void main()
{
    vec4 base_colour = vec4(outColor, 1.0f);

    if(use_texture == 1){ //if there is texture use colour of texture
        base_colour = texture(tex, TexCoord * uvScale);
        frag_colour = vec4((another_colour * another_colour[3]) + base_colour * (1.0 - another_colour[3])); //blend with "another_colour"
    }else{ //if no texture use the base colour given by the vertices
        frag_colour = vec4(((another_colour* another_colour[3]) + base_colour* (1.0 - another_colour[3]))); //blend with "another_colour"
    }
}