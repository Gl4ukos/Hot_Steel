#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout(location = 2) in vec2 aTexCoord; //texture coords, position 2

uniform float y_offset;
uniform float x_rotation;
uniform float y_rotation;

uniform vec3 cameraPos;

out vec3 ourColor;
out vec2 TexCoord;


void main()
{
    vec3 Pos = aPos; 
    
    //apply y rotation
    float x = aPos[0]*cos(y_rotation) + aPos[2] * sin(y_rotation);
    float y = aPos[1];
    float z = -aPos[0]*sin(y_rotation) + aPos[2]*cos(y_rotation);

    //apply x rotation
    float y2 = y*cos(x_rotation) + z*sin(x_rotation);
    float z2 = -y*sin(x_rotation) + z*cos(x_rotation); 

    //apply translation
    y2 += y_offset;


    Pos[0] = x;
    Pos[1] = y2;
    Pos[2] = z2;

    //adding camera translation
    Pos += cameraPos;

    gl_Position = vec4(Pos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data

    TexCoord = aTexCoord;
}   