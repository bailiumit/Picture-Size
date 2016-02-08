//编写者：刘柏
//编写时间：2015.11.10
//功能：创建Pixel类，方便操作

#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <string>

class Pixel
{
public:
    Pixel();
    ~Pixel();
    void CreatePixel();
    void DeletePixel();
    void SetSize(int n_height, int n_width);

    int height, width;
    int*** content;
};

Pixel::Pixel()
{
    height = 0;
    width = 0;
    content = NULL;
}

Pixel::~Pixel()
{
}

void Pixel::CreatePixel()
{
    //动态创建三维数组
    content = new int**[height];
    for (int i = 0; i < height; i++)
    {
        content[i] = new int*[width];
        for (int j = 0; j < width; j++)
            content[i][j] = new int[3];
    }
}

void Pixel::DeletePixel()
{
    //释放三维数组
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            delete[]content[i][j];
            content[i][j] = NULL;
        }
        delete[]content[i];
        content[i] = NULL;
    }
    delete[]content;
    content = NULL;
}

void Pixel::SetSize(int n_height, int n_width)
{
    height = n_height;
    width = n_width;
}

#endif
