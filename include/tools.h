#ifndef TOOLS_H
#define TOOLS_H

#include "Image.h"

/* ---- BASIC IMAGE OPERATIONS ---- */

MyImage Crop(MyImage img, int x1, int y1, int x2, int y2);

MyImage RotateAntiClock(MyImage img);
MyImage RotateClock(MyImage img);

MyImage FlipX(MyImage img);  /* Horizontal flip */
MyImage FlipY(MyImage img);  /* Vertical flip  */

/* Transparency (Opacity) function */
MyImage Occupacity(MyImage img, int a);  

MyImage InvertColor(MyImage img);

#endif
