#ifndef TOOLS_H
#define TOOLS_H
#include"Image.h"

MyImage Crop(MyImage Img, int x1 , int x2 , int y1 , int y2);
MyImage RotateAntiClock(MyImage Img);
MyImage FlipX(MyImage img);
MyImage FlipY (MyImage img);
MyImage RotateClock (MyImage img);
MyImage Occupacity (MyImage img , int a);
MyImage InvertColor (MyImage img);

#endif
