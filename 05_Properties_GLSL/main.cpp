#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource="#version 330 core\n"
                                "layout (location =0 ) in vec3 aPos;\n"
                                "layout (location =1 ) in vec3 aColor;\n"
                                "out vec3 ourColor;\n"
                                "void main()\n"
                                "{\n"
                                "gl_Position=vec4(aPos,1.0f);\n"
                                "ourColor=aColor;\n"
                                "}\n";

const char* fragmentShaderSource="#version 330 core\n"
                                "out vec4 fragColor;\n"
                                "in vec3 ourColor;\n"
                                "void main()\n"
                                "{\n"
                                "fragColor=vec4(ourColor,1.0f);\n"
                                "}\n";

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

    GLfloat vertexes[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
    };

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexes),vertexes,GL_STATIC_DRAW);

    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);

    unsigned int shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    //0表示第一个属性，3这个属性的分量个数，GL_FLOAT属性的数据类型，GL_FALSE属性是否进行归一化，6*size(float)指定连续顶点属性之间的字节偏移量，(void*)0当前属性的偏移量
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3* sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);


    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f,0.5f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glBindVertexArray(0);
    glfwTerminate();
    return 0;
}