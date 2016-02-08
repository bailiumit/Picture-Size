//编写者：刘柏
//编写时间：2015.11.10
//功能：双三次插值计算

#ifndef BICUBIC_H
#define BICUBIC_H

#include <iostream>
#include <string>
#include <cmath>
#include "pixel.h"

using namespace std;

float BicubicCore(float x)
{
    float S;
    float absX = fabs(x);

    if (absX <= 1)
        S = 1 - 2 * absX*absX + absX*absX*absX;
    else if (absX > 1 && absX < 2)
        S = 4 - 8 * absX + 5 * absX*absX - absX*absX*absX;
    else
        S = 0;

    return S;
}


int BicubicValue(Pixel srcPixel, float originR, float originC, int k)
{
    int intOriginR[4];														//原图中对应整像素点行坐标
    int intOriginC[4];														//原图中对应整像素点列坐标
    int originValue[4][4];													//原图中整点的像素值
    float valueA[4], valueC[4];												//双三次插值过程中的矩阵A，矩阵C
    float diffR, diffC;														//原图中像素点坐标与整点坐标的差值
    int finalValue = 0;

    intOriginR[1] = (int)originR;
    intOriginC[1] = (int)originC;
    diffR = originR - intOriginR[1];
    diffC = originC - intOriginC[1];

    //初始化valueA，valueC
    for (int i = 0; i < 4; i++)
    {
        valueA[i] = BicubicCore(diffR + 1 - i);
        valueC[i] = BicubicCore(diffC + 1 - i);
    }
    //初始化intOriginR
    if (intOriginR[1] == 0)
        intOriginR[0] = intOriginR[1];
    else
        intOriginR[0] = intOriginR[1] - 1;
    if (intOriginR[1] == srcPixel.height - 1)
    {
        intOriginR[2] = intOriginR[1];
        intOriginR[3] = intOriginR[1];
    }
    else if (intOriginR[1] == srcPixel.height - 2)
    {
        intOriginR[2] = intOriginR[1] + 1;
        intOriginR[3] = intOriginR[2];
    }
    else
    {
        intOriginR[2] = intOriginR[1] + 1;
        intOriginR[3] = intOriginR[2] + 1;
    }
    //初始化intOriginC
    if (intOriginC[1] == 0)
        intOriginC[0] = intOriginC[1];
    else
        intOriginC[0] = intOriginC[1] - 1;
    if (intOriginC[1] == srcPixel.width - 1)
    {
        intOriginC[2] = intOriginC[1];
        intOriginC[3] = intOriginC[1];
    }
    else if (intOriginC[1] == srcPixel.width - 2)
    {
        intOriginC[2] = intOriginC[1] + 1;
        intOriginC[3] = intOriginC[2];
    }
    else
    {
        intOriginC[2] = intOriginC[1] + 1;
        intOriginC[3] = intOriginC[2] + 1;
    }
    //初始化originValue
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            originValue[i][j] = srcPixel.content[intOriginR[i]][intOriginC[j]][k];
        }
    }

    //计算插值结果
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            finalValue += valueA[j] * originValue[j][i] * valueC[i];
        }
    }

    //限制finalValue的值，防止出现毛刺
    if (finalValue < 0)
    {
        finalValue= 0;
    }
    else if(finalValue > 255)
    {
        finalValue = 255;
    }

    return finalValue;
}


Pixel BicubicSize(Pixel srcPixel, float heightTimes, float widthTimes)
{
    int heightNew = srcPixel.height * heightTimes;
    int widthNew = srcPixel.width * widthTimes;
    float originR, originC;													//原图中对应像素点坐标
    Pixel destPixel;

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
                destPixel.content[i][j][k] = BicubicValue(srcPixel, originR, originC, k);
            }
        }
    }

    return destPixel;
}


Pixel BicubicRotate(Pixel srcPixel, float rotateDegree)
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
                    destPixel.content[i][j][k] = BicubicValue(srcPixel, originR, originC, k);
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
