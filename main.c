/*
 * Project: AquaEngine
 * Author: Abdelkader Kantaoui
 * License: GNU General Public License v3.0
 * Documentation: https://perso.limos.fr/~abkantaoui/MyWork/FishEngine/index.html
 * Portfolio: https://perso.limos.fr/~abkantaoui/
 */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aquaengine.h"
#include "aquaSDL.h"

int WINDOW[2] = {1000, 700};

#ifdef GRAPHIC_MODE_FOR_THE_ENGINE

bool low_debugmode = false;
bool rotational_debugmode = false;
char *where_fish = "assets/animales2.png";
char *where_background = "assets/cx3t_mc71_230315.jpg";
char *where_predateur = "assets/Whale.png";
SDL_Texture *spriteSheet_Fish = NULL;
SDL_Texture *spriteSheet_Predator = NULL;
#endif

int main() {
    srand(time(NULL));


    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    SDL_Renderer* renderer;
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Créer la fenêtre
    SDL_Window* window = SDL_CreateWindow("Fish Engineoo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW[pX], WINDOW[pY], SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Créer le renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur lors de la création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }



    // Charger le sprite de poisson

    SDL_Surface* spriteSurface = IMG_Load(where_fish);//"poisson.png"
    SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_Surface* spriteSurfacePredator = IMG_Load(where_predateur);
    SDL_Texture* spriteTexturePredator = SDL_CreateTextureFromSurface(renderer, spriteSurfacePredator);

    if(!low_debugmode)
    {
        
        if (spriteSurface == NULL) {
            fprintf(stderr, "Erreur lors du chargement du sprite de poisson : %s\n |--> debugMode : ON\n", IMG_GetError());
            low_debugmode = true;
        }

        // Créer la texture du sprit// fmax(peace_time[0] , peace_time[1])*M_PI;e de poisson

        
        if (spriteTexture == NULL) {
            fprintf(stderr, "Erreur lors de la création de la texture du sprite de poisson : %s\n |--> debugMode : ON\n", SDL_GetError());
            SDL_FreeSurface(spriteSurface);
            low_debugmode = true;
        }
        spriteSheet_Fish = spriteTexture;


        // Charger le sprite de shark
        
        if (spriteSurface == NULL) {
            fprintf(stderr, "Erreur lors du chargement du sprite de poisson : %s\n |--> debugMode : ON\n", IMG_GetError());
            low_debugmode = true;
        }

        // Créer la texture du sprite de shark

        
        if (spriteTexture == NULL) {
            fprintf(stderr, "Erreur lors de la création de la texture du sprite de poisson : %s\n |--> debugMode : ON\n", SDL_GetError());
            SDL_FreeSurface(spriteSurface);
            low_debugmode = true;
        }
        spriteSheet_Predator = spriteTexturePredator;
    }
    SDL_Surface* surface_background = IMG_Load(where_background);
    if (surface_background == NULL) {
        fprintf(stderr, "surface_background : %s\n--> No background will be loaded\n", IMG_GetError());
        
    }


    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface_background);
    if (backgroundTexture == NULL) {
        fprintf(stderr, "backgroundTexture : %s\n--> No background will be loaded\n", SDL_GetError());
        
    }

    SDL_Event event;
    #else
    void* renderer = NULL;
    #endif


    Cluster *cluster = malloc(sizeof(Cluster));
    Creature *predator = malloc(2 * sizeof(Creature));

    if (cluster == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire pour le cluster\n");
        #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        #endif
        return 1;
    }

    Initialize_Predator(predator ,NULL);
    Initialize_Predator(predator +1 ,predator);
    
    
    Initialize_Cluster(cluster , predator);
   

    bool quit = false;
    while (!quit) 
    {
        #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        // Effacer l'écran
        SDL_RenderClear(renderer);
        if(!low_debugmode && backgroundTexture) SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        #endif

        if(INITIAL_FISH_NUMBER) AnimateFish(cluster ,renderer ,predator);
        AnimatePredator(cluster ,renderer ,predator);
        
        #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
        SDL_RenderPresent(renderer);
        #endif
        
    }


    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(surface_background);
    SDL_Quit();
    #endif

    Free_Cluster(&cluster);


    return 0;
}



