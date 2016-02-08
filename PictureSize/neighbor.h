//编写者：刘柏
//编写时间：2015.11.10
//功能：最近邻插值计算

#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <iostream>
#include <string>
#include <cmath>
#include "pixel.h"

using namespace std;

int NeighborValue(Pixel srcPixel, float originR, float originC, int k)
{
    int intOriginR, intOriginC;										//原图中对应整像素点坐标
    int finalValue;													//第二次插值后得到的值

    if(originR - (int)originR<=0.5 || srcPixel.height - originR < 1)
    {
        intOriginR = (int)originR;
    }
    else
    {
        intOriginR = (int)originR + 1;
    }
    if(originC - (int)originC <= 0.5 || srcPixel.width - originC < 1)
    {
        intOriginC = (int)originC;
    }
    else
    {
        intOriginC = (int)originC + 1;
    }    // 根据像素点与边缘的关系确定对应坐标值
    finalValue = srcPixel.content[intOriginR][intOriginC][k];

    return finalValue;
}


Pixel NeighborSize(Pixel srcPixel, float heightTimes, float widthTimes)
{
    int heightNew, widthNew;
    float originR, originC;											//原图中对应的像素点坐标
    Pixel destPixel;

    heightNew = srcPixel.height * heightTimes;
    widthNew = srcPixel.width * widthTimes;

    destPixel.SetSize(heightNew, widthNew);
    destPixel.CreatePixel();

    for (int i = 0; i < heightNew; i++)
    {
        for (int j = 0; j < widthNew; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                //计算对应原图的坐标
                originR = i / heightTimes;
                originC = j / widthTimes;
                //计算并进行像素点赋值
                destPixel.content[i][j][k] = NeighborValue(srcPixel, originR, originC, k);
            }
        }
    }

    return destPixel;
}

Pixel NeighborRotate(Pixel srcPixel, float rotateDegree)
{
    int heightNew, widthNew;
    float rotateDegreeRad;
    float originR, originC;											//原图中对应的像素点坐标
    Pixel destPixel;

    rotateDegreeRad = rotateDegree / 180 * 3.1415926;
    heightNew = (int)(srcPixel.width * fabs(sin(rotateDegreeRad)) + srcPixel.height * fabs(cos(rotateDegreeRad)));
    widthNew = (int)(srcPixel.width * fabs(cos(rotateDegreeRad)) + srcPixel.height * fabs(sin(rotateDegreeRad)));

    destPixel.SetSize(heightNew, widthNew);
    destPixel.CreatePixel();

    for (int i = 0; i < heightNew; i++)
    {
        for (int j = 0; j < widthNew; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                originR = (srcPixel.height / 2 + (i - heightNew / 2) * cos(rotateDegreeRad) -
                    (j - widthNew / 2) * sin(rotateDegreeRad));
                originC = (srcPixel.width / 2 + (i - heightNew / 2) * sin(rotateDegreeRad) +
                    (j - widthNew / 2) * cos(rotateDegreeRad));
                //分情况对像素点赋值
                if (srcPixel.height - originR > 0 && srcPixel.height - originR <= srcPixel.height
                    && srcPixel.width - originC > 0 && srcPixel.width - originC <= srcPixel.width)
                {
                    destPixel.content[i][j][k] = NeighborValue(srcPixel, originR, originC, k);
                }
                else
                {
                    destPixel.content[i][j][k] = 0;
                }
            }
        }
    }

    return destPixel;
}


#endif
