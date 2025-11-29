#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Image.h"


MyImage LoadBMP(FILE *f)
{
    MyImage img;
    strcpy(img.Name, "BMP");

    unsigned char header[14];
    unsigned char *Image;
    int offset;
    size_t total_size;

    fread(header, 1, 14, f);

    offset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);

    total_size = header[2] | (header[3] << 8) | (header[4] << 16) | (header[5] << 24);

    Image = malloc(total_size);

    fseek(f, 0, SEEK_SET);
    fread(Image, 1, total_size, f);

    img.width  = Image[18] | (Image[19] << 8) | (Image[20] << 16) | (Image[21] << 24);
    img.height = Image[22] | (Image[23] << 8) | (Image[24] << 16) | (Image[25] << 24);
    img.channel = (Image[28] | (Image[29] << 8)) / 8;

    size_t pixel_count = img.width * img.height;
    img.data = malloc(pixel_count * sizeof(Pixel));

    if (img.channel == 4)
    {
        size_t loop = total_size - offset;

        for (size_t i = 0; i < loop; i++)
        {
            size_t p = i / 4;

            if ((i % 4) == 0) img.data[p].b = Image[offset + i];
            else if ((i % 4) == 1) img.data[p].g = Image[offset + i];
            else if ((i % 4) == 2) img.data[p].r = Image[offset + i];
            else if ((i % 4) == 3) img.data[p].a = Image[offset + i];
        }
    }
    else if (img.channel == 3)
    {
        int pad = (4 - (img.width * 3) % 4) % 4;

        for (int y = 0; y < img.height; y++)
        {
            for (int x = 0; x < img.width; x++)
            {
                size_t pixelstart = offset + y*(img.width*3 + pad) + x*3;
                img.data[y*img.width + x].b = Image[pixelstart + 0];
                img.data[y*img.width + x].g = Image[pixelstart + 1];
                img.data[y*img.width + x].r = Image[pixelstart + 2];
                img.data[y*img.width + x].a = 255;
            }
        }
    }

    free(Image);
    return img;
}

int SaveBMP(FILE *f, MyImage img)
{
    if (!f) return 1;

    int width = img.width;
    int height = img.height;

    int channels = img.channel;
    if (channels != 3 && channels != 4)
    {
        printf("Unsupported channel count.\n");
        fclose(f);
        return 1;
    }

    int bytesPerPixel = channels;
    int rowBytes = width * bytesPerPixel;

    int padding = 0;
    if (channels == 3)
        padding = (4 - (rowBytes % 4)) % 4;

    int rowSize = rowBytes + padding;
    int pixelDataSize = rowSize * height;

    int headerSize = 54;
    int fileSize = headerSize + pixelDataSize;

    unsigned char header[54] = {0};

    // BMP signature
    header[0] = 'B';
    header[1] = 'M';

    // File size
    header[2] = (unsigned char)(fileSize);
    header[3] = (unsigned char)(fileSize >> 8);
    header[4] = (unsigned char)(fileSize >> 16);
    header[5] = (unsigned char)(fileSize >> 24);

    // Pixel data offset
    header[10] = 54;

    // DIB header size
    header[14] = 40;

    // Width
    header[18] = (unsigned char)(width);
    header[19] = (unsigned char)(width >> 8);
    header[20] = (unsigned char)(width >> 16);
    header[21] = (unsigned char)(width >> 24);

    // Height (MUST be positive)
    header[22] = (unsigned char)(height);
    header[23] = (unsigned char)(height >> 8);
    header[24] = (unsigned char)(height >> 16);
    header[25] = (unsigned char)(height >> 24);

    // Planes
    header[26] = 1;

    // Bits per pixel
    header[28] = (channels == 3) ? 24 : 32;

    // Compression (0 = BI_RGB)
    header[30] = 0;
    header[31] = 0;
    header[32] = 0;
    header[33] = 0;

    // Image size
    header[34] = (unsigned char)(pixelDataSize);
    header[35] = (unsigned char)(pixelDataSize >> 8);
    header[36] = (unsigned char)(pixelDataSize >> 16);
    header[37] = (unsigned char)(pixelDataSize >> 24);

    fwrite(header, 1, 54, f);

    unsigned char pad[3] = {0,0,0};

    // Write pixels (BMP is bottom-up)
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            Pixel px = img.data[y * width + x];

            fwrite(&px.b, 1, 1, f);
            fwrite(&px.g, 1, 1, f);
            fwrite(&px.r, 1, 1, f);

            if (channels == 4)
                fwrite(&px.a, 1, 1, f);
        }

        if (channels == 3)
            fwrite(pad, 1, padding, f);
    }

    fclose(f);
    return 0;
}



MyImage LoadImg(const char *filepath)
{
    FILE *f = fopen(filepath, "rb");
    MyImage img;

    if (!f)
    {
        printf("File does not exist\n");
        return img;
    }

    unsigned char header[2];
    fread(header, 1, 2, f);

    if (header[0] == 'B' && header[1] == 'M')
    {
        fseek(f, 0, SEEK_SET);   // FIXED
        img = LoadBMP(f);
    }

    fclose(f);
    return img;
}


int SaveImg (MyImage img, const char *filepath)
{
    FILE *f = fopen(filepath, "wb");
    if (!f)
    {
        printf("Cannot open file for writing.\n");
        return 1;
    }


    if (strcmp(img.Name, "BMP"))
    {
       int a = SaveBMP(f ,img);
       return a;
    }
}
