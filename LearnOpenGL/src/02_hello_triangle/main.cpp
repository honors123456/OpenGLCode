#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //创建窗口
    GLFWwindow* window=glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"learn opengl",nullptr,nullptr);
    if(!window)
    {
        std::cout<<" create window failed!"<<std::endl;
        glfwTerminate();
        return -1;
    }

    //在当前上下文使用创建的窗口
    glfwMakeContextCurrent(window);

    //初始化glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<" init glad failed!";
        return -1;
    }

    //设置视口
    glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);

    
    //定义顶点数组对象
    GLfloat vertical[]={
        0.5f,-0.5f,0.0f,
        0.5f,0.5f,0.0f,
        -0.5f,-0.5f,0.0f,
        -0.5f,0.5f,0.0f
    };


    //定义顶点索引数组
    unsigned int indexs[]={
        0,1,3,
        0,2,3
    };

    //创建顶底数组对象
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    //绑定
    glBindVertexArray(VAO);

    //定义顶点缓冲对象
    unsigned int VBO;
    //创建缓冲区对象
    glGenBuffers(1,&VBO);
    //绑定缓冲区对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //缓冲区添加数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertical),vertical,GL_STATIC_DRAW);

    //定义顶点索引对象
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    //绑定索引对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //填充EBO数据到
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexs),indexs,GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    //启用顶点属性
    glEnableVertexAttribArray(0);


    //定义顶点着色器
    const char* vertexShaderSource="#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
                                    "}\n";
    //创建顶点着色器
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    //附加着色器源码
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //编译着色器源码
    glCompileShader(vertexShader);
    //检测是否编译成工
    int ok;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&ok);
    if(!ok)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout<<" compile vertex shader failed! "<<infoLog<<std::endl;
        return -1;
    }

    //定义片段着色器
    const char* frameShaderSource="#version 330 core\n"
                                    "out vec4 fragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "fragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
                                    "}\n";
    //创建片段着色器
    unsigned int frameShader=glCreateShader(GL_FRAGMENT_SHADER);
    //附加着色器源码
    glShaderSource(frameShader,1,&frameShaderSource,NULL);
    //编译着色器源码
    glCompileShader(frameShader);
    //检测着色器源码是否编译成功
    int success;
    char inflog[512];
    glGetShaderiv(frameShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(frameShader,512,NULL,inflog);
        std::cout<<" compile fragment shader failed! "<<inflog<<std::endl;
        return -1;
    }

    //创建程序对象
    unsigned int shaderProgram=glCreateProgram();
    //附加着色器到程序对象
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,frameShader);

    //链接程序对象
    glLinkProgram(shaderProgram);
    //检测是否连接成功
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,inflog);
        std::cout<<" link shader program failed! "<<std::endl;
        return -1;
    }

    //使用程序对象
    glUseProgram(shaderProgram);

    //删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(frameShader);
    glBindVertexArray(0);   //解绑顶点数组对象



    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES,0,3);

        //使用索引对象绘图
        glDrawElements(GL_LINE_LOOP,6,GL_UNSIGNED_INT,0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();
    return 0;
}