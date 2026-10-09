#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; //texture coords, position 2

uniform mat4 model;
uniform vec3 camera_pos;

vec4 scaling = vec4(0.6f, 0.9f, 1.0f, 1.0f) ;

vec4 worldPos;
out vec3 outColor;
out vec2 TexCoord;


void main()
{
    vec3 Pos = aPos; 

    gl_Position = (scaling) * (model * vec4(Pos, 1.0) - vec4(camera_pos, 0.0f));
    outColor = aColor; // set ourColor to the input color we got from the vertex data
    TexCoord = aTexCoord;
}   