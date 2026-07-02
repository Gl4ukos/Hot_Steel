#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
  
uniform float y_offset;
uniform float x_rotation;
uniform float y_rotation;

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    vec3 Pos_inv = aPos; 
    

    //apply y rotation
    float x = Pos_inv[0]*cos(y_rotation) + Pos_inv[2] * sin(y_rotation);
    float y = Pos_inv[1];
    float z = -Pos_inv[0]*sin(y_rotation) + Pos_inv[2]*cos(y_rotation);

    //apply x rotation
    y = y*cos(x_rotation) + z*sin(x_rotation);
    z = -y*sin(x_rotation) + z*cos(x_rotation); 


    gl_Position = vec4(x, y+ y_offset, z, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}   