#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 TexCoord;  

uniform vec4 another_color;
uniform sampler2D tex;
uniform int use_texture;
uniform vec2 uvScale;

void main()
{
    vec4 base_color = vec4(outColor, 1.0f);
    vec4 texColor;

    if(use_texture == 1){
        FragColor = texture(tex, TexCoord * uvScale);
    }else{
        texColor = vec4(1.0); // white no texture
        FragColor = vec4(((another_color* another_color[3]) + base_color* (1.0 - another_color[3])));
    }
}