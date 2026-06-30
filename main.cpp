#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h> //GLAD must be included before GLFW, because GLFW can include OpenGL headers internally

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

float cycle_colour(float &value, float &increment){
    value += increment; 
    
    if(value > 1.0f){
        increment *= -1.0f;
    }else if(value < 0.0f){
        increment *= -1.0f;
    }
    return value;
}

int main()
{
    // Initializing GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // This means I want OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //This means I want OpenGL x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //CORE PROFILE = modern OpenGL

    // Creating window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //Creting GLFW context


    // Initializing GLAD (MUST BE DONE AFTER CREATING GLFW CONTEXT)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    // adding callback for each time the window gets fumbled with
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    float r = 0.4;
    float g = 0.0;
    float b = 0.9;
    float increment1 = 0.01;
    float increment2 = 0.01;
    float increment3 = 0.01;

    while(!glfwWindowShouldClose(window)){ //loops until the X button is pressed
        processInput(window);
        
        r = cycle_colour(r, increment1);
        g = cycle_colour(g, increment2);
        b = cycle_colour(b, increment3);
        
        glClearColor(r,b,g, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        // Note that we also specify the color to clear the screen with using glClearColor. 
        // Whenever we call glClear and clear the color buffer, 
        // the entire color buffer will be filled with the color as configured by glClearColor. 
        // This will result in a dark green-blueish color. 

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate(); //closes the window
    return 0;
}