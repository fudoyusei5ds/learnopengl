#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 载入线性代数库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 载入着色编译器
#include "shader.h"

// 载入图片库
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    glViewport(0, 0, 800, 600);
    // 注冊窗體調整函數
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /***************************************************************/

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    Shader shader("shader.vs","shader.fs");

    // 导入图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    // 生成紋理
    unsigned int texture1;
    glGenTextures(1, &texture1);
    // 綁定紋理
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 使用之前的圖片來生成紋理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // 自動生成多級漸遠紋理
    glGenerateMipmap(GL_TEXTURE_2D);
    // 釋放圖像數據
    stbi_image_free(data);


    shader.use();
    shader.setInt("texture1",0);

    // 绘制一个正方体, 这里是它每个点的坐标
    // 共6个面, 每个面4个点, 这样才能正确传递纹理坐标
    float vertices[] = {
        // 点坐标          纹理坐标
        // 前
        .5f, .5f,  .5f, 1.0f, 1.0f,
        .5f, -.5f, .5f, 1.0f, 0.0f,
        -.5f,-.5f, .5f, 0.0f, 0.0f,
        -.5f,.5f,  .5f, 0.0f, 1.0f,
        // 上
        .5f, .5f, -.5f, 1.0f, 1.0f,
        .5f, .5f,  .5f, 1.0f, 0.0f,
        -.5f,.5f,  .5f, 0.0f, 0.0f,
        -.5f,.5f, -.5f, 0.0f, 1.0f,
        // 下
        .5f, -.5f, .5f, 1.0f, 1.0f,
        .5f, -.5f,-.5f, 1.0f, 0.0f,
        -.5f,-.5f,-.5f, 0.0f, 0.0f,
        -.5f,-.5f, .5f, 0.0f, 1.0f,
        // 右
        .5f, .5f, -.5f, 1.0f, 1.0f,
        .5f, -.5f,-.5f, 1.0f, 0.0f,
        .5f, -.5f, .5f, 0.0f, 0.0f,
        .5f, .5f,  .5f, 0.0f, 1.0f,
        // 左
        -.5f, .5f, .5f, 1.0f, 1.0f,
        -.5f,-.5f, .5f, 1.0f, 0.0f,
        -.5f,-.5f,-.5f, 0.0f, 0.0f,
        -.5f, .5f,-.5f, 0.0f, 1.0f,
        // 后
        .5f, .5f, -.5f, 1.0f, 1.0f,
        .5f,-.5f, -.5f, 1.0f, 0.0f,
        -.5f,-.5f,-.5f, 0.0f, 0.0f,
        -.5f,.5f, -.5f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        // 绘制索引
        0, 1, 3,
        1, 2, 3,

        4, 5, 7,
        5, 6, 7,

        8, 9, 11,
        9, 10, 11,

        12, 13, 15,
        13, 14, 15,

        16, 17, 19,
        17, 18, 19,

        20, 21, 23,
        21, 22, 23
    };

    // 生成一個頂點緩衝區對象
    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 第一次綁定VAO
    glBindVertexArray(VAO);

    // 綁定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 傳值
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 將vbo與GL_ARRAY_BUFFER綁定,之後任何在GL_ARRAY_BUFFER上的緩衝調用都會配置當前綁定的緩衝
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 將用戶定義的數據復制到當前綁定緩衝上,GL_STATIC_DRAW指示數據幾乎不會改變,所以不需要顯卡對其加速
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 設置緩衝區數據的使用方法
    // 在这里配置位置为0的位置数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 在這裏配置紋理坐標的數據
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /***************************************************************/

    // 一個渲染循環, 在窗體退出前一直渲染
    // 首先檢查窗體是否被要求退出 

    while(!glfwWindowShouldClose(window))
    {
        // 接收輸入
        processInput(window);  

        // 設置顏色
        glClearColor(0.0f, .5f, 0.4f, 1.0f);
        // 清除顏色緩衝區, 並使用上面設置的顏色進行填充 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        shader.use();

        // 模型矩阵, 用于处理模型内部的变换
        glm::mat4 model;
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, .5f, 0.0f));

        // 观察矩阵, 用于处理模型和世界坐标的相对位置
        glm::mat4 view;
        view = glm::translate(view, glm::vec3(0, 0, -3.0f));

        // 投影矩阵
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800/600.0f, .1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 绑定VAO, 绘制图形
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 交換緩衝, 繪制屏幕
        glfwSwapBuffers(window);
        // 檢查觸發事件, 調整窗體狀態
        glfwPollEvents(); 
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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