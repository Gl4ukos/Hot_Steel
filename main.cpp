#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


#include "utilities/Meshes.h" // !!! INCLUDES GLAD AND GLFW AND SHADER

float gravity = -15.0;
float friction = 10.0;
float frameTime;

//resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  


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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        mesh->velocity.y = mesh->jump_boost;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        mesh->velocity.y = mesh->jump_boost;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        mesh->velocity.y = -mesh->jump_boost;
    }

    if(mesh->acceleration.x ==0 ){ //if no forces on x, apply friction
        mesh->acceleration.x -= (mesh->velocity.x*friction);
    }

    if(mesh->transform.position.y <= -0.99){
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

float cycle_rads_wrap(float &value, float &increment){
    value += increment; 
    
    if(value > M_PI){
        value *= -1.0f;
    }else if(value < -M_PI){
        value *= -1.0f;
    }
    return value;
}

float cycle_colour(float &value, float &increment){
    value += increment; 
    
    if(value > 1.0f){
        increment *= -1.0f;
    }else if(value < 0.0f){
        increment *= -1.0f;
    }
    return value;
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

    // temp variables
    float r = 0.4;
    float g = 0.0;
    float b = 0.9;
    float increment1 = 0.01;
    float increment2 = 0.01;
    float increment3 = 0.01;

    float y_offset = 0.0f;
    float y_offset_inc = 0.01;

    float y_rotation = 0.0f;
    float y_rotation_inc = 0.02;

    //creating the objects
    Pyramid beacon;
    Hitbox beacon_hitbox;
    beacon.transform.position = glm::vec3(0.5f, 0.0f, 0.0f);
    beacon.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    beacon.transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);

    //creating the platforms
    Rectangle platforms[4];
    platforms[0].transform.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    platforms[0].transform.scale = glm::vec3(10.0f, 1.0f, 1.0f);

    platforms[1].transform.position = glm::vec3(0.2, -0.8, 0.0f);
    platforms[1].transform.scale = glm::vec3(2.0f, 2.0f, 1.0f);

    platforms[2].transform.position = glm::vec3(-0.2, 0.0, 0.0f);
    platforms[2].transform.scale = glm::vec3(1.5f, 1.0f, 1.0f);

    platforms[3].transform.position = glm::vec3(-0.9, 0.3, 0.0f);
    platforms[3].transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

    Hitbox platform_hitboxes[4];


    // MAIN LOOP
    while(!glfwWindowShouldClose(window)){ //loops until the Esc button is pressed
        
        //calculating FPS
        float currentTime = glfwGetTime();
        frameTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        uint fps = 1 / frameTime;
        std::cout<< "\rFPS:"<<fps<<std::flush;

        // CYCLING THE COLOURS
        r = cycle_colour(r, increment1);
        g = cycle_colour(g, increment2);
        b = cycle_colour(b, increment3);
        y_offset = cycle_colour(y_offset, y_offset_inc);
        y_rotation = cycle_rads_wrap(y_rotation, y_rotation_inc);
        y_rotation += y_rotation_inc;


        // check for keystrokes
        control_object(window, &beacon);

        // updating the background
        glClearColor(r,b,g, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // equiping the current shader
        shader.use();
        
        // drawing beacon

        //updating translation
        beacon.transform.position += beacon.velocity * frameTime;
        beacon.velocity += beacon.acceleration * frameTime;

        //check for collisions        
        for(int i=0; i<(sizeof(platform_hitboxes)/sizeof(Hitbox)); i++){
            platform_hitboxes[i] = platforms[i].get_hitbox();

            glm::vec3 collision_displacement = check_collision_2d(beacon.get_hitbox(), platform_hitboxes[i]);

            if(collision_displacement.x != 0.0f || collision_displacement.y != 0.0f){ //in case of collision, then displace accordingly
                beacon.transform.position += collision_displacement;
                if(collision_displacement.y != 0.0){
                    beacon.velocity.y = 0.0f;
                }
                if(collision_displacement.x != 0.0){
                    beacon.velocity.x = 0.0f;
                }
            }
        }
    

        //updating rotation
        beacon.velocity.x = std::max(std::min(beacon.velocity.x, beacon.horizontal_speed_cap), -beacon.horizontal_speed_cap);
        beacon.velocity.y = std::min(beacon.velocity.y, beacon.vertical_speed_cap);

        beacon.transform.rotation.z = std::max(std::min((beacon.velocity.x / beacon.horizontal_speed_cap), 0.8f),-0.8f);
        beacon.transform.rotation.x = std::max(std::min((beacon.velocity.y / beacon.vertical_speed_cap), 0.6f), -0.6f);
        
        if(std::abs(beacon.velocity.x) > 0.5){
            y_rotation = 0.0f;
        }
        beacon.transform.rotation.y = y_rotation;

        beacon.draw(shader);

        // drawing platforms
        for(int i=0; i<(sizeof(platform_hitboxes)/sizeof(Hitbox)); i++){
            platforms[i].draw(shader);
        }

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MORPHEUS ENGINE", NULL, NULL);
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