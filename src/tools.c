#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Image.h"
#include<tools.h>

#define COPY_META(dest, src) \
    strcpy(dest.Name, src.Name); \
    dest.channel = src.channel;

/* ============================================================
   CROP
   ============================================================ */
MyImage Crop(MyImage Img, int x1 , int y1 , int x2 , int y2)
{
    /* FIX: correct order (you used x1,x2,y1,y2) */

    if (x1 > x2) { int t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { int t = y1; y1 = y2; y2 = t; }

    MyImage Result;
    Result.width = x2 - x1;
    Result.height = y2 - y1;
    Result.channel = Img.channel;
    strcpy(Result.Name, Img.Name);

    Result.data = malloc(Result.width * Result.height * sizeof(Pixel));

    for (int y = 0; y < Result.height; y++)
        for (int x = 0; x < Result.width; x++)
            Result.data[y * Result.width + x] =
                Img.data[(y1 + y) * Img.width + (x1 + x)];

    return Result;  /* FIX: removed free(Img.data) */
}

/* ============================================================
   ROTATE 90° ANTI CLOCKWISE
   ============================================================ */
MyImage RotateAntiClock(MyImage Img)
{
    MyImage R;
    R.width  = Img.height;
    R.height = Img.width;
    R.channel = Img.channel;
    strcpy(R.Name, Img.Name);

    R.data = malloc(R.width * R.height * sizeof(Pixel));

    /* Correct formula */
    for (int y = 0; y < R.height; y++)
        for (int x = 0; x < R.width; x++)
            R.data[y * R.width + x] =
                Img.data[x * Img.width + (Img.width - 1 - y)];

    return R;
}

/* ============================================================
   FLIP VERTICALLY (TOP <-> BOTTOM)
   ============================================================ */
MyImage FlipX(MyImage Img)
{
    MyImage R;
    R.width = Img.width;
    R.height = Img.height;
    R.channel = Img.channel;
    strcpy(R.Name, Img.Name);

    R.data = malloc(R.width * R.height * sizeof(Pixel));

    for (int y = 0; y < Img.height; y++)
        for (int x = 0; x < Img.width; x++)
            R.data[(Img.height - 1 - y) * Img.width + x] =
                Img.data[y * Img.width + x];

    return R;
}

/* ============================================================
   FLIP HORIZONTALLY (LEFT <-> RIGHT)
   ============================================================ */
MyImage FlipY(MyImage Img)
{
    MyImage R;
    R.width = Img.width;
    R.height = Img.height;
    R.channel = Img.channel;
    strcpy(R.Name, Img.Name);

    R.data = malloc(R.width * R.height * sizeof(Pixel));

    for (int y = 0; y < Img.height; y++)
        for (int x = 0; x < Img.width; x++)
            R.data[y * Img.width + (Img.width - 1 - x)] =
                Img.data[y * Img.width + x];

    return R;
}

/* ============================================================
   ROTATE 90° CLOCKWISE
   ============================================================ */
MyImage RotateClock(MyImage Img)
{
    MyImage R;
    R.width  = Img.height;
    R.height = Img.width;
    R.channel = Img.channel;
    strcpy(R.Name, Img.Name);

    R.data = malloc(R.width * R.height * sizeof(Pixel));

    /* Correct formula */
    for (int y = 0; y < R.height; y++)
        for (int x = 0; x < R.width; x++)
            R.data[y * R.width + x] =
                Img.data[(Img.height - 1 - x) * Img.width + y];

    return R;
}

/* ============================================================
   OPACITY
   ============================================================ */
MyImage Occupacity(MyImage Img, int a)
{
    MyImage R;
    R.width = Img.width;
    R.height = Img.height;
    R.channel = Img.channel;
    strcpy(R.Name, Img.Name);

    R.data = malloc(R.width * R.height * sizeof(Pixel));

    for (int i = 0; i < Img.width * Img.height; i++)
    {
        R.data[i].r = Img.data[i].r;
        R.data[i].g = Img.data[i].g;
        R.data[i].b = Img.data[i].b;

        if (Img.channel == 4)
            R.data[i].a = a;
        else
            R.data[i].a = 255;   /* RGB → keep opaque */
    }

    return R;
}

/* ============================================================
   INVERT COLORS
   ============================================================ */
MyImage InvertColor(MyImage Img)
{
    MyImage R;
    R.width = Img.width;
    R.height = Img.height;
    R.channel = Img.channel;
    strcpy(R.Name, Img.Name);

    R.data = malloc(R.width * R.height * sizeof(Pixel));

    for (int i = 0; i < Img.width * Img.height; i++)
    {
        R.data[i].r = 255 - Img.data[i].r;
        R.data[i].g = 255 - Img.data[i].g;
        R.data[i].b = 255 - Img.data[i].b;
        R.data[i].a = (Img.channel == 4 ? Img.data[i].a : 255);
    }

    return R;
}
