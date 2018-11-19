#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>


// 頂點着色器源碼
const char *vertexShaderSource=
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;"
    "}\n\0";
// 片段着色器源碼
const char *fragmentShaderSource=
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{ FragColor = vec4(ourColor, 1.0f); }\n\0";


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
    
    /***************************************************************/

    // 創建一個頂點着色器對象,並用id引用
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 將着色器源碼附到着色器對象上,然後編譯
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 頂點着色器源碼編譯檢查
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 創建片段着色器對象並進行編譯
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 檢查片段着色器編譯
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 創建一個着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // 將着色器對象附到程序上,然後鏈接
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 鏈接結果檢查
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 刪除着色器對象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // 繪制兩個三角形
    float vertices[] = {
        0.5f,   0.5f,   0.0f,   1.0f,   0.0f,   0.0f,  // 右上角
        0.5f,   -0.5f,  0.0f,   0.0f,   1.0f,   0.0f,  // 右下角
        -0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,   1.0f,// 左下角
        -0.5f,  0.5f,   0.0f,   0.0f,   1.0f,   0.0f  // 左上角
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 在这里配置位置为1的颜色数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /***************************************************************/

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // 啓用線框模式繪制圖形

    // float timeValue,greenValue;
    // int vertexColorLocation;

    // 一個渲染循環, 在窗體退出前一直渲染
    // 首先檢查窗體是否被要求退出
    while(!glfwWindowShouldClose(window))
    {
        // 接收輸入
        processInput(window);  

        // 設置顏色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清除顏色緩衝區, 並使用上面設置的顏色進行填充 
        glClear(GL_COLOR_BUFFER_BIT);

        // 通过glfw提供的函数计算时间
        // timeValue = glfwGetTime();
        // greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // // 查询uniform变量的地址
        // vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        glUseProgram(shaderProgram);

        // 设置uniform变量的值
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


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