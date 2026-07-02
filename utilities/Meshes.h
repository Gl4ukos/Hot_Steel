#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

class Pyramid{

private:
    float vertices[24] = {
        // positions         // colors
        0.27f, 0.43f, 0.15f,  1.0f, 0.0f, 0.0f,   // top right front
        -0.27f, 0.43f, 0.15f,  0.0f, 1.0f, 0.0f,  // top left front
        0.0f,  -0.3f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom mid
        0.0f, 0.43f, -0.31f,  0.0f, 0.0f, 1.0f   // top mid back
    };

    //indices for pyramid faces
    unsigned int indices[12] = {
        0, 1, 2, // front
        1, 3, 2, // left
        3, 0, 2, // right
        0, 3, 1  // back
    };



    unsigned int VBO; // a Vertex Buffer Object (stores all raw vertex data (coordinates, colors, etc))
    unsigned int VAO; // a Vertex Array Object (stores all info about how the raw vertex data will be interpreted)
    unsigned int EBO; // element buffer object (these will be the indices telling OpenGL which vertices form triangles)

public:
    Pyramid(){
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




    }

    unsigned int get_vao(){
        return VAO;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    void destroy(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};