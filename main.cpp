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

    World world(&tex_lib);

    Kaelen_Voss player;
    player.textures[0] = &tex_lib.textures[PLAYER_LEFT];
    player.textures[1] = &tex_lib.textures[PLAYER_RIGHT];
    player.textures[2] = &tex_lib.textures[PLAYER_IDLE];

    Kike kike;
    kike.textures[0] = &tex_lib.textures[KIKE];
    kike.textures[1] = &tex_lib.textures[KIKE];
    kike.textures[2] = &tex_lib.textures[KIKE];

    // MAIN LOOP
    shader.use();
    while(!glfwWindowShouldClose(window)){ //loops until the Esc button is pressed
        //calculating FPS
        float currentTime = glfwGetTime();
        frameTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        uint fps = 1 / frameTime;
        std::cout<< "\rFPS:"<<fps<<std::flush;

        // ***************************
        // UPDATING BACKGROUND
        // ***************************
        world.draw(shader);

        // *********************
        // UPDATING PLAYER
        // *********************
        control_player(window, &player);        
        player.mesh.transform.position += player.mesh.velocity * frameTime;
        player.mesh.velocity += player.mesh.acceleration * frameTime;
        player.mesh.velocity.x = std::max(std::min(player.mesh.velocity.x, player.horizontal_speed_cap), -player.horizontal_speed_cap);
        // player.mesh.transform.rotation.x = std::max(std::min((player.mesh.velocity.y / player.mesh.vertical_speed_cap), 0.6f), -0.6f); //update rotation y
        player.mesh.update_hitbox();

        // **********************
        // UPDATING KIKE
        // **********************
        kike.mesh.acceleration.y = gravity;
        kike.mesh.transform.position += kike.mesh.velocity * frameTime;
        kike.mesh.velocity += kike.mesh.acceleration * frameTime;
        kike.mesh.velocity.x = std::max(std::min(kike.mesh.velocity.x, kike.horizontal_speed_cap), - kike.horizontal_speed_cap);
        if(kike.mesh.transform.position.x > player.mesh.transform.position.x){
            if(kike.mesh.velocity.x > 0){
                kike.mesh.velocity.x = 0.0f;
                kike.mesh.acceleration.x = 0.0f;
            }
            kike.mesh.acceleration.x -= kike.horizontal_acc;
        }else if(kike.mesh.transform.position.x < player.mesh.transform.position.x){
            if(kike.mesh.velocity.x < 0){
                kike.mesh.velocity.x = 0.0f;
                kike.mesh.acceleration.x = 0.0f;
            }
            kike.mesh.acceleration.x += kike.horizontal_acc;

        }
        kike.update_hitbox();


        glm::vec3 collision_displacement = world.get_total_collision_displacement(player.mesh.hitbox);

        if(collision_displacement.x != 0.0 || collision_displacement.y != 0.0){
            player.mesh.transform.position += collision_displacement;
            if(collision_displacement.y != 0.0){
                player.mesh.velocity.y = 0.0f;
                jumpsLeft = 3;
            }
            if(collision_displacement.x != 0.0){
                player.mesh.velocity.x = 0.0f;
            }
        }


        collision_displacement = get_collision_displacement(player.mesh.hitbox, kike.mesh.hitbox);
        if(collision_displacement.x != 0.0 || collision_displacement.y != 0.0){ //in case of collision, then displace accordingly
            player.mesh.transform.position += collision_displacement;
            if(collision_displacement.y != 0.0){
                player.mesh.velocity.y = 0.0f;
                jumpsLeft = 3;
            }
            if(collision_displacement.x != 0.0){
                player.mesh.velocity.x = 0.0f;
            }
        }

        // checking for Kike collisions
        collision_displacement = world.get_total_collision_displacement(kike.mesh.hitbox);
        if(collision_displacement.x != 0.0 || collision_displacement.y != 0.0){ //in case of collision, then displace accordingly
            kike.mesh.transform.position += collision_displacement;
            if(collision_displacement.y != 0.0){
                kike.mesh.velocity.y = 0.0f;
            }
            if(collision_displacement.x != 0.0){
                kike.mesh.velocity.x = 0.0f;
            }
        }



        // *************
        // DRAWING
        // *************        
        if(player.mesh.velocity.x <-0.3){
            player.state = RUN_LEFT;
        }else if(player.mesh.velocity.x >0.3){
            player.state = RUN_RIGHT;
        }else if(std::abs(player.mesh.velocity.y) < 0.1){
            player.state = IDLE;
        }
        player.draw(shader);  

        kike.draw(shader);

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