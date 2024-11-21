#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "shader.h"
#include "camera.h"
#include "RenderText.h"

#define M_PI 3.14159265358979323846

std::vector<float> verticesXZ(float pt1x, float pt1z, float pt2x, float pt2z, int num)
{
    std::vector<float> vertices;
    
    float _xLen = (pt2x - pt1x) / num;
    float _zLen = (pt2z - pt1z) / num;
    float y = 0;

    for (int zi = 0; zi <= num; ++zi) {
        float z = pt1z + zi * _zLen;

        vertices.push_back(pt1x); vertices.push_back(y); vertices.push_back(z);
        vertices.push_back(pt2x); vertices.push_back(y); vertices.push_back(z);
    }

    for (int xi = 0; xi <= num; ++xi) {
        float x = pt1x + xi * _xLen;

        vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt1z);
        vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt2z);
    }

    return vertices;
}

std::vector<float> verticesXY(float pt1x, float pt1y, float pt2x, float pt2y, int num)
{
    std::vector<float> vertices;
    
    float _xLen = (pt2x - pt1x) / num;
    float _yLen = (pt2y - pt1y) / num;
    float z = 0;

    for (int xi = 0; xi <= num; ++xi) {
        float x = pt1x + xi * _xLen;

        vertices.push_back(x); vertices.push_back(pt1y); vertices.push_back(z);
        vertices.push_back(x); vertices.push_back(pt2y); vertices.push_back(z);
    }

    for (int yi = 0; yi <= num; ++yi) {
        float y = pt1y + yi * _yLen;

        vertices.push_back(pt1x); vertices.push_back(y); vertices.push_back(z);
        vertices.push_back(pt2x); vertices.push_back(y); vertices.push_back(z);
    }

    return vertices;
}

std::vector<float> verticesYZ(float pt1y, float pt1z, float pt2y, float pt2z, int num)
{
    std::vector<float> vertices;
    
    float _yLen = (pt2y - pt1y) / num;
    float _zLen = (pt2z - pt1z) / num;
    float x = 0;

    for (int zi = 0; zi <= num; ++zi) {
        float z = pt1z + zi * _zLen;

        vertices.push_back(x); vertices.push_back(pt1y); vertices.push_back(z);
        vertices.push_back(x); vertices.push_back(pt2y); vertices.push_back(z);
    }

    for (int yi = 0; yi <= num; ++yi) {
        float y = pt1y + yi * _yLen;

        vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt1z);
        vertices.push_back(x); vertices.push_back(y); vertices.push_back(pt2z);
    }

    return vertices;
}

std::vector<float> createConeX(float xEnd, float radius, int segments) {
    std::vector<float> vertices;

    // 锥体顶点（顶点在轴的延长线上）
    vertices.push_back(xEnd + 2.0f); // 锥体顶点的 X 坐标
    vertices.push_back(0.0f);        // Y
    vertices.push_back(0.0f);        // Z

    // 圆形底面的顶点
    float angleStep = 2.0f * M_PI / segments;
    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        float y = radius * cos(angle);
        float z = radius * sin(angle);

        // 每个圆形顶点位置
        vertices.push_back(xEnd);  // X 坐标固定在轴线末端
        vertices.push_back(y);     // Y 坐标
        vertices.push_back(z);     // Z 坐标
    }

    return vertices;
}

std::vector<float> createConeY(float yEnd, float radius, int segments) {
    std::vector<float> vertices;

    // 锥体顶点（顶点在轴的延长线上）
    vertices.push_back(0.0f); // 锥体顶点的 X 坐标
    vertices.push_back(yEnd+2.0f);        // Y
    vertices.push_back(0.0f);        // Z

    // 圆形底面的顶点
    float angleStep = 2.0f * M_PI / segments;
    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // 每个圆形顶点位置
        vertices.push_back(x);  // X 坐标固定在轴线末端
        vertices.push_back(yEnd);     // Y 坐标
        vertices.push_back(z);     // Z 坐标
    }

    return vertices;
}

std::vector<float> createConeZ(float zEnd, float radius, int segments) {
    std::vector<float> vertices;

    // 锥体顶点（顶点在轴的延长线上）
    vertices.push_back(0.0f); // 锥体顶点的 X 坐标
    vertices.push_back(0.0f);        // Y
    vertices.push_back(zEnd+2.0f);        // Z

    // 圆形底面的顶点
    float angleStep = 2.0f * M_PI / segments;
    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        float y = radius * cos(angle);
        float x = radius * sin(angle);

        // 每个圆形顶点位置
        vertices.push_back(x);  // X 坐标固定在轴线末端
        vertices.push_back(y);     // Y 坐标
        vertices.push_back(zEnd);     // Z 坐标
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window=glfwCreateWindow(1000,1000,"Shader",NULL,NULL);
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

    glViewport(0,0,1000,1000);

    std::vector<float> versXZ = verticesXZ(0.0f, 0.0f, 20.0f, 20.0f, 20);
    std::vector<float> versXY = verticesXY(0.0f, 0.0f, 20.0f, 20.0f, 20);
    std::vector<float> versYZ = verticesYZ(0.0f, 0.0f, 20.0f, 20.0f, 20);
    std::vector<float> axisX = {0.0f, 0.0f, 0.0f, 25.0f, 0.0f,0.0f};
    std::vector<float> axisY = {0.0f, 0.0f, 0.0f, 0.0f, 25.0f,0.0f};
    std::vector<float> axisZ = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,25.0f};
    std::vector<float> coneX = createConeX(24.0f, 0.3f, 16);
    std::vector<float> coneY = createConeY(24.0f, 0.3f, 16);
    std::vector<float> coneZ = createConeZ(24.0f, 0.3f, 16);

    unsigned int VAO_XZ, VBO_XZ;
    unsigned int VAO_XY, VBO_XY;
    unsigned int VAO_YZ, VBO_YZ;
    unsigned int VAO_X_AXIS, VBO_X_AXIS;
    unsigned int VAO_Y_AXIS, VBO_Y_AXIS;
    unsigned int VAO_Z_AXIS, VBO_Z_AXIS;
    unsigned int VAO_X_CONE, VBO_X_CONE;
    unsigned int VAO_Y_CONE, VBO_Y_CONE;
    unsigned int VAO_Z_CONE, VBO_Z_CONE;

    glGenVertexArrays(1, &VAO_X_AXIS);
    glGenBuffers(1, &VBO_X_AXIS);
    glBindVertexArray(VAO_X_AXIS);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_X_AXIS);
    glBufferData(GL_ARRAY_BUFFER, axisX.size() * sizeof(float), axisX.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_Y_AXIS);
    glGenBuffers(1, &VBO_Y_AXIS);
    glBindVertexArray(VAO_Y_AXIS);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Y_AXIS);
    glBufferData(GL_ARRAY_BUFFER, axisY.size() * sizeof(float), axisY.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_Z_AXIS);
    glGenBuffers(1, &VBO_Z_AXIS);
    glBindVertexArray(VAO_Z_AXIS);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Z_AXIS);
    glBufferData(GL_ARRAY_BUFFER, axisZ.size() * sizeof(float), axisZ.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 生成和绑定 VAO 和 VBO
    glGenVertexArrays(1, &VAO_XZ);
    glGenBuffers(1, &VBO_XZ);
    glBindVertexArray(VAO_XZ);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_XZ);
    glBufferData(GL_ARRAY_BUFFER, versXZ.size() * sizeof(float), versXZ.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_XY);
    glGenBuffers(1, &VBO_XY);
    glBindVertexArray(VAO_XY);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_XY);
    glBufferData(GL_ARRAY_BUFFER, versXY.size() * sizeof(float), versXY.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_YZ);
    glGenBuffers(1, &VBO_YZ);
    glBindVertexArray(VAO_YZ);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_YZ);
    glBufferData(GL_ARRAY_BUFFER, versYZ.size() * sizeof(float), versYZ.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_X_CONE);
    glGenBuffers(1, &VBO_X_CONE);
    glBindVertexArray(VAO_X_CONE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_X_CONE);
    glBufferData(GL_ARRAY_BUFFER, coneX.size() * sizeof(float), coneX.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_Y_CONE);
    glGenBuffers(1, &VBO_Y_CONE);
    glBindVertexArray(VAO_Y_CONE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Y_CONE);
    glBufferData(GL_ARRAY_BUFFER, coneY.size() * sizeof(float), coneY.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO_Z_CONE);
    glGenBuffers(1, &VBO_Z_CONE);
    glBindVertexArray(VAO_Z_CONE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Z_CONE);
    glBufferData(GL_ARRAY_BUFFER, coneZ.size() * sizeof(float), coneZ.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //shader
    Shader shader("../../shaders/vertexShader.glsl","../../shaders/fragmentShader.glsl");
    glEnable(GL_MULTISAMPLE);

    TextRenderer textRenderer(shader.programID(), 1000, 1000);
    textRenderer.init();


    // 解绑 VAO 和 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glEnable(GL_BLEND); // 启用混合
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader.use();
        glUniform3f(glGetUniformLocation(shader.programID(), "gridColor"), 1.0f, 1.0f, 1.0f); // 灰色网格
        glUniform1f(glGetUniformLocation(shader.programID(), "alpha"), 0.3f); // 设置透明度为 0.5

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.programID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.programID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.programID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // 绘制 XZ 平面
        glBindVertexArray(VAO_XZ);
        glDrawArrays(GL_LINES, 0, versXZ.size() / 3);
        glBindVertexArray(0);

        // 绘制 XY 平面
        glBindVertexArray(VAO_XY);
        glDrawArrays(GL_LINES, 0, versXY.size() / 3);
        glBindVertexArray(0);

        // 绘制 YZ 平面
        glBindVertexArray(VAO_YZ);
        glDrawArrays(GL_LINES, 0, versYZ.size() / 3);
        glBindVertexArray(0);

        // 绘制 X 轴
        glBindVertexArray(VAO_X_AXIS);
        glDrawArrays(GL_LINES, 0, axisX.size() / 3);
        glBindVertexArray(0);

        glBindVertexArray(VAO_X_CONE);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 16 + 2);
        glBindVertexArray(0);

        // 绘制 Y 轴
        glBindVertexArray(VAO_Y_AXIS);
        glDrawArrays(GL_LINES, 0, axisY.size() / 3);
        glBindVertexArray(0);

        glBindVertexArray(VAO_Y_CONE);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 16 + 2);
        glBindVertexArray(0);

        // 绘制 Z 轴
        glBindVertexArray(VAO_Z_AXIS);
        glDrawArrays(GL_LINES, 0, axisZ.size() / 3);
        glBindVertexArray(0);

        glBindVertexArray(VAO_Z_CONE);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 16 + 2);
        glBindVertexArray(0);

        textRenderer.renderText("OpenGL", 10, 10, 10, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindVertexArray(0);
    glfwTerminate();
    return 0;
}