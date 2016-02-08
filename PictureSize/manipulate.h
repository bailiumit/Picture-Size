//编写者：刘柏
//编写时间：2015.11.10
//功能：执行操作

#ifndef MANIPULATE
#define MANIPULATE

#include <QImage>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QString>

#include "pixel.h"
#include "neighbor.h"
#include "bilinear.h"
#include "bicubic.h"


Pixel CropImage(Pixel midPixel, Pixel destPixel)
{
    int midHeight, midWidth, destHeight, destWidth;
    int diffHeight, diffWidth;
    Pixel cropPixel;

    midHeight = midPixel.height;
    midWidth = midPixel.width;
    destHeight = destPixel.height;
    destWidth = destPixel.width;
    diffHeight = (int)(destHeight - midHeight)/2;
    diffWidth = (int)(destWidth - midWidth)/2;

    cropPixel.SetSize(midHeight, midWidth);
    cropPixel.CreatePixel();

    for (int i = 0; i < midHeight; i++)
    {
        for (int j = 0; j < midWidth; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                cropPixel.content[i][j][k] = destPixel.content[i + diffHeight][j + diffWidth][k];
            }
        }
    }

    return cropPixel;
}


QImage* Manipulate(QImage *srcImg, float heightTimes, float widthTimes, float rotateDegree, int mode, int method)
{

    //变量定义
    QImage* destImg;											//处理后的图像
    Pixel srcPixel, middlePixel, destPixel;											//存储处理后像素点信息的数组
    QRgb srcRGB, destRGB;

    //动态创建三维数组
    srcPixel.SetSize(srcImg->height(), srcImg->width());
    srcPixel.CreatePixel();

    //将像素点信息存到三维数组中
    for (int i = 0; i < srcPixel.height; i++)
    {
        for (int j = 0; j < srcPixel.width; j++)
        {
            srcRGB = srcImg->pixel(j, i);
            srcPixel.content[i][j][0] = qRed(srcRGB);
            srcPixel.content[i][j][1] = qGreen(srcRGB);
            srcPixel.content[i][j][2] = qBlue(srcRGB);
        }
    }

    //根据不同差值方式处理像素点
    switch (method)
    {
    case 0:
        middlePixel = NeighborSize(srcPixel, heightTimes, widthTimes);
        destPixel = NeighborRotate(middlePixel, rotateDegree);
        break;
    case 1:
        middlePixel = BilinearSize(srcPixel, heightTimes, widthTimes);
        destPixel = BilinearRotate(middlePixel, rotateDegree);
        break;
    case 2:
        middlePixel = BicubicSize(srcPixel, heightTimes, widthTimes);
        destPixel = BicubicRotate(middlePixel, rotateDegree);
        break;
    default:
        break;
    }

    //根据旋转模式截取相应图像大小
    if (mode == 0)
        destPixel = CropImage(middlePixel, destPixel);

    //将处理后的像素点转化为图片
    destImg = new QImage(destPixel.width, destPixel.height, QImage::Format_RGB32);
    for (int i = 0; i < destPixel.height; i++)
    {
        for (int j = 0; j < destPixel.width; j++)
        {
            destRGB = qRgb(destPixel.content[i][j][0],
                    destPixel.content[i][j][1], destPixel.content[i][j][2]);
            destImg->setPixel(j, i, destRGB);
        }
    }

    return destImg;
}

#endif // MANIPULATE

