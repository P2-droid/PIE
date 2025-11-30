#include <stdio.h>
#include <stdlib.h>
#include "Image.h"
#include "tools.h" 

void PrintMenu() {
    printf("\n================ IMAGE EDITOR ================\n");
    printf("1. Load Image\n");
    printf("2. Save Image\n");
    printf("3. Crop Image\n");
    printf("4. Rotate Image Clockwise\n");
    printf("5. Rotate Image Anti Clockwise\n");
    printf("6. Adjust Occupacity\n");
    printf("7. Flip Horizontal\n");
    printf("8. Flip Vertical\n");
    printf("9. Invert Colors\n");
    printf("10. Exit\n");
    printf("==============================================\n");
    printf("Choose option: ");
}

int main() {
    MyImage img;
    int loaded = 0;
    int choice;

    while (1)
    {
        PrintMenu();
        scanf("%d", &choice);

        if (choice == 10)
        {
            printf("Exiting editor...\n");
            break;
        }

        switch (choice)
        {
            case 1:
            {
                char filename[100];
                printf("Enter image filename to load: ");
                scanf("%s", filename);

                img = LoadImg(filename);
                if (img.data == NULL)
                {
                    printf("Failed to load image!\n");
                    loaded = 0;
                }
                else
                {
                    loaded = 1;
                    printf("Image loaded successfully!\n");
                }
                break;
            }

            case 2:
            {
                if (!loaded) { printf("No image loaded!\n"); break; }

                char filename[100];
                printf("Enter filename to save as: ");
                scanf("%s", filename);

                SaveImg(img, filename);
                printf("Image saved successfully!\n");
                break;
            }

            case 3:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                int x1, y1, x2, y2;
                printf("Enter x1 y1 x2 y2 for crop: ");
                scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

                img = Crop(img, x1, y1, x2, y2); 
                printf("Image cropped!\n");
                break;
            }

            case 4:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                img = RotateClock(img); 
                printf("Image rotated clockwise!\n");
                break;
            }

            case 5:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                img = RotateAntiClock(img); 
                printf("Image rotated anti clockwise!\n");
                break;
            }

            case 6:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                int value;
                printf("Occupacity amount (0 to 255): ");
                scanf("%d", &value);

                img = Occupacity(img, value);
                printf("Occupacity changed!\n");
                break;
            }

            case 7:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                img = FlipY(img);
                printf("Flipped horizontally!\n");
                break;
            }

            case 8:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                img = FlipX(img);
                printf("Flipped vertically!\n");
                break;
            }

            case 9:
            {
                if (!loaded) { printf("Load image first!\n"); break; }

                img = InvertColor(img);
                printf("Colors inverted!\n");
                break;
            }

            default:
                printf("Invalid option!\n");
        }
    }

    if (loaded)
        free(img.data);  
    return 0;
}
