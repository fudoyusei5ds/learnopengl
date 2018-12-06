***
### 18/10/24
1. 对于用GCC编译的Linux用户建议使用这个命令行选项-lGLEW -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi
2. 主要在配置環境, 還不知道配置的對不對
3. 一個OpenGL程序需要引用許多動態鏈接庫, 有些沒有找到, 於是直接上絕對路徑
4. 距離[關於拯救世界的十二種方法]還有四萬光年
***
### 18/10/29
1. 顶点数组对象：Vertex Array Object，VAO
2. 顶点缓冲对象：Vertex Buffer Object，VBO
3. 索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO
4. 圖形渲染管線, 用於將3d空間上的3維坐標轉換維2維坐標,以及將2維坐標轉化爲一個有顏色的像素
  
圖形渲染步驟:
1. 定義圖形的頂點數據,並將其發送給頂點着色器,其會使用頂點緩衝對象來管理保存頂點數據的內存
2. 頂點着色器處理
3. 片段着色器處理
  
啊 三角形 等腰的 
***