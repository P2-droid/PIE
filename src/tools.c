#include <stdio.h>
#include <stdlib.h>
#include "Image.h"

//Crop tool

MyImage Crop(MyImage Img, int x1 , int x2 , int y1 , int y2)
{
    int temp;
    if (x1 > x2)
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    
    if (y1 > y2)
    {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    
    MyImage Result;

    Result.width = x2 - x1;
    Result.height = y2 - y1;
    Result.channel = Img.channel;
    Result.data = malloc( Result.width * Result.height * sizeof(Pixel));
    
    for (int i = 0 ; i < Result.height ; i++)
    {
        for (int j = 0 ; j < Result.width; j++)
        {
            Result.data[i * Result.width + j] = Img.data[(y1+i) * Img.width + x1 + j] ;
        }
    }

    free(Img.data);

    return Result;
}

//Rotate Anti-Clockwise.

MyImage RotateAntiClock(MyImage Img)
{
    MyImage Result;

    Result.width = Img.height;
    Result.height = Img.width;
    Result.channel = Img.channel;
    Result.data = malloc(Result.width * Result.height * sizeof(Pixel));

    for (int i = 0 ; i < Result.height ; i++)
    {
        for (int j = 0 ; j < Result.width ; j++)
        {
            Result.data[i * Result.width + j] = Img.data[(Img.width-1)+j*(Img.width) - i ];
        }
    }

    free(Img.data);

    return Result;

}

//Flip on X axis. Reflection on a Vertical mirror.
MyImage FlipX(MyImage img)
{
    MyImage Result;

    Result.width = img.width;
    Result.height = img.height;

    size_t Totalpixels = img.width*img.height;
    Result.data = malloc(Totalpixels * sizeof(Pixel));

    for (int i = (img.height - 1); i >= 0; i--)
    {
        for (size_t j = 0; j < img.width; j++)
        {
            Result.data[(img.height - (i + 1))*img.width + j] = img.data[(i * img.width ) + j];
        }
    }

    free(img.data);

    return Result;
}

//Flip on Y axis. Reflection on a Horizontal mirror.
MyImage FlipY (MyImage img)
{
    MyImage Intermediate1 = RotateAntiClock(img);

    MyImage Intermediate2 = RotateAntiClock(Intermediate1);

    MyImage Result = FlipX(Intermediate2);

    return Result;
}

//Rotate Clock-wise.
MyImage RotateClock (MyImage img)
{
    MyImage Intermediate = RotateAntiClock(img);

    MyImage Result = FlipX(Intermediate);

    return Result;
}

//Change Occupacity of image.
MyImage Occupacity (MyImage img , int a)
{
    MyImage Result;
    
    Result.width = img.width;
    Result.height = img.height;
    Result.channel = img.channel;
    Result.data = malloc((Result.width*Result.height) * sizeof(Pixel));

    for (int i = 0; i < img.height; i++)
    {
        for(int j = 0; j < img.width; j++)
        {
            (Result.data + (i*img.width + j))->b = (img.data + (i*img.width + j))->b;
            (Result.data + (i*img.width + j))->g = (img.data + (i*img.width + j))->g;
            (Result.data + (i*img.width + j))->r = (img.data + (i*img.width + j))->r;
            (Result.data + (i*img.width + j))->a = a;

        }
    }

    free(img.data);
    
    return Result;
}

MyImage InvertColor (MyImage img)
{
    MyImage Result;
    
    Result.width = img.width;
    Result.height = img.height;
    Result.channel = img.channel;
    Result.data = malloc((Result.width*Result.height) * sizeof(Pixel));

    if(Result.channel == 4)
    {
        for (int i = 0; i < img.height; i++)
        {
            for(int j = 0; j < img.width; j++)
            {
                (Result.data + (i*img.width + j))->b = 255 - (img.data + (i*img.width + j))->b;
                (Result.data + (i*img.width + j))->g = 255 - (img.data + (i*img.width + j))->g;
                (Result.data + (i*img.width + j))->r = 255 - (img.data + (i*img.width + j))->r;
                (Result.data + (i*img.width + j))->a = (img.data + (i*img.width + j))->a;

            }
        }   
    }
    else if(Result.channel == 4)
    {

    for (int i = 0; i < img.height; i++)
    {
        for(int j = 0; j < img.width; j++)
        {
            (Result.data + (i*img.width + j))->b = 255 - (img.data + (i*img.width + j))->b;
            (Result.data + (i*img.width + j))->g = 255 - (img.data + (i*img.width + j))->g;
            (Result.data + (i*img.width + j))->r = 255 - (img.data + (i*img.width + j))->r;
            (Result.data + (i*img.width + j))->a = 255;

        }
    }

    }

    free(img.data);

    return Result;
}