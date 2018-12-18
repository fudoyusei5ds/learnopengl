### VAO, VBO, EBO
#### 1.图形渲染管线
将3D坐标转化为2D坐标的处理过程是由OPENGL的图形渲染管线管理的, 分为两个主要部分:  
1. 把3D坐标转化为2D坐标  
2. 把2D坐标转化为实际的有颜色的像素  
  
图形渲染管线接收一组3D坐标, 然后转变为屏幕上的有色2D像素输出, 一共分为6个阶段:  
1. 顶点着色器: 将一个单独的顶点作为输入, 将3D坐标转化为另一种3D坐标. 同时允许对顶点属性进行一些基本处理.  
2. 图元装配: 将顶点着色器输出的所有顶点作为输入, 将顶点装配成指定图元的形状.  
3. 几何着色器: 把图元形式的一系列顶点的集合作为输入, 可以通过产生新顶点构造出新的图元来生成其他形状.  
4. 光栅化阶段: 接收几何着色器的输出, 将图元映射为最终屏幕上相应的像素, 生成供片段着色器使用的片段. 同时会执行裁切, 去除超出屏幕范围的所有像素, 用来提升执行效率. 一个片段是OPENGL渲染一个像素所需的所有数据.  
5. 片段着色器: 用以计算一个像素的最终颜色.  
6. ALPHA测试和混合: 检测片段对应的深度, 用来判断这个像素是其它物体的前面还是后面, 决定是否应该丢弃. 同时检查ALPHA值并对物体进行混合.  

以上阶段很容易并行执行, 当今大多数显卡都有成千上万的小处理核心, 它们在GPU上为每一个(渲染管线)阶段运行各自的小程序, 从而在图形渲染管线中快速处理数据. 这些小程序叫做着色器.  

#### 2.标准化设备坐标
当顶点坐标在顶点着色器中处理过后, 其应该是标准化设备坐标, 即X, Y, Z值都是-1.0~1.0的一小段空间, 任何落在范围外的坐标都会被裁切掉.  
标准化设备坐标接着会变化为屏幕空间坐标, 然后又被变化为片段输入到片段着色器中.  

#### 3.顶点缓冲对象(VBO)
顶点着色器会在GPU上创建内存储存接收的顶点数据. 而VBO用于管理这个内存, 它会在GPU内存(显存)中储存大量顶点. 使用缓冲对象的好处是可以一次向的发送一大批数据(多个顶点的数据)到显卡上, 而不是每个顶点发送一次. 
``` 
    unsigned int VBO;
    glGenBuffers(1, &VBO);  //生成一个缓冲对象, 对象的ID为VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //将新创建的缓冲对象绑定到OPENGL的顶点缓冲对象类型上
```
创建并绑定顶点缓冲对象之后, 可以使用glBufferData函数把顶点数据复制到当前绑定的缓冲的内存中.  
```
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //用于发送数据到显卡
```
GL_STATIC_DRAW表示VBO中的数据将不会被改动(一次指定多次使用), "GL_DYNAMIC_DRAW"表示数据将会被频繁改动(反复指定与使用), "GL_STREAM_DRAW"表示每帧数据都要改变, STATIC与STREAM使用显卡内存, 而DYNAMIC使用AGP内存.  
glBufferData专门把用户定义的数据复制到当前绑定缓冲. 其第一个参数是目标缓冲的类型. 第二个参数指定传输数据的大小(以字节为单位). 第三个参数是发送的实际数据. 第四个参数指定显卡如何管理给定的数据

#### 4.GLSL
1. 顶点着色器  
顶点着色器内置的vec4类型的gl_Position值是该顶点着色器的输出.  
一般来说, 顶点着色器输入的数据通常都不是标准化设备坐标, 所以我们首先必须把它们转换成OPENGL的可视领域内的点.  
2. 片段着色器  
用于计算像素最后的颜色输出.  
片段着色器只有一个输出变量, 这是一个vec4类型的变量并使用out关键字声明.

#### 5.编译着色器
1. 创建一个着色器对象, 并创建一个ID进行绑定  
```
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
```
2. 为着色器对象添加源码, 并编译源码  
```
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
```
3. 错误检测
```
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
```

#### 6.着色器对象
着色器程序对象是多个着色器合并之后最终链接完成的版本. 如果要使用刚才编译的着色器, 需要将其链接为一个着色器程序对象, 然后在渲染对象的时候激活这个着色器程序. 已激活着色器程序的着色器将在发送渲染调用的时候被使用.  
当链接着色器至一个程序的时候, 它会把每个着色器的输出链接到下个着色器的输入.  
1. 创建着色器程序, 并绑定到ID:  
```
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
```
2. 将已编译的着色器附加到着色器程序上, 然后编译:  
```
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
```
3. 激活着色器对象, 并删除着色器: 
```
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
```

#### 7.链接顶点属性
顶点着色器允许任何以顶点属性为形式的输入, 必须在渲染前指定输入数据的哪一部分对应顶点着色器的哪一个顶点属性.  
解析顶点数据(应用到每个顶点属性上)
```
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
```
glVertexAttribPointer的参数:  
* 第一个参数指定要配置的顶点属性.  
* 第二个参数指定顶点属性的大小.  
* 第三个参数指定数据的类型.  
* 第四个参数指定数据是否被映射到0-1之间
* 第五个参数指定连续顶点属性组之间的间隔
* 第六个参数指定位置数据在缓冲中起始位置的偏移量. 其为void* 类型

每个顶点属性从一个VBO管理的内存中获得它的数据, 而具体从那个VBO(程序中可以有多个VBO)获取则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的.  
glEnableVertexAttribArray以顶点属性位置值为参数, 启用顶点属性. (顶点属性默认是禁用的)  

#### 8.顶点数组对象(VAO)
VAO可以像VBO那样被绑定, 任何随后的顶点属性调用都会储存在这个VAO中, 当配置顶点属性指针时, 只需要将那些调用执行依次, 之后再绘制物体的时候只需要绑定相应的VAO就行了.  
一个VAO会储存以下内容:  
* glEnableVertexAttribArray和glDisableVertexAttribArray的调用.  
* 通过glVertexAttribPointer设置的顶点属性配置.  
* 通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象.  

创建一个VAO:  
```
    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //绑定顶点数组对象到指定的ID
```
要想使用VAO, 要做的只是使用  
```
    glBindVertexArray(VAO);  
```  
绑定VAO. 从绑定之后起, 我们应该绑定和配置对应的VBO和属性指针, 之后解绑VAO供之后使用. 当之后打算绘制一个物体的时候, 只要在绘制物体前把VAO绑定到希望使用的绑定上就行了.  
当要绘制多个物体时, 首先要生成/配置所有的VAO(和必须的VBO及属性指针), 然后储存它们供之后使用. 当打算绘制物体的时候绑定对应的VAO, 绘制完之后再解绑VAO.  

#### 9.索引缓冲对象(EBO)
索引缓冲对象用于储存点的索引顺序以指定opengl绘制点的顺序:  
1. 创建索引缓冲对象并绑定ID  
```
    unsigned int EBO;
    glGenBuffers(1, &EBO);  
```
2. 绑定EBO到缓冲类型上, 然后将数据从内存复制到显卡内存中:  
```
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```
3. 最后opengl依照索引顺序绘制点, 它会使用当前绑定的索引缓冲对象中的索引进行绘制:  
```
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

顶点数组对象同样可以保存索引缓冲对象的绑定状态. VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象. 这样, 在绑定VAO时, 会自动绑定EBO.  
注意: 当目标是GL_ELEMENT_ARRAY_BUFFER的时候, VAO会储存glBindBuffer的函数调用. 这也意味着它也会储存解绑调用, 所以确保在解绑VAO之前解绑索引数组缓冲, 否则它就没有这个EBO配置了.  
