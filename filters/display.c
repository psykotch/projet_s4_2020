#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "image_processing.c"
#include <string.h>

void action(int x, SDL_Surface* image_surface)
{
    if (x > -1 && x < 4)
    {
        convolutionFilter(image_surface, x);
        display_image(image_surface);
    }
    else if (x == 4)
    {
        printf("Brightness (-100,100):\n");
        float percent;
        scanf("%f", &percent);
        brightness(image_surface, percent);
        display_image(image_surface);
    }
    else if (x == 5)
    {
        printf("Grayscale:\n");
        grayscale(image_surface);
        display_image(image_surface);
    } 
    else if (x == 6)
    {
        printf("Binarization:\n");
        binarization(image_surface);
        display_image(image_surface);
    }
}

void menu()
{
    char file[30];
    int exit = 1;
    char filters[7][14] = {
        "CONTRAST",
        "GAUSSIAN BLUR",
        "RELIEF",
        "EDGE DETECTION",
        "BRIGHTNESS",
        "GRAYSCALE",
        "BINARIZATION"
    };
    SDL_Surface* image_surface = load_image("my_image.jpg");
    SDL_Surface* screen_surface = display_image(image_surface);

    system("clear");
    printf("***PHOTOSHOP*** \n");
    printf("\n");
    printf("Choose a file: ");
    char txt[] = "";
    scanf("%s",&txt);
    strcpy(file, txt);
    printf("%s", file);
    image_surface = load_image(file);
    display_image(image_surface);


    do
    {
        system("clear"); 

        printf("***PHOTOSHOP*** \n");
        printf("\n");

        for (int i = 0; i < 7; i++)
            printf("| %d | %s\n", i, filters[i]);

        printf("\n");

        init_sdl();

        int choice;
        printf("choice: ");
        scanf("%d",&choice);
                
        action(choice, image_surface);

        system("clear");

        printf("| 0 | CONTINUE \n| 1 | RESET \n| 2 | OPEN FILE\n| 3 | SAVE \n| 4 | QUIT\n\n");
        printf("choice: ");
        scanf("%d",&exit);   
        printf("\n");

        if (exit == 1)
        {
            image_surface = load_image(file);
            display_image(image_surface); 
        }

        if (exit == 2)
        {
            system("clear");
            printf("*** PHOTOEDIT *** \n");
            printf("\n");
            printf("*** CHOOSE FILE ***\n\n");
            printf("File: ");

            char txt[] = "";
            scanf("%s",&txt);
            strcpy(file, txt);
            printf("%s", file);
            image_surface = load_image(file);
            display_image(image_surface);
        }

        if (exit == 3)
        {
            system("clear");
            printf("*** PHOTOEDIT *** \n");
            printf("*** SAVE FILE ***\n\n");
            SDL_SaveBMP(image_surface, "out.bmp");
            printf("File Saved.");
            exit = 0;
        }

    } while (exit != 4);
    
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

}

int main()
{

    menu();
    return 0;
}
