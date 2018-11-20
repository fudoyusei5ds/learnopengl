#ifndef VEC2_H
#define VEC2_H
#include <cmath>


# define M_PI		3.14159265358979323846

// 一个普通的二维向量类
// 包含向量的移动,向量的旋转两个公有方法
// 以及向量的坐标
class Vec2
{
public:
    int x;
    int y;
    
    // 初始化构造函数
    Vec2():x(0),y(0){}
    Vec2(int a,int b):x(a),y(b){}

    // 向量的移动
    void move(int a,int b)
    {
        x+=a;
        y+=b;
    }

    // 向量的旋转
    // 输入倒数
    void rotate(int theta)
    {
        double a=x;
        double b=y;
        x=a*std::cos(M_PI/theta)-b*std::sin(M_PI/theta);
        y=a*std::sin(M_PI/theta)+b*std::cos(M_PI/theta);
    }
};

#endif