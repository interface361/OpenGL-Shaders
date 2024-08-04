#include <iostream>
#include "../Dependencies/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../Header/Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../Textures/ImageSTB.h"

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

// build and compile our shader zprogram
    // ------------------------------------
    Shader ShaderTest("Shader//VertexShader.vs", "Shader//FragmentShader.fs"); 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    unsigned char *data = stbi_load("Dependencies//Images//container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    while (!glfwWindowShouldClose(Canvas))
    {
        InputManager(Canvas);

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ShaderTest.use();
        ShaderTest.setFloat("Xoffset", Xoffset);
        ShaderTest.setFloat("Yoffset", Yoffset);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(Canvas);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}