#ifndef IMAGE_H
#define IMAGE_H

void DisplayImage (char path_image[]);
void LoadImage(char path_image[], Uint32 **pixels);
Uint32 GetPixel(SDL_Surface *surface, int x, int y);
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif
