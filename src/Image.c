#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Image.h"

static unsigned int le32(const unsigned char *b)
{
    return (unsigned int)b[0] | ((unsigned int)b[1] << 8) | ((unsigned int)b[2] << 16) | ((unsigned int)b[3] << 24);
}
static unsigned int le16(const unsigned char *b)
{
    return (unsigned int)b[0] | ((unsigned int)b[1] << 8);
}

MyImage LoadBMP(FILE *f)
{
    MyImage img;
    img.width = img.height = img.channel = 0;
    img.data = NULL;
    img.Name[0] = '\0';
    strcpy(img.Name, "BMP");

    if (!f) return img;

    /* Find file size */
    if (fseek(f, 0, SEEK_END) != 0) return img;
    long total_size_long = ftell(f);
    if (total_size_long <= 0) return img;
    size_t total_size = (size_t) total_size_long;
    rewind(f);

    unsigned char *filebuf = (unsigned char*)malloc(total_size);
    if (!filebuf) return img;

    if (fread(filebuf, 1, total_size, f) != total_size)
    {
        free(filebuf);
        return img;
    }

    /* Basic header checks */
    if (total_size < 54 || filebuf[0] != 'B' || filebuf[1] != 'M')
    {
        free(filebuf);
        return img;
    }

    unsigned int offset = le32(&filebuf[10]);
    /* DIB header size usually at 14..17 */
    unsigned int dibSize = le32(&filebuf[14]);

    /* width/height/bpp locations depend on DIB header, but for BITMAPINFOHEADER (40 bytes) use offsets 18,22,28 */
    if (total_size < 54)
    {
        free(filebuf);
        return img;
    }

    img.width  = (int) le32(&filebuf[18]);
    img.height = (int) le32(&filebuf[22]); 
    unsigned int raw_bpp = le16(&filebuf[28]);
    if (raw_bpp == 0) raw_bpp = 24; 
    img.channel = (int)(raw_bpp / 8);

    unsigned int pixelDataSize = le32(&filebuf[34]);
    if (pixelDataSize == 0)
    {
        if (offset <= total_size) pixelDataSize = (unsigned int)(total_size - offset);
        else pixelDataSize = 0;
    }

    if (img.width <= 0 || img.height == 0 || (img.channel != 3 && img.channel != 4))
    {
        /* unsupported or corrupt */
        free(filebuf);
        return img;
    }

    /* Allocate image pixels (width * abs(height)) */
    int absHeight = img.height > 0 ? img.height : -img.height;
    size_t pixel_count = (size_t)img.width * (size_t)absHeight;
    img.data = (Pixel*)malloc(pixel_count * sizeof(Pixel));
    if (!img.data)
    {
        free(filebuf);
        img.width = img.height = img.channel = 0;
        return img;
    }

    if (img.channel == 4)
    {
        /* 32bpp: usually no row padding; BMP stores B,G,R,A in that order */
        /* file rows are usually bottom-up if height > 0 */
        size_t expected = (size_t)absHeight * (size_t)img.width * 4;
        if (pixelDataSize < expected) {
            /* still try to avoid overflow; clamp loop */
            expected = pixelDataSize;
        }

        /* iterate through each file-row (fr), map to image row (ir) */
        for (int fr = 0; fr < absHeight; ++fr)
        {
            int ir = (img.height > 0) ? (absHeight - 1 - fr) : fr; /* bottom-up -> flip */
            size_t rowBase = (size_t)offset + (size_t)fr * (size_t)img.width * 4;
            for (int x = 0; x < img.width; ++x)
            {
                size_t idx = rowBase + (size_t)x * 4;
                if (idx + 3 >= total_size) {
                    img.data[(size_t)ir * img.width + x].b = 0;
                    img.data[(size_t)ir * img.width + x].g = 0;
                    img.data[(size_t)ir * img.width + x].r = 0;
                    img.data[(size_t)ir * img.width + x].a = 255;
                } else {
                    img.data[(size_t)ir * img.width + x].b = filebuf[idx + 0];
                    img.data[(size_t)ir * img.width + x].g = filebuf[idx + 1];
                    img.data[(size_t)ir * img.width + x].r = filebuf[idx + 2];
                    img.data[(size_t)ir * img.width + x].a = filebuf[idx + 3];
                }
            }
        }
    }
    else /* img.channel == 3 */
    {
        /* 24bpp: rows padded to 4-byte boundaries */
        int rowStride = img.width * 3;
        int pad = (4 - (rowStride % 4)) % 4;
        int fileRowBytes = rowStride + pad;

        for (int fr = 0; fr < absHeight; ++fr)
        {
            int ir = (img.height > 0) ? (absHeight - 1 - fr) : fr;
            size_t rowBase = (size_t)offset + (size_t)fr * (size_t)fileRowBytes;

            for (int x = 0; x < img.width; ++x)
            {
                size_t idx = rowBase + (size_t)x * 3;
                if (idx + 2 >= total_size) {
                    img.data[(size_t)ir * img.width + x].b = 0;
                    img.data[(size_t)ir * img.width + x].g = 0;
                    img.data[(size_t)ir * img.width + x].r = 0;
                    img.data[(size_t)ir * img.width + x].a = 255;
                } else {
                    img.data[(size_t)ir * img.width + x].b = filebuf[idx + 0];
                    img.data[(size_t)ir * img.width + x].g = filebuf[idx + 1];
                    img.data[(size_t)ir * img.width + x].r = filebuf[idx + 2];
                    img.data[(size_t)ir * img.width + x].a = 255;
                }
            }
        }
    }

    free(filebuf);
    return img;
}

int SaveBMP(FILE *f, MyImage img)
{
    if (!f) return 1;

    int width = img.width;
    int height = img.height;
    int channels = img.channel;
    if (channels != 3 && channels != 4) return 1;

    int absHeight = (height >= 0) ? height : -height; 

    int bytesPerPixel = channels;
    int rowBytes = width * bytesPerPixel;
    int padding = 0;
    if (channels == 3)
        padding = (4 - (rowBytes % 4)) % 4;

    int rowSize = rowBytes + padding;
    unsigned int pixelDataSize = (unsigned int)rowSize * (unsigned int)absHeight;
    unsigned int headerSize = 54;
    unsigned int fileSize = headerSize + pixelDataSize;

    unsigned char header[54];
    memset(header, 0, 54);

    header[0] = 'B';
    header[1] = 'M';

    /* file size */
    header[2] = (unsigned char)(fileSize & 0xFF);
    header[3] = (unsigned char)((fileSize >> 8) & 0xFF);
    header[4] = (unsigned char)((fileSize >> 16) & 0xFF);
    header[5] = (unsigned char)((fileSize >> 24) & 0xFF);


    /* offset to pixel data = 54 */
    header[10] = (unsigned char)(headerSize & 0xFF);
    header[11] = (unsigned char)((headerSize >> 8) & 0xFF);
    header[12] = (unsigned char)((headerSize >> 16) & 0xFF);
    header[13] = (unsigned char)((headerSize >> 24) & 0xFF);

    /* DIB header size = 40 (BITMAPINFOHEADER) */
    header[14] = 40;
    header[15] = header[16] = header[17] = 0;

    /* width */
    header[18] = (unsigned char)(width & 0xFF);
    header[19] = (unsigned char)((width >> 8) & 0xFF);
    header[20] = (unsigned char)((width >> 16) & 0xFF);
    header[21] = (unsigned char)((width >> 24) & 0xFF);

    /* height: store positive to indicate bottom-up */
    header[22] = (unsigned char)(absHeight & 0xFF);
    header[23] = (unsigned char)((absHeight >> 8) & 0xFF);
    header[24] = (unsigned char)((absHeight >> 16) & 0xFF);
    header[25] = (unsigned char)((absHeight >> 24) & 0xFF);

    /* planes (2 bytes) = 1 */
    header[26] = 1;
    header[27] = 0;

    /* bits per pixel (2 bytes) */
    unsigned short bits = (unsigned short)(channels * 8);
    header[28] = (unsigned char)(bits & 0xFF);
    header[29] = (unsigned char)((bits >> 8) & 0xFF);

    /* compression (4 bytes) = 0 (BI_RGB) */
    header[30]=header[31]=header[32]=header[33]=0;

    /* image size (4 bytes) */
    header[34] = (unsigned char)(pixelDataSize & 0xFF);
    header[35] = (unsigned char)((pixelDataSize >> 8) & 0xFF);
    header[36] = (unsigned char)((pixelDataSize >> 16) & 0xFF);
    header[37] = (unsigned char)((pixelDataSize >> 24) & 0xFF);

    if (fwrite(header, 1, 54, f) != 54) {
        fclose(f);
        return 1;
    }

    unsigned char padbuf[3] = {0,0,0};

    /* write pixel rows bottom-up */
    for (int y = absHeight - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            Pixel px = img.data[y * width + x];
            /* write B G R */
            if (fwrite(&px.b, 1, 1, f) != 1) { fclose(f); return 1; }
            if (fwrite(&px.g, 1, 1, f) != 1) { fclose(f); return 1; }
            if (fwrite(&px.r, 1, 1, f) != 1) { fclose(f); return 1; }
            if (channels == 4)
            {
                if (fwrite(&px.a, 1, 1, f) != 1) { fclose(f); return 1; }
            }
        }

        if (channels == 3)
        {
            if (padding > 0)
            {
                if (fwrite(padbuf, 1, padding, f) != (size_t)padding) { fclose(f); return 1; }
            }
        }
    }

    fclose(f);
    return 0;
}

MyImage LoadImg(const char *filepath)
{
    MyImage img;
    img.width = img.height = img.channel = 0;
    img.data = NULL;
    img.Name[0] = '\0';

    FILE *f = fopen(filepath, "rb");
    if (!f)
    {
        fprintf(stderr, "File does not exist: %s\n", filepath);
        return img;
    }

    unsigned char magic[2];
    if (fread(magic, 1, 2, f) != 2) { fclose(f); return img; }
    rewind(f);

    if (magic[0] == 'B' && magic[1] == 'M')
    {
        img = LoadBMP(f);
    }
    else
    {
        fprintf(stderr, "Unsupported image format: %c%c\n", magic[0], magic[1]);
    }

    fclose(f);
    return img;
}

int SaveImg (MyImage img, const char *filepath)
{
    FILE *f = fopen(filepath, "wb");
    if (!f)
    {
        fprintf(stderr, "Cannot open file for writing: %s\n", filepath);
        return 1;
    }

    if (strcmp(img.Name, "BMP") == 0)
    {
       int a = SaveBMP(f, img);
       return a;
    }
    else
    {
       /* unsupported type */
       fprintf(stderr, "Unsupported image type for saving: %s\n", img.Name);
       fclose(f);
       return 1;
    }
}
