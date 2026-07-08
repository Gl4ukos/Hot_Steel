#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


#include "utilities/Meshes.h" // !!! INCLUDES GLAD AND GLFW AND SHADER
#include "utilities/Entities.h"
#include "utilities/TextureLibrary/Textures.h"


//resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

int jumpsLeft = 3;
bool spaceWasDown = false;
Movement_Control_Input movement_control_input;


void handle_key_input(GLFWwindow* window){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        movement_control_input.left = 1;
    }else{
        movement_control_input.left = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        movement_control_input.right = 1;
    }else{
        movement_control_input.right = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        movement_control_input.down = 1;
    }else{
        movement_control_input.down = 0;
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        movement_control_input.up = 1;
    }else{
        movement_control_input.up = 0;
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
float frameTime;
double lastFrameTime = glfwGetTime();
int main()
{
    //setting up
    GLFWwindow* window = initialise_glfw();
    Shader shader ("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    // glEnable(GL_DEPTH_TEST); //enabling depth
    glEnable(GL_BLEND); //enabling blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //creating the objects
    Texture_Library tex_lib;

    World world(&tex_lib);
    Kaelen_Voss player(&tex_lib);
    Tracker_robot tracker_robot(&tex_lib);


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
        handle_key_input(window);  
        player.update_movement_state(movement_control_input, frameTime);   
        player.mesh.update_hitbox();

        // **********************
        // UPDATING tracker_robot
        // **********************

        float x_diff = player.mesh.transform.position.x - tracker_robot.mesh.transform.position.x;
        float y_diff = player.mesh.transform.position.y - tracker_robot.mesh.transform.position.y;
        Movement_Control_Input tracker_robot_decision = tracker_robot.think(x_diff, y_diff);
        tracker_robot.update_movement_state(tracker_robot_decision, frameTime);
        tracker_robot.update_hitbox();


        // get collision displacement from environment
        glm::vec3 collision_displacement = world.get_total_collision_displacement(player.mesh.hitbox);
        // apply collision displacement to player
        if(collision_displacement.x != 0.0 || collision_displacement.y != 0.0){
            player.mesh.transform.position += collision_displacement;
            if(collision_displacement.y != 0.0){
                player.mesh.velocity.y = 0.0f;
                player.jumpsLeft = 3;
            }
            if(collision_displacement.x != 0.0){
                player.mesh.velocity.x = 0.0f;
            }
        }

        // get collision displacement from player and tracker robot and apply
        // collision_displacement = get_collision_displacement(player.mesh.hitbox, tracker_robot.mesh.hitbox);
        // if(collision_displacement.x != 0.0 || collision_displacement.y != 0.0){ //in case of collision, then displace accordingly
        //     player.mesh.transform.position += collision_displacement;
        //     if(collision_displacement.y != 0.0){
        //         player.mesh.velocity.y = 0.0f;
        //         player.jumpsLeft = 3;
        //     }
        //     if(collision_displacement.x != 0.0){
        //         player.mesh.velocity.x = 0.0f;
        //     }
        // }

        // checking for tracker_robot collision with environmnet and applying displacement
        collision_displacement = world.get_total_collision_displacement(tracker_robot.mesh.hitbox);
        if(collision_displacement.x != 0.0 || collision_displacement.y != 0.0){ //in case of collision, then displace accordingly
            tracker_robot.mesh.transform.position += collision_displacement;
            if(collision_displacement.y != 0.0){
                tracker_robot.mesh.velocity.y = 0.0f;
            }
            if(collision_displacement.x != 0.0){
                tracker_robot.mesh.velocity.x = 0.0f;
            }
        }



        // *************
        // DRAWING
        // *************        
        player.draw(shader);  

        tracker_robot.draw(shader);

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