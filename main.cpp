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

void control_object(GLFWwindow* window, Mesh* mesh)
{
    mesh->acceleration = glm::vec3(0.0f, gravity, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        if(mesh->velocity.x > 0){
            mesh->velocity.x = 0;
        }
        mesh->acceleration.x = -mesh->horizontal_acc;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        if(mesh->velocity.x < 0){
            mesh->velocity.x =0;
        }
        mesh->acceleration.x = mesh->horizontal_acc;
    }

    bool spaceDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool spacePressed = spaceDown && !spaceWasDown;
    spaceWasDown = spaceDown;
    if (spacePressed && jumpsLeft>0){
        mesh->velocity.y = mesh->jump_boost;
        jumpsLeft-=1;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        mesh->velocity.y = -mesh->jump_boost;
    }

    if(mesh->acceleration.x ==0 ){ //if no forces on x, apply friction
        mesh->acceleration.x -= (mesh->velocity.x*friction);
    }

    if(mesh->transform.position.y <= -0.99){
        jumpsLeft =2;
        mesh->velocity.y = std::max(0.0f, mesh->velocity.y);
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

    // temp variables
    float y_rotation = 0.0f;
    float y_rotation_inc = 0.02;

    //creating the objects
    Texture_Library tex_lib;

    Kaelen_Voss player;
    player.texture = &tex_lib.textures[PLAYER];

    Platform ground;
    ground.texture = &tex_lib.textures[PLATFORM];
    ground.mesh.transform.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    ground.mesh.transform.scale = glm::vec3(10.0f, 0.5f, 1.0f);

    Pyramid beacon;
    Background background;
    background.texture = &tex_lib.textures[BACKGROUND];
    
    // int platform_count = 1;
    // Rectangle platforms[1];
    // platforms[0].transform.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    // platforms[0].transform.scale = glm::vec3(10.0f, 0.5f, 1.0f);

    // platforms[1].transform.position = glm::vec3(0.2, -0.8, 0.0f);
    // platforms[1].transform.scale = glm::vec3(2.0f, 1.0f, 1.0f);

    // platforms[2].transform.position = glm::vec3(-0.2, 0.0, 0.0f);
    // platforms[2].transform.scale = glm::vec3(1.5f, 0.5f, 1.0f);

    // platforms[3].transform.position = glm::vec3(-0.9, 0.3, 0.0f);
    // platforms[3].transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);


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
        // control_object(window, &player.mesh);        
        // player.mesh.transform.position += player.mesh.velocity * frameTime;
        // player.mesh.velocity += player.mesh.acceleration * frameTime;
        // player.mesh.velocity.x = std::max(std::min(player.mesh.velocity.x, player.mesh.horizontal_speed_cap), -player.mesh.horizontal_speed_cap);
        // player.mesh.transform.rotation.x = std::max(std::min((player.mesh.velocity.y / player.mesh.vertical_speed_cap), 0.6f), -0.6f); //update rotation y
        // player.mesh.update_hitbox();

        // *********************
        // UPDATING BEACON
        // *********************

        control_object(window, &beacon); //keystroke check
        beacon.transform.position += beacon.velocity * frameTime; //position update
        beacon.velocity += beacon.acceleration * frameTime; // velocity update
        beacon.velocity.x = std::max(std::min(beacon.velocity.x, beacon.horizontal_speed_cap), -beacon.horizontal_speed_cap); //clamp x speed
        beacon.velocity.y = std::min(beacon.velocity.y, beacon.vertical_speed_cap);  //clamp y speed
        beacon.transform.rotation.z = std::max(std::min((beacon.velocity.x / beacon.horizontal_speed_cap), 0.8f),-0.8f); //update rotation z
        beacon.transform.rotation.x = std::max(std::min((beacon.velocity.y / beacon.vertical_speed_cap), 0.6f), -0.6f); //update rotation y
        // beacon.additional_colour = glm::vec4(background.rectangle.additional_colour[0], background.rectangle.additional_colour[1], background.rectangle.additional_colour[2], 0.3f); //colour update
        beacon.update_hitbox(); //hitbox update


        

        //check for collisions 
        // for(int i=0; i<(platform_count); i++){
        //     platforms[i].update_hitbox();
        //     glm::vec3 collision_displacement = check_collision_2d(beacon.hitbox, platforms[i].hitbox);

        //     if(collision_displacement.x != 0.0f || collision_displacement.y != 0.0f){ //in case of collision, then displace accordingly
        //         beacon.transform.position += collision_displacement;
        //         if(collision_displacement.y != 0.0){
        //             beacon.velocity.y = 0.0f;
        //             jumpsLeft = 3;
        //         }
        //         if(collision_displacement.x != 0.0){
        //             beacon.velocity.x = 0.0f;
        //         }
        //     }
        // }

        // **************************
        // UPDATING PLATFORMS
        // **************************
        // shader.set_int("tex", 0);
        // for (int i=0; i<platform_count; i++){
        //     platforms[i].additional_colour = glm::vec4(background.rectangle.additional_colour[0], background.rectangle.additional_colour[1], background.rectangle.additional_colour[2], 0.1f); //colour update
        // }

        // *************
        // DRAWING
        // *************
        background.draw(shader);
        player.draw(shader);
        beacon.draw(shader,0);
        ground.draw(shader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    beacon.destroy();
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