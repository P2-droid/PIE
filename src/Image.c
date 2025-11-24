#include <stdio.h>
#include "Image.h"


MyImage LoadBMP(FILE *f)
{

    MyImage img; // a image to read data in.
    unsigned char header[14];
    unsigned char *Image;
    int offset;
    size_t loop;
    size_t total_size;

    fread(header, 1, 14, f);

    offset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);

    total_size = header[2] | (header[3] << 8) | (header[4] << 16) | (header[5] << 24);

    Image = malloc(total_size);
    
    fseek(f, 0, SEEK_SET);
    fread(Image, 1, total_size, f);

    img.width = Image[18] | (Image[19] << 8) | (Image[20] << 16) | (Image[21] << 24);
    img.height = Image[22] | (Image[23] << 8) | (Image[24] << 16) | (Image[25] << 24);
    img.channel = (Image[28] | (Image[29] << 8))/8;

    loop = total_size - offset;

    if (img.channel == 4)
    {
        size_t pixel_count = loop/4;
        img.data = malloc(pixel_count * sizeof(Pixel));

        for (int i = 0; i < loop; i++)
        {
            if ((i % 4) == 0)
            {
                (img.data + i/4)->b = Image[offset + i]; 
            }

            else if ((i % 4) == 1)
            {
                (img.data + i/4)->g = Image[offset + i];
            }

            else if ((i % 4) == 2)
            {
                (img.data + i/4)->r = Image[offset + i];
            }

            else if ((i % 4) == 3)
            {
                (img.data + i/4)->a = Image[offset + i];
            }
        }
    }

    else if (img.channel == 3)
    {
        int x = (4 - (img.width * 3) % 4) % 4; //padding
        size_t pixel_count = img.width * img.height;
        img.data = malloc(pixel_count * sizeof(Pixel));

        for (int i = 0; i < img.height; i++)
        { 
            for (int j = 0; j < img.width; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    size_t pixelstart = offset + i * (img.width * 3 + x) + j * 3;
                    if(k == 0)
                    {
                        (img.data + (i*img.width + j))->b = Image[pixelstart + 0];
                    }

                    else if(k == 1)
                    {
                        (img.data + (i*img.width + j))->g =  Image[pixelstart + 1];
                    }

                    else if(k == 2)
                    {
                        (img.data + (i*img.width + j))->r =  Image[pixelstart + 2];
                    }

                    else if(k == 3)
                    {
                        (img.data + (i*img.width + j))->a = 255;
                    }

                }
            }
        }

    }

}


MyImage LoadImg(const char *filepath) 
{
    // To load image we need to copy the pixel data from file to the image data structure. This would make processing the data very easy.

    MyImage img ;

    FILE *f = fopen("filepath" , "rb" );

    if (!f)
    printf("File does not exist.");

    unsigned char header[2];
    fread(header, 1, 2, f);
    
    if (header[0] == 'B' && header[1] == 'M') 
    {
        img = LoadBMP(f);
    }
    
    return img;

}

