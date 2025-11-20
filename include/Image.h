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
} Image;


//Functions Prototype

Image LoadImg(const char *filepath);

int SaveImg(Image img, const char *filepath);


#endif