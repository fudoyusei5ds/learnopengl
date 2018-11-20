#include "vec2.h"
#include <stdio.h>

int main()
{
    Vec2 veca(1,0);
    veca.rotate(2);
    printf("%d,%d",veca.x,veca.y);
    return 0;
}