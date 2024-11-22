#define STB_IMAGE_IMPLEMENTATION
#include "RenderText.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include "src/stb_image.h"

// 初始化TextRenderer，加载字体和初始化FreeType库
TextRenderer::TextRenderer(GLuint program, GLuint screenWidth, GLuint screenHeight)
    : program(program) {
    // 创建VAO、VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

// 析构函数，释放资源
TextRenderer::~TextRenderer() {
    // 删除字符纹理缓存
    for (auto &pair : characterTextures) {
        glDeleteTextures(1, &pair.second);
    }
    // 删除缓冲区
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // 释放FreeType资源
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

// 初始化FreeType并加载字体
void TextRenderer::init() {
    // 初始化FreeType库
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library" << std::endl;
        exit(1);
    }

    // 加载字体
    if (FT_New_Face(ft, "../../arial.ttf", 0, &face)) {
        std::cerr << "Could not load font" << std::endl;
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 10);  // 设置字体大小
}

// 加载字符纹理
GLuint TextRenderer::loadCharacterTexture(FT_Face face, char c) {
    if (characterTextures.find(c) != characterTextures.end()) {
        return characterTextures[c];
    }

    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        std::cerr << "Failed to load character" << std::endl;
        exit(1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    characterTextures[c] = texture;
    return texture;
}

// 渲染文本
void TextRenderer::renderText(const std::string &text, float x, float y, float z, float scale, const glm::vec3 &color) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // // 渲染每个字符
    // for (char c : text) {
    //     GLuint texture = loadCharacterTexture(face, c);

    //     // 计算字符的 3D 坐标，使用 (x, y, z) 来渲染字符
    //     float xpos = x;
    //     float ypos = y;
    //     float w = face->glyph->bitmap.width * scale;
    //     float h = face->glyph->bitmap.rows * scale;

    //     // 更新 VBO 数据，字符的 3D 坐标 (x, y, z)
    //     float vertices[6][4] = {
    //         { xpos,     ypos + h,   0.0f, 1.0f },
    //         { xpos + w, ypos,       0.0f, 0.0f },
    //         { xpos,     ypos,       0.0f, 0.0f },

    //         { xpos,     ypos + h,   0.0f, 1.0f },
    //         { xpos + w, ypos + h,   0.0f, 1.0f },
    //         { xpos + w, ypos,       0.0f, 0.0f }
    //     };

    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    //     // 绘制字符
    //     glBindTexture(GL_TEXTURE_2D, texture);
    //     glDrawArrays(GL_TRIANGLES, 0, 6);

    //     x += (w + 1);  
    // }

    //加载图片
    int width,height,nrChannels;
    unsigned char* data=stbi_load("../../res/container.jpg",&width,&height,&nrChannels,0);
    
    if(data)
    {
        //arg1:指定纹理目标 arg2:多级渐远纹理级别，arg3:纹理格式 arg4,arg5:纹理宽高，arg6:无意义，必须为0 arg7,arg8:源图格式和数据
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr<< " failed to load texture "<<std::endl;
    }

    //释放图像内存
    stbi_image_free(data);

    float vertexes[] = {
//     ---- 位置 ----    
     0.5f,  0.5f, 0.0f,   
     0.5f, -0.5f, 0.0f,   
    -0.5f, -0.5f, 0.0f,   
    -0.5f,  0.5f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexes), vertexes);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
