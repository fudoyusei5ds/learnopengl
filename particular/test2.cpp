// 創建兩個三角形, 不使用EBO, 使用不同的VAO和VBO
// 答案, 對於每個VAO都要單獨配置


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glad.c"

// 頂點着色器源碼
const char *vertexShaderSource=
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";
// 片段着色器源碼
const char *fragmentShaderSource=
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{ FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); }\n\0";


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
    float vertices1[] = {
        // 第一个三角形
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, 0.5f, 0.0f,  // 左上角
    };
    // 第二個三角形的數據
    float vertices2[] = {
        // 第二个三角形
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    // 生成一個頂點緩衝區對象
    unsigned int VBO1,VAO1;
    unsigned int VBO2,VAO2;

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glGenVertexArrays(1,&VAO2);
    glGenBuffers(1, &VBO2);

    // 對每個圖形的VAO, 都要單獨配置並進行綁定
    // 第一次綁定VAO
    glBindVertexArray(VAO1);
    // 將vbo與GL_ARRAY_BUFFER綁定,之後任何在GL_ARRAY_BUFFER上的緩衝調用都會配置當前綁定的緩衝
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    // 將用戶定義的數據復制到當前綁定緩衝上,GL_STATIC_DRAW指示數據幾乎不會改變,所以不需要顯卡對其加速
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    /***************************************************************/

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   // 啓用線框模式繪制圖形

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


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES,0,3);

        // 交換緩衝, 繪制屏幕
        glfwSwapBuffers(window);
        // 檢查觸發事件, 調整窗體狀態
        glfwPollEvents(); 
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);

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