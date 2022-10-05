#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void showFPS(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *APP_TITLE = "Intro OpenGL: Window 1";
bool glFullscreen = true;

// For now, we keep our GLSL (super basic) shader defined in our main.cpp1
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = NULL;
    if (glFullscreen)
    {
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
        if (videoMode != NULL)
        {
            window = glfwCreateWindow(videoMode->width, videoMode->height, APP_TITLE, primaryMonitor, NULL);
        }
    }
    else
    {
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APP_TITLE, NULL, NULL);
    }
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};
    std::cout << &vertices << std::endl;

    // Normally called a VBO, the vertex buffer object lets us store
    // a lot of vertices in memory on the graphics card;
    // It's slow to get data from the CPU -> GPU, so we want to get
    // as much into the graphics cards at once (batching)
    unsigned int vertex_buffer_object;
    // Create a new buffer
    glGenBuffers(1, &vertex_buffer_object);
    // Bind the newly-created buffer to the GL_ARRAY_BUFFER target;
    // This means that any calls we make on the GL_ARRAY_BUFFER target will
    // be made on the currently-bound vertex_buffer_object
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    // Copy our user-defined data into the bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Compile our source code shader into the vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Test that our vertex shader worked flawlessly
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        showFPS(window);

        // input
        // -----
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        glClearColor(0.8f, 0.3f, 0.3f, 0.2f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        std::cout << "Pressing space..." << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void showFPS(GLFWwindow *window)
{
    static double prevSeconds = 0.0;
    static int frameCount = 0;
    double currSeconds = glfwGetTime(); // number of seconds since GLFW started
    double elapsedSeconds = currSeconds - prevSeconds;

    // limit to 4 updates/second
    if (elapsedSeconds > 0.25)
    {
        prevSeconds = currSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << APP_TITLE << "\tFPS: " << fps << "\tFrame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }

    frameCount++;
}
