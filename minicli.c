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

#include "neuralNetwork/fromqueue.h"

void image() {
    int val = 0;
    
    SDL_Surface *image = NULL;
    
    int h = 0, w = 0;
    Pixel **pixels = NULL;
    int **matrix = NULL;

    Queue *queue;

    while (val != 9) {

        printf("Image treatment:\n");
        printf("\t1-> set path\n");
        printf("\t2-> show image\n");
        printf("\t3-> run otsu & binarisation\n");
        printf("\t4-> show segmentation\n");
        printf("\t5-> automatic rotation\n");
        printf("\t6-> run text recognotion\n");
        printf("\t7-> add values to dataset using image\n");

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
					Median_Filter(pixels, h, w);
                    int threshold = Otsu(pixels, h, w);
                    OtsuBinarization(pixels, h, w, threshold);
                    BinarizeMatrix(pixels, matrix, h, w);
                }
                break;
            case 4:
                if (image != NULL) 
                    queue =  Segmentation(matrix, h, w);
                    ShowSegmentation();
                break;
            case 5:
                if (image != NULL) {
                }
                break;/*
            case 6:
                if (queue == NULL)
                    break;

                char* s = extractstring("test.nn", queue);
                
                printf("String is : \"%s\"\n", s);
                free(s);
                break;       
            case 7:
                if (queue == NULL)
                    break;

                adddatatoset(queue);*/
            }
    }
    
    FreeMatrix((void**)matrix, h);
    FreeMatrix((void**)pixels, h);
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
