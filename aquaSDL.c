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


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>
#include "aquaengine.h"
#include "aquaSDL.h"


#ifdef GRAPHIC_MODE_FOR_THE_ENGINE
void drawRotatedRect(SDL_Renderer* renderer, double x, double y, double width, double height, double angle) {
    double radians = angle * (M_PI / 180.0);
    double centerX = x + width / 2;
    double centerY = y + height / 2;

    // Calculate the four corner points of the rotated rectangle
    double x1 = centerX + (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y1 = centerY + (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x2 = centerX - (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y2 = centerY - (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x3 = centerX - (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y3 = centerY - (width / 2) * sin(radians) - (height / 2) * cos(radians);

    double x4 = centerX + (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y4 = centerY + (width / 2) * sin(radians) - (height / 2) * cos(radians);

    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
    SDL_RenderDrawLine(renderer, (int)x2, (int)y2, (int)x3, (int)y3);
    SDL_RenderDrawLine(renderer, (int)x3, (int)y3, (int)x4, (int)y4);
    SDL_RenderDrawLine(renderer, (int)x4, (int)y4, (int)x1, (int)y1);
}

int* getMaxDims(double width, double height, double angle) {
    double radians = -angle * (M_PI / 180.0);
    // Calculate the four corner points of the rotated rectangle
    double x1 = + (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y1 = + (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x2 = - (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y2 = - (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x3 = - (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y3 = - (width / 2) * sin(radians) - (height / 2) * cos(radians);

    double x4 = + (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y4 = + (width / 2) * sin(radians) - (height / 2) * cos(radians);

    int* Return_max = malloc(2*sizeof(int));
    Return_max[0] = fmax(fmax(x1,x2),fmax(x3,x4));
    Return_max[1] = fmax(fmax(y1,y2),fmax(y3,y4));
    return Return_max;
}
void DrawObject_In_debugmode(SDL_Renderer *renderer ,SDL_Rect MainRect,double *Center)
{
    int MirrorX = MainRect.x ;
    if(MirrorX < 0) MirrorX = WINDOW[pX] - abs(MirrorX);
    else if(MirrorX >= WINDOW[pX] - MainRect.w) MirrorX -= WINDOW[pX];

    int MirrorY = MainRect.y ;
    if(MirrorY < 0) MirrorY = WINDOW[pY] - abs(MirrorY);
    else if(MirrorY >= WINDOW[pY] - MainRect.h) MirrorY -= WINDOW[pY];

    SDL_Rect Mirror_rect;
    Mirror_rect.x = 0;
    Mirror_rect.y = 0;
    Mirror_rect.w = MainRect.w;
    Mirror_rect.h = MainRect.h;

    if(MirrorX != MainRect.x && MirrorY != MainRect.y)
    {
        Mirror_rect.x = MirrorX;
        Mirror_rect.y = MirrorY;
        SDL_RenderFillRect(renderer, &Mirror_rect);
    }
    if(MirrorX != MainRect.x)
    {
        Mirror_rect.x = MirrorX;
        Mirror_rect.y = MainRect.y;
        SDL_RenderFillRect(renderer, &Mirror_rect);
    }
    if(MirrorY != MainRect.y)
    {
        Mirror_rect.x = MainRect.x;
        Mirror_rect.y = MirrorY;
        SDL_RenderFillRect(renderer, &Mirror_rect);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, (int)Center[pX],(int) Center[pY],
                    (int) (MainRect.x + MainRect.w/2), 
                    (int) (MainRect.y + MainRect.h/2));
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void DrawObject_In_Normal(SDL_Renderer *renderer ,SDL_Rect *rect,SDL_Rect *fictif_rect,double angle,SDL_Texture* spriteSheet_type,SDL_Rect SpirteRect)
{
    SDL_RenderCopyEx(renderer, spriteSheet_type, 
                &SpirteRect, 
                rect, 
                angle, NULL, SDL_FLIP_NONE);

    if(!fictif_rect) fictif_rect = rect;

    int MirrorX = (*rect).x ;
    int MirrorY = (*rect).y ;

    int fictifMirrorX = (*fictif_rect).x ;
    if(fictifMirrorX < 0) 
    {
        fictifMirrorX = WINDOW[pX] - abs(fictifMirrorX);
        MirrorX += WINDOW[pX];
    }
    else if(fictifMirrorX >= WINDOW[pX] - (*fictif_rect).w) 
    {
        fictifMirrorX -= WINDOW[pX];
        MirrorX -= WINDOW[pX];
    }

    int fictifMirrorY = (*fictif_rect).y ;
    if(fictifMirrorY < 0) 
    {
        fictifMirrorY = WINDOW[pY] - abs(fictifMirrorY);
        MirrorY += WINDOW[pY];
    }
    else if(fictifMirrorY >= WINDOW[pY] - (*fictif_rect).h) 
    {
        fictifMirrorY -= WINDOW[pY];
        MirrorY -= WINDOW[pY];
    }
    
    SDL_Rect Mirror_rect;
    Mirror_rect.x = 0;
    Mirror_rect.y = 0;
    Mirror_rect.w = (*rect).w;
    Mirror_rect.h = (*rect).h;



    if(fictifMirrorX != (*fictif_rect).x && fictifMirrorY != (*fictif_rect).y)
    {
        Mirror_rect.x = MirrorX;
        Mirror_rect.y = MirrorY;
        SDL_RenderCopyEx(renderer, spriteSheet_type, 
                &SpirteRect, 
                &Mirror_rect, 
                angle, NULL, SDL_FLIP_NONE);
        drawOutlineRectangles(renderer,angle, &Mirror_rect, NULL);
    }
    if(fictifMirrorX != (*fictif_rect).x)
    {
        Mirror_rect.x = MirrorX;
        Mirror_rect.y = (*rect).y;
        SDL_RenderCopyEx(renderer, spriteSheet_type, 
                &SpirteRect, 
                &Mirror_rect, 
                angle, NULL, SDL_FLIP_NONE);
        drawOutlineRectangles(renderer,angle, &Mirror_rect, NULL);
    }
    if(fictifMirrorY != (*fictif_rect).y)
    {
        Mirror_rect.x = (*rect).x;
        Mirror_rect.y = MirrorY;
        SDL_RenderCopyEx(renderer, spriteSheet_type, 
                &SpirteRect, 
                &Mirror_rect, 
                angle, NULL, SDL_FLIP_NONE);
        drawOutlineRectangles(renderer,angle, &Mirror_rect, NULL);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
void drawOutlineRectangles(SDL_Renderer *renderer ,double alpha, SDL_Rect * rect , SDL_Rect ** Returned_fictif_rect)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    
    int* Max_dims = getMaxDims((*rect).w,(*rect).h,alpha);
    
    if(rotational_debugmode)
            drawRotatedRect(renderer, (*rect).x + (*rect).w/2 - Max_dims[0], 
                        (*rect).y + (*rect).h/2 - Max_dims[1] , 2*Max_dims[0], 2*Max_dims[1], 0);

    SDL_Rect *fictif_Rect = malloc(sizeof(SDL_Rect));
    
    fictif_Rect->x = (*rect).x + (*rect).w/2 - Max_dims[0];
    fictif_Rect->y = (*rect).y + (*rect).h/2 - Max_dims[1];
    fictif_Rect->w = 2*Max_dims[0];
    fictif_Rect->h = 2*Max_dims[1];
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    if(rotational_debugmode) drawRotatedRect(renderer, (*rect).x , (*rect).y , (*rect).w, (*rect).h, alpha);
    if(Max_dims)free(Max_dims);

    if(Returned_fictif_rect) *Returned_fictif_rect = fictif_Rect;
    else if(fictif_Rect) free(fictif_Rect);
    
    
}
#endif


void AnimatePredator(Cluster *cluster ,void *renderer , Creature *predator)
{   
    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    SDL_Rect mainRect[2] = {
        {predator[0].core.pos[pX],predator[0].core.pos[pY],predator[0].core.dim[pX],predator[0].core.dim[pY]},
        {predator[1].core.pos[pX],predator[1].core.pos[pY],predator[1].core.dim[pX],predator[1].core.dim[pY]}
    };
    SDL_Rect ** fictif_Rect = malloc(2* sizeof(SDL_Rect *));


    for(int i=0;i<2;i++)
    {
        fictif_Rect[i] = NULL;
        #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
        drawOutlineRectangles(renderer,predator[i].anime.Srotation, &mainRect[i], fictif_Rect + i);
        #endif
    }

    if(!low_debugmode)
    {
        for(int i=0;i<2;i++)
        {
            DrawObject_In_Normal(renderer, mainRect+ i ,fictif_Rect[i],predator[i].anime.Srotation, spriteSheet_Predator,predator[i].anime.spriteClipsPredator[ predator[i].anime.current ]);
            predator[i].anime.current =(predator[i].anime.current + 1) %10;
        }
        
    }
    else{
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &mainRect[0]);
        SDL_RenderFillRect(renderer, &mainRect[1]);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);



        SDL_SetRenderDrawColor(renderer,4, 255, 90, 255);
        DrawObject_In_debugmode(renderer,mainRect[0],predator[0].core.gravityCenter);
        SDL_SetRenderDrawColor(renderer,4, 255, 90, 255);
        DrawObject_In_debugmode(renderer,mainRect[1],predator[1].core.gravityCenter);
        SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
    }
    for(int i=0;i<2;i++)
    {
        if(fictif_Rect[i]) free(fictif_Rect[i]);
    }
    if(fictif_Rect) free(fictif_Rect);

    #endif
    mv_object(cluster ,predator ,predator , true);
    mv_object(cluster ,predator+1 ,predator , true);

    
    

}

void AnimateFish(Cluster *cluster ,void *renderer , Creature *predator)
{
    for(Cluster * tmp_cluster = cluster; tmp_cluster ; tmp_cluster = tmp_cluster->Next) 
    {   
        for(int i = 0;i<THREADS_NORM && !IsRectEmpty(tmp_cluster->fish[i].core);i++)
        {
            #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
            SDL_Rect mainRect = {tmp_cluster->fish[i].core.pos[pX],tmp_cluster->fish[i].core.pos[pY],tmp_cluster->fish[i].core.dim[pX],tmp_cluster->fish[i].core.dim[pY]};

            SDL_Rect * fictif_Rect = NULL;
            
            drawOutlineRectangles(renderer,tmp_cluster->fish[i].anime.Srotation, &mainRect, &fictif_Rect);
            if(!low_debugmode)
            {
                DrawObject_In_Normal(renderer,&mainRect,fictif_Rect, tmp_cluster->fish[i].anime.Srotation, spriteSheet_Fish,
                                    tmp_cluster->fish[i].anime.spriteClips[tmp_cluster->fish[i].anime.fish_type][tmp_cluster->fish[i].anime.current]);

                tmp_cluster->fish[i].anime.current = (tmp_cluster->fish[i].anime.current + 1) %3;
            }
            else
            {
                
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                SDL_RenderFillRect(renderer, &mainRect);


                SDL_SetRenderDrawColor(renderer,4, 255, 2, 255);
                DrawObject_In_debugmode(renderer,mainRect,tmp_cluster->fish[i].core.gravityCenter);
            }
            if(fictif_Rect) free(fictif_Rect);

            SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
            #endif
            
            mv_object(cluster ,&(tmp_cluster->fish[i]),predator , false);

        }
    }
    
}