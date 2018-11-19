#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include "shader.h"
#include "pipe.h"


#define createBuffers(A, B, E,VERT,INDI) \
do{\
    glGenVertexArrays(1, &A);\
    glGenBuffers(1, &B);\
    glGenBuffers(1, &E);\
\
    glBindVertexArray(A);\
\
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, E);\
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDI), INDI, GL_STATIC_DRAW);\
\
    glBindBuffer(GL_ARRAY_BUFFER, B);\
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERT), VERT, GL_STATIC_DRAW);\
\
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);\
    glEnableVertexAttribArray(0);\
\
    glBindBuffer(GL_ARRAY_BUFFER, 0);\
    glBindVertexArray(0);\
}while(0)

// 這個函數用來改變視口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    // 初始化
    glfwInit();
    // 設置glfw所使用的OpenGL的版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 設置glfw使用OpenGL的一個子集以及啓用OpenGL的核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 在Macos中使用
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 創建窗體
    GLFWwindow* window = glfwCreateWindow(400, 600, "LearnOpenGL", NULL, NULL);
    // 如果創建失敗, 輸出錯誤信息並釋放資源
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 設置窗體上下文爲線程主上下文
    glfwMakeContextCurrent(window);

    // 初始化glad, 用於返回OpenGL函數的地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 設置窗體維度
    glViewport(0, 0, 400, 600);
    // 注冊窗體調整函數
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /***************************************************************/

    // 先写两个正方形

    // 初始化着色器对象
    Shader shader("./shader.vs", "./shader.fs");

    // 第一根水管的初始坐標
    float vertices1[] = {
        // 上部的矩形
        1.50f,  1.00f,    0.0f,  // 右上角
        1.50f,  -1.0f,   0.0f,  // 右下角
        1.00f, -1.00f,   0.0f,  // 左下角
        1.00f, 1.00f,    0.0f  // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    // 第二根水管的初始坐標
    float vertices2[] = {
        // 上部的矩形
        3.50f,  1.0f,    0.0f,  // 右上角
        3.50f,  -1.0f,   0.0f,  // 右下角
        3.00f, -1.0f,   0.0f,  // 左下角
        3.00f, 1.0f,    0.0f  // 左上角
    };

    unsigned int indices1[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    Shader shader1("./shader.vs", "./shader.fs");
    unsigned int VAO,VBO,EBO;
    createBuffers(VAO,VBO,EBO,vertices1,indices);
    Shader shader2("./shader.vs", "./shader.fs");
    unsigned int VAO1,VBO1,EBO1;
    createBuffers(VAO1,VBO1,EBO1,vertices2,indices);


    /***************************************************************/

    // 一個渲染循環, 在窗體退出前一直渲染
    // 首先檢查窗體是否被要求退出

    int passX=400;
    clock_t lastFrame,newFrame;
    lastFrame=clock();
    while(!glfwWindowShouldClose(window))
    {
        // 接收輸入
        processInput(window);  
        // 設置顏色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // 清除顏色緩衝區, 並使用上面設置的顏色進行填充 
        glClear(GL_COLOR_BUFFER_BIT);

        if(passX==-400)
        {
            passX=400;
        }
        else
        {
            passX-=2;
        }
        float pos=passX/400.0f;
        shader.use();
        shader.setFloat("ourx",pos);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // shader.use();
        // glBindVertexArray(VAO1);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // 交換緩衝, 繪制屏幕
        glfwSwapBuffers(window);
        // 檢查觸發事件, 調整窗體狀態
        glfwPollEvents(); 

        newFrame=clock();
        if(newFrame-lastFrame<=10000)
        {
            usleep(10000-newFrame+lastFrame);
        }
        lastFrame=newFrame;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glDeleteVertexArrays(1, &VAO1);
    // glDeleteBuffers(1, &VBO1);

    // 最後釋放資源
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 當窗體大小改變時, 調用這個函數重新設置視口大小
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // 按下esc, 發出窗體退出信號
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
