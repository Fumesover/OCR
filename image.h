#ifndef IMAGE_H
#define IMAGE_H

typedef struct Pixel Pixel;
struct Pixel {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

void DisplayImage (char path_image[]);
void LoadImage(char * path_image);
void FillPixels(Pixel **pixels, SDL_Surface *image, int h, int w);
Uint32 GetPixel(SDL_Surface *surface, int x, int y);
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif
