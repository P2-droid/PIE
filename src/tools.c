#include <stdio.h>
#include "Image.h"

//Crop tool

Image Crop(Image Img, int x1 , int x2 , int y1 , int y2)
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
    
    Image Result;

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

    return Result;
}

//Rotate Tool

Image Rotate(Image Img)
{
    Image Result;

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

    return Result;

}
