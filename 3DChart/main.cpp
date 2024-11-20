#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"

std::vector<float> gridVertices(float pt1x, float pt1y, float pt1z, float pt2x, float pt2y, float pt2z, int num)
{
    std::vector<float> vertices;
    
    // 网格间距
    float _xLen = (pt2x - pt1x) / num;
    float _yLen = (pt2y - pt1y) / num;
    float _zLen = (pt2z - pt1z) / num;

    // 绘制平行于 XY 平面的网格
    for (int zi = 0; zi <= num; ++zi) {
        float z = pt1z + zi * _zLen;
        for (int xi = 0; xi <= num; ++xi) {
            float x = pt1x + xi * _xLen;
            // 绘制 XY 平面上的线条
            vertices.push_back(x); vertices.push_back(pt1y); vertices.push_back(z);
            vertices.push_back(x); vertices.push_back(pt2y); vertices.push_back(z);
        }
    }

    // 绘制平行于 XZ 平面的网格
    for (int yi = 0; yi <= num; ++yi) {
        float y = pt1y + yi * _yLen;
        for (int xi = 0; xi <= num; ++xi) {
            float x = pt1x + xi * _xLen;
            // 绘制 XZ 平面上的线条
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt1z);
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt2z);
        }
    }

    // 绘制平行于 YZ 平面的网格
    for (int xi = 0; xi <= num; ++xi) {
        float x = pt1x + xi * _xLen;
        for (int yi = 0; yi <= num; ++yi) {
            float y = pt1y + yi * _yLen;
            // 绘制 YZ 平面上的线条
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt1z);
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt2z);
        }
    }

    return vertices;
}

int main(int argc,char** argv)
{
    //初始化glfw环境
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window=glfwCreateWindow(640,480,"Shader",NULL,NULL);
    if(!window)
    {
        const char* description;
        int errCode=glfwGetError(&description);
        std::cerr<<"create window failed! err code is "<<errCode<<" with "<<description<<std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr<<" init glad failed!"<<std::endl;
        return -1;
    }

    glViewport(0,0,640,480);


    std::vector<float> vertices = gridVertices(-5.0f, -5.0f, -5.0f, 5.0f, 5.0f, 5.0f, 10);

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //shader
    Shader shader("../../shaders/vertexShader.glsl","../../shaders/fragmentShader.glsl");

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glEnable(GL_BLEND); // 启用混合
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID(), "gridColor"), 1.0f, 1.0f, 1.0f); // 灰色网格
        glUniform1f(glGetUniformLocation(shader.programID(), "alpha"), 0.8f); // 设置透明度为 0.5

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640 / 480, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.programID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.programID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.programID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindVertexArray(0);
    glfwTerminate();
    return 0;
}