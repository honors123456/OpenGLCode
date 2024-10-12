#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // 读取并构造着色器
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    // 激活使用程序
    void use();
    // uniform工具
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setUniform(const std::string &name, glm::mat4 value) const;

    unsigned int getProgram();

private:
    // 程序ID
    unsigned int ID;
};

#endif