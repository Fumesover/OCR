#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>

#include "image.h"
#include "matrix.h"
#include "queue.h"
#include "../GUI/interface.h"

int fdp(int argc, char** argv)
{
    // TODO : Select file from interface file chooser
    /*char *path_image = "/home/rhaeven/Documents/OCR/scan1.png";
    char *enter = "", *tmp;
    enter = malloc(200*sizeof(char));
   
    int opt;
    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                path_image = optarg;
                break;
            default:
                break;
        }
    }

    printf("***Test du traitement de l'image *** \n"
           "Fichier actuel : %s,\n"
           "Appuyez sur entrer pour conserver ce chemin "
           "ou entrer nom du nouveau chemin :\n", path_image);
    enter = fgets(enter, 100, stdin);

    if (strcmp(enter, "\n") != 0)
        path_image = enter;


    if ((tmp=strchr(enter, '\n')) != NULL)
        *tmp = '\0';

    //DisplayImage(image);

	// Return the corresponding Pixel matrix of the image
	LoadImage(path_image);

    free(enter);
    return 0;*/

    //StartGUI(argc, argv);

}


