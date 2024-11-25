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


struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
class Shader;
class TextRenderer {
public:
    // 修改构造函数，传入外部着色器程序
    TextRenderer();
    ~TextRenderer();
    
    // 初始化FreeType库
    void init();
    
    // 渲染文本
    void renderText(Shader &shader,const std::string &text, float x, float y, float z, float scale, const glm::vec3 &color);

private:
    GLuint VAO, VBO; // 用于渲染的缓冲区
    FT_Library ft;  // FreeType库
    FT_Face face;   // 字体面
    
    std::map<GLchar, Character> Characters;
};

#endif
