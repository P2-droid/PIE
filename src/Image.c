#include <stdio.h>
#include "Image.h"
#include "raylib.h"

Image LoadImg(const char *filepath) 
{
    Image img = LoadImage(filepath); 

    if (img.data == NULL) 
    {
        printf("Failed to load image: %s\n", filepath);
    } 
    else 
    {
        printf("Loaded image: %s (%d x %d)\n", filepath, img.width, img.height);
    }

    return img;
}


int SaveImg(Image img, const char *filepath) 
{
    if (img.data == NULL) 
    {
        printf("No image to save!\n");
        return 0;   
    }

    int success = ExportImage(img, filepath);

    if (success)
        printf("Saved image to: %s\n", filepath);
    else
        printf("Failed to save image: %s\n", filepath);

    return success;
}


