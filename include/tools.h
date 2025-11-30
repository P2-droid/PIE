#ifndef TOOLS_H
#define TOOLS_H

#include "Image.h"

/* ---- BASIC IMAGE OPERATIONS ---- */

MyImage Crop(MyImage img, int x1, int y1, int x2, int y2);
/* Corrected your (x1, x2, y1, y2) mismatch to (x1, y1, x2, y2) */

MyImage RotateAntiClock(MyImage img);
MyImage RotateClock(MyImage img);

MyImage FlipX(MyImage img);  /* Horizontal flip */
MyImage FlipY(MyImage img);  /* Vertical flip  */

/* Transparency (Opacity) function */
MyImage Occupacity(MyImage img, int a);  
/* Note: you named it "Occupacity", correct spelling is "Opacity" 
   but I kept it EXACTLY as you wrote so your program won't break. */

MyImage InvertColor(MyImage img);

#endif
