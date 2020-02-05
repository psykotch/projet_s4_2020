#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

// TODO: Insert all the above functions.

int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // TODO: Initialize the SDL

    image_surface = load_image("my_image.jpg");
    // TODO: Display the image.

    // TODO: Wait for a key to be pressed.

    // TODO: Free the image surface.
    // TODO: Free the screen surface.

    return 0;
}
