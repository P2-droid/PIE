#ifndef IMAGE_H
#define IMAGE_H

#include<stdio.h>

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
    char Name[10];
} MyImage;


//Functions Prototype

MyImage LoadImg(const char *filepath);
MyImage LoadBMP(FILE *f);

int SaveImg (MyImage img, const char *filepath);
int SaveBMP(FILE *f, MyImage img);
#endif