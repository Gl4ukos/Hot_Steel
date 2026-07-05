#include "Meshes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"



// **********************************
// Mesh
// **********************************

glm::mat4 Mesh::getModelMatrix(){
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, transform.position);
    model = glm::rotate(model, transform.rotation.x, glm::vec3(1,0,0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0,0,1));
    model = glm::scale(model, transform.scale);

    return model;
}

void Mesh::destroy(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

unsigned int Mesh::get_vao(){
    return VAO;
}

// **********************************
// PYRAMID
// **********************************

Pyramid::Pyramid(){

    glGenBuffers(1, &VBO); // OpenGL creates one buffer object internally and gives its ID to VBO and so on below
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds VBO as GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy the vertice into the buffer memory 
    // (use GL_DYNAMIC_DRAW if the data changes often and get used often)
    // (use GL_STATIC_DRAW if the data changes rarely, but gets used often)

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    // explanation for agruments given:
    // 1	attribute location in shader (layout(location = 1))
    // 3	number of components (r,g,b)
    // GL_FLOAT	data type
    // GL_FALSE	don’t normalize
    // 6*sizeof(float)	distance between TWO vertices
    // 3*sizeof(float)	where color starts inside each vertex
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST); //enabling depth

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 



    // //loading textures
    // // creating a texture on gl
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // // set the texture wrapping/filtering options (on the currently bound texture object)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load and generate the texture
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("textures/wall.jpg", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);

    mass = 10;
    elasticity_factor = 0.0;
    vertical_speed_cap = 4.0;
    horizontal_speed_cap = 2.5;
    vertical_acc = 30;
    horizontal_acc = 30;
    jump_boost = 3.2;

    additional_colour = glm::vec4(0.4f, 0.0f, 0.9f, 0.5f);
    transform.position = glm::vec3(0.5f, 0.0f, 0.0f);
    transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);

            
}

void Pyramid::draw(Shader& shader){
    glm::mat4 model = getModelMatrix();
    shader.set_mat4("model", model);
    shader.set_vec4("another_color", additional_colour);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

float Pyramid::cycle_rads_wrap(float &value, float &increment){
    value += increment; 
    
    if(value > M_PI){
        value *= -1.0f;
    }else if(value < -M_PI){
        value *= -1.0f;
    }
    return value;
}

float Pyramid::cycle_value(float value, float increment){
    value += increment;

    if(value > 1.0f){
        increment *= -1.0f;
    }else if(value < 0.0f){
        increment *= -1.0f;
    }
    return value;
}

void Pyramid::update_hitbox()
{
    // Local bounds of the pyramid
    float min_x = vertices[6];
    float min_y = vertices[13];
    float min_z = vertices[20];
    float max_x = vertices[0];
    float max_y = vertices[1];
    float max_z = vertices[2];

    glm::vec3 localMin(min_x, min_y, min_z);
    glm::vec3 localMax(max_x, max_y, max_z);

    // Apply scale
    localMin *= transform.scale;
    localMax *= transform.scale;

    // Apply translation
    hitbox.min = localMin + transform.position;
    hitbox.max = localMax + transform.position;
}


// **********************************
// RECTANGLE
// **********************************

Rectangle::Rectangle(){
    glGenBuffers(1, &VBO); // OpenGL creates one buffer object internally and gives its ID to VBO and so on below
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds VBO as GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy the vertice into the buffer memory 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    mass = -1; //immobile 
    elasticity_factor=0.0;
}

void Rectangle::draw(Shader& shader){
    glm::mat4 model = getModelMatrix();
    shader.set_mat4("model", model);
    shader.set_vec4("another_color", additional_colour);
    shader.set_vec2("uvScale", transform.scale.x, transform.scale.y);
    texture.bind(0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    shader.set_int("use_texture", 0);
    
}

void Rectangle::update_hitbox()
{
    // Local bounds of the rectangle
    float min_x = vertices[0];
    float min_y = vertices[17];
    float min_z = vertices[2];
    float max_x = vertices[8];
    float max_y = vertices[1];
    float max_z = vertices[2];

    glm::vec3 localMin(min_x, min_y, min_z);
    glm::vec3 localMax(max_x, max_y, max_z);

    // Apply scale
    localMin *= transform.scale;
    localMax *= transform.scale;

    // Apply translation
    hitbox.min = localMin + transform.position;
    hitbox.max = localMax + transform.position;
}

void Rectangle::load_texture(std::string filename){
    if(!texture.load(filename)){
        std::cout<<"TEXTURE NOT FOUND "<< filename<<" \n";
    }
}



void Background::cycle_colour(){
    for(int i=0; i<3; i++){
        additional_colour[i] += colour_increment[i];

        if(additional_colour[i] > 1.0f){
            colour_increment[i] *= -1.0f;
        }else if(additional_colour[i] < 0.0f){
            colour_increment[i] *= -1.0f;
        }
    }
}

void Background::draw(){
    glClearColor(additional_colour[0], additional_colour[1], additional_colour[2], additional_colour[3]); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


bool Texture::load(const std::string& path)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data) return false;

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 w, h, 0,
                 format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // IMPORTANT default settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::bind(GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}