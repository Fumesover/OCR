#ifndef IMAGE_H
#define IMAGE_H

typedef struct Pixel Pixel;
struct Pixel {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

/* SDL */
void DisplayImage(SDL_Surface *image);
void WaitForKeypressed();
Uint32 GetPixel(SDL_Surface *surface, int x, int y);
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/* MATRIX TREATMENT */
void LoadImage(char *path);


/* MATRIX CONVERSION */
void SurfaceToMatrix(Pixel **pixels, SDL_Surface *image, int h, int w);
SDL_Surface *MatrixToSurface(Pixel **pixels, int h, int w);
void BinToPixels(int **matrix, Pixel **pixels, int h, int w);

/* BINARIZATION */
void GreyScale(Pixel **pixels, int h, int w);
int Otsu(Pixel **pixels, int h, int w);
void Binarization(Pixel **pixels, int h, int w, int threshold);
void BinarizeMatrix(Pixel **pixels, int **binarized, int h, int w);
void DebinarizeMatrix(int **binarized, Pixel **pixels, int h, int w);

#endif
