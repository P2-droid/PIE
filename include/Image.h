#ifndef IMAGE_H
#define IMAGE_H

//Image Data

typedef struct 
{
    unsigned char r, g, b, a;
} Pixel;

typedef struct 
{
    int width;
    int height;
    int channel;
    Pixel *data;
} MyImage;


//Functions Prototype

MyImage LoadImg(const char *filepath);
MyImage LoadBMP(const char *filepath);

#endif