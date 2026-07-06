#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


#include "utilities/Meshes.h" // !!! INCLUDES GLAD AND GLFW AND SHADER
#include "utilities/Entities.h"
#include "utilities/TextureLibrary/Textures.h"

float gravity = -15.0;
float friction = 10.0;
float frameTime;

//resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

int jumpsLeft = 3;
bool spaceWasDown = false;

void control_player(GLFWwindow* window, Kaelen_Voss* player)
{
    player->mesh.acceleration = glm::vec3(0.0f, gravity, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        if(player->mesh.velocity.x > 0){
            player->mesh.velocity.x = 0;
        }
        player->mesh.acceleration.x = -player->horizontal_acc;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        if(player->mesh.velocity.x < 0){
            player->mesh.velocity.x =0;
        }
        player->mesh.acceleration.x = player->horizontal_acc;
    }

    bool spaceDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool spacePressed = spaceDown && !spaceWasDown;
    spaceWasDown = spaceDown;
    if (spacePressed && jumpsLeft>0){
        player->mesh.velocity.y = player->jump_boost;
        jumpsLeft-=1;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        player->mesh.velocity.y = -player->jump_boost;
    }

    if(player->mesh.acceleration.x ==0 ){ //if no forces on x, apply friction
        player->mesh.acceleration.x -= (player->mesh.velocity.x*friction);
    }

    if(player->mesh.transform.position.y <= -0.99){
        jumpsLeft =2;
        player->mesh.velocity.y = std::max(0.0f, player->mesh.velocity.y);
    }
}


glm::vec3 check_collision_2d(const Hitbox& a, const Hitbox& b)
{
    glm::vec3 displacement(0.0f, 0.0f, 0.0f);
    bool in_x = false;

    if(a.min.x <= b.max.x && a.max.x >= b.min.x){
        float right_dis = b.max.x - a.min.x;
        float left_dis = b.min.x - a.max.x;

        if(std::abs(right_dis) > std::abs(left_dis)){
            displacement.x = left_dis;
        }else{
            displacement.x = right_dis;
        }

        if(a.min.y <= b.max.y && a.max.y >= b.min.y){
            right_dis = b.max.y - a.min.y;
            left_dis = b.min.y - a.max.y;

            if(std::abs(right_dis) > std::abs(left_dis)){
                displacement.y = left_dis;
            }else{
                displacement.y = right_dis;
            }

            // COLLISION DATECTED
            // we keep only the least displacement to return
            if(std::abs(displacement.x) > std::abs(displacement.y)){
                displacement.x = 0.0f; 
            }else{
                displacement.y = 0.0f;
            }
        }else{
            displacement.x = 0.0f;
        }
    }
    return displacement;
}

std::string load_file_to_string(std::string filename){
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* initialise_glfw();
double lastFrameTime = glfwGetTime();
int main()
{
    //setting up
    GLFWwindow* window = initialise_glfw();
    Shader shader ("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    glEnable(GL_DEPTH_TEST); //enabling depth
    glEnable(GL_BLEND); //enabling blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //creating the objects
    Texture_Library tex_lib;

    Kaelen_Voss player;
    player.texture = &tex_lib.textures[PLAYER];

    int platform_count = 4;
    Platform ground[4];
    ground[0].texture = &tex_lib.textures[PLATFORM];
    ground[0].mesh.transform.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    ground[0].mesh.transform.scale = glm::vec3(10.0f, 0.5f, 1.0f);

    ground[1].texture = &tex_lib.textures[PLATFORM];
    ground[1].mesh.transform.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    ground[1].mesh.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);

    ground[2].texture = &tex_lib.textures[PLATFORM];
    ground[2].mesh.transform.position = glm::vec3(-0.0f, -0.0f, 0.0f);
    ground[2].mesh.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);

    ground[3].texture = &tex_lib.textures[PLATFORM];
    ground[3].mesh.transform.position = glm::vec3(0.5f, 0.5f, 0.0f);
    ground[3].mesh.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);


    Background background;
    background.texture = &tex_lib.textures[BACKGROUND];
    
    // MAIN LOOP
    while(!glfwWindowShouldClose(window)){ //loops until the Esc button is pressed
        shader.use();
        //calculating FPS
        float currentTime = glfwGetTime();
        frameTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        uint fps = 1 / frameTime;
        std::cout<< "\rFPS:"<<fps<<std::flush;

        // ***************************
        // UPDATING BACKGROUND
        // ***************************
        background.cycle_colour();


        // *********************
        // UPDATING PLAYER
        // *********************
        control_player(window, &player);        
        player.mesh.transform.position += player.mesh.velocity * frameTime;
        player.mesh.velocity += player.mesh.acceleration * frameTime;
        player.mesh.velocity.x = std::max(std::min(player.mesh.velocity.x, player.horizontal_speed_cap), -player.horizontal_speed_cap);
        // player.mesh.transform.rotation.x = std::max(std::min((player.mesh.velocity.y / player.mesh.vertical_speed_cap), 0.6f), -0.6f); //update rotation y
        player.mesh.update_hitbox();



        //check for collisions 
        for(int i=0; i<(platform_count); i++){
            ground[i].update_hitbox();
            glm::vec3 collision_displacement = check_collision_2d(player.mesh.hitbox, ground[i].get_hitbox());
            if(collision_displacement.x != 0.0f || collision_displacement.y != 0.0f){ //in case of collision, then displace accordingly
                player.mesh.transform.position += collision_displacement;
                if(collision_displacement.y != 0.0){
                    player.mesh.velocity.y = 0.0f;
                    jumpsLeft = 3;
                }
                if(collision_displacement.x != 0.0){
                    player.mesh.velocity.x = 0.0f;
                }
            }
        }

        // *************
        // DRAWING
        // *************
        background.draw(shader);
        player.draw(shader);

        for(int i=0; i<platform_count; i++){
            ground[i].draw(shader);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); //closes the window
    return 0;
}

GLFWwindow* initialise_glfw(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // This means I want OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //This means I want OpenGL x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //CORE PROFILE = modern OpenGL

    // Creating window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HOT STEEL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window); //Creting GLFW context
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// adding callback for each time the window gets fumbled with


    // Initializing GLAD (MUST BE DONE AFTER CREATING GLFW CONTEXT)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    return window;
}