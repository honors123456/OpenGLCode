#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
 
const char* vertexShaderSource = "#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                "}\n\0";
 
int main()
{
    //初始化glfw环境
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建窗口
    GLFWwindow* window=glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"hello triangle",NULL,NULL);
    if(window==NULL)
    {
        const char* description;
        int code = glfwGetError(&description);
        std::cerr << "GLFW create window failed!. Error code: " << code << ", description: " << description << std::endl;
        glfwTerminate();
        return -1;
    }

    //上下文绑定窗口
    glfwMakeContextCurrent(window);

    //初始化glad环境
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<" init glad failed!"<<std::endl;
        return -1;
    }

    //设置视口
    glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);

    //顶点着色器
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);     //创建着色器
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);        //设置着色器源码
    glCompileShader(vertexShader);                                  //编译着色器源码

    int success;                                                    //检测并打印编译失败日志
    char log[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,log);
        std::cerr<<" compile vertex shader failed!"<<std::endl;
        return -1;
    }

    //片段着色器
    unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER); //创建着色器
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);    //设置着色器源码
    glCompileShader(fragmentShader);                                //编译着色器源码

    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,log);
        std::cerr<<" compile fragment shader failed!"<<std::endl;
        return -1;
    }

    //着色器程序
    unsigned int shaderProgram=glCreateProgram();                   //创建着色器程序对象
    glAttachShader(shaderProgram,vertexShader);                     //附加着色器到程序对象
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);                                   //链接程序对象
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);          //检测并打印链接失败日志
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,log);
        std::cerr<<" link shader program failed!"<<std::endl;
        return -1;
    }

    glUseProgram(shaderProgram);                                    //使用程序对象

    //顶点数组
    GLfloat vertexes[]={
        0.5f,0.5f,0.0f,
        -0.5f,0.5f,0.0f,
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f
    };

    //索引数组
    GLint indexs[]={
        0,1,2,
        0,1,3
    };

    //顶点数组对象
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //顶点缓冲对象
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    //索引缓冲对象
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    //向两个缓冲区对象添加数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexes),vertexes,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexs),indexs,GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);


    //渲染循环(类似于Qt的exec())
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //绘制图案
        glDrawArrays(GL_TRIANGLES,0,3);                     //使用顶点缓冲对象绘图

        //glDrawElements(GL_LINE_LOOP,6,GL_UNSIGNED_INT,0); //使用索引缓冲对象绘图
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glBindVertexArray(0);
    //退出glfw环境
    glfwTerminate();
    return 0;
}