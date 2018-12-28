#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    /***************************************************************/

    Shader shader("shader.vs","shader.fs");

    // 導入圖片
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

    // 載入第二張圖片紋理
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("ABCD_A.png", &width, &height, &nrChannels, 0);
    unsigned int texture2;
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // png格式的圖片紋理需要加上alpha通道
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    shader.use();

    shader.setInt("texture1",0);
    shader.setInt("texture2",1);

    // 繪制兩個三角形
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 在这里配置位置为1的颜色数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // 在這裏配置紋理坐標的數據
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /***************************************************************/

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // 啓用線框模式繪制圖形

    // float timeValue,greenValue;
    // int vertexColorLocation;

    // 一個渲染循環, 在窗體退出前一直渲染
    // 首先檢查窗體是否被要求退出
    float alpha=0.2;

    while(!glfwWindowShouldClose(window))
    {
        // 接收輸入
        processInput(window);  

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if(alpha<=1.0f)
            {
                alpha+=0.01f;
            }
        }

        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if(alpha>=0.0f)
            {
                alpha-=0.01f;
            }
        }

        // 設置顏色
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        // 清除顏色緩衝區, 並使用上面設置的顏色進行填充 
        glClear(GL_COLOR_BUFFER_BIT);

        // 通过glfw提供的函数计算时间
        // timeValue = glfwGetTime();
        // greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // // 查询uniform变量的地址
        // vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");


        // 设置uniform变量的值
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();
        shader.setFloat("alpha",alpha);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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