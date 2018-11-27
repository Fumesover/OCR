#include <stdio.h>
#include "neuralNetwork/neuralNet.h"
#include <stdlib.h>
#include <stdio.h>
//#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>

#include "imageTreatment/image.h"
#include "imageTreatment/matrix.h"
#include "imageTreatment/queue.h"
#include "imageTreatment/segmentation.h"
#include "imageTreatment/matrix.h"

/*
void neuralnet() {
    // INIT
    int   nbInp     = 2;
    float inpVals[] = {1, 1};
    int   nbHidden  = 1;
    int   hidden[]  = {3};
    int   nbOut     = 1;
    float target[]  = {0};
    char* filepath  = "xor.nn";

    float updateRate = 1.0f;

    neuNet* n = NNinit(nbInp, nbHidden, hidden, nbOut);

    if (!n) // TODO: Better errors
        return;

    // RANDOMISE
    NNrand(n);

    float err;
    int itteration = 0;
    float tests[4][3] = {
        {1.0f,1.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {1.0f,0.0f,1.0f},
        {0.0f,1.0f,1.0f},
    };
    int testPos = 0;
    int testLen = 4;

    do {
        err = 0.0f;
        for (testPos = 0; testPos < testLen; testPos++) {
            inpVals[0] = tests[testPos][0];
            inpVals[1] = tests[testPos][1];
            target[0]  = tests[testPos][2];

            float errthis = NNTrain(n, inpVals, target, updateRate);
            err += errthis;
            
            itteration++;
            printf("  n°%d", itteration++);
            printf(" => error : %f", errthis);
            printf(" vals : %1.f xor %1.f", inpVals[0], inpVals[1]);
            printf(" => %1.f (%f)\n", tests[testPos][2], n->neuOutput[0]);
        }

        printf("n°%d : batch error : ", itteration / testLen);
        printf("%f -- update rate : %f\n", err, updateRate);
    } while (err > 0.01f);

    printf("\n========\n\n");

    printf("Saving neural network to %s\n", filepath);
    NNsave(n, filepath);
    NNfree(n);

    printf("Loading file %s\n", filepath);
    neuNet* loaded = NNload(filepath);

    for (int i = 0; i < testLen; i++) {
        inpVals[0] = tests[i][0];
        inpVals[1] = tests[i][1];

        float* result = NNGuess(loaded, inpVals);
        printf(" %1.f XOR %1.f => %f\n", inpVals[0], inpVals[1], result[0]);
    }

    NNfree(loaded);
    return;
}
 */

void image() {
    int val = 0;
    
    SDL_Surface *image = NULL;
    
    int h = 0, w = 0;
    Pixel **pixels = NULL;
    int **matrix = NULL;

    while (val != 9) {

        printf("Image treatment:\n");
        printf("\t1-> set path\n");
        printf("\t2-> show image\n");
        printf("\t3-> run otsu & binarisation\n");
        printf("\t4-> show segmentation\n");
        printf("\t5-> resize image\n");

        printf("\t9-> exit\n");
        
        printf("\n> ");
        scanf("%d", &val);

        switch (val) {
            case 1:         
                printf("Path to picture: ");
                char buffer[100];
                scanf("%99s", buffer);

                image = IMG_Load(buffer);
                
                if (image == NULL) {
                    printf("Couldn't load %s: %s\n", buffer, SDL_GetError());
                } else {
                    printf("%s successfully loaded\n", buffer);
                    h = image->h;
                    w = image->w;
                
                    pixels = InitPixelMatrix(h, w);
                    matrix = InitIntMatrix(h, w);

                    SurfaceToMatrix(pixels, image, h, w);
                }
                break;
            case 2:
                if (image != NULL) {
                    DisplayImage(MatrixToSurface(pixels, h, w));
                }
                break;
            case 3:
                if (image != NULL) {
                    GreyScale(pixels, h, w);
                    int threshold = Otsu(pixels, h, w);
                    Binarization(pixels, h, w, threshold);
                    BinarizeMatrix(pixels, matrix, h, w);
                }
                break;
            case 4:
                if (image != NULL) 
                    Segmentation(matrix, h, w);
                
                break;
            case 5:
                if (image != NULL) {
                    int t = h > w ? h : w;
                    int** square = SquareMatrix(matrix, h, w);

                    Pixel** pix = InitPixelMatrix(t, t);

                    BinToPixels(square, pix, t, t);
                    SDL_Surface *n = MatrixToSurface(pix, t, t);
                    SDL_Surface *res = ResizeMatrix(n, 150);
                    DisplayImage(res);
                }
                break;
            }
    }
    
    FreeMatrix(matrix, h, w);
    FreeMatrix(pixels, h, w);
    /*
    if (matrix != NULL && pixels != NULL) {
        for (int j = 0; j < h; j++) {
            free(pixels[j]);
            free(matrix[j]);
        }
    }

    free(pixels);
    free(matrix);
     */
}

int main() {
    
    int val = 0;
    while (val != 3) {
        printf("\nExOCRists\n");
        printf("\t1-> neural network\n");
        printf("\t2-> image treatment\n");
        printf("\t3-> exit\n");
    
        printf("\n> ");
        scanf("%d", &val);
        
        switch (val) {
            case 1:
                //neuralnet();
                break;
            case 2:
                image();
                break;
        }
    }

    return 0;
}
