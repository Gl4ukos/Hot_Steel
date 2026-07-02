#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


#include "utilities/Meshes.h" // !!! INCLUDES GLAD AND GLFW AND SHADER

//resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

// checks for keystrokes
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Takes the window as input together with a key, returns if this key is currently being pressed
        glfwSetWindowShouldClose(window, true);
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
    float y_rotation_inc = 0.03;

    Pyramid beacon;
    Rectangle surface;

    // MAIN LOOP
    while(!glfwWindowShouldClose(window)){ //loops until the X button is pressed
        

        //calculating FPS
        double currentTime = glfwGetTime();
        double frameTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        uint fps = 1 / frameTime;
        std::cout << "\rFPS:"<<fps<<std::flush;


        // calculating a new colour
        r = cycle_colour(r, increment1);
        g = cycle_colour(g, increment2);
        b = cycle_colour(b, increment3);
        y_offset = cycle_colour(y_offset, y_offset_inc);
        y_rotation = cycle_rads_wrap(y_rotation, y_rotation_inc);
        y_rotation += y_rotation_inc;

        // check for keystrokes
        processInput(window);

        // updating the background
        glClearColor(r,b,g, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // equiping the current shader
        shader.use();
        
        // drawing beacon
        beacon.transform.y_offset = -y_offset * 0.15f;
        beacon.transform.y_rotation = y_rotation;

        beacon.colour.r = 1.0f - r;
        beacon.colour.g = 1.0f - g;
        beacon.colour.b = 1.0f - b;
        beacon.colour.a = 0.7f; //opacity

        beacon.draw(shader);

        // drawing surface
        surface.colour.r = 1.0f;
        surface.colour.g = 0.5f;
        surface.colour.b = 0.2f;
        surface.colour.a = 0.8f;
        surface.draw(shader);

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