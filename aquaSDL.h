#ifndef AQUASDL
#define AQUASDL


#ifdef GRAPHIC_MODE_FOR_THE_ENGINE
extern char * where_fish ;//= "animales2.png";
extern char * where_background ;//= "cx3t_mc71_230315.jpg";
extern char * where_predateur ;//= "Whale.png";


extern SDL_Texture* spriteSheet_Fish ;//= NULL;
extern SDL_Texture* spriteSheet_Predator ;//= NULL;


#define BLEU_FISH 0
#define BLEU_YELLOW 1
#define BLEU_RED 7







extern bool low_debugmode;// = true;
extern bool rotational_debugmode;// = false;

void drawRotatedRect(SDL_Renderer* renderer, double x, double y, double width, double height, double angle);
int* getMaxDims(double width, double height, double angle);
void DrawObject_In_debugmode(SDL_Renderer *renderer ,SDL_Rect MainRect,double *Center);
void DrawObject_In_Normal(SDL_Renderer *renderer ,SDL_Rect *rect,SDL_Rect *fictif_rect,double angle,SDL_Texture* spriteSheet_type,SDL_Rect SpirteRect);
void drawOutlineRectangles(SDL_Renderer *renderer ,double alpha, SDL_Rect * rect , SDL_Rect ** Returned_fictif_rect);
#endif

void AnimatePredator(Cluster *cluster ,void *renderer , Creature *predator);
void AnimateFish(Cluster *cluster ,void *renderer , Creature *predator);
#endif