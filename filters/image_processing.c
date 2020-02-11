#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <math.h>
#include <string.h>


void array_copy(double a[], double b[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        b[i] = a[i];
    }
}

// TODO: Insert all the above functions.
void init_sdl(){
   	// Init only the video part.
   	// If it fails, die with an error message.
   	if(SDL_Init(SDL_INIT_VIDEO) == 1)
    	errx(1,"Could not initialize SDL: %s.\n", SDL_GetError()); 
}

SDL_Surface* load_image(char *path)
{
	SDL_Surface *img;

	// Load an image using SDL_image with format detection
	// If it fails, die with an error message.
	img = IMG_Load(path);
	if (!img)
		errx(3, "can't load %s: %s", path, IMG_GetError());

	return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void grayscale(SDL_Surface* image)
{
    int w = image->w;
    int h = image->h;
    Uint32 pixel;
    Uint8 r, g, b;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            float average = 0.3*r + 0.59*g + 0.11*b;

            pixel = SDL_MapRGB(image->format, average, average, average);
            put_pixel(image, x, y, pixel);
        }
    }
    display_image(image);
    //wait_for_keypressed();
}

float constrain(float value, float min, float max)
{
    if (value > max) return max;
    else if (value < min) return min;
    return value;
}

void brightness(SDL_Surface* image, float percentage)
{
    int w = image->w;
    int h = image->h;
    Uint32 pixel;
    Uint8 r, g, b;
    float newR, newG, newB;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            newR = r + percentage * 2.5;
            newG = g + percentage * 2.5;
            newB = b + percentage * 2.5;

            newR = constrain(newR, 0, 255);
            newG = constrain(newG, 0, 255);
            newB = constrain(newB, 0, 255);

            pixel = SDL_MapRGB(image->format, newR, newG, newB);
            put_pixel(image, x, y, pixel);
        }
    }
    display_image(image);
    //wait_for_keypressed();
}

void binarization(SDL_Surface* image)
{
    int w = image->w;
    int h = image->h;
    Uint32 pixel;
    Uint8 r, g, b;
    float newR, newG, newB;

    float treshold;
    float average = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            average += (r + g + b)/3;
        }
    }
    
    treshold = average/(w*h);
    
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            average = (r + g + b)/3;
            if (average < treshold)
            {
                newR = 0;
                newG = 0;
                newB = 0;
            }
            else
            {
                newR = 255;
                newG = 255;
                newB = 255;
            }

            pixel = SDL_MapRGB(image->format, newR, newG, newB);
            put_pixel(image, x, y, pixel);
        }
    }
    display_image(image);
    //wait_for_keypressed();
}

void convolutionFilter(SDL_Surface* image, Uint8 filter)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface* newImage = image;

    
    double gaussian_blur[9] = {
    0.0625,0.125,0.0625,
    0.125,0.25,0.125,
    0.0625,0.125,0.0625
    };

    double edge_detection[9] = {
        -1,-1,-1,
        -1, 8,-1,
        -1,-1,-1
    };

    double contrast[9] = {
         0,-1, 0,
        -1, 5,-1,
         0,-1, 0
    };

    double relief[9] = {
        -2,-1, 0,
        -1, 1, 1,
         0, 1, 2
    };

    double kernel[9] = {
        0,0,0,
        0,0,0,
        0,0,0
    };

    if (filter == 0) 
        array_copy(contrast, kernel, 9);
    else if (filter == 1) 
        array_copy(gaussian_blur, kernel, 9);
    else if (filter == 2)
        array_copy(relief, kernel, 9);
    else
        array_copy(edge_detection, kernel, 9);
    

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (x > 0 && x < w - 1 && y > 0 && y < h - 1){
                Uint32 pixels[9] = {
                get_pixel(newImage,x-1,y-1),get_pixel(newImage,x,y-1),get_pixel(newImage,x+1,y-1),
                get_pixel(newImage,x-1,y  ),get_pixel(newImage,x,y  ),get_pixel(newImage,x+1,y  ),
                get_pixel(newImage,x-1,y+1),get_pixel(newImage,x,y+1),get_pixel(newImage,x+1,y+1)
                };
    
                float newR = 0;
                float newG = 0;
                float newB = 0;
                
                for(size_t i = 0; i < 9; i++){
                    Uint8 r,g,b;
                    SDL_GetRGB(pixels[i], newImage->format, &r,&g,&b);
                    newR += kernel[i] * r;
                    newG += kernel[i] * g;
                    newB += kernel[i] * b;
                }

                newR = constrain(newR, 0, 255);
                newG = constrain(newG, 0, 255);
                newB = constrain(newB, 0, 255);

                Uint32 pixel = SDL_MapRGB(image->format, newR, newG, newB);
                put_pixel(image,x,y,pixel);
            }
        }
    }
    display_image(image);
    //wait_for_keypressed();
}
























