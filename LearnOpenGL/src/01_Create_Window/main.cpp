#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frameBufferSizeChanged(GLFWwindow* window,int width,int height)
{
    //当窗口大小变化时，打印高宽，改变视口大小以适应窗口变化
    std::cout<<" window size changed,width is "<<width<<" ,height is "<<height<<std::endl;
    glViewport(0,0,width,height);
}

//捕获按键事件
    void pressKey(GLFWwindow* window)
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window,true);
        }
    }

int main()
{
    //初始化glfw环境
    glfwInit();

    //指定使用OpenGL3.3版本,glfwWindowHint是创建窗口之前的配置设置函数
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //使用Opengl核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建窗口
    GLFWwindow* window=glfwCreateWindow(640,480,"OpenGL Window",NULL,NULL);
    if(!window)
    {
        std::cout<<" create glfw window failed!"<<std::endl;
        glfwTerminate();
        return -1;
    }

    //将创建的窗口和当前线程上下文绑定
    glfwMakeContextCurrent(window);

    //初始化glad,glad是管理opengl函数指针,使用OpenGL函数前必须初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置视口，即渲染窗口,左下角是(0,0),OpenGL坐标-1---1映射到640,480
    glViewport(0,0,640,480);

    //注册窗口变化时的回调函数
    glfwSetFramebufferSizeCallback(window,frameBufferSizeChanged);

    //进行窗口渲染循环，以保持窗口显示,glfwWindowShouldClose表示窗口是否应该关闭，glfwSwapBuffers双缓冲区交换，glfwPollEvents窗口事件监视
    while(!glfwWindowShouldClose(window))
    {
        pressKey(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //释放资源，退出glfw环境
    glfwTerminate();
    return 0;
}