#define GRAPHIC_MODE_FOR_THE_ENGINE

#ifndef AQUAENGINE
#define AQUAENGINE

#ifdef GRAPHIC_MODE_FOR_THE_ENGINE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//384x256
#define FSPRITE_WIDTH 32
#define FSPRITE_HEIGHT 32

//1280 * 61
#define PSPRITE_WIDTH 128
#define PSPRITE_HEIGHT 61

#define THREADS_NORM 2

// 384x256
#define FNUM_SPRITES_PER_ROW 12
#define FNUM_SPRITES_PER_COL 8

#define NOMBRE_SPRITES 11

typedef struct {
    SDL_Rect spriteClips[8][3];
    SDL_Rect spriteClipsPredator[10];
    int fish_type;
    int current;
    double Srotation;
}Animation ;

#endif


#define FishHeadBoxW 25
#define FishHeadBoxH 15

#define PredatorHeadBoxW 84
#define PredatorHeadBoxH 40

#define SAFERADIUS 200
#define CRITIQUERADIUS 100

// Width ,Height
extern int WINDOW[2];// = {1000   ,700   };


#define INITIAL_FISH_VELOCITY 4
#define INITIAL_PREDATOR_VELOCITY 2
#define INITIAL_FISH_NUMBER 500

#define pX 0
#define pY 1


typedef struct {
    double pos[2];
    double dim[2];
    double gravityCenter[2];
}Core ;

typedef struct kk{
    Core core;
    double velocitY;
    double rotation;
    #ifdef GRAPHIC_MODE_FOR_THE_ENGINE
    Animation anime;
    #endif
} Creature;

typedef struct kp{
    Creature fish[THREADS_NORM];
    struct kp *Next;
}Cluster;

void Initialize_fish(Creature* fishy ,Creature * predator);
void Eaten(Creature *creature);
void Initialize_Predator(Creature* predator , Creature* TheOtherPredator);
bool IsRectEmpty(Core core);
void Initialize_Cluster(Cluster* cluster ,Creature * predator);
void Free_Creature(Creature *fishy);
void Free_Cluster(Cluster **Clusty);
double *getFictifCoordinates(double pivot,double guest,int axis_dimension);
double Folded_Space_Distance(double x1,double y1,double x2,double y2 /*int Entity[2][4]*/);
double Customized_Selection(double k, double l);
double Standard_Evaluation(double k, double l);
void Centrer_Gravity(Creature *creature, Cluster *cluster, double *t_X, double *t_Y ,double (*Evaluate)(double, double));
double Critique_Probability(Creature *creature, Creature *predator, double p, double l,double s,double B,double *EscapeAngle);
void Revolving_Around(Creature *creature, double disk_radius, double CenterX, double CenterY,Creature *predator);
void Seeking_Around(Creature *creature, double CenterX, double CenterY ,Creature *predator);
void mv_object(Cluster *cluster, Creature *creature ,Creature *predator ,bool Is_Predator);

#endif
