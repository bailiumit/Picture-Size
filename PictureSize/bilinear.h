//编写者：刘柏
//编写时间：2015.11.10
//功能：双线性插值计算

#ifndef BILINEAR_H
#define BILINEAR_H

#include <iostream>
#include <string>
#include <cmath>
#include "pixel.h"

using namespace std;

int BilinearValue(Pixel srcPixel, float originR, float originC, int k)
{
    int intOriginR[2];												//原图中对应整像素点行坐标
    int intOriginC[2];												//原图中对应整像素点列坐标
    int isEdgeX, isEdgeY;											//原图中的像素点是否在边缘
    int originValue[2][2];											//原图中对应点周边像素点的值
    int middleValue[2];												//第一次插值后得到的值
    int finalValue;													//第二次插值后得到的值

    intOriginR[0] = (int)originR;
    intOriginC[0] = (int)originC;
    // 根据像素点与边缘的关系确定对应坐标值
    if (intOriginR[0] == srcPixel.height - 1)
    {
        isEdgeX = 1;
    }
    else
    {
        isEdgeX = 0;
    }
    if (intOriginC[0] == srcPixel.width - 1)
    {
        isEdgeY = 1;
    }
    else
    {
        isEdgeY = 0;
    }
    intOriginR[1] = intOriginR[0] + 1 - isEdgeX;
    intOriginC[1] = intOriginC[0] + 1 - isEdgeY;
    //获取原图中的像素值
    originValue[0][0] = srcPixel.content[intOriginR[0]][intOriginC[0]][k];
    originValue[0][1] = srcPixel.content[intOriginR[0]][intOriginC[1]][k];
    originValue[1][0] = srcPixel.content[intOriginR[1]][intOriginC[0]][k];
    originValue[1][1] = srcPixel.content[intOriginR[1]][intOriginC[1]][k];
    //双线性插值
    middleValue[0] = (originR - intOriginR[0]) * originValue[0][1] + (intOriginR[1] - originR + isEdgeX) * originValue[0][1];
    middleValue[1] = (originR - intOriginR[0]) * originValue[1][0] + (intOriginR[1] - originR + isEdgeX) * originValue[1][1];
    finalValue = (originC - intOriginC[0]) * middleValue[0] + (intOriginC[1] - originC + isEdgeY) * middleValue[1];

    return finalValue;
}


Pixel BilinearSize(Pixel srcPixel, float heightTimes, float widthTimes)
{
    int heightNew, widthNew;                                        //缩放过后的图像高度、宽度
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
                //插值并进行像素点赋值
                destPixel.content[i][j][k] = BilinearValue(srcPixel, originR, originC, k);
            }
        }
    }

    return destPixel;
}


Pixel BilinearRotate(Pixel srcPixel, float rotateDegree)
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
                    destPixel.content[i][j][k] = BilinearValue(srcPixel, originR, originC, k);
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

