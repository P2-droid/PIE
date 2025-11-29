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
    printf("9. Invert Contrast\n");
    printf("10. Exit\n");
    printf("==============================================\n");
    printf("Choose option: ");
}

int main() {
    MyImage img;  
    int loaded = 0;   // flag: do we have an image loaded?
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
                loaded = 1;
                printf("Image loaded successfully!\n");
                break;
            }

            case 2: 
            {
                if (!loaded) 
                {
                    printf("No image loaded!\n");
                    break;
                }
                char filename[100];
                printf("Enter filename to save as: ");
                scanf("%s", filename);

                SaveImg(img, filename);
                printf("Image saved successfully!\n");
                break;
            }

            case 3: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); 
                    break; 
                }

                int x1, y1, x2, y2;
                printf("Enter x1 y1 x2 y2 for crop: ");
                scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

                img = Crop(img, x1, x2, y1, y2);
                printf("Image cropped!\n");
                break;
            }

            case 4: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); break; 
                }

                RotateClock(img);
                printf("Image rotated!\n");
                break;
            }

            case 5: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); break; 
                }

                RotateAntiClock(img);
                printf("Image rotated!\n");
                break;
            }

            case 6: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); 
                    break; 
                }

                int value;
                printf("occupacity amount (0 to 255): ");
                scanf("%d", &value);

                img = Occupacity(img, value);
                printf("Occupacity changed!\n");
                break;
            }

            case 7: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); 
                    break; 
                }

                img = FlipY(img);
                printf("Flipped horizontally!\n");
                break;
            }

            case 8: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); 
                    break; 
                }

                img = FlipX(img);
                printf("Flipped vertically!\n");
                break;
            }

            case 9: 
            {
                if (!loaded) 
                { 
                    printf("Load image first!\n"); 
                    break;
                }

                img = InvertColor(img);
                printf("Inverted Contrast\n");
                break;
            }

            default:
            {
                printf("Invalid option!\n");
            }
        }
    }

    return 0;
}
