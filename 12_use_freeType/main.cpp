#define STB_IMAGE_IMPLEMENTATION
#include "src/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

int main(int argc,char** argv)
{
    //初始化glfw环境
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window=glfwCreateWindow(500,500,"Shader",NULL,NULL);
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

    glViewport(0,0,500,500);

    
    float vertexes[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[]={
        0,1,3,
        1,2,3
    };

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexes),vertexes,GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //0表示第一个属性，3这个属性的分量个数，GL_FLOAT属性的数据类型，GL_FALSE属性是否进行归一化，6*size(float)指定连续顶点属性之间的字节偏移量，(void*)0当前属性的偏移量
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3* sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    //纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    FT_Library ft;
    FT_Face face;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library" << std::endl;
        exit(1);
    }

    if (FT_New_Face(ft, "../../arial.ttf", 0, &face)) {
        std::cerr << "Could not load font" << std::endl;
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0,50);

    if (FT_Load_Char(face, 'Y', FT_LOAD_RENDER)) {
        std::cerr << "Failed to load character" << std::endl;
        exit(1);
    }

    //创建纹理对象
    unsigned int texture1;
    glGenTextures(1,&texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture1);

    int width = face->glyph->bitmap.width;
    int height = face->glyph->bitmap.rows;
    unsigned char* pixels = face->glyph->bitmap.buffer;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    std::cout<<width<<" "<<height<<std::endl;

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    //shader
    Shader shader("../../shaders/shader.vs","../../shaders/shader.fs");
    shader.use();

    shader.setInt("ourTexture1",0);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f,0.5f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}