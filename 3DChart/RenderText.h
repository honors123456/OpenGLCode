#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <glm/glm.hpp>

class TextRenderer {
public:
    // 修改构造函数，传入外部着色器程序
    TextRenderer(GLuint program, GLuint screenWidth, GLuint screenHeight);
    ~TextRenderer();
    
    // 初始化FreeType库
    void init();
    
    // 渲染文本
    void renderText(const std::string &text, float x, float y, float z, float scale, const glm::vec3 &color);

private:
    GLuint program;  // 外部传入的着色器程序
    GLuint VAO, VBO; // 用于渲染的缓冲区
    FT_Library ft;  // FreeType库
    FT_Face face;   // 字体面
    
    std::map<char, GLuint> characterTextures; // 字符纹理缓存

    GLuint loadCharacterTexture(FT_Face face, char c);  // 加载单个字符的纹理
};

#endif
