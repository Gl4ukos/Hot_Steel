#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
  
uniform vec4 another_color;

void main()
{
    vec4 hom_ourColor = vec4(ourColor, 1.0f);
    FragColor = vec4(((another_color* another_color[3]) + hom_ourColor* (1.0 - another_color[3])));
}