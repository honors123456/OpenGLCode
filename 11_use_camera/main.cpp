#define STB_IMAGE_IMPLEMENTATION
#include "src/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

void windowSizeChanged_callback(GLFWwindow *window, int width, int height);
void mouseCursor_callback(GLFWwindow *window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow *window, int button, int action, int mods);
void mouseWheel_callback(GLFWwindow *window, double xoffset, double yoffset);
void keyInput_callback(GLFWwindow *window);

// camera value
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// delta time
float deltaTime = 0.0f;
float lastTime = 0.0f;

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  512.0f / 2.0;
float lastY =  512.0f / 2.0;
float fov   =  45.0f;
float angle = 0.0f;

unsigned int program;

int main(int argc, char **argv)
{
    // 初始化glfw环境
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 1000, "Use_Camera", NULL, NULL);
    if (!window)
    {
        const char *description;
        int errCode = glfwGetError(&description);
        std::cerr << "create window failed! err code is " << errCode << " with " << description << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << " init glad failed!" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 1000);

    Shader shader("../../shaders/vertex.glsl", "../../shaders/fragment.glsl");
    program = shader.getProgram();

    //注册窗口变化监听事件
    glfwSetFramebufferSizeCallback(window,windowSizeChanged_callback);

    //鼠标光标事件监听
    glfwSetCursorPosCallback(window,mouseCursor_callback);

    //鼠标按钮事件监听
    glfwSetMouseButtonCallback(window,mouseButton_callback);

    //鼠标滚轮事件监听
    glfwSetScrollCallback(window,mouseWheel_callback);

    // float vertexes[] = {
    //     //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
    //     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    // };

    float windowAspectRatio = (float)800 / (float)1000;
    float imageAspectRatio = (float)3072 / (float)2048;

    unsigned int VAO;
    unsigned int VBO;

    if (windowAspectRatio > imageAspectRatio) {
        // 窗口比较宽，以图片高度为基准，调整宽度
        float scale = imageAspectRatio / windowAspectRatio;
        float vertexes[] = {
            //     ---- 位置 ----         ---- 颜色 ----       - 纹理坐标 -
            scale * 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上角
            scale * 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下角
            -scale * 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下角
            -scale * 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上角
        };

        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

    } else {
        // 窗口比较窄，以图片宽度为基准，调整高度
        float scale = windowAspectRatio / imageAspectRatio;
        float vertexes[] = {
            //     ---- 位置 ----         ---- 颜色 ----       - 纹理坐标 -
            0.5f,  scale * 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上角
            0.5f, -scale * 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下角
            -0.5f, -scale * 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下角
            -0.5f,  scale * 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上角
        };
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    }

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 创建纹理对象
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../../res/1.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << " failed to load texture " << std::endl;
    }

    // 释放图像内存
    stbi_image_free(data);

    // shader
    
    shader.use();

    shader.setInt("ourTexture", 0);

    int i=0;
    while (!glfwWindowShouldClose(window))
    {
        keyInput_callback(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        //glw库旋转用的都是弧度制，要使用glm::radians函数转换为角度制
        glm::qua<float> qu = glm::qua<float>(glm::radians(glm::vec3(0.0f, 0.0f, angle)));
        model = glm::mat4_cast(qu);
        shader.setUniform("model", model);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setUniform("view", view);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)800 / (float)1000, 0.1f, 100.0f);
        shader.setUniform("projection", projection);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void windowSizeChanged_callback(GLFWwindow *window, int width, int height)
{
    int viewportWidth, viewportHeight;
    int offsetX = 0, offsetY = 0;

    float scaleX=(float)width/800.0f;
    float scaleY=(float)height/1000.0f;
    float scale=(scaleX>scaleY?scaleX:scaleY);

    viewportWidth=800.0f*scale;
    viewportHeight=1000.0f*scale;

    offsetX = (width - viewportWidth) / 2;
    offsetY = (height - viewportHeight) / 2;

    glViewport(offsetX, offsetY, (int)viewportWidth, (int)viewportHeight);
}

void mouseCursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    // if(firstMouse)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos; 
    // lastX = xpos;
    // lastY = ypos;

    // float sensitivity = 0.05;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // yaw   += xoffset;
    // pitch += yoffset;

    // if(pitch > 89.0f)
    //     pitch = 89.0f;
    // if(pitch < -89.0f)
    //     pitch = -89.0f;

    // glm::vec3 front;
    // front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // front.y = sin(glm::radians(pitch));
    // front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // cameraFront = glm::normalize(front);
}

void mouseButton_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
      std::cout << "mouse left" << std::endl;
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      std::cout << "mouse middle" << std::endl;
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      std::cout << "mouse right" << std::endl;
      break;
    }
}

void mouseWheel_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    // if(fov >= 0.1f && fov <= 45.0f)
    //     fov -= yoffset;
    // if(fov <= 0.1f)
    //     fov = 0.1f;
    // if(fov >= 45.0f)
    //     fov = 45.0f;

    float cameraSpeed = 10.0f * deltaTime;
    if(yoffset > 0)
        cameraPos += cameraSpeed * cameraFront;
    if(yoffset < 0)
        cameraPos -= cameraSpeed * cameraFront;
}

void keyInput_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // 相机按键控制
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        std::cout<<" press key W"<<std::endl;
        //cameraPos += cameraSpeed * cameraFront;
        cameraPos -= cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        std::cout<<" press key S"<<std::endl;
        //cameraPos -= cameraSpeed * cameraFront;
        cameraPos += cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        std::cout<<" press key A"<<std::endl;
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        std::cout<<" press key D"<<std::endl;
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        std::cout<<" press key SPACE"<<std::endl;
        angle += 2.0f;
    }
}