#include <iostream>
#include "../Dependencies/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../Header/Shader.hpp"

namespace GLWindow
{
    const char *Title = "OpenGL - Shaders";
    unsigned int Width = 800;
    unsigned int Height = 600;
}

void ResizeCallback(GLFWwindow *window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

float Xoffset = 0.0f;
float Yoffset = 0.0f;

// Basic Controls

void InputManager(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        Yoffset += 0.05f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        Xoffset -= 0.05f;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        Yoffset -= 0.05f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        Xoffset += 0.05f;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        Xoffset = 0.0f;
        Yoffset = 0.0f;
    }
        
}

int main()
{
    if (glfwInit() == GL_FALSE)
    {
        std::cout << "[-] Failed to load GLFW\n";
        return -1;
    }
    else
    {
        std::cout << "[+] GLFW initialized\n";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *Canvas = glfwCreateWindow(GLWindow::Width, GLWindow::Height, GLWindow::Title, NULL, NULL);

    if (Canvas == NULL)
    {
        std::cout << "[-] Failed to create canvas\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Canvas);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[-] Failed to initialize GLad\n";
        return -1;
    }

    glViewport(0, 0, GLWindow::Width, GLWindow::Height);

    glfwSetFramebufferSizeCallback(Canvas, ResizeCallback);

    float Vertices[] = {
        // positions         // colors
        0.2f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.2f, -0.2f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 1.0f    // top

    };

    // CREATE VBO & VAO
    unsigned int VBO, VAO;
    // ASSIGN UID VAO
    glGenVertexArrays(1, &VAO);
    // ASSIGN UID VBO
    glGenBuffers(1, &VBO);
    // BIND VAO
    glBindVertexArray(VAO);
    // ASSIGN VBO BUFFER TYPE
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // PASS VERTICES TO CPU BUFFER -> TO GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Shader ShaderTest("Shader//VertexShader.vs", "Shader//FragmentShader.fs");

    while (!glfwWindowShouldClose(Canvas))
    {
        InputManager(Canvas);

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ShaderTest.use();
        ShaderTest.setFloat("Xoffset", Xoffset);
        ShaderTest.setFloat("Yoffset", Yoffset);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(Canvas);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}