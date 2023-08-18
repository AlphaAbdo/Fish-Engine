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

void Initialize_fish(Creature* fishy ,Creature * predator)
{
    double * Xx[2] = {NULL,NULL};
    double * Yy[2] = {NULL,NULL};

    fishy->core.dim[pX] = FishHeadBoxW;
    fishy->core.dim[pY] = FishHeadBoxH;

    fishy->core.pos[pX] = rand() % (int)(WINDOW[pX] - fishy->core.dim[pX]);
    fishy->core.pos[pY] = rand() % (int)(WINDOW[pY] - fishy->core.dim[pY]);
    
    fishy->core.gravityCenter[pX] = 0;
    fishy->core.gravityCenter[pY] = 0;
    
    fishy->rotation = 0;

    fishy->rotation = atan2(rand(), rand());

    fishy->velocitY = INITIAL_FISH_VELOCITY ;
    

    while((Xx[0] && Xx[1] && Yy[0] && Yy[1] )? 
            (   pow(Xx[0][0] - Xx[0][1],2) + pow(Yy[0][0] - Yy[0][1],2) <= pow(SAFERADIUS,2) ||
                pow(Xx[1][0] - Xx[1][1],2) + pow(Yy[1][0] - Yy[1][1],2) <= pow(SAFERADIUS,2)) : true)
    {
        fishy->core.pos[pX] = rand() % (int)(WINDOW[pX] - fishy->core.dim[pX]);
        fishy->core.pos[pY] = rand() % (int)(WINDOW[pY] - fishy->core.dim[pY]);
        for(int i=0;i<2;i++)
        {
            Xx[i] = getFictifCoordinates(fishy->core.pos[pX] + fishy->core.dim[pX]/2 , predator[i].core.pos[pX] + predator[i].core.dim[pX]/2, WINDOW[pX]);
            Yy[i] = getFictifCoordinates(fishy->core.pos[pY] + fishy->core.dim[pY]/2, predator[i].core.pos[pY] + predator[i].core.dim[pY]/2, WINDOW[pY]);
        }
    }
    
    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    double dice =rand()%3;

    if(dice < 1) fishy->anime.fish_type = 0;
    else if(dice < 2) fishy->anime.fish_type = 1;
    else fishy->anime.fish_type = 6;



    fishy->anime.current = 0;
    fishy->anime.Srotation = 0;
    for (int i = 0; i < 3; ++i) 
    {
        fishy->anime.spriteClips[fishy->anime.fish_type][i].x = (3 * (fishy->anime.fish_type %4 ) +i) * FSPRITE_WIDTH;
        fishy->anime.spriteClips[fishy->anime.fish_type][i].y = (4 * (fishy->anime.fish_type /4)) * FSPRITE_HEIGHT;
        fishy->anime.spriteClips[fishy->anime.fish_type][i].w = FSPRITE_WIDTH;
        fishy->anime.spriteClips[fishy->anime.fish_type][i].h = FSPRITE_HEIGHT;
            
    }
    #endif
}
void Eaten(Creature *creature)
{
    // creature->core.dim[pY] = 0;
    // creature->core.dim[pX] = 0;
}

void Initialize_Predator(Creature* predator , Creature* TheOtherPredator)
{
    predator->core.dim[pX] = PredatorHeadBoxW;
    predator->core.dim[pY] = PredatorHeadBoxH;

    predator->core.pos[pX] = 0;
    predator->core.pos[pY] = 0;
    
    predator->core.gravityCenter[pX] = PredatorHeadBoxW;
    predator->core.gravityCenter[pY] = PredatorHeadBoxH;

    
    predator->rotation = 0;

    predator->rotation = atan2(rand(), rand());

    predator->velocitY = INITIAL_PREDATOR_VELOCITY ;

    double dist = 0;
    do
    {
        predator->core.pos[pX] = rand() % (int)(WINDOW[pX] - predator->core.pos[pX]) ;
        predator->core.pos[pY] = rand() % (int)(WINDOW[pY] - predator->core.pos[pY]) ;
        if(TheOtherPredator) 
        {
            dist = Folded_Space_Distance(   TheOtherPredator->core.pos[pX], TheOtherPredator->core.pos[pY],
                                            predator->core.pos[pX], predator->core.pos[pY]);
        }

    } while( dist*dist <= pow(SAFERADIUS / 2 ,2) && TheOtherPredator);

    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    // for sprite.
    predator->anime.current = 0;
    predator->anime.Srotation = 0;
    for (int i = 0; i < 10; ++i) 
    {
        predator->anime.spriteClipsPredator[i].x = i * PSPRITE_WIDTH;
        predator->anime.spriteClipsPredator[i].y = 0;
        predator->anime.spriteClipsPredator[i].w = PSPRITE_WIDTH;
        predator->anime.spriteClipsPredator[i].h = PSPRITE_HEIGHT;
            
    }
    #endif
}

bool IsRectEmpty(Core core)
{
    return (core.dim[pX] == 0 || core.dim[pY] == 0);
}


void Initialize_Cluster(Cluster* cluster ,Creature * predator)
{
    cluster->Next = NULL;
    if (!cluster)
        printf("Cluster not created\n");

    Cluster* cls_tmp = cluster;
    for (int i = 0; i < INITIAL_FISH_NUMBER; i++)
    {
        if (i % THREADS_NORM == 0 && i != 0)
        {
            cls_tmp->Next = malloc(sizeof(Cluster));
            if (!(cls_tmp->Next))
                printf("Memory allocation failed\n");
            cls_tmp = cls_tmp->Next;
            cls_tmp->Next = NULL;
        }
        Initialize_fish(&(cls_tmp->fish[i % THREADS_NORM]), predator);
        
        if (i + 1 == INITIAL_FISH_NUMBER && INITIAL_FISH_NUMBER % THREADS_NORM)
        {
            for (int j = INITIAL_FISH_NUMBER % THREADS_NORM; j < THREADS_NORM; j++)
            {
                Initialize_fish(&(cls_tmp->fish[j]), predator);
                cls_tmp->fish[j].core.dim[pX] = 0;
                
            }
        }
    }

}

void Free_Creature(Creature *fishy)
{
    if (fishy)
    {
        free(fishy);
    }
}

void Free_Cluster(Cluster **Clusty)
{
    if (*Clusty)
    {
        while (*Clusty)
        {
            if ((*Clusty)->Next)
            {
                Free_Cluster(&(*Clusty)->Next);
            }
            free(*Clusty);
            *Clusty = NULL;
        }
    }
}

double *getFictifCoordinates(double pivot,double guest,int axis_dimension)
{
    double* fictifCoordinates = malloc(2*sizeof(double));

    fictifCoordinates[0] = pivot;
    fictifCoordinates[1] = guest;

    if(fabs(guest - pivot) > fabs(guest - (axis_dimension + pivot)) )
    {
        fictifCoordinates[1] = -axis_dimension + guest;
    }
    else if(fabs(guest - pivot) > fabs((axis_dimension + guest) - pivot) )
    {
        fictifCoordinates[1] = axis_dimension + guest;
    }

    return fictifCoordinates;
}

double Folded_Space_Distance(double x1, double y1, double x2, double y2)
{
    double *Xx = getFictifCoordinates(x1, x2, WINDOW[pX]);
    double *Yy = getFictifCoordinates(y1, y2, WINDOW[pY]);

    if (Xx && Yy) {
        double dist = sqrt(pow(Xx[0] - Xx[1], 2) + pow(Yy[0] - Yy[1], 2));
        free(Xx);
        free(Yy);
        return dist;
    } else {
        // Handle the case when getFictifCoordinates returns NULL
        // For example, you can return a special value or raise an error.
        // In this case, I'm returning -1 to indicate an error.
        return -1.0;
    }
}
double Customized_Selection(double k, double l)
{
    double opt = exp(-sqrt(l)/80);
    return k*opt;
}
double Standard_Evaluation(double k, double l) 
{
    return k*l;
}

void Centrer_Gravity(Creature *creature, Cluster *cluster, double *t_X, double *t_Y ,double (*Evaluate)(double, double))
{
    double disttotal = 0;
    double * Xx = NULL;
    double * Yy = NULL;

    *t_X = 0;
    *t_Y = 0;

    for (Cluster *tmp_cluster = cluster; tmp_cluster; tmp_cluster = tmp_cluster->Next)
    {
        for (int i = 0; i < THREADS_NORM; i++)
        {
            if (!IsRectEmpty(tmp_cluster->fish[i].core) )
            {
                
                double dist_native = 0;
                Xx = getFictifCoordinates( creature->core.pos[pX] + creature->core.dim[pX] / 2 ,
                                                tmp_cluster->fish[i].core.pos[pX] + tmp_cluster->fish[i].core.dim[pX] / 2 ,WINDOW[pX]);
                Yy = getFictifCoordinates( creature->core.pos[pY] + creature->core.dim[pY] / 2 ,
                                                tmp_cluster->fish[i].core.pos[pY] + tmp_cluster->fish[i].core.dim[pY] / 2 ,WINDOW[pY]);

                dist_native = pow(Xx[0] - Xx[1],2) + pow(Yy[0] - Yy[1],2);

                *t_X += (Evaluate)? Evaluate(Xx[1] , dist_native) : Xx[1]*dist_native;
                *t_Y += (Evaluate)? Evaluate(Yy[1] , dist_native) : Yy[1]*dist_native;
                disttotal += (Evaluate)? Evaluate(1,dist_native) : dist_native; // Attention not always 1, give it the neutral element of the group you are operating on, in my case I operated in (R,*) group
                if(Xx) free(Xx);
                if(Yy) free(Yy);
            }
        }
    }
    if(disttotal)
    {
        *t_X /= disttotal;
        *t_Y /= disttotal;
    }
}
double ln(double x)
{
    return log(x)/log(exp(1));
}

double Critique_Probability(Creature *creature, Creature *predator, double p, double l,double s,double B,double *EscapeAngle)
{
    double x = Folded_Space_Distance(   creature->core.pos[pX] + creature->core.dim[pX] / 2 ,creature->core.pos[pY] + creature->core.dim[pY] / 2,
                                        predator->core.pos[pX] + predator->core.dim[pX] / 2 , predator->core.pos[pY] + predator->core.dim[pY] / 2 );
    
    //eaten checking : 
    if(x < 3* CRITIQUERADIUS/4) Eaten(creature);
    double proba = 0;
    
    double a = CRITIQUERADIUS;
    double b = SAFERADIUS;

    if(x<a) proba = 2.0 - pow(2.0 - p, pow(x/a,s)); // F function
    else 
    {
        //double B = -1;
        s = ln(p/l)/ln((b-B)/(a-B));
        proba= sqrt(p*l) * pow((a-B)*(b-B)/pow(x-B,2) ,s/2); //G function
    }

    if(EscapeAngle)
    {
        double *Xx = getFictifCoordinates( creature->core.pos[pX] + creature->core.dim[pX] / 2 ,
                                        predator->core.pos[pX] + predator->core.dim[pX] / 2 ,WINDOW[pX]);
        double *Yy = getFictifCoordinates( creature->core.pos[pY] + creature->core.dim[pY] / 2 ,
                                        predator->core.pos[pY] + predator->core.dim[pY] / 2 ,WINDOW[pY]);

        *EscapeAngle = M_PI + atan2(  +(creature->core.pos[pY] + creature->core.dim[pY] / 2) -Yy[1],
                                +(creature->core.pos[pX] + creature->core.dim[pX] / 2) -Xx[1]);

        free(Xx);
        free(Yy);
    }
    
    return proba;
}

void Revolving_Around(Creature *creature, double disk_radius, double CenterX, double CenterY,Creature *predator)
{
    
    double dist = sqrt(pow(creature->core.pos[pX] - CenterX, 2) + pow(creature->core.pos[pY] - CenterY, 2));
    

    //Regular
    disk_radius = 25;
    double x = dist/disk_radius;
    double n = 0.001;
    double A = 1.001;
    double Collapsing = pow(A, - x);
    double AntiCollapsing = 1/pow(x+ pow(2,1/n) ,n);


    // //Alternative
    // disk_radius = 100;
    // double x = dist/disk_radius;
    // double n = 90;
    // double p = 0.97;
    // double s = 0.6;
    // double l = 50;
    // double Collapsing = pow(p, pow(fabs(2*x - 1),s) );
    // double AntiCollapsing = 1.0/2 + 1/pow(x*l+ pow(2,1/n) ,n);


    // printf("AntiCollapsing = %lf\n",AntiCollapsing);
    double Revolving_theta = AntiCollapsing * Collapsing;
    
    double EscapeAngle[2]={0,0};
    double peace_time[] = { Critique_Probability(creature, predator,0.85,0.15,7,-1,EscapeAngle),
                            Critique_Probability(creature, predator+1,0.85,0.15,7,-1,EscapeAngle+1)};

    double forwardAngle = atan2(creature->core.pos[pY] + creature->core.dim[pY]/2 - CenterY, creature->core.pos[pX] + creature->core.dim[pX]/2 - CenterX);
    
    double target_angle = forwardAngle + Revolving_theta* M_PI;
    
    
    creature->rotation = fmin(1-peace_time[0],1-peace_time[1])*target_angle + EscapeAngle[0] * peace_time[0] + EscapeAngle[1] * peace_time[1] ;
    
    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    creature->anime.Srotation = creature->rotation * 180/M_PI + 90;
    #endif
}

void Seeking_Around(Creature *creature, double CenterX, double CenterY ,Creature *predator)
{
    Creature *Reciproque = predator;
    if(creature == predator) 
        Reciproque++;
        
    double forwardAngle =  atan2(creature->core.pos[pY] + creature->core.dim[pY]/2 - CenterY, creature->core.pos[pX] + creature->core.dim[pX]/2 - CenterX);
    double Diverging_probability = 0;
    double dist = Folded_Space_Distance(creature->core.pos[pX] + creature->core.dim[pX] /2, creature->core.pos[pY] + creature->core.dim[pY]/2,
                                Reciproque->core.pos[pX] + Reciproque->core.dim[pX] /2, Reciproque->core.pos[pY] + Reciproque->core.dim[pY]/2);
    double x = dist/(2*CRITIQUERADIUS);
    

    double Div = 6;
    Diverging_probability = 1 /(1 + pow(x,Div) );
    
    double DiverAngle = 0;

    double *Xx = getFictifCoordinates( predator[0].core.gravityCenter[pX] ,
                                    predator[1].core.gravityCenter[pX] ,WINDOW[pX]);
    double *Yy = getFictifCoordinates( predator[0].core.gravityCenter[pY] ,
                                    predator[1].core.gravityCenter[pY] ,WINDOW[pY]);

    if(predator[0].core.pos[pY] != predator[1].core.pos[pY] || predator[0].core.pos[pX] != predator[1].core.pos[pX])
        DiverAngle = ((creature == predator)? -1 : 1) * 
                                        (M_PI + atan2(  Xx[0] - Xx[1],
                                                        Yy[0] - Yy[1]));
    
    free(Xx);
    free(Yy);

    Diverging_probability = Critique_Probability(creature, Reciproque,0.85,0.15,7,-1, NULL);
    
    double target_angle = (1-Diverging_probability)*forwardAngle + Diverging_probability * DiverAngle; 
    
    creature->rotation = target_angle ;
    
    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    
    creature->anime.Srotation = target_angle * 180/M_PI -180;
    #endif
}

void mv_object(Cluster *cluster, Creature *creature ,Creature *predator ,bool Is_Predator)
{
    if(false || IsRectEmpty(creature->core));
        else{
        
        if ( (-1 * creature->core.dim[pX] > creature->core.pos[pX] ) || 
            (creature->core.pos[pX] > WINDOW[pX] + 1 * creature->core.dim[pX]  ))
        {
            if(creature->core.pos[pX]< 0) creature->core.pos[pX] += WINDOW[pX] ;
            else if(creature->core.pos[pX] > WINDOW[pX] )
                creature->core.pos[pX] -= WINDOW[pX] ;

            
        }
        if((-1 * creature->core.dim[pY]  > creature->core.pos[pY]) ||
                (creature->core.pos[pY] > WINDOW[pY] + 1 * creature->core.dim[pY] ))
        {
            if(creature->core.pos[pY]< 0) creature->core.pos[pY] = WINDOW[pY] - fabs(creature->core.pos[pY]);
            else if(creature->core.pos[pY] > WINDOW[pY])
                creature->core.pos[pY] -= WINDOW[pY] ;
        }

        double (*Operation)(double ,double) = NULL;
        if(Is_Predator) Operation = Customized_Selection;
        else Operation = Customized_Selection;
        Centrer_Gravity(creature, cluster, creature->core.gravityCenter + pX, creature->core.gravityCenter + pY ,Operation);
        
        
        if(!Is_Predator) Revolving_Around(creature, 250, creature->core.gravityCenter[pX], creature->core.gravityCenter[pY], predator);
        else  Seeking_Around(creature, creature->core.gravityCenter[pX], creature->core.gravityCenter[pY], predator);

        double VelX = 0; double VelY = 0;
        VelX = - creature->velocitY * cos(creature->rotation);
        VelY = - creature->velocitY * sin(creature->rotation);

        creature->core.pos[pX] += VelX;
        creature->core.pos[pY] += VelY;
        
    }
}
