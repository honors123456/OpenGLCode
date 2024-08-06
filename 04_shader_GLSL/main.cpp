#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <iomanip>

const char* vertexShaderSource="#version 330 core\n"
                                "layout (location =0 ) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "gl_Position=vec4(aPos,1.0f);\n"
                                "}\n";

const char* fragmentShaderSource="#version 330 core\n"
                                "out vec4 fragColor;\n"
                                "uniform vec4 cusColor;\n"
                                "void main()\n"
                                "{\n"
                                "fragColor=cusColor;\n"
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

    GLfloat vertexes[]={
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f,
        -0.5f,-0.5f,0.0f
    };

    GLint indexes[]={
        0,1,2
    };

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO,EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexes),vertexes,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexes),indexes,GL_STATIC_DRAW);

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

    glVertexAttribPointer(0,3,GL_FLOAT,false,3*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f,0.5f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //测试大概16ms刷新一次界面
        // auto now = std::chrono::system_clock::now();
        // auto now_time_t = std::chrono::system_clock::to_time_t(now);
        // auto duration = now.time_since_epoch();
        // auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
        // std::tm local_tm = *std::localtime(&now_time_t);
        // std::cerr << std::put_time(&local_tm, "%F %T") << "." << std::setw(3) << std::setfill('0') << millis << std::endl;

        //通过uniform来设置输出图案颜色
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0f + 0.5f);
        //获取cusColor的位置
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "cusColor");
        //通过位置来设置uniform的值
        glUniform4f(vertexColorLocation, greenValue, greenValue, greenValue, 1.0f);

        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
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